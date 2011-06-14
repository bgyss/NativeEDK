/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_EXCEPTION_H
#define _FABRIC_EXCEPTION_H

#include <string>
#include <stdarg.h>
#include <stdio.h>

namespace Fabric
{
  class Exception
  {
  public:
  
    Exception( const char *format, ... )
    {
      char buffer[4096];
      va_list argList;
      va_start( argList, format );
      vsnprintf( buffer, 4096, format, argList );
      va_end( argList );
      m_desc = buffer;
    }
    
    Exception( std::string const &desc )
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
    
    std::string const &getDesc() const
    {
      return m_desc;
    }
    
    operator std::string() const
    {
      return m_desc;
    }

  private:
  
    std::string m_desc;
  };
  
  inline Exception operator +( std::string const &left, Exception const &right )
  {
    return Exception( left + right.getDesc() );
  }
  
  inline Exception operator +( const char *left, Exception const &right )
  {
    return std::string(left) + right.getDesc();
  }
  
  inline Exception operator +( Exception const &left, std::string const &right )
  {
    return Exception( left.getDesc() + right );
  }
  
  inline Exception operator +( Exception const &left, const char *right )
  {
    return left.getDesc() + std::string(right);
  }
};

#endif //_FABRIC_EXCEPTION_H
