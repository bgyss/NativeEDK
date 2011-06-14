/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_WEAK_CONTAINER_H
#define _FABRIC_RC_WEAK_CONTAINER_H

#include <Fabric/Base/RC/Object.h>

namespace Fabric
{
  namespace RC
  {
    class WeakContainer : public Object
    {
    public:
    
      WeakContainer( Object *object )
        : m_object( object )
      {
      }
      
      Object *object() const
      {
        return m_object;
      }
      
      void invalidate()
      {
        m_object = 0;
      }
      
    private:
    
      Object *m_object;
    };
  };
};

#endif //_FABRIC_RC_WEAK_CONTAINER_H
