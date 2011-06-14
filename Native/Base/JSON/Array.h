/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_ARRAY_H
#define _FABRIC_JSON_ARRAY_H

#include <Fabric/Base/JSON/Value.h>

#include <vector>

namespace Fabric
{
  namespace JSON
  {
    class Array : public Value
    {
      typedef std::vector< RC::ConstHandle<Value> > Vector;
      
    public:
    
      static RC::Handle<Array> Create( size_t initialSize = 0 )
      {
        return new Array( initialSize );
      }
      
      virtual bool isArray() const { return true; }
            
      size_t size() const
      {
        return m_array.size();
      }
      
      RC::ConstHandle<Value> get( unsigned index ) const
      {
        return m_array[index];
      }
      
      void set( unsigned index, RC::ConstHandle<Value> const &value )
      {
        m_array[index] = value;
      }
      
      void push_back( RC::ConstHandle<Value> const &value )
      {
        m_array.push_back( value );
      }
      
      void append( RC::ConstHandle<Array> const &array )
      {
        size_t arraySize = array->size();
        for ( size_t i=0; i<arraySize; ++i )
          push_back( array->get(i) );
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_array_open( yajlHandle );
        for ( Vector::const_iterator it=m_array.begin(); it!=m_array.end(); ++it )
        {
          RC::ConstHandle<Value> const &value = *it;
          value->generate( yajlHandle );
        }
        yajl_gen_array_close( yajlHandle );
      }
    
    protected:
    
      Array( size_t initialSize )
      {
        m_array.resize( initialSize );
      }
      
    private:
    
      Vector m_array;
    };
  };
};

#endif //_FABRIC_JSON_ARRAY_H
