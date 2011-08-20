/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
 
#include <Fabric/EDK/EDK.h>

#include <time.h>

using namespace Fabric::EDK;
IMPLEMENT_FABRIC_EDK_ENTRIES

FABRIC_EXT_EXPORT void currentTimeString(
  KL::String &result
  )
{
  time_t nowTime;
  time( &nowTime );
  
  struct tm *localTM = localtime( &nowTime );
  
  char buffer[64];
  strftime( buffer, 64, "%Y-%m-%d %H:%M:%S", localTM );
  
  result = buffer;
}
