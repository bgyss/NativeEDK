/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#ifndef _FABRIC_JSON_SCALAR_H
#define _FABRIC_JSON_SCALAR_H

#include <Fabric/Base/JSON/Value.h>

#include <yajl_gen.h>

namespace Fabric
{
  namespace JSON
  {
    class Scalar : public Value
    {
    public:
    
      static RC::Handle<Scalar> Create( double value )
      {
        return new Scalar( value );
      }
      
      virtual bool isScalar() const { return true; }
      
      double value() const
      {
        return m_value;
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_double( yajlHandle, (double)m_value );
      }
    
    protected:
    
      Scalar( double value )
        : m_value(value)
      {
      }
    
    private:
    
      double m_value;
    };
  };
};

#endif //_FABRIC_JSON_SCALAR_VALUE_H
