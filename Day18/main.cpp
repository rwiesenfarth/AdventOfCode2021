
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


struct SnailfishNumber;
std::istream &operator>>( std::istream &stream, SnailfishNumber &number );
std::ostream &operator<<( std::ostream &stream, const SnailfishNumber &number );


struct SnailfishNumber
{
  SnailfishNumber() = default;


  SnailfishNumber( std::istream &stream )
    : SnailfishNumber()
  {
    stream >> *this;
  }


  SnailfishNumber( const std::string &line )
    : SnailfishNumber()
  {
    std::istringstream stream( line );
    stream >> *this;
  }


  SnailfishNumber( const SnailfishNumber &other )
    : m_isPair( other.m_isPair )
    , m_value( other.m_value )
  {
    if( m_isPair )
    {
      m_left.reset( new SnailfishNumber( *other.m_left ) );
      m_right.reset( new SnailfishNumber( *other.m_right ) );
    }
  }


  SnailfishNumber &operator=( const SnailfishNumber &other )
  {
    if( m_isPair )
    {
      m_left.reset();
      m_right.reset();
    }
    m_isPair = other.m_isPair;
    m_value = other.m_value;
    if( m_isPair )
    {
      m_left.reset( new SnailfishNumber( *other.m_left ) );
      m_right.reset( new SnailfishNumber( *other.m_right ) );
    }

    return *this;
  }


  int *leftRegular()
  {
    return m_isPair ? m_left->leftRegular() : &m_value;
  }


  int *rightRegular()
  {
    return m_isPair ? m_right->rightRegular() : &m_value;
  }


  bool explode( int depth, int *nextLeft, int *nextRight )
  {
    bool result = false;

    //std::cout << "explode " << *this << " at depth " << depth << " left is " << ( nextLeft ? *nextLeft : -1 ) <<
    //  " right is " << ( nextRight ? *nextRight : -1 ) << std::endl;

    if( m_isPair )
    {
      if( depth == 4 )
      {
        if( nextLeft )
        {
          *nextLeft += m_left->m_value;
        }
        if( nextRight )
        {
          *nextRight += m_right->m_value;
        }
        m_left.reset();
        m_right.reset();
        m_isPair = false;
        m_value = 0;
        result = true;
      }
      else if( m_left->explode( depth + 1, nextLeft, m_right->leftRegular() ) )
      {
        result = true;
      }
      else if( m_right->explode( depth + 1, m_left->rightRegular(), nextRight ) )
      {
        result = true;
      }
    }
    return result;
  }


  bool split()
  {
    bool result = false;

    if( m_isPair )
    {
      result = m_left->split() || m_right->split();
    }
    else if( m_value >= 10 )
    {
      m_isPair = true;
      m_left.reset( new SnailfishNumber );
      m_right.reset( new SnailfishNumber );
      m_left->m_value = m_value / 2;
      m_right->m_value = ( m_value + 1 ) / 2;
      m_value = 0;
      result = true;
    }
    return result;
  }


  SnailfishNumber add( const SnailfishNumber &other ) const
  {
    SnailfishNumber result;

    result.m_isPair = true;
    result.m_left.reset( new SnailfishNumber( *this ) );
    result.m_right.reset( new SnailfishNumber( other ) );

    //std::cout << "added    : " << result << std::endl;
    bool didExplode = false;
    bool didSplit = false;
    do
    {
      //IMPORTANT: multiple explodes, but only one split per round - to avoid depths > 4!
      didExplode = false;
      while( result.explode( 0, nullptr, nullptr ) )
      {
        //std::cout << "exploded : " << result << std::endl;
        didExplode = true;
      }
      didSplit = false;
      if( result.split() )
      {
        //std::cout << "split    : " << result << std::endl;
        didSplit = true;
      }
    }
    while( didExplode || didSplit );

    //std::cout << "final    : " << result << std::endl;
    //std::cout << std::endl;
    return result;
  }


  std::uint64_t magnitude() const
  {
    return m_isPair ? ( 3 * m_left->magnitude() + 2 * m_right->magnitude() ) : m_value;
  }


  bool                             m_isPair = false;
  std::unique_ptr<SnailfishNumber> m_left;
  std::unique_ptr<SnailfishNumber> m_right;
  int                              m_value = 0;
};


std::istream &operator>>( std::istream &stream, SnailfishNumber &number )
{
  if( stream.peek() == '[' )
  {
    number.m_isPair = true;
    stream.get(); // '['
    number.m_left.reset( new SnailfishNumber( stream ) );
    stream.get(); // ','
    number.m_right.reset( new SnailfishNumber( stream ) );
    stream.get(); // ']'
  }
  else
  {
    number.m_isPair = false;
    stream >> number.m_value;
  }
  return stream;
}


std::ostream &operator<<( std::ostream &stream, const SnailfishNumber &number )
{
  if( number.m_isPair )
  {
    stream << '[' << *number.m_left << ',' << *number.m_right << ']';
  }
  else
  {
    stream << number.m_value;
  }
  return stream;
}


int main()
{
  //SnailfishNumber number( "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]" );

  //SnailfishNumber number( "[[[[4,3],4],4],[7,[[8,4],9]]]" );
  //number = number.add( SnailfishNumber( "[1,1]" ) );
  //std::cout << number << std::endl;

  //SnailfishNumber toExplode( "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]" );
  //SnailfishNumber toExplode( "[[[[4,0],[5,4]],[[7,7],[6,0]]],[[[6,6],[5,0]],[[6,6],[8,[[5,6],8]]]]]" );
  //std::cout << toExplode << std::endl;
  //toExplode.explode( 0, nullptr, nullptr );
  //std::cout << toExplode << std::endl;

  //SnailfishNumber toSplit( "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]" );
  //std::cout << toSplit << std::endl;
  //toSplit.split();
  //std::cout << toSplit << std::endl;

  //SnailfishNumber number( "[1,1]" );
  //number = number.add( SnailfishNumber( "[2,2]" ) );
  //number = number.add( SnailfishNumber( "[3,3]" ) );
  //number = number.add( SnailfishNumber( "[4,4]" ) );
  //number = number.add( SnailfishNumber( "[5,5]" ) );
  //number = number.add( SnailfishNumber( "[6,6]" ) );

  //SnailfishNumber number( "[[[[4,3],4],4],[7,[[8,4],9]]]" );
  //number = number.add( SnailfishNumber( "[1,1]" ) );

  std::ifstream input( R"(D:\Develop\AdventOfCode2021\Day18\input.txt)" );
  std::string   textLine;
  std::vector<SnailfishNumber> operands;

  std::getline( input, textLine );
  SnailfishNumber number( textLine );
  operands.push_back( number );

  while( std::getline( input, textLine ) )
  {
    SnailfishNumber summand( textLine );
    operands.push_back( summand );

    //std::cout << "   " << number << std::endl;
    //std::cout << "+  " << summand << std::endl;
    number = number.add( summand );
    //std::cout << "=  " << number << std::endl;
    //std::cout << std::endl;
  }

  std::cout << "result: " << number << std::endl;
  std::cout << "magnitude: " << number.magnitude() << std::endl;

  int maxMagnitude = 0;

  for( int i = 0; i < operands.size(); i++ )
  {
    for( int j = 0; j < operands.size(); j++ )
    {
      if( i == j )
      {
        continue;
      }

      SnailfishNumber sum = operands[i].add( operands[j] );
      int magnitude = sum.magnitude();

      if( magnitude > maxMagnitude )
      {
        maxMagnitude = magnitude;
      }
    }
  }

  std::cout << "maximum magnitude is " << maxMagnitude << std::endl;

  return 0;
}