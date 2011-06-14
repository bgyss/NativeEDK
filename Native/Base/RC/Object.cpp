/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/WeakContainer.h>

namespace Fabric
{
  namespace RC
  {
    void Object::retain() const
    {
      m_refCount.increment();
    }
    
    void Object::release() const
    {
      if ( m_refCount.decrementAndGetValue() == 0 )
        delete this;
    }
    
    Object::Object()
      : m_refCount( 0 )
      , m_weakContainer( 0 )
    {
    }

    Object::~Object()
    {
      if ( m_weakContainer )
      {
        m_weakContainer->invalidate();
        m_weakContainer->release();
      }
    }

    WeakContainer *Object::weakContainer() const
    {
      if ( !m_weakContainer )
      {
        m_weakContainer = new WeakContainer( const_cast<Object *>(this) );
        m_weakContainer->retain();
      }
      return m_weakContainer;
    }
  };
};
