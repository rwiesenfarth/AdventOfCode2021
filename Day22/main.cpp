
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


struct Cube
{
  Cube( int xMin, int xMax, int yMin, int yMax, int zMin, int zMax, bool state = false )
    : m_xMin( xMin )
    , m_xMax( xMax )
    , m_yMin( yMin )
    , m_yMax( yMax )
    , m_zMin( zMin )
    , m_zMax( zMax )
    , m_state( state )
  { }

  Cube( const Cube & ) = default;

  Cube( const std::string &line )
  {
    std::string s( line );

    if( s.substr( 0, 2 ) == "on" )
    {
      m_state = true;
      s = s.substr( 5 );
    }
    else
    {
      s = s.substr( 6 );
    }
    auto posY = s.find( 'y' );
    auto posZ = s.find( 'z' );
    auto posDot = s.find( '.' );
    m_xMin = std::stoi( s );
    m_xMax = std::stoi( s.substr( posDot + 2 ) );
    posDot = s.find( '.', posY );
    m_yMin = std::stoi( s.substr( posY + 2 ) );
    m_yMax = std::stoi( s.substr( posDot + 2 ) );
    posDot = s.find( '.', posZ );
    m_zMin = std::stoi( s.substr( posZ + 2 ) );
    m_zMax = std::stoi( s.substr( posDot + 2 ) );
  }

  void print( std::ostream &stream ) const
  {
    stream << "Cube " << ( m_state ? "off" : "ON " ) << " [ " << m_xMin << " .. " << m_xMax << " ] , [ " <<
      m_yMin << " .. " << m_yMax << " ] , [ " << m_zMin << " .. " << m_zMax << " ]" << std::endl;
  }

  std::vector<Cube> add( const Cube &other ) const;

  std::uint64_t count() const
  {
    if( !m_state )
    {
      return 0;
    }
    std::uint64_t dx = m_xMax - m_xMin + 1;
    std::uint64_t dy = m_yMax - m_yMin + 1;
    std::uint64_t dz = m_zMax - m_zMin + 1;
    return dx * dy * dz;
  }

  bool m_state = false;
  int m_xMin = 0;
  int m_xMax = 0;
  int m_yMin = 0;
  int m_yMax = 0;
  int m_zMin = 0;
  int m_zMax = 0;
};


std::vector<Cube> Cube::add( const Cube &other ) const
{
  std::vector<Cube> result;

  if( ( m_state == other.m_state ) ||
    ( other.m_xMax < m_xMin ) || ( other.m_xMin > m_xMax ) ||
    ( other.m_yMax < m_yMin ) || ( other.m_yMin > m_yMax ) ||
    ( other.m_zMax < m_zMin ) || ( other.m_zMin > m_zMax ) )
  {
    result.push_back( *this );
  }
  else
  {
    Cube working( *this );

    if( other.m_xMin > working.m_xMin )
    {
      result.push_back( Cube( working.m_xMin, other.m_xMin - 1, working.m_yMin, working.m_yMax,
        working.m_zMin, working.m_zMax, working.m_state ) );
      working.m_xMin = other.m_xMin;
    }
    if( other.m_xMax < working.m_xMax )
    {
      result.push_back( Cube( other.m_xMax + 1, working.m_xMax, working.m_yMin, working.m_yMax,
        working.m_zMin, working.m_zMax, working.m_state ) );
      working.m_xMax = other.m_xMax;
    }
    if( other.m_yMin > working.m_yMin )
    {
      result.push_back( Cube( working.m_xMin, working.m_xMax, working.m_yMin, other.m_yMin - 1,
        working.m_zMin, working.m_zMax, working.m_state ) );
      working.m_yMin = other.m_yMin;
    }
    if( other.m_yMax < working.m_yMax )
    {
      result.push_back( Cube( working.m_xMin, working.m_xMax, other.m_yMax + 1, working.m_yMax,
        working.m_zMin, working.m_zMax, working.m_state ) );
      working.m_yMax = other.m_yMax;
    }
    if( other.m_zMin > working.m_zMin )
    {
      result.push_back( Cube( working.m_xMin, working.m_xMax, working.m_yMin, working.m_yMax,
        working.m_zMin, other.m_zMin - 1, working.m_state ) );
      working.m_zMin = other.m_zMin;
    }
    if( other.m_zMax < working.m_zMax )
    {
      result.push_back( Cube( working.m_xMin, working.m_xMax, working.m_yMin, working.m_yMax,
        other.m_zMax + 1, working.m_zMax, working.m_state ) );
      working.m_zMax = other.m_zMax;
    }
    working.m_state = other.m_state;
    result.push_back( working );
  }

  return result;
}


int main()
{
  std::ifstream input( R"(..\Day22\input.txt)" );
  std::string   textLine;
  std::vector<Cube> cubes;
  std::vector<Cube> switchingCubes;

  while( std::getline( input, textLine ) )
  {
    Cube newCube( textLine );

    std::cout << "New cube:" << std::endl;
    newCube.print( std::cout );
    switchingCubes.emplace_back( newCube );
  }

  int xMin = 0;
  int xMax = 0;
  int yMin = 0;
  int yMax = 0;
  int zMin = 0;
  int zMax = 0;
  for( const auto &cube : switchingCubes )
  {
    if( cube.m_xMin < xMin )
    {
      xMin = cube.m_xMin;
    }
    if( cube.m_xMax > xMax )
    {
      xMax = cube.m_xMax;
    }
    if( cube.m_yMin < yMin )
    {
      yMin = cube.m_yMin;
    }
    if( cube.m_yMax > yMax )
    {
      yMax = cube.m_yMax;
    }
    if( cube.m_zMin < zMin )
    {
      zMin = cube.m_zMin;
    }
    if( cube.m_zMax > zMax )
    {
      zMax = cube.m_zMax;
    }
  }

  // for step 2
  Cube initialCube( xMin, xMax, yMin, yMax, zMin, zMax );
  std::cout << "Starting cube is:" << std::endl;
  initialCube.print( std::cout );
  cubes.emplace_back( initialCube );
  // for step 1
  //cubes.emplace_back( Cube( -50, 50, -50, 50, -50, 50 ) );

  for( const auto &newCube : switchingCubes )
  {
    std::vector<Cube> newCubes;

    for( const auto &cube : cubes )
    {
      auto addCubes = cube.add( newCube );
      for( const auto &addCube : addCubes )
      {
        newCubes.push_back( addCube );
      }
    }

    cubes = newCubes;
    //std::cout << std::endl;
    //std::cout << "Current status:" << std::endl;
    //for( const auto &cube : cubes )
    //{
    //  cube.print( std::cout );
    //}
    //std::cout << std::endl;
    std::cout << "Now handling " << cubes.size() << " subcubes" << std::endl;
  }

  std::uint64_t totalCount = 0;
  for( const auto &cube : cubes )
  {
    totalCount += cube.count();
  }

  std::cout << "Final cubes on : " << totalCount << std::endl;

  return 0;
}