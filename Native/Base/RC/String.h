/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_STRING_H
#define _FABRIC_RC_STRING_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/Util/SimpleString.h>

#include <string>

namespace Fabric
{
  namespace RC
  {
    class String : public Object
    {
    public:
    
      static Handle<String> Create()
      {
        return new String;
      }
      static Handle<String> Create( char const *data, size_t length )
      {
        return new String( data, length );
      }
      static Handle<String> Create( char const *cString )
      {
        return new String( cString );
      }
      static Handle<String> Create( std::string const &string )
      {
        return new String( string );
      }
      static Handle<String> Create( Util::SimpleString const &simpleString )
      {
        return new String( simpleString );
      }
      
      char const *data() const
      {
        return m_simpleString.data();
      }
      
      size_t length() const
      {
        return m_simpleString.length();
      }
      
      char const *c_str() const
      {
        return m_simpleString.c_str();
      }
      
      std::string stdString() const
      {
        return m_simpleString.stdString();
      }
            
    protected:
    
      String()
      {
      }
      
      String( char const *data, size_t length )
        : m_simpleString( data, length )
      {
      }
      
      String( char const *cString )
        : m_simpleString( cString )
      {
      }
      
      String( std::string const &string )
        : m_simpleString( string )
      {
      }
      
      String( Util::SimpleString const &simpleString )
        : m_simpleString( simpleString )
      {
      }
      
    private:
    
      Util::SimpleString m_simpleString;
    };
  };
};

#endif //_FABRIC_RC_STRING_H
