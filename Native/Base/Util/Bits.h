/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_BITS_H
#define _FABRIC_UTIL_BITS_H

#include <Fabric/Base/Config.h>
#include <vector>
#include <stdint.h>

namespace Fabric
{
  namespace Util
  {
    inline uint8_t hi( uint16_t value )
    {
      return uint8_t( value >> 8 );
    }
    
    inline uint8_t lo( uint16_t value )
    {
      return uint8_t( value );
    }
    
    inline uint16_t hi( uint32_t value )
    {
      return uint16_t( value >> 16 );
    }
    
    inline uint16_t lo( uint32_t value )
    {
      return uint16_t( value  );
    }
    
    inline uint32_t hi( uint64_t value )
    {
      return uint32_t( value >> 32 );
    }
    
    inline uint32_t lo( uint64_t value )
    {
      return uint32_t( value );
    }
      
    // Divide-and-conquer binary counting. Roughly
    // 3x - 6x faster than the 8-bit array lookup, 
    // depending on bit size of the number being
    // counted.
    inline uint8_t countBits( uint8_t v )
    {
      v = (v & 0x55) + ((v >> 1) & 0x55);
      v = (v & 0x33) + ((v >> 2) & 0x33);
      v = (v & 0x0F) + ((v >> 4) & 0x0F);
      return v;
    }

    inline uint8_t countBits( uint16_t v )
    {
      v = (v & 0x5555) + ((v >> 1) & 0x5555);
      v = (v & 0x3333) + ((v >> 2) & 0x3333);
      v = (v & 0x0F0F) + ((v >> 4) & 0x0F0F);
      v = (v & 0x00FF) + ((v >> 8) & 0x00FF);
      return uint8_t( v );
    }

    inline uint8_t countBits( uint32_t v )
    {
      v = (v & 0x55555555) + ((v >>  1) & 0x55555555);
      v = (v & 0x33333333) + ((v >>  2) & 0x33333333);
      v = (v & 0x0F0F0F0F) + ((v >>  4) & 0x0F0F0F0F);
      v = (v & 0x00FF00FF) + ((v >>  8) & 0x00FF00FF);
      v = (v & 0x0000FFFF) + ((v >> 16) & 0x0000FFFF);
      return uint8_t( v );
    }


    inline uint8_t countBits( uint64_t v )
    {
      v = (v & 0x5555555555555555ULL) + ((v >>  1) & 0x5555555555555555ULL);
      v = (v & 0x3333333333333333ULL) + ((v >>  2) & 0x3333333333333333ULL);
      v = (v & 0x0F0F0F0F0F0F0F0FULL) + ((v >>  4) & 0x0F0F0F0F0F0F0F0FULL);
      v = (v & 0x00FF00FF00FF00FFULL) + ((v >>  8) & 0x00FF00FF00FF00FFULL);
      v = (v & 0x0000FFFF0000FFFFULL) + ((v >> 16) & 0x0000FFFF0000FFFFULL);
      v = (v & 0x00000000FFFFFFFFULL) + ((v >> 32) & 0x00000000FFFFFFFFULL);
      return uint8_t( v );
    }
    
#if defined(FABRIC_OS_MACOSX)
    // [pzion 20110228] I don't know why size_t doesn't equate
    // to one of the unsigned integers on Mac OS X; it's probably
    // a compiler version issue, so there might be a better way
    // of dealing with this.
    inline uint8_t countBits( size_t value )
    {
      switch ( sizeof( value ) )
      {
        case 1:
          return countBits( uint8_t( value ) );
        case 2:
          return countBits( uint16_t( value ) );
        case 4:
          return countBits( uint32_t( value ) );
        case 8:
          return countBits( uint64_t( value ) );
      }
    }
#endif

    inline uint8_t nextPowerOfTwoMinusOne( uint8_t value )
    {
      static std::vector<uint8_t> lookupTable;
      if ( lookupTable.empty() )
      {
        lookupTable.resize( 256 );
        lookupTable[0] = 0;
        lookupTable[1] = 1;
        for ( size_t i=2; i<4; ++i )
          lookupTable[i] = 3;
        for ( size_t i=4; i<8; ++i )
          lookupTable[i] = 7;
        for ( size_t i=8; i<16; ++i )
          lookupTable[i] = 15;
        for ( size_t i=16; i<32; ++i )
          lookupTable[i] = 31;
        for ( size_t i=32; i<64; ++i )
          lookupTable[i] = 63;
        for ( size_t i=64; i<128; ++i )
          lookupTable[i] = 127;
        for ( size_t i=128; i<256; ++i )
          lookupTable[i] = 255;
      }
      return lookupTable[value];
    }

    inline uint16_t nextPowerOfTwoMinusOne( uint16_t value )
    {
      uint16_t h = hi(value);
      if ( h )
        return (uint16_t( nextPowerOfTwoMinusOne(h) ) << 8) | 0xFF;
      else return nextPowerOfTwoMinusOne( lo(value) );
    }

    inline uint32_t nextPowerOfTwoMinusOne( uint32_t value )
    {
      uint32_t h = hi(value);
      if ( h )
        return (uint32_t( nextPowerOfTwoMinusOne(h) ) << 16) | 0xFFFF;
      else return nextPowerOfTwoMinusOne( lo(value) );
    }

    inline uint64_t nextPowerOfTwoMinusOne( uint64_t value )
    {
      uint64_t h = hi(value);
      if ( h )
        return (uint64_t( nextPowerOfTwoMinusOne(h) ) << 32) | 0xFFFFFFFF;
      else return nextPowerOfTwoMinusOne( lo(value) );
    }
    
#if defined(FABRIC_OS_MACOSX)
    // [pzion 20110228] I don't know why size_t doesn't equate
    // to one of the unsigned integers on Mac OS X; it's probably
    // a compiler version issue, so there might be a better way
    // of dealing with this.
    inline size_t nextPowerOfTwoMinusOne( size_t value )
    {
      switch ( sizeof( value ) )
      {
        case 1:
          return nextPowerOfTwoMinusOne( uint8_t( value ) );
        case 2:
          return nextPowerOfTwoMinusOne( uint16_t( value ) );
        case 4:
          return nextPowerOfTwoMinusOne( uint32_t( value ) );
        case 8:
          return nextPowerOfTwoMinusOne( uint64_t( value ) );
      }
    }
#endif
    
    inline bool IsZero( size_t size, void const *data )
    {
      switch ( size )
      {
        case 1:
          return *static_cast<uint8_t const *>(data) == 0;
        case 2:
          return *static_cast<uint16_t const *>(data) == 0;
        case 4:
          return *static_cast<uint32_t const *>(data) == 0;
        case 8:
          return *static_cast<uint64_t const *>(data) == 0;
        default:
        {
          uint8_t const *p = static_cast<uint8_t const *>(data);
          uint8_t const *pEnd = p + size;
          for (; p!=pEnd; ++p)
            if ( *p != 0 )
              return false;
        }
        return true;
      }
    }
  };
};

#endif //_FABRIC_UTIL_BITS_H
