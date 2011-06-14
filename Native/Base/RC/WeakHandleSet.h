/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_WEAK_HANDLE_SET_H
#define _FABRIC_RC_WEAK_HANDLE_SET_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>
#include <Fabric/Base/RC/WeakHandle.h>

#include <set>
#include <vector>

namespace Fabric
{
  namespace RC
  {
    template< class RCO > class WeakHandleSet
    {
      typedef RC::WeakHandle<RCO> WH;
      typedef std::set< WH > WHSet;
      typedef typename WHSet::iterator WHSetIter;
      
    public:
    
      WeakHandleSet()
      {
      }
      
      void add( WeakHandle<RCO> const &handle )
      {
        m_set.insert( handle );
      }
      
      std::vector< Handle<RCO> > resolve()
      {
        std::vector< Handle<RCO> > result;
        WHSetIter it = m_set.begin();
        while ( it != m_set.end() )
        {
          WHSetIter thisIT = it++;
          Handle<RCO> handle = thisIT->makeStrong();
          if ( handle )
            result.push_back( handle );
          else m_set.erase(thisIT);
        }
        return result;
      }
            
    private:
    
      WHSet m_set;
    };
  };
};

#endif //_FABRIC_RC_WEAK_HANDLE_SET_H
