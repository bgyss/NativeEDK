/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_EDK_H
#define _FABRIC_EDK_H

#include <Fabric/Base/Config.h>
#include <Fabric/Base/Util/Bits.h>
#include <Fabric/Base/Util/AtomicSize.h>
#include <Fabric/Base/Exception.h>
#include <Fabric/EDK/Common.h>

#if defined(FABRIC_OS_WINDOWS)
# define FABRIC_EXT_EXPORT extern "C" __declspec(dllexport)
# define FABRIC_EXT_KL_STRUCT( name, content ) \
 __pragma( pack(push, 1) ) \
 struct name content \
 __pragma( pack(pop) )
#else
# define FABRIC_EXT_EXPORT extern "C" __attribute__ ((visibility("default")))
# define FABRIC_EXT_KL_STRUCT( name, content ) \
 struct __attribute__((__packed__)) name content
#endif

#if defined(FABRIC_OS_LINUX)
  #include <stdlib.h>
  #include <string.h>
  #include <math.h>
#endif

namespace Fabric
{
  namespace EDK
  {
    static Callbacks s_callbacks;
    
    void throwException( size_t length, char const *data )
    {
      s_callbacks.m_throwException( length, data );
    }
    
    void throwException( char const *cStrFormat, ... )
    {
      char cStr[4096];
      va_list argList;
      va_start( argList, cStrFormat );
      int vsnprintfResult = vsnprintf( cStr, 4096, cStrFormat, argList );
      va_end( argList );
      if ( vsnprintfResult < 0 )
      {
        static const char *malformattedException = "internal error: malformed exception";
        throwException( strlen( malformattedException ), malformattedException );
      }
      else throwException( vsnprintfResult, cStr );
    }
    
    void throwException( std::string const &string )
    {
      throwException( string.length(), string.data() );
    }

    namespace KL
    {
      typedef bool Boolean;
      typedef uint8_t Byte;
      typedef int32_t Integer;
      typedef size_t Size;
      typedef float Scalar;
      typedef void *Data;
    
      class StringBase
      {
        struct bits_t
        {
          Util::AtomicSize refCount;
          size_t allocSize;
          size_t length;
          char cStr[0]; // [pzion 20101225] Will be extended to length of string plus null terminator
        };

      public:
    
        StringBase &assign( StringBase const &that )
        {
          if ( m_bits != that.m_bits )
          {
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              ( *s_callbacks.m_free )( m_bits );
            m_bits = that.m_bits;
            if ( m_bits )
              m_bits->refCount.increment();
          }
          return *this;
        }
      
        StringBase &operator =( StringBase const &that )
        {
          return assign( that );
        }
      
        StringBase &operator =( char const *cString )
        {
          setData( cString, strlen(cString) );
          return *this;
        }
      
        char const *data() const
        {
          return m_bits? m_bits->cStr: 0;
        }
      
        size_t length() const
        {
          return m_bits? m_bits->length: 0;
        }
      
        StringBase &append( char const *data, size_t length )
        {
          memcpy( expand( length ), data, length );
          return *this;
        }
      
        StringBase &append( char ch )
        {
          return append( &ch, 1 );
        }
      
        StringBase &operator +=( char ch )
        {
          return append( ch );
        }
      
        StringBase &append( char const *cString )
        {
          return append( cString, strlen(cString) );
        }
      
        StringBase &operator +=( char const *cString )
        {
          return append( cString );
        }
      
        StringBase &append( StringBase const &that )
        {
          if ( m_bits )
          {
            size_t thatLength = that.length();
            memcpy( expand( thatLength ), that.data(), thatLength );
            return *this;
          }
          else return assign( that );
        }

        StringBase &operator +=( StringBase const &that )
        {
          return append( that );
        }
    
