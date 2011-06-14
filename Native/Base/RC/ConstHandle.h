/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_CONST_HANDLE_H
#define _FABRIC_RC_CONST_HANDLE_H

#include <Fabric/Base/RC/Handle.h>

namespace Fabric
{
  namespace RC
  {
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // [pzion 20100802] WARNING: this code looks like there's
    // a lot of redundancy, but it's necessary so that the STL
    // sees the existance of certain operators and thereby doesn't
    // just copy the bits for these objects
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    template< class RCO > class ConstHandle
    {
    public:
    
      ConstHandle()
        : m_rco( 0 )
      {
      }
      
      ConstHandle( RCO const *rco )
        : m_rco( rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      template< class OtherRCO > ConstHandle( OtherRCO const *rco )
        : m_rco( rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      ConstHandle( ConstHandle<RCO> const &that )
        : m_rco( that.m_rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      template< class OtherRCO > ConstHandle( ConstHandle<OtherRCO> const &that )
        : m_rco( that.m_rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      ConstHandle( Handle<RCO> const &that )
        : m_rco( that.m_rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      template< class OtherRCO > ConstHandle( Handle<OtherRCO> const &that )
        : m_rco( that.m_rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      ~ConstHandle()
      {
        if ( m_rco )
        {
          m_rco->release();
#ifndef NDEBUG
          m_rco = (RCO *)0xDEADBEEF;
#endif
        }
      }
      
      ConstHandle<RCO> &operator =( RCO const *rco )
      {
        if ( m_rco != rco )
        {
          if ( m_rco )
            m_rco->release();
          m_rco = rco;
          if ( m_rco )
            m_rco->retain();
        }
        return *this;
      }
      
      template< class OtherRCO > ConstHandle<RCO> &operator =( OtherRCO const *rco )
      {
        if ( m_rco != rco )
        {
          if ( m_rco )
            m_rco->release();
          m_rco = rco;
          if ( m_rco )
            m_rco->retain();
        }
        return *this;
      }
      
      ConstHandle<RCO> &operator =( ConstHandle<RCO> const &that )
      {
        if ( m_rco != that.m_rco )
        {
          if ( m_rco )
            m_rco->release();
          m_rco = that.m_rco;
          if ( m_rco )
            m_rco->retain();
        }
        return *this;
      }
      
      template< class OtherRCO > ConstHandle<RCO> &operator =( ConstHandle<OtherRCO> const &that )
      {
        if ( m_rco != that.m_rco )
        {
          if ( m_rco )
            m_rco->release();
          m_rco = that.m_rco;
          if ( m_rco )
            m_rco->retain();
        }
        return *this;
      }
      
      ConstHandle<RCO> &operator =( Handle<RCO> const &that )
      {
        if ( m_rco != that.m_rco )
        {
          if ( m_rco )
            m_rco->release();
          m_rco = that.m_rco;
          if ( m_rco )
            m_rco->retain();
        }
        return *this;
      }
      
      template< class OtherRCO > ConstHandle<RCO> &operator =( Handle<OtherRCO> const &that )
      {
        if ( m_rco != that.m_rco )
        {
          if ( m_rco )
            m_rco->release();
          m_rco = that.m_rco;
          if ( m_rco )
            m_rco->retain();
        }
        return *this;
      }
      
      template< class OtherRCO > static ConstHandle<RCO> StaticCast( OtherRCO const *otherRCO )
      {
        RCO const *rco = static_cast<RCO const *>( otherRCO );
        return ConstHandle<RCO>( rco );
      }
      
      template< class OtherRCO > static ConstHandle<RCO> StaticCast( ConstHandle<OtherRCO> const &that )
      {
        RCO const *rco = static_cast<RCO const *>( that.m_rco );
        return ConstHandle<RCO>( rco );
      }
      
      template< class OtherRCO > static ConstHandle<RCO> StaticCast( Handle<OtherRCO> const &that )
      {
        RCO const *rco = static_cast<RCO const *>( that.m_rco );
        return ConstHandle<RCO>( rco );
      }
      
      bool isNull() const
      {
        return m_rco == 0;
      }
      
      operator bool() const
      {
        return !isNull();
      }
      
      RCO const *ptr() const
      {
        return m_rco;
      }
      
      RCO const &operator *() const
      {
        return *ptr();
      }
      
      RCO const *operator ->() const
      {
        return ptr();
      }
      
      RCO const *take()
      {
        RCO const *result = m_rco;
        m_rco = 0;
        return result;
      }
      
      bool operator ==( ConstHandle<RCO> const &that ) const
      {
        return m_rco == that.m_rco;
      }
      
      bool operator !=( ConstHandle<RCO> const &that ) const
      {
        return m_rco != that.m_rco;
      }
      
      bool operator <( ConstHandle<RCO> const &that ) const
      {
        return m_rco < that.m_rco;
      }
      
    private:
    
      RCO const *m_rco;
      
      template< class OtherRCO > friend class ConstHandle;
      template< class OtherRCO > friend class Handle;
    };
  };
};

#endif //_FABRIC_RC_CONST_HANDLE_H
