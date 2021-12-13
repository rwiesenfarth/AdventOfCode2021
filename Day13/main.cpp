
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>


struct Dot
{
  int m_x = 0;
  int m_y = 0;

  Dot() = default;
  Dot( int x, int y )
    : m_x( x )
    , m_y( y )
  { }

  Dot &operator=( const Dot & ) = default;

  bool operator==( const Dot &other ) const
  {
    return( ( m_x == other.m_x ) && ( m_y == other.m_y ) );
  }

  Dot horizontalFolded( int at ) const
  {
    return Dot( ( m_x > at ) ? 2 * at - m_x : m_x, m_y );
  }

  Dot verticalFolded( int at ) const
  {
    return Dot( m_x, ( m_y > at ) ? 2 * at - m_y : m_y );
  }
};


namespace std
{
template <>
class hash <Dot>
{
  public:
  size_t operator()( const Dot &dot ) const
  {
    size_t h = ( std::hash<int>()( dot.m_x ) << 5 ) ^ std::hash<int>()( dot.m_y );
    return  h;
  }
};
}


std::unordered_set<Dot> horizontalFold( const std::unordered_set<Dot> &input, int at)
{
  std::unordered_set<Dot> result;

  for( const auto &dot : input )
  {
    result.insert( dot.horizontalFolded( at ) );
  }
  return result;
}


std::unordered_set<Dot> verticalFold( const std::unordered_set<Dot> &input, int at )
{
  std::unordered_set<Dot> result;

  for( const auto &dot : input )
  {
    result.insert( dot.verticalFolded( at ) );
  }
  return result;
}


void printout( const std::unordered_set<Dot> &dots )
{
  int xMax = 0;
  int yMax = 0;

  for( const auto &dot : dots )
  {
    if( dot.m_x > xMax )
    {
      xMax = dot.m_x;
    }
    if( dot.m_y > yMax )
    {
      yMax = dot.m_y;
    }
  }

  std::vector<std::string> output( yMax + 1, std::string( xMax + 1, ' ' ) );

  for( const auto &dot : dots )
  {
    output[dot.m_y][dot.m_x] = '#';
  }

  std::cout << "Decoded output:" << std::endl;

  for( const auto &line : output )
  {
    std::cout << line << std::endl;
  }
}


int main()
{
  std::ifstream                           input( R"(D:\Develop\AdventOfCode2021\Day13\input.txt)" );
  std::string                             textLine;
  std::unordered_set<Dot> dots;

  while( std::getline( input, textLine ) )
  {
    if( textLine.empty() )
    {
      break;
    }

    auto pos = textLine.find( ',' );
    dots.insert( { std::stoi( textLine.substr( 0, pos ) ), std::stoi( textLine.substr( pos + 1 ) ) } );
  }

  while( std::getline( input, textLine ) )
  {
    std::cout << "Intermediate result has " << dots.size() << " dots." << std::endl;

    auto pos = textLine.find( '=' );
    auto at = std::stoi( textLine.substr( pos + 1 ) );
    if( textLine[11] == 'x' )
    {
      std::cout << "Folding at x = " << at << std::endl;
      dots = horizontalFold( dots, at );
    }
    else
    {
      std::cout << "Folding at y = " << at << std::endl;
      dots = verticalFold( dots, at );
    }
  }

  std::cout << "Final result has " << dots.size() << " dots." << std::endl;

  printout( dots );

  return 0;
}