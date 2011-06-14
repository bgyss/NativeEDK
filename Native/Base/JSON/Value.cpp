/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#include <Fabric/Base/JSON/Value.h>
#include <Fabric/Base/JSON/Null.h>
#include <Fabric/Base/JSON/Boolean.h>
#include <Fabric/Base/JSON/Integer.h>
#include <Fabric/Base/JSON/Scalar.h>
#include <Fabric/Base/JSON/String.h>
#include <Fabric/Base/JSON/Object.h>
#include <Fabric/Base/JSON/Array.h>
#include <Fabric/Base/Exception.h>

namespace Fabric
{
  namespace JSON
  {
    RC::ConstHandle<Null> Value::toNull( char const *message ) const
    {
      if ( !isNull() )
        throw Exception( message );
      return RC::ConstHandle<Null>::StaticCast( this );
    }
    
    RC::ConstHandle<Boolean> Value::toBoolean( char const *message ) const
    {
      if ( !isBoolean() )
        throw Exception( message );
      return RC::ConstHandle<Boolean>::StaticCast( this );
    }
    
    RC::ConstHandle<Integer> Value::toInteger( char const *message ) const
    {
      if ( !isInteger() )
        throw Exception( message );
      return RC::ConstHandle<Integer>::StaticCast( this );
    }
    
    RC::ConstHandle<Scalar> Value::toScalar( char const *message ) const
    {
      if ( !isScalar() )
        throw Exception( message );
      return RC::ConstHandle<Scalar>::StaticCast( this );
    }
    
    RC::ConstHandle<String> Value::toString( char const *message ) const
    {
      if ( !isString() )
        throw Exception( message );
      return RC::ConstHandle<String>::StaticCast( this );
    }
    
    RC::ConstHandle<JSON::Object> Value::toObject( char const *message ) const
    {
      if ( !isObject() )
        throw Exception( message );
      return RC::ConstHandle<JSON::Object>::StaticCast( this );
    }
    
    RC::ConstHandle<Array> Value::toArray( char const *message ) const
    {
      if ( !isArray() )
        throw Exception( message );
      return RC::ConstHandle<Array>::StaticCast( this );
    }
  };
};
