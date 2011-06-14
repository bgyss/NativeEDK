/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_NULL_H
#define _FABRIC_JSON_NULL_H

#include <Fabric/Base/JSON/Value.h>

#include <yajl_gen.h>

namespace Fabric
{
  namespace JSON
  {
    class Null : public Value
    {
    public:
    
      static RC::Handle<Null> Create()
      {
        return new Null;
      }
      
      virtual bool isNull() const { return true; }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_null( yajlHandle );
      }
      
    protected:
    
      Null()
      {
      }
    };
  };
};

#endif //_FABRIC_JSON_NULL_H
