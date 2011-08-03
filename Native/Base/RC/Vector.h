/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_RC_VECTOR_H
#define _FABRIC_RC_VECTOR_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/Handle.h>

#include <vector>

namespace Fabric
{
  namespace RC
  {
    template<class T> class Vector : public Object, public std::vector<T>
    {
    public:
    
      static Handle<Vector> Create()
      {
        return new Vector;
      }
      static Handle<Vector> Create( T const &t )
      {
        return new Vector( t );
      }
      
      T const &get( size_t index ) const
      {
        return (*this)[index];
      }
            
    protected:
    
      Vector()
      {
      }
      
      Vector( T const &t )
        : std::vector<T>( t )
      {
      }
    };
  };
};

#endif //_FABRIC_RC_VECTOR_H