        void reserve( size_t capacity )
        {
          if ( (!m_bits && capacity > 0)
            || (m_bits && m_bits->refCount.getValue() > 1)
            || (m_bits && capacity == 0)
            || (m_bits && m_bits->allocSize < capacity + 1)
            )
          {
            bits_t *newBits;
            if ( capacity )
            {
              size_t newAllocSize = AllocSizeForLength( capacity );
              newBits = reinterpret_cast<bits_t *>( ( *s_callbacks.m_malloc )( sizeof(bits_t) + newAllocSize ) );
              newBits->refCount.setValue( 1 );
              newBits->allocSize = newAllocSize;
              if ( m_bits )
                memcpy( newBits->cStr, m_bits->cStr, newBits->length = m_bits->length );
              else newBits->length = 0;
            }
            else newBits = 0;
          
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              ( *s_callbacks.m_free )( m_bits );
            m_bits = newBits;
          }
        }
      
      protected:
    
        static size_t AllocSizeForLength( size_t length )
        {
          return std::max( size_t(31), Util::nextPowerOfTwoMinusOne( length+1 ) );
        }
    
        void setData( char const *data, size_t length )
        {
          reserve( length );
          if ( m_bits )
          {
            m_bits->length = length;
            memcpy( m_bits->cStr, data, length );
            m_bits->cStr[length] = '\0';
          }
        }
      
        char *expand( size_t delta )
        {
          size_t oldLength = length();
          size_t newLength = oldLength + delta;
        
          reserve( newLength );
          if ( m_bits )
          {
            m_bits->length = newLength;
            m_bits->cStr[newLength] = '\0';
            return &m_bits->cStr[oldLength];
          }
          else return 0;
        }
      
        bits_t *m_bits;
      };
    
      class String : public StringBase
      {
      public:
    
        String()
        {
          m_bits = 0;
        }
      
        String( char const *cString )
        {
          m_bits = 0;
          size_t length = strlen( cString );
          memcpy( expand( length ), cString, length );
        }

        String( String const &that )
        {
          m_bits = that.m_bits;
          if ( m_bits )
            m_bits->refCount.increment();
        }
        
        ~String()
        {
          if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
            ( *s_callbacks.m_free )( m_bits );
        }
        
        typedef StringBase IN;
        typedef StringBase &IO;
      };
    
      FABRIC_EXT_KL_STRUCT( RGBA, {
        Byte r;
        Byte g;
        Byte b;
        Byte a;
      } );
    
      FABRIC_EXT_KL_STRUCT( RGB, {
        Byte r;
        Byte g;
        Byte b;
      } );
    
      FABRIC_EXT_KL_STRUCT( Color, {
        Scalar r;
        Scalar g;
        Scalar b;
        Scalar a;
      } );
      
      FABRIC_EXT_KL_STRUCT( Vec2, {
        Scalar x;
        Scalar y;
      } );
    
      FABRIC_EXT_KL_STRUCT( Vec3, {
        Scalar x;
        Scalar y;
        Scalar z;
      } );
    
      FABRIC_EXT_KL_STRUCT( Vec4, {
        Scalar x;
        Scalar y;
        Scalar z;
        Scalar t;
      } );

      FABRIC_EXT_KL_STRUCT( Quat, {
        Vec3 v;
        Scalar w;
      } );
      
      FABRIC_EXT_KL_STRUCT( Xfo, {
        Quat ori;
        Vec3 tr;
        Vec3 sc;
      } );

      FABRIC_EXT_KL_STRUCT( Mat22, {
        Vec2 row0;
        Vec2 row1;
      } );
    
      FABRIC_EXT_KL_STRUCT( Mat33, {
        Vec3 row0;
        Vec3 row1;
        Vec3 row2;
      } );
    
      FABRIC_EXT_KL_STRUCT( Mat44, {
        Vec4 row0;
        Vec4 row1;
        Vec4 row2;
        Vec4 row3;
      } );

