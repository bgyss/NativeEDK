/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_EXCEPTION_H
#define _FABRIC_EXCEPTION_H

#include <stdarg.h>
#include <stdio.h>
#include <Fabric/Base/Util/SimpleString.h>

namespace Fabric
{
  class Exception
  {
  public:
  
    Exception( size_t length, char const *data )
      : m_desc( data, length )
    {
    }
  
    Exception( const char *format, ... )
    {
      char buffer[4096];
      va_list argList;
      va_start( argList, format );
      vsnprintf( buffer, 4096, format, argList );
      va_end( argList );
      m_desc = buffer;
    }
    
    Exception( Util::SimpleString const &desc )
      : m_desc( desc )
    {
    }
    
    Exception( Exception const &that )
      : m_desc( that.m_desc )
    {
    }
    
    Exception &operator =( Exception const &that )
    {
      m_desc = that.m_desc;
      return *this;
    }

    virtual ~Exception()
    {
    }

    Util::SimpleString const &getDesc() const
    {
      return m_desc;
    }
    
    operator Util::SimpleString() const
    {
      return m_desc;
    }

    operator char const *() const
    {
      return m_desc.getCString();
    }

    char const *getData() const
    {
      return m_desc.getData();
    }

    size_t getLength() const
    {
      return m_desc.getLength();
    }

  private:
  
    Util::SimpleString m_desc;
  };
  
  inline Exception operator +( Util::SimpleString const &left, Exception const &right )
  {
    return left + right.getDesc();
  }
  
  inline Exception operator +( const char *left, Exception const &right )
  {
    return left + right.getDesc();
  }
  
  inline Exception operator +( Exception const &left, Util::SimpleString const &right )
  {
    return left.getDesc() + right;
  }
  
  inline Exception operator +( Exception const &left, const char *right )
  {
    return left.getDesc() + right;
  }
};

#endif //_FABRIC_EXCEPTION_H
