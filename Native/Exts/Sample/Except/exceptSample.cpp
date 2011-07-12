/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

using namespace Fabric::EDK;

FABRIC_EXT_EXPORT void exceptThrow(
  KL::String const &desc
  )
{
  throw Exception( std::string( desc.data(), desc.length() ) );
}