      template< class Member, bool copyOnWrite = true > class VariableArray
      {
        struct bits_t
        {
          Util::AtomicSize refCount;
          size_t allocSize;
          size_t size;
          Member members[0];
        };

      public:
    
        VariableArray()
          : m_bits(0)
        {
        }
        
        VariableArray( size_t size )
        {
          size_t newAllocSize = AllocSizeForSize( size );
          m_bits = reinterpret_cast<bits_t *>( malloc( sizeof(bits_t) + newAllocSize * sizeof(Member) ) );
          m_bits->refCount.setValue( 1 );
          m_bits->allocSize = newAllocSize;
          m_bits->size = size;
          memset( &m_bits->members[0], 0, size * sizeof(Member) );
        }
        
        VariableArray( VariableArray const &that )
          : m_bits( that.m_bits )
        {
          if ( m_bits )
            m_bits->refCount.increment();
        }
      
        VariableArray &assign( VariableArray const &that )
        {
          if ( m_bits != that.m_bits )
          {
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              ( *s_callbacks.m_free )( m_bits );
            m_bits = that.m_bits;
            if ( m_bits )
              m_bits->refCount.increment();
          }
          return *this;
        }
      
        VariableArray &operator =( VariableArray const &that )
        {
          return assign( that );
        }
      
        Member const &member( size_t index ) const
        {
          return m_bits->members[index];
        }
      
        Member const &operator[]( size_t index ) const
        {
          return member( index );
        }
      
        Member &member( size_t index )
        {
          // [pzion 20110928] FIXME: we should split here if our reference count is greater than one
          return m_bits->members[index];
        }
      
        Member &operator[]( size_t index )
        {
          return member( index );
        }
      
        size_t size() const
        {
          return m_bits? m_bits->size: 0;
        }
      
        void resize( size_t size )
        {
          if ( !copyOnWrite )
            throwException( "cannot resize a non-copy-on-write variable array" );
            
          if ( (!m_bits && size > 0)
            || (m_bits && m_bits->refCount.getValue() > 1)
            || (m_bits && size == 0)
            || (m_bits && m_bits->allocSize < size)
            )
          {
            bits_t *newBits;
            if ( size )
            {
              size_t newAllocSize = AllocSizeForSize( size );
              newBits = reinterpret_cast<bits_t *>( ( *s_callbacks.m_malloc )( sizeof(bits_t) + newAllocSize * sizeof(Member) ) );
              newBits->refCount.setValue( 1 );
              newBits->allocSize = newAllocSize;
              newBits->size = size;
              size_t oldSize = m_bits? m_bits->size: 0;
              if ( oldSize > 0 )
                memcpy( &newBits->members[0], &m_bits->members[0], oldSize * sizeof(Member) );
              if ( size > oldSize )
                memset( &newBits->members[oldSize], 0, (size - oldSize) * sizeof(Member) );
            }
            else newBits = 0;
          
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              ( *s_callbacks.m_free )( m_bits );
            m_bits = newBits;
          }
          else if(m_bits)
            m_bits->size = size;
        }
      
      protected:
    
        static size_t AllocSizeForSize( size_t size )
        {
          return std::max( size_t(31), Util::nextPowerOfTwoMinusOne( size ) );
        }
      
      private:
    
        bits_t *m_bits;
      };

      template< class Member > class SlicedArray
      {
      public:
    
        SlicedArray( size_t size )
          : m_offset( 0 )
          , m_size( size )
          , m_variableArray( size )
        {
        }
      
        SlicedArray( SlicedArray const &that )
          : m_offset( that.m_offset )
          , m_size( that.size )
          , m_variableArray( that.m_variableArray )
        {
        }
      
        SlicedArray( SlicedArray const &that, size_t offset, size_t size )
          : m_offset( that.m_offset )
          , m_size( that.size )
          , m_variableArray( that.m_variableArray )
        {
          if ( offset + size > m_variableArray.size() )
            throw Exception( "SlicedArray: offset and/or size out of range" );
        }
      
        SlicedArray &operator =( SlicedArray const &that )
        {
          m_offset = that.m_offset;
          m_size = that.m_size;
          m_variableArray = that.m_variableArray;
        }
      
        Member const &operator[]( size_t index ) const
        {
          return m_variableArray[m_offset + index];
        }
      
        Member &operator[]( size_t index )
        {
          return m_variableArray[m_offset + index];
        }
      
        size_t size() const
        {
          return m_size;
        }
      
      private:
    
        size_t m_offset;
        size_t m_size;
        VariableArray< Member, false > m_variableArray;
      };
    };
  };
};

#define IMPLEMENT_FABRIC_EDK_ENTRIES \
  FABRIC_EXT_EXPORT void FabricEDKInit( const Fabric::EDK::Callbacks& callbacks ) \
  { \
    Fabric::EDK::s_callbacks = callbacks; \
  }

#endif //_FABRIC_EDK_H
