/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_HANDLE_H
#define _FABRIC_RC_HANDLE_H

namespace Fabric
{
  namespace RC
  {
    template < class RCO > class ConstHandle;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // [pzion 20100802] WARNING: this code looks like there's
    // a lot of redundancy, but it's necessary so that the STL
    // sees the existance of certain operators and thereby doesn't
    // just copy the bits for these objects
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    template< class RCO > class Handle
    {
    public:
    
      Handle()
        : m_rco( 0 )
      {
      }
      
      Handle( RCO *rco )
        : m_rco( rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      template< class OtherRCO > Handle( OtherRCO *rco )
        : m_rco( rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      Handle( Handle<RCO> const &that )
        : m_rco( that.m_rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      template< class OtherRCO > Handle( Handle<OtherRCO> const &that )
        : m_rco( that.m_rco )
      {
        if ( m_rco )
          m_rco->retain();
      }
      
      ~Handle()
      {
        if ( m_rco )
        {
          m_rco->release();
#ifndef NDEBUG
          m_rco = (RCO *)0xDEADBEEF;
#endif
        }
      }
      
      Handle<RCO> &operator =( RCO *rco )
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
      
      template< class OtherRCO > Handle<RCO> &operator =( OtherRCO *rco )
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
      
      Handle<RCO> &operator =( Handle<RCO> const &that )
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
      
      template< class OtherRCO > Handle<RCO> &operator =( Handle<OtherRCO> const &that )
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
      
      template< class OtherRCO > static Handle<RCO> StaticCast( OtherRCO *otherRCO )
      {
        RCO *rco = static_cast<RCO *>( otherRCO );
        return Handle<RCO>( rco );
      }
      
      template< class OtherRCO > static Handle<RCO> StaticCast( Handle<OtherRCO> const &that )
      {
        RCO *rco = static_cast<RCO *>( that.m_rco );
        return Handle<RCO>( rco );
      }
      
      bool isNull() const
      {
        return m_rco == 0;
      }
      
      operator bool() const
      {
        return !isNull();
      }
      
      RCO *ptr() const
      {
        return m_rco;
      }
      
      RCO &operator *() const
      {
        return *ptr();
      }
      
      RCO *operator ->() const
      {
        return ptr();
      }
      
      RCO *take()
      {
        RCO *result = m_rco;
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
      
      bool operator ==( Handle<RCO> const &that ) const
      {
        return m_rco == that.m_rco;
      }
      
      bool operator !=( Handle<RCO> const &that ) const
      {
        return m_rco != that.m_rco;
      }
      
      bool operator <( Handle<RCO> const &that ) const
      {
        return m_rco < that.m_rco;
      }
      
    private:
    
      RCO *m_rco;
      
      template< class OtherRCO > friend class ConstHandle;
      template< class OtherRCO > friend class Handle;
    };
  };
};

#endif //_FABRIC_RC_HANDLE_H
