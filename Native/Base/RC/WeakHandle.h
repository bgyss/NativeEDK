/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_WEAK_HANDLE_H
#define _FABRIC_RC_WEAK_HANDLE_H

#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Base/RC/WeakContainer.h>

namespace Fabric
{
  namespace RC
  {
    template< class RCO > class WeakHandle
    {
    public:
    
      WeakHandle()
      {
      }
      
      WeakHandle( RCO *rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      template< class OtherRCO > WeakHandle( OtherRCO *rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      WeakHandle( Handle<RCO> const &rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      template< class OtherRCO > WeakHandle( Handle<OtherRCO> const &rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      WeakHandle( WeakHandle<RCO> const &that )
        : m_weakContainer( that.m_weakContainer )
      {
      }
      
      template< class OtherRCO > WeakHandle( WeakHandle<OtherRCO> const &that )
        : m_weakContainer( that.m_weakContainer )
      {
      }
      
      WeakHandle const &operator =( RCO *rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      template< class OtherRCO > WeakHandle const &operator =( OtherRCO *rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      WeakHandle const &operator =( Handle<RCO> const &rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      template< class OtherRCO > WeakHandle const &operator =( Handle<OtherRCO> const &rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      WeakHandle const &operator =( WeakHandle<RCO> const &that )
      {
        m_weakContainer = that.m_weakContainer;
        return *this;
      }
      
      template< class OtherRCO > WeakHandle const &operator =( WeakHandle<OtherRCO> const &that )
      {
        m_weakContainer = that.m_weakContainer;
        return *this;
      }
      
      ~WeakHandle()
      {
      }
      
      bool isNull() const
      {
        return !m_weakContainer || !m_weakContainer->object();
      }
      
      operator bool() const
      {
        return !isNull();
      }
      
      bool operator ==( WeakHandle<RCO> const &that ) const
      {
        return m_weakContainer == that.m_weakContainer;
      }
      
      bool operator !=( WeakHandle<RCO> const &that ) const
      {
        return m_weakContainer != that.m_weakContainer;
      }
      
      bool operator <( WeakHandle<RCO> const &that ) const
      {
        return m_weakContainer < that.m_weakContainer;
      }

      Handle<RCO> makeStrong() const
      {
        Object *object;
        if ( m_weakContainer )
          object = m_weakContainer->object();
        else object = 0;
        return Handle<RCO>( static_cast<RCO *>( object ) );
      }
    
    private:
    
      RC::ConstHandle< WeakContainer > m_weakContainer;
    };
  };
};

#endif //_FABRIC_RC_WEAK_HANDLE_H
