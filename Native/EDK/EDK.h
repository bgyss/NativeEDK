/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_EDK_H
#define _FABRIC_EDK_H

/*
#include <Fabric/Core/LIB/Null.h>
#include <Fabric/Core/LIB/Void.h>
#include <Fabric/Core/LIB/Boolean.h>
#include <Fabric/Core/LIB/UInt8.h>
#include <Fabric/Core/LIB/Int32.h>
#include <Fabric/Core/LIB/Size.h>
#include <Fabric/Core/LIB/Float32.h>
#include <Fabric/Core/LIB/CopiedString.h>
#include <Fabric/Core/LIB/ReferencedString.h>
#include <Fabric/Core/LIB/TakenString.h>
#include <Fabric/Core/LIB/Object.h>
#include <Fabric/Core/LIB/NakedObject.h>
#include <Fabric/Core/LIB/BlessedObject.h>
#include <Fabric/Core/LIB/Callback.h>
#include <Fabric/Core/LIB/Array.h>
#include <Fabric/Core/LIB/NakedArray.h>
*/
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
    
  /*
    class Value
    {
    protected:
    
      Value( RC::Handle<LIB::Value> const &libValue )
        : m_libValue( libValue )
      {
      }
      
    public:
    
      static SDK::Value Bind( RC::Handle<LIB::Value> const &libValue )
      {
        return SDK::Value( libValue );
      }
      
      static RC::Handle<LIB::Value> Unbind( SDK::Value sdkValue )
      {
        return sdkValue.m_libValue;
      }
      
      static std::vector< RC::Handle<LIB::Value> > SDKArgsToLIBArgs( std::vector<SDK::Value> const &sdkArgs )
      {
        std::vector< RC::Handle<LIB::Value> > libArgs;
        libArgs.resize( sdkArgs.size() );
        for ( size_t i=0; i<sdkArgs.size(); ++i )
          libArgs[i] = sdkArgs[i].m_libValue;
        return libArgs;
      }
      
      static std::vector<SDK::Value> LIBArgsToSDKArgs( std::vector< RC::Handle<LIB::Value> > const &libArgs )
      {
        std::vector<SDK::Value> sdkArgs;
        sdkArgs.resize( libArgs.size() );
        for ( size_t i=0; i<libArgs.size(); ++i )
          sdkArgs[i] = Bind( libArgs[i] );
        return sdkArgs;
      }
    
      Value()
      {
      }
      
      Value( Value const &that )
        : m_libValue( that.m_libValue )
      {
      }
      
      Value &operator =( Value const &that )
      {
        m_libValue = that.m_libValue;
        return *this;
      }

      // String methods
      
    protected:
    
      RC::ConstHandle<LIB::String> libString() const
      {
        if ( !m_libValue->isString() )
          throw Exception( "value is not a string" );
        return RC::ConstHandle<LIB::String>::StaticCast( m_libValue );
      }
    
    public:
    
      static SDK::Value CreateString( char const *data, size_t length )
      {
        return SDK::Value( LIB::CopiedString::Create( data, length ) );
      }
      
      static SDK::Value CreateString( std::string const &string )
      {
        return SDK::Value( LIB::CopiedString::Create( string ) );
      }
      
      std::string stringValue() const
      {
        RC::ConstHandle<LIB::String> ls = libString();
        return std::string( ls->data(), ls->length() );
      }  
      
      // Array methods
      
    public:
    
      static SDK::Value CreateArray( size_t size = 0 )
      {
        return SDK::Value( LIB::NakedArray::Create( size ) );
      }

      // Object methods
      
    protected:
    
      RC::ConstHandle<LIB::Object> libObject() const
      {
        if ( !m_libValue->isObject() )
          throw Exception( "value is not an object" );
        return RC::ConstHandle<LIB::Object>::StaticCast( m_libValue );
      }
    
      RC::Handle<LIB::Object> libObject()
      {
        if ( !m_libValue->isObject() )
          throw Exception( "value is not an object" );
        return RC::Handle<LIB::Object>::StaticCast( m_libValue );
      }
    
    public:
      
      SDK::Value get( std::string const &name ) const
      {
        return SDK::Value( libObject()->get( name ) );
      }
      
      SDK::Value operator []( std::string const &name ) const
      {
        return get( name );
      }
      
      void set( std::string const &name, SDK::Value value )
      {
        libObject()->set( name, value.m_libValue );
      }

      // BlessedObject methods
      
    protected:
    
      RC::ConstHandle<LIB::BlessedObject> libBlessedObject() const
      {
        if ( !libObject()->isBlessedObject() )
          throw Exception( "value is not a blessed object" );
        return RC::ConstHandle<LIB::BlessedObject>::StaticCast( m_libValue );
      }
      
      RC::Handle<LIB::BlessedObject> libBlessedObject()
      {
        if ( !libObject()->isBlessedObject() )
          throw Exception( "value is not a blessed object" );
        return RC::Handle<LIB::BlessedObject>::StaticCast( m_libValue );
      }
      
    public:
        
      SDK::Value invokeMethod( std::string const &name, std::vector<SDK::Value> const &args )
      {
        return SDK::Value( libBlessedObject()->invokeMethod( name, SDKArgsToLIBArgs( args ) ) );
      }
      
      SDK::Value invokeMethod( std::string const &name )
      {
        std::vector<SDK::Value> args;
        return invokeMethod( name, args );
      }
      
      SDK::Value invokeMethod( std::string const &name, SDK::Value arg1 )
      {
        std::vector<SDK::Value> args;
        args.push_back( arg1 );
        return invokeMethod( name, args );
      }
      
      SDK::Value invokeMethod( std::string const &name, SDK::Value arg1, SDK::Value arg2 )
      {
        std::vector<SDK::Value> args;
        args.push_back( arg1 );
        args.push_back( arg2 );
        return invokeMethod( name, args );
      }

      // Callback methods
      
    protected:
    
      RC::ConstHandle<LIB::Callback> libCallback() const
      {
        if ( !m_libValue->isCallback() )
          throw Exception( "value is not a callback" );
        return RC::ConstHandle<LIB::Callback>::StaticCast( m_libValue );
      }
      
      RC::Handle<LIB::Callback> libCallback()
      {
        if ( !m_libValue->isCallback() )
          throw Exception( "value is not a callback" );
        return RC::Handle<LIB::Callback>::StaticCast( m_libValue );
      }
      
    public:
    
      SDK::Value construct( std::vector<SDK::Value> const &args ) const
      {
        return SDK::Value( libCallback()->construct( SDKArgsToLIBArgs( args ) ) );
      }
      
      SDK::Value construct() const
      {
        std::vector<SDK::Value> args;
        return construct( args );
      }
      
      SDK::Value construct( SDK::Value arg1 ) const
      {
        std::vector<SDK::Value> args;
        args.push_back( arg1 );
        return construct( args );
      }
      
      SDK::Value construct( SDK::Value arg1, SDK::Value arg2 ) const
      {
        std::vector<SDK::Value> args;
        args.push_back( arg1 );
        args.push_back( arg2 );
        return construct( args );
      }
      
      // Array methods
    
    protected:
    
      RC::ConstHandle<LIB::Array> libArray() const
      {
        if ( !libObject()->isArray() )
          throw Exception( "value is not an array" );
        return RC::ConstHandle<LIB::Array>::StaticCast( m_libValue );
      }
      
      RC::Handle<LIB::Array> libArray()
      {
        if ( !libObject()->isArray() )
          throw Exception( "value is not an array" );
        return RC::Handle<LIB::Array>::StaticCast( m_libValue );
      }
      
    public:
    
      void push( SDK::Value value )
      {
        libArray()->push( value.m_libValue );
      }
    
    private:
    
      RC::Handle<LIB::Value> m_libValue;
    };
    
    inline SDK::Value String( std::string const &string )
    {
      return SDK::Value::CreateString( string );
    }
    
    inline SDK::Value Array( size_t size = 0 )
    {
      return SDK::Value::CreateArray( size );
    }
  */
    
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