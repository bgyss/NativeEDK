/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_OBJECT_H
#define _FABRIC_JSON_OBJECT_H

#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/Exception.h>

#include <yajl_gen.h>
#include <map>

namespace Fabric
{
  namespace JSON
  {
    class Object : public Value
    {
      typedef std::map< std::string, RC::ConstHandle<Value> > Map;

    public:
    
      static RC::Handle<Object> Create()
      {
        return new Object;
      }
      
      virtual bool isObject() const { return true; }
      
      bool has( std::string const &key ) const
      {
        return m_map.find( key ) != m_map.end();
      }
      
      RC::ConstHandle<Value> maybeGet( std::string const &key ) const
      {
        Map::const_iterator it = m_map.find( key );
        RC::ConstHandle<Value> result;
        if ( it != m_map.end() )
          result = it->second;
        return result;
      }
      
      RC::ConstHandle<Value> get( std::string const &key ) const
      {
        RC::ConstHandle<Value> result = maybeGet( key );
        if ( !result )
          throw Exception( "missing" );
        return result;
      }
      
      void set( std::string const &key, RC::ConstHandle<Value> const &value )
      {
        m_map[key] = value;
      }
      
      typedef Map::const_iterator const_iterator;
      const_iterator begin() const
      {
        return m_map.begin();
      }
      const_iterator end() const
      {
        return m_map.end();
      }
      
      virtual void generate( yajl_gen yajlHandle ) const
      {
        yajl_gen_map_open( yajlHandle );
        for ( Map::const_iterator it=m_map.begin(); it!=m_map.end(); ++it )
        {
          std::string const &key = it->first;
          yajl_gen_string( yajlHandle, (const unsigned char *)key.data(), key.length() );
          RC::ConstHandle<Value> const &value = it->second;
          value->generate( yajlHandle );
        }
        yajl_gen_map_close( yajlHandle );
      }
      
    protected:  
      
      Object()
      {
      }

    private:
    
      Map m_map;
    };
  };
};

#endif //_FABRIC_UTIL_JSON_H
