/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_ATOMIC_SIZE_H
#define _FABRIC_UTIL_ATOMIC_SIZE_H

#include <Fabric/Base/Config.h>

#if defined( FABRIC_WIN32 )
# include <intrin.h>
# include <stddef.h>
#elif defined( FABRIC_POSIX )
# include <stddef.h>
#endif 

namespace Fabric
{
  namespace Util
  {
    // [pzion 20110321] It is essential that the size of this class be
    // exactly the size of size_t, since we will make this assumption
    // in the KL code.
    class AtomicSize
    {
    public:
    
      AtomicSize( size_t initialValue = 0 )
        : m_value( initialValue )
      {
      }
      
      size_t getValue() const
      {
        return m_value;
      }
      
      void setValue( size_t newValue )
      {
        m_value = newValue;
      }
      
      void increment()
      {
#if defined(FABRIC_WIN32)
# if defined(WIN64)
        ::_InterlockedIncrement64( (__int64 *)&m_value );
# else
        ::_InterlockedIncrement( (long *)&m_value );
# endif
#elif defined(FABRIC_POSIX)
        __sync_fetch_and_add( &m_value, 1 );
#else
# error "Unsupported platform"
#endif //_FABRIC_UTIL_ATOMIC_SIZE_H
      }
      
      size_t getValueAndIncrement()
      {
#if defined(FABRIC_WIN32)
# if defined(WIN64)
        return ::_InterlockedExchangeAdd64( (__int64 *)&m_value, 1 );
# else
        return ::_InterlockedExchangeAdd( (long *)&m_value, 1 );
# endif
#elif defined(FABRIC_POSIX)
        return __sync_fetch_and_add( &m_value, 1 );
#else
# error "Unsupported platform"
#endif //_FABRIC_UTIL_ATOMIC_SIZE_H
      }
      
      size_t decrementAndGetValue()
      {
#if defined(FABRIC_WIN32)
# if defined(WIN64)
        return ::_InterlockedDecrement64( (__int64 *)&m_value );
# else
        return ::_InterlockedDecrement( (long *)&m_value );
# endif
#elif defined(FABRIC_POSIX)
        return __sync_sub_and_fetch( &m_value, 1 );
#else
# error "Unsupported platform"
#endif //_FABRIC_UTIL_ATOMIC_SIZE_H
      }
      
      void decrement()
      {
#if defined(FABRIC_WIN32)
# if defined(WIN64)
        ::_InterlockedDecrement64( (__int64 *)&m_value );
# else
        ::_InterlockedDecrement( (long *)&m_value );
# endif
#elif defined(FABRIC_POSIX)
        __sync_sub_and_fetch( &m_value, 1 );
#else
# error "Unsupported platform"
#endif //_FABRIC_UTIL_ATOMIC_SIZE_H
      }
      
    private:
    
      size_t m_value;
    };
  };
};

#endif //_FABRIC_UTIL_ATOMIC_SIZE_H
