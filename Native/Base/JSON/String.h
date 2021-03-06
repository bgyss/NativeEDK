/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_STRING_H
#define _FABRIC_JSON_STRING_H

#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/RC/String.h>

#include <string.h>
#include <string>
#include <yajl_gen.h>

#if defined(FABRIC_OS_LINUX)
	#include <stdlib.h>
#endif

namespace Fabric
{
  namespace JSON
  {
    class String : public Value
    {
    public:
    
      static RC::Handle<String> Create( char const *cString )
      {
        return new String( cString );
      }

      static RC::Handle<String> Create( char const *data, size_t length )
      {
        return new String( data, length );
      }

      static RC::Handle<String> Create( std::string const &string )
      {
        return new String( string );
      }

      static RC::Handle<String> Create( RC::ConstHandle<RC::String> const &string )
      {
        return new String( string );
      }

      virtual bool isString() const { return true; }
      
      std::string const &value() const
      {
        return m_string;
      }
      
      char const *data() const
      {
        return m_string.data();
      }
      
      size_t length() const
      {
        return m_string.length();
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_string( yajlHandle, (const unsigned char *)m_string.data(), m_string.length() );
      }
      
    protected:
    
      String( char const *cString )
        : m_string( cString )
      {
      }
    
      String( std::string const &string )
        : m_string( string )
      {
      }
    
      String( char const *data, size_t length )
        : m_string( data, length )
      {
      }
    
      String( RC::ConstHandle<RC::String> const &string )
        : m_string( string->stdString() )
      {
      }
    
    private:
    
      std::string m_string;
    };
  };
};

#endif //_FABRIC_JSON_STRING_H
