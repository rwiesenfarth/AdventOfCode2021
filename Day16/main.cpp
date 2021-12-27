
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <queue>
#include <stack>


struct ByteStream
{
  ByteStream( const std::string &data )
    : m_data( data )
  { }

  bool eof()
  {
    return ( m_position == m_data.size() );
  }

  char getByte()
  {
    if( eof() )
    {
      return 0;
    }

    char c = m_data[m_position++];
    return ( c >= 'A' ) ? c + 10 - 'A' : c - '0';
  }

  std::string m_data;
  int         m_position = 0;
};


class BitStream
{
  public:

  virtual bool eof() = 0;
  virtual bool getBit() = 0;
  virtual void skip() = 0;

  protected:
  BitStream() = default;
};


class ByteBitStream : public BitStream
{
  public:

  ByteBitStream( const std::string &data )
    : m_bytes( data )
  { }

  bool eof() override
  {
    return ( m_bitMask == 0 ) && m_bytes.eof();
  }

  bool getBit() override
  {
    if( eof() )
    {
      return false;
    }

    bool result = false;

    if( m_bitMask == 0 )
    {
      m_currentByte = m_bytes.getByte();
      m_bitMask = 8;
    }
    result = ( m_currentByte & m_bitMask );
    m_bitMask >>= 1;

    return result;
  }

  void skip() override
  {
    m_bitMask = 0;
  }

  ByteStream m_bytes;
  char       m_currentByte = 0;
  int        m_bitMask = 0;
};


class SubBitStream : public BitStream
{
  public:

  SubBitStream( BitStream &stream, int length )
    : m_stream( stream )
    , m_length( length )
  { }

  bool eof() override
  {
    return ( m_position >= m_length );
  }

  bool getBit() override
  {
    if( eof() )
    {
      return false;
    }

    m_position++;
    return m_stream.getBit();
  }

  void skip() override
  {
    while( !eof() )
    {
      getBit();
    }
  }

  BitStream &m_stream;
  int        m_length;
  int        m_position = 0;
};


struct Packet
{
  Packet( BitStream &stream, int level = 0 );

  std::int64_t getNumber( BitStream &stream, int numBits )
  {
    std::int64_t result = 0;

    for( int bit = 0; bit < numBits; bit++ )
    {
      result = ( result << 1 ) + ( stream.getBit() ? 1 : 0 );
    }
    return result;
  }

  std::int64_t getValue( BitStream &stream )
  {
    std::int64_t result   = 0;
    bool hasNext = false;

    do
    {
      hasNext = stream.getBit();
      result = ( result << 4 ) + getNumber( stream, 4 );
    }
    while( hasNext );

    return result;
  }

  int versionSum() const
  {
    int result = m_version;
    for( const auto &child : m_children )
    {
      result += child.versionSum();
    }
    return result;
  }

  std::int64_t evaluate() const;

  int                 m_level   = 0;
  int                 m_version = 0;
  int                 m_type    = 0;
  std::int64_t        m_value   = 0;
  std::vector<Packet> m_children;
};


Packet::Packet( BitStream &stream, int level )
  : m_level( level )
{
  m_version = getNumber( stream, 3 );
  m_type = getNumber( stream, 3 );

  if( m_type == 4 )
  {
    m_value = getValue( stream );
  }
  else
  {
    bool shortLength = stream.getBit();
    if( shortLength )
    {
      int length = getNumber( stream, 11 );
      for( int i = 0; i < length; i++ )
      {
        m_children.push_back( Packet( stream, m_level + 1 ) );
      }
    }
    else
    {
      int length = getNumber( stream, 15 );
      SubBitStream subStream( stream, length );
      while( !subStream.eof() )
      {
        m_children.push_back( Packet( subStream, m_level + 1 ) );
      }
    }
  }

  //stream.skip();
}


