/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_WEAK_CONST_HANDLE_H
#define _FABRIC_RC_WEAK_CONST_HANDLE_H

#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/ConstHandle.h>
#include <Fabric/Base/RC/WeakContainer.h>

namespace Fabric
{
  namespace RC
  {
    template< class RCO > class WeakConstHandle
    {
    public:
    
      WeakConstHandle()
      {
      }
      
      WeakConstHandle( RCO const *rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      template< class OtherRCO > WeakConstHandle( OtherRCO const *rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      WeakConstHandle( ConstHandle<RCO> const &rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      template< class OtherRCO > WeakConstHandle( ConstHandle<OtherRCO> const &rco )
        : m_weakContainer( rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>() )
      {
      }
      
      WeakConstHandle( WeakConstHandle<RCO> const &that )
        : m_weakContainer( that.m_weakContainer )
      {
      }
      
      template< class OtherRCO > WeakConstHandle( WeakConstHandle<OtherRCO> const &that )
        : m_weakContainer( that.m_weakContainer )
      {
      }
      
      WeakConstHandle const &operator =( RCO const *rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      template< class OtherRCO > WeakConstHandle const &operator =( OtherRCO const *rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      WeakConstHandle const &operator =( ConstHandle<RCO> const &rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      template< class OtherRCO > WeakConstHandle const &operator =( ConstHandle<OtherRCO> const &rco )
      {
        m_weakContainer = rco? rco->weakContainer(): RC::ConstHandle<WeakContainer>();
        return *this;
      }
      
      WeakConstHandle const &operator =( WeakConstHandle<RCO> const &that )
      {
        m_weakContainer = that.m_weakContainer;
        return *this;
      }
      
      template< class OtherRCO > WeakConstHandle const &operator =( WeakConstHandle<OtherRCO> const &that )
      {
        m_weakContainer = that.m_weakContainer;
        return *this;
      }
      
      ~WeakConstHandle()
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
      
      bool operator ==( WeakConstHandle<RCO> const &that ) const
      {
        return m_weakContainer == that.m_weakContainer;
      }
      
      bool operator !=( WeakConstHandle<RCO> const &that ) const
      {
        return m_weakContainer != that.m_weakContainer;
      }
      
      bool operator <( WeakConstHandle<RCO> const &that ) const
      {
        return m_weakContainer < that.m_weakContainer;
      }

      ConstHandle<RCO> makeStrong() const
      {
        Object const *object;
        if ( m_weakContainer )
          object = m_weakContainer->object();
        else object = 0;
        return ConstHandle<RCO>( static_cast<RCO const *>( object ) );
      }
    
    private:
    
      RC::ConstHandle< WeakContainer > m_weakContainer;
    };
  };
};

#endif //_FABRIC_RC_WEAK_CONST_HANDLE_H
