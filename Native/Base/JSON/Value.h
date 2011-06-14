/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_JSON_VALUE_H
#define _FABRIC_JSON_VALUE_H

#include <Fabric/Base/RC/Object.h>
#include <Fabric/Base/RC/ConstHandle.h>

#include <yajl_gen.h>

namespace Fabric
{
  namespace JSON
  {
    class Null;
    class Boolean;
    class Integer;
    class Scalar;
    class String;
    class Object;
    class Array;
    
    class Value : public RC::Object
    {
    public:
    
      virtual bool isNull() const { return false; }
      virtual bool isBoolean() const { return false; }
      virtual bool isInteger() const { return false; }
      virtual bool isScalar() const { return false; }
      virtual bool isString() const { return false; }
      virtual bool isObject() const { return false; }
      virtual bool isArray() const { return false; }
      
      RC::ConstHandle<Null> toNull( char const *message = "must be a null" ) const;
      RC::ConstHandle<Boolean> toBoolean( char const *message = "must be a boolean" ) const;
      RC::ConstHandle<Integer> toInteger( char const *message = "must be an integer" ) const;
      RC::ConstHandle<Scalar> toScalar( char const *message = "must be a scalar" ) const;
      RC::ConstHandle<String> toString( char const *message = "must be a string" ) const;
      RC::ConstHandle<JSON::Object> toObject( char const *message = "must be an object" ) const;
      RC::ConstHandle<Array> toArray( char const *message = "must be an array" ) const;
      
      virtual void generate( yajl_gen yajlHandle ) const = 0;
      
    protected:
    
      Value()
      {
      }
    
      virtual ~Value()
      {
      }
    };
  };
};

#endif //_FABRIC_JSON_VALUE_H
