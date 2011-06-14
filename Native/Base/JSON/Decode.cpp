/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/JSON/Decode.h>
#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Base/JSON/Boolean.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Scalar.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>

#include <yajl_parse.h>
#include <list>
#include <string.h>

namespace Fabric
{
  namespace JSON
  {
    class Parser
    {
    public:
    
      Parser()
      {
        m_yajlHandle = yajl_alloc( &s_yajlCallbacks, &s_yajlParserConfig, &s_yajlAllocFuncs, this );
      }
      
      ~Parser()
      {
        yajl_free( m_yajlHandle );
      }
      
      RC::ConstHandle<Value> parse( char const *data, unsigned len )
      {
        yajl_status yajlStatus;
        RC::ConstHandle<Value> result;
        m_contextStack.push_back( new DirectContext( result ) );
        yajlStatus = yajl_parse( m_yajlHandle, (const unsigned char *)data, len );
        if ( yajlStatus != yajl_status_ok )
          throw Exception( "JSON parser error: " + std::string( yajl_status_to_string( yajlStatus ) ) );
        yajlStatus = yajl_parse_complete( m_yajlHandle );
        if ( yajlStatus != yajl_status_ok )
          throw Exception( "JSON parser error: " + std::string( yajl_status_to_string( yajlStatus ) ) );
        Context *lastContext = m_contextStack.back();
        m_contextStack.pop_back();
        delete lastContext;
        return result;
      }
    
    protected:
    
      int nullCallback()
      {
        m_contextStack.back()->set( Null::Create() );
				return 1;
      }
      
      static int NullCallback( void * ctx )
      {
        return static_cast<Parser *>(ctx)->nullCallback();
      }
      
      int booleanCallback( bool value )
      {
        m_contextStack.back()->set( Boolean::Create( value ) );
				return 1;
      }
      
      static int BooleanCallback( void * ctx, int boolVal )
      {
        return static_cast<Parser *>(ctx)->booleanCallback( boolVal );
      }
      
      int integerCallback( long value )
      {
        m_contextStack.back()->set( Integer::Create( value ) );
				return 1;
      }
      
      static int IntegerCallback( void * ctx, long integerVal )
      {
        return static_cast<Parser *>(ctx)->integerCallback( integerVal );
      }
      
      int doubleCallback( double value )
      {
        m_contextStack.back()->set( Scalar::Create( value ) );
				return 1;
      }
      
      static int DoubleCallback( void * ctx, double doubleVal )
      {
        return static_cast<Parser *>(ctx)->doubleCallback( doubleVal );
      }
      
      int stringCallback( char const *data, unsigned length )
      {
        m_contextStack.back()->set( String::Create( data, length ) );
 				return 1;
     }
      
      static int StringCallback( void *ctx, const unsigned char *stringVal, unsigned int stringLen)
      {
        return static_cast<Parser *>(ctx)->stringCallback( (char const *)stringVal, stringLen );
      }

      int startMapCallback()
      {
        RC::Handle<Object> object = Object::Create();
        m_contextStack.back()->set( object );
        m_contextStack.push_back( new MapContext( object ) );
        return 1;
      }
      
      static int StartMapCallback( void * ctx )
      {
        return static_cast<Parser *>(ctx)->startMapCallback();
      }
      
      int mapKeyCallback( char const *keyData, unsigned keyLen )
      {
        std::string key( keyData, keyLen );
        MapContext *mapContext = static_cast<MapContext *>( m_contextStack.back() );
        mapContext->setKey( key );
				return 1;
      }
      
      static int MapKeyCallback( void * ctx, const unsigned char * key, unsigned int stringLen )
      {
        return static_cast<Parser *>(ctx)->mapKeyCallback( (char const *)key, stringLen );
      }

      int endMapCallback()
      {
        Context *lastContext = m_contextStack.back();
        m_contextStack.pop_back();
        delete lastContext;
        return 1;
      }
      
      static int EndMapCallback( void * ctx )
      {
        return static_cast<Parser *>(ctx)->endMapCallback();
      }  
      
      int startArrayCallback()
      {
        RC::Handle<Array> array = Array::Create();
        m_contextStack.back()->set( array );
        m_contextStack.push_back( new ArrayContext( array ) );
				return 1;
      }

      static int StartArrayCallback( void * ctx )
      {
        return static_cast<Parser *>(ctx)->startArrayCallback();
      }
      
      int endArrayCallback()
      {
        Context *lastContext = m_contextStack.back();
        m_contextStack.pop_back();
        delete lastContext;
        return 1;
      }
      
      static int EndArrayCallback( void * ctx )
      {
        return static_cast<Parser *>(ctx)->endArrayCallback();
      }
      
      static void *Malloc( void *ctx, unsigned int sz )
      {
        return malloc( sz );
      }
      
      static void Free( void *ctx, void *ptr )
      {
        free( ptr );
      }
      
      static void *Realloc( void *ctx, void *ptr, unsigned int sz )
      {
        return realloc( ptr, sz );
      }
      
    private:
    
      class Context
      {
      public:
      
        virtual ~Context()
        {
        }

        virtual void set( RC::ConstHandle<Value> const &value ) = 0;
        
      protected:
      
        Context() {}
      };
      
      class DirectContext : public Context
      {
      public:
      
        DirectContext( RC::ConstHandle<Value> &valueRef )
          : m_valueRef( valueRef )
        {
        }
        
        virtual void set( RC::ConstHandle<Value> const &value )
        {
          m_valueRef = value;
        }
        
      private:
      
        RC::ConstHandle<Value> &m_valueRef;
      };
      
      class MapContext : public Context
      {
      public:
      
        MapContext( RC::Handle<Object> const &object )
          : m_object( object )
        {
        }
        
        void setKey( std::string const &key )
        {
          m_key = key;
        }
        
        virtual void set( RC::ConstHandle<Value> const &value )
        {
          m_object->set( m_key, value );
        }
        
      private:
      
        RC::Handle<Object> m_object;
        std::string m_key;
      };
      
      class ArrayContext : public Context
      {
      public:
      
        ArrayContext( RC::Handle<Array> const &array )
          : m_array( array )
        {
        }
        
        virtual void set( RC::ConstHandle<Value> const &value )
        {
          m_array->push_back( value );
        }
        
      private:

        RC::Handle<Array> m_array;
      };
    
      static yajl_callbacks s_yajlCallbacks;
      static yajl_parser_config s_yajlParserConfig;
      static yajl_alloc_funcs s_yajlAllocFuncs;
      
      yajl_handle m_yajlHandle;
      std::list< Context * > m_contextStack;
    };
    
    yajl_callbacks Parser::s_yajlCallbacks = {
      &Parser::NullCallback,
      &Parser::BooleanCallback,
      &Parser::IntegerCallback,
      &Parser::DoubleCallback,
      NULL,
      &Parser::StringCallback,
      &Parser::StartMapCallback,
      &Parser::MapKeyCallback,
      &Parser::EndMapCallback,
      &Parser::StartArrayCallback,
      &Parser::EndArrayCallback
    };
    
    yajl_parser_config Parser::s_yajlParserConfig = {
      0,
      0
    };
    
    yajl_alloc_funcs Parser::s_yajlAllocFuncs = {
      &Parser::Malloc,
      &Parser::Realloc,
      &Parser::Free,
      NULL
    };

    RC::ConstHandle<Value> decode( char const *data, unsigned len )
    {
      return Parser().parse( data, len );
    }

    RC::ConstHandle<Value> decode( std::string const &string )
    {
      return decode( string.data(), string.length() );
    }
  };
};
