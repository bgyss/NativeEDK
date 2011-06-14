/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_DECODE_H
#define _FABRIC_JSON_DECODE_H

#include <Fabric/Base/RC/ConstHandle.h>

#include <string>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    
    RC::ConstHandle<Value> decode( char const *data, unsigned len );
    
    RC::ConstHandle<Value> decode( std::string const &string );
  };
};

#endif //_FABRIC_JSON_DECODE_H
