/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_EDK_COMMON_H
#define _FABRIC_EDK_COMMON_H

namespace Fabric
{
  namespace EDK
  {
    struct Callbacks
    {
      Callbacks()
        : m_malloc(NULL)
        , m_realloc(NULL)
        , m_free(NULL)
        , m_throwException(NULL)
      {}

      void *( *m_malloc )( size_t );
      void *( *m_realloc )( void *, size_t );
      void ( *m_free )( void * );
      void (*m_throwException)( size_t length, char const *data );
    };
  };
};

typedef void (*FabricEDKInitPtr)( const Fabric::EDK::Callbacks& callbacks );

#endif // _FABRIC_EDK_COMMON_H
