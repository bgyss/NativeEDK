/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_OBJECT_H
#define _FABRIC_RC_OBJECT_H

#include <Fabric/Base/Util/AtomicSize.h>

namespace Fabric
{
  namespace RC
  {
    class WeakContainer;
    template < class RCO > class WeakHandle;
    template < class RCO > class WeakConstHandle;
    
    class Object
    {
      template < class RCO > friend class WeakHandle;
      template < class RCO > friend class WeakConstHandle;

    public:
    
      void retain() const;
      void release() const;
      
    protected:
    
      Object();
      virtual ~Object();
    
      WeakContainer *weakContainer() const;
      
    private:
      
      mutable Util::AtomicSize m_refCount;

      mutable WeakContainer *m_weakContainer;
    };
  };
};

#endif //_FABRIC_RC_OBJECT_H
