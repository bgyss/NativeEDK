/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/JSON/Encode.h>
#include <Fabric/Base/JSON/Value.h>

#include <yajl_gen.h>
#include <list>
#include <string.h>
#include <stdlib.h>

namespace Fabric
{
  namespace JSON
  {
    class Generator
    {
    public:
    
      Generator()
      {
        m_yajlHandle = yajl_gen_alloc2( &Generator::PrintCallback, &s_yajlGeneratorConfig, &s_yajlAllocFuncs, this );
      }
      
      ~Generator()
      {
        yajl_gen_free( m_yajlHandle );
      }
      
      std::string generate( RC::ConstHandle<Value> const &value )
      {
        m_result = "";
        value->generate( m_yajlHandle );
        return m_result;
      }
    
    protected:
    
      void printCallback( const char * str, unsigned int len )
      {
        m_result.append( str, len );
      }
      
      static void PrintCallback( void * ctx, const char * str, unsigned int len )
      {
        static_cast<Generator *>(ctx)->printCallback( str, len );
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
      
      static yajl_gen_config s_yajlGeneratorConfig;
      static yajl_alloc_funcs s_yajlAllocFuncs;
      
      yajl_gen m_yajlHandle;
      std::string m_result;
    };
    
    yajl_gen_config Generator::s_yajlGeneratorConfig = {
      0,
      0
    };
    
    yajl_alloc_funcs Generator::s_yajlAllocFuncs = {
      &Generator::Malloc,
      &Generator::Realloc,
      &Generator::Free,
      NULL
    };

    std::string encode( RC::ConstHandle<Value> const &value )
    {
      return Generator().generate( value );
    }
  };
};