std::int64_t Packet::evaluate() const
{
  std::int64_t result = 0;
  bool isSet = false;
  std::int64_t eval;
  std::int64_t eval1;
  std::int64_t eval2;
  std::vector<std::int64_t> values;
  std::string prefix;

  for( int i = 0; i < m_level; i++ )
  {
    prefix += "  ";
  }

  switch( m_type )
  {
    case 0:
      for( const auto &child : m_children )
      {
        isSet = true;
        eval = child.evaluate();
        values.push_back( eval );
        result += eval;
      }
      std::cout << prefix << "Adding";
      for( auto v : values )
      {
        std::cout << " " << v;
      }
      std::cout << " results in " << result << std::endl;
      break;
    case 1:
      for( const auto &child : m_children )
      {
        eval = child.evaluate();
        values.push_back( eval );
        if( isSet )
        {
          result *= eval;
        }
        else
        {
          result = eval;
          isSet = true;
        }
      }
      std::cout << prefix << "Multiplying";
      for( auto v : values )
      {
        std::cout << " " << v;
      }
      std::cout << " results in " << result << std::endl;
      break;
    case 2:
      for( const auto &child : m_children )
      {
        eval = child.evaluate();
        values.push_back( eval );
        if( isSet )
        {
          if( eval < result )
          {
            result = eval;
          }
        }
        else
        {
          result = eval;
          isSet = true;
        }
      }
      std::cout << prefix << "Minimum of";
      for( auto v : values )
      {
        std::cout << " " << v;
      }
      std::cout << " results in " << result << std::endl;
      break;
    case 3:
      for( const auto &child : m_children )
      {
        eval = child.evaluate();
        values.push_back( eval );
        if( isSet )
        {
          if( eval > result )
          {
            result = eval;
          }
        }
        else
        {
          result = eval;
          isSet = true;
        }
      }
      std::cout << prefix << "Maximum of";
      for( auto v : values )
      {
        std::cout << " " << v;
      }
      std::cout << " results in " << result << std::endl;
      break;
    case 4:
      result = m_value;
      std::cout << prefix << "Looking up value results in " << result << std::endl;
      break;
    case 5:
      eval1 = m_children[0].evaluate();
      eval2 = m_children[1].evaluate();
      std::cout << prefix << "Greater than of " << eval1 << " " << eval2;
      result = ( eval1 > eval2 ) ? 1 : 0;
      std::cout << " results in " << result << std::endl;
      break;
    case 6:
      eval1 = m_children[0].evaluate();
      eval2 = m_children[1].evaluate();
      std::cout << prefix << "Less than of " << eval1 << " " << eval2;
      result = ( eval1 < eval2 ) ? 1 : 0;
      std::cout << " results in " << result << std::endl;
      break;
    case 7:
      eval1 = m_children[0].evaluate();
      eval2 = m_children[1].evaluate();
      std::cout << prefix << "Equals of " << eval1 << " " << eval2;
      result = ( eval1 == eval2 ) ? 1 : 0;
      std::cout << " results in " << result << std::endl;
      break;
  }
  return result;
}


std::ostream &operator<<( std::ostream &stream, const Packet &packet )
{
  for( int i = 0; i < packet.m_level; i++ )
  {
    stream << "    ";
  }
  stream << "Version " << packet.m_version << ", Type " << packet.m_type << ", Value " << packet.m_value;
  for( const auto &child : packet.m_children )
  {
    stream << std::endl << child;
  }
  return stream;
}


int main()
{
  std::ifstream input( R"(..\Day16\input.txt)" );
  std::string   textLine;

  std::getline( input, textLine );

  ByteBitStream stream( textLine );
  //ByteBitStream stream( "9C0141080250320F1802104A08" );

  Packet packet( stream );

  std::cout << packet << std::endl;
  std::cout << "Version sum is " << packet.versionSum() << std::endl;
  std::cout << "Evaluation gives " << packet.evaluate() << std::endl;

  return 0;
}