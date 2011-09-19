/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */

#ifndef _FABRIC_UTIL_SIMPLE_STRING_H
#define _FABRIC_UTIL_SIMPLE_STRING_H

#include <Fabric/Base/Util/Bits.h>

#include <algorithm>
#include <string.h>

namespace Fabric
{
  namespace Util
  {
    class SimpleString
    {
    public:
    
      SimpleString()
        : m_data( 0 )
        , m_length( 0 )
        , m_allocSize( 0 )
      {
        initFcts();
      }
      
      SimpleString( char const *data, size_t length )
      {
        initFcts();
        init( data, length );
      }
      
      SimpleString( char const *cString )
      {
        initFcts();
        init( cString, strlen( cString ) );
      }
      
      SimpleString( std::string const &string )
      {
        initFcts();
        init( string.data(), string.length() );
      }
      
      SimpleString( char const *lhsData, size_t lhsLength, char const *rhsData, size_t rhsLength )
      {
        initFcts();
        init( lhsData, lhsLength, rhsData, rhsLength );
      }

      SimpleString( SimpleString const &that )
      {
        initFcts();
        init( that.m_data, that.m_length );
      }
      
      ~SimpleString()
      {
        if ( m_data )
          (*m_freeFct)( m_data );
      }
      
      SimpleString &operator =( SimpleString const &that )
      {
        if ( m_data )
          (*m_freeFct)( m_data );
        init( that.m_data, that.m_length );
        return *this;
      }
      
      char const *getData() const
      {
        return m_data;
      }
      
      char *getData()
      {
        return m_data;
      }
      
      char const *data() const
      {
        return getData();
      }
      
      char const *getCString() const
      {
        if ( m_data )
          m_data[m_length] = '\0';
        return m_data;
      }
      
      operator char const *() const
      {
        return getCString();
      }
      
      char const *c_str() const
      {
        return getCString();
      }
      
      std::string getStdString() const
      {
        return std::string( m_data, m_length );
      }
      
      std::string stdString() const
      {
        return getStdString();
      }
      
      char *getTail()
      {
        return &m_data[m_length];
      }
      
      size_t getLength() const
      {
        return m_length;
      }
      
      size_t length() const
      {
        return getLength();
      }
      
      // Ensure we can hold at least length+1 characters
      char *reserve( size_t length )
      {
        size_t lengthPlusOne = length + 1;
        if ( lengthPlusOne > m_allocSize )
        {
          m_allocSize = AllocSizeForLengthPlusOne( lengthPlusOne );
          if ( m_data )
            m_data = (char *)(m_reallocFct)( m_data, m_allocSize );
          else m_data = (char *)(*m_mallocFct)( m_allocSize );
        }
        return m_data;
      }
      
      // Ensure that length can be expanded by at least delta
      char *expand( size_t delta )
      {
        return &reserve( m_length + delta )[m_length];
      }
      
      void increaseLength( size_t delta )
      {
        m_length += delta;
      }
      
      void append( char ch )
      {
        expand(1)[0] = ch;
        increaseLength( 1 );
      }
      
      SimpleString &operator +=( char ch )
      {
        append( ch );
        return *this;
      }
      
      void append( char const *data, size_t length )
      {
        memcpy( expand( length ), data, length );
        increaseLength( length );
      }
      
      void append( char const *cString )
      {
        append( cString, strlen( cString ) );
      }
      
      SimpleString &operator +=( char const *cString )
      {
        append( cString );
        return *this;
      }
      
      void append( std::string const &string )
      {
        append( string.data(), string.length() );
      }
       
      SimpleString &operator +=( std::string const &string )
      {
        append( string );
        return *this;
      }
    
    protected:
    
      size_t AllocSizeForLengthPlusOne( size_t lengthPlusOne )
      {
        return std::max<size_t>( nextPowerOfTwoMinusOne(lengthPlusOne), 255 );
      }
      
      void initFcts()
      {
        //[JCG 20110822 Ensure a consistant malloc/free through different DLLs]
        m_mallocFct = malloc;
        m_reallocFct = realloc;
        m_freeFct = free;
      }
    
      void init( char const *data, size_t length )
      {
        if ( (m_length = length) )
        {
          m_allocSize = AllocSizeForLengthPlusOne( length + 1 );
          m_data = (char *)(*m_mallocFct)( m_allocSize );
          memcpy( m_data, data, length );
          m_data[length] = '\0';
        }
        else
        {
          m_allocSize = 0;
          m_data = 0;
        }
      }
    
      void init( char const *lhsData, size_t lhsLength, char const *rhsData, size_t rhsLength )
      {
        size_t totalLength = lhsLength + rhsLength;
        if ( (m_length = totalLength) )
        {
          m_allocSize = AllocSizeForLengthPlusOne( totalLength + 1 );
          m_data = (char *)(*m_mallocFct)( m_allocSize );
          memcpy( m_data, lhsData, lhsLength );
          memcpy( &m_data[lhsLength], rhsData, rhsLength );
        }
        else
        {
          m_allocSize = 0;
          m_data = 0;
        }
      }
      
    private:
    
      char *m_data;
      size_t m_length;
      size_t m_allocSize;

      //[JCG 20110822 Ensure a consistant malloc/free through different DLLs]
      void *( *m_mallocFct )( size_t );
      void *( *m_reallocFct )( void *, size_t );
      void ( *m_freeFct )( void * );
    };

    inline SimpleString operator +( Util::SimpleString const &left, Util::SimpleString const &right )
    {
      return SimpleString( left.getData(), left.getLength(), right.getData(), right.getLength() );
    }

    inline SimpleString operator +( const char *left, Util::SimpleString const &right )
    {
      return SimpleString( left, strlen(left), right.getData(), right.getLength() );
    }
  
    inline SimpleString operator +( Util::SimpleString const &left, const char *right )
    {
      return SimpleString( left.getData(), left.getLength(), right, strlen(right) );
    }
  };
};


#endif //_FABRIC_UTIL_SIMPLE_STRING_H
