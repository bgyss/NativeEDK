/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_EDK_H
#define _FABRIC_EDK_H

#include <Fabric/Base/Config.h>
#include <Fabric/Base/Util/Bits.h>
#include <Fabric/Base/Util/AtomicSize.h>
#include <Fabric/Base/Exception.h>

#if defined(FABRIC_OS_WINDOWS)
# define FABRIC_EXT_EXPORT extern "C" __declspec(dllexport)
#else
# define FABRIC_EXT_EXPORT extern "C" __attribute__ ((visibility("default")))
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
    typedef Fabric::Exception Exception;
    
    namespace KL
    {
      typedef bool Boolean;
      typedef uint8_t Byte;
      typedef int32_t Integer;
      typedef size_t Size;
      typedef float Scalar;
      typedef void *Data;
    
      class String
      {
        struct bits_t
        {
          Util::AtomicSize refCount;
          size_t allocSize;
          size_t length;
          char cStr[0]; // [pzion 20101225] Will be extended to length of string plus null terminator
        };

      public:
    
        String()
          : m_bits(0)
        {
        }
      
        String( char const *cString )
          : m_bits(0)
        {
          size_t length = strlen( cString );
          memcpy( expand( length ), cString, length );
        }

        String( String const &that )
          : m_bits( that.m_bits )
        {
          if ( m_bits )
            m_bits->refCount.increment();
        }
      
        String &assign( String const &that )
        {
          if ( m_bits != that.m_bits )
          {
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              free( m_bits );
            m_bits = that.m_bits;
            if ( m_bits )
              m_bits->refCount.increment();
          }
        }
      
        String &operator =( String const &that )
        {
          return assign( that );
        }
      
        String &operator =( char const *cString )
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
      
        String &append( char const *data, size_t length )
        {
          memcpy( expand( length ), data, length );
          return *this;
        }
      
        String &append( char ch )
        {
          return append( &ch, 1 );
        }
      
        String &operator +=( char ch )
        {
          return append( ch );
        }
      
        String &append( char const *cString )
        {
          return append( cString, strlen(cString) );
        }
      
        String &operator +=( char const *cString )
        {
          return append( cString );
        }
      
        String &append( String const &that )
        {
          if ( m_bits )
          {
            size_t thatLength = that.length();
            memcpy( expand( thatLength ), that.data(), thatLength );
            return *this;
          }
          else return assign( that );
        }

        String &operator +=( String const &that )
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
              newBits = reinterpret_cast<bits_t *>( malloc( sizeof(bits_t) + newAllocSize ) );
              newBits->refCount.setValue( 1 );
              newBits->allocSize = newAllocSize;
              if ( m_bits )
                memcpy( newBits->cStr, m_bits->cStr, newBits->length = m_bits->length );
              else newBits->length = 0;
            }
            else newBits = 0;
          
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              free( m_bits );
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
      
      private:
    
        bits_t *m_bits;
      };
    
      struct RGBA
      {
        Byte r;
        Byte g;
        Byte b;
        Byte a;
      };
    
      struct RGB
      {
        Byte r;
        Byte g;
        Byte b;
      };
    
      struct Color
      {
        Scalar r;
        Scalar g;
        Scalar b;
        Scalar a;
      };
      
      struct Vec2
      {
        Scalar x;
        Scalar y;
      };
    
      struct Vec3
      {
        Scalar x;
        Scalar y;
        Scalar z;
      };
    
      struct Vec4
      {
        Scalar x;
        Scalar y;
        Scalar z;
        Scalar t;
      };
      
      struct Mat22
      {
        Vec2 row0;
        Vec2 row1;
      };
    
      struct Mat33
      {
        Vec3 row0;
        Vec3 row1;
        Vec3 row2;
      };
    
      struct Mat44
      {
        Vec4 row0;
        Vec4 row1;
        Vec4 row2;
        Vec4 row3;
      };

      template< class Member > class VariableArray
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
              free( m_bits );
            m_bits = that.m_bits;
            if ( m_bits )
              m_bits->refCount.increment();
          }
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
          if ( (!m_bits && size > 0)
            || (m_bits && m_bits->refCount.getValue() > 1)
            || (m_bits && size == 0)
            || (m_bits && m_bits->allocSize < size + 1)
            )
          {
            bits_t *newBits;
            if ( size )
            {
              size_t newAllocSize = AllocSizeForSize( size );
              newBits = reinterpret_cast<bits_t *>( malloc( sizeof(bits_t) + newAllocSize * sizeof(Member) ) );
              newBits->refCount.setValue( 1 );
              newBits->allocSize = newAllocSize;
              if ( m_bits )
              {
                newBits->size = m_bits->size;
                memset( newBits->members, 0, newBits->size * sizeof(Member) );
                for ( size_t i=0; i<newBits->size; ++i )
                  newBits->members[i] = m_bits->members[i];
              }
              else newBits->size = size;
            }
            else newBits = 0;
          
            if ( m_bits && m_bits->refCount.decrementAndGetValue() == 0 )
              free( m_bits );
            m_bits = newBits;
          }
        }
      
      protected:
    
        static size_t AllocSizeForSize( size_t size )
        {
          return std::max( size_t(31), Util::nextPowerOfTwoMinusOne( size ) );
        }
      
      private:
    
        bits_t *m_bits;
      };
    };
  };
};

#endif //_FABRIC_EDK_H
