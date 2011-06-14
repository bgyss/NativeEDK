/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_BOOLEAN_H
#define _FABRIC_JSON_BOOLEAN_H

#include <Fabric/Base/JSON/Value.h>

#include <yajl_gen.h>

namespace Fabric
{
  namespace JSON
  {
    class Boolean : public Value
    {
    public:
    
      static RC::Handle<Boolean> Create( bool value )
      {
        return new Boolean( value );
      }
      
      virtual bool isBoolean() const { return true; }
      
      bool value() const
      {
        return m_value;
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_bool( yajlHandle, (int)m_value );
      }
      
    protected:
      
      Boolean( bool value )
        : m_value(value)
      {
      }
    
    private:
    
      bool m_value;
    };
  };
};

#endif //_FABRIC_JSON_BOOLEAN_H
