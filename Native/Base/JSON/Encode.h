/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_ENCODE_H
#define _FABRIC_JSON_ENCODE_H

#include <Fabric/Base/RC/ConstHandle.h>

#include <string>

namespace Fabric
{
  namespace JSON
  {
    class Value;
    
    std::string encode( RC::ConstHandle<Value> const &value, bool pretty = false );
  };
};

#endif //_FABRIC_JSON_ENCODE_H
