
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


enum AmphipodType
{
  NoneAmphipod,
  Amber,
  Bronze,
  Copper,
  Desert,
  AnyAmphipod
};


struct Location
{
  enum Type
  {
    Hallway,
    Passage,
    Room
  };

  //Location( Type type, AmphipodType occupantType, const std::vector<int> nextRooms )
  //  : m_type( type )
  //  , m_occupantType( occupantType )
  //  , m_nextRooms( nextRooms )
  //{ }
  Location( Type type, AmphipodType occupantType, bool enableNeighbor )
    : m_type( type )
    , m_occupantType( occupantType )
    , m_enableNeighbor( enableNeighbor )
  {}

  Type         m_type;
  AmphipodType m_occupantType;
  bool         m_enableNeighbor;
  //std::vector<int> m_nextRooms;
};


struct Route
{
  Route( int start, int end, const std::vector<int> &via )
    : m_start( start )
    , m_end( end )
    , m_via( via )
  { }

  int              m_start;
  int              m_end;
  std::vector<int> m_via;
};


struct Amphipod
{
  Amphipod( AmphipodType type, int stepCost )
    : m_type( type )
    , m_stepCost( stepCost )
  { }

  AmphipodType   m_type;
  int            m_stepCost;
};


struct StateTwoFootprint
{
  StateTwoFootprint() = default;

  bool operator==( const StateTwoFootprint &other ) const
  {
    return ( m_states[0] == other.m_states[0] ) && ( m_states[1] == other.m_states[1] ) &&
      ( m_states[2] == other.m_states[2] ) && ( m_states[3] == other.m_states[3] );
  }

  void set( int index, const std::vector<int> &rooms )
  {
    std::vector<int> sortedRooms = rooms;
    int value = 0;

    std::sort( sortedRooms.begin(), sortedRooms.end() );
    m_states[index] = ( ( sortedRooms[0] * 27 + sortedRooms[1] ) * 27 + sortedRooms[2] ) * 27 + sortedRooms[3];
  }

  std::uint32_t m_states[4] = { 0, 0, 0, 0 };
};


namespace std
{
template <>
class hash <StateTwoFootprint>
{
  public:
  size_t operator()( const StateTwoFootprint &fp ) const
  {
    size_t h = ( (size_t)( fp.m_states[0] ) << 45 ) ^ ( (size_t)( fp.m_states[1] ) << 30 ) ^
      ( (size_t)( fp.m_states[2] ) << 15 ) ^ (size_t)( fp.m_states[3] );
    return  h;
  }
};
}


struct StateTwo
{
  StateTwo()
  {
    for( int i = 11; i <= 26; i++ )
    {
      m_roomOccupied[i] = true;
      m_roomEnabled[i] = ( i >= 23 );
    }
  }
  StateTwo( const StateTwo & ) = default;

  StateTwo &operator=( const StateTwo & ) = default;

  std::vector<bool> m_roomOccupied = std::vector<bool>( 27, false );
  std::vector<bool> m_roomEnabled = std::vector<bool>( 27, true );
  std::vector<bool> m_amphipodDone = std::vector<bool>( 16, false );
  std::vector<int>  m_amphipodRoom = std::vector<int>( 16, 0 );

  StateTwoFootprint footprint()
  {
    StateTwoFootprint result;

    result.set( 0, { m_amphipodRoom[0], m_amphipodRoom[1], m_amphipodRoom[2], m_amphipodRoom[3] } );
    result.set( 1, { m_amphipodRoom[4], m_amphipodRoom[5], m_amphipodRoom[6], m_amphipodRoom[7] } );
    result.set( 2, { m_amphipodRoom[8], m_amphipodRoom[9], m_amphipodRoom[10], m_amphipodRoom[11] } );
    result.set( 3, { m_amphipodRoom[12], m_amphipodRoom[13], m_amphipodRoom[14], m_amphipodRoom[15] } );
    return result;
  }
};


struct State
{
  State()
  {
    m_roomOccupied[11] = true;
    m_roomOccupied[12] = true;
    m_roomOccupied[13] = true;
    m_roomOccupied[14] = true;
    m_roomOccupied[15] = true;
    m_roomOccupied[16] = true;
    m_roomOccupied[17] = true;
    m_roomOccupied[18] = true;
    m_roomEnabled[11] = false;
    m_roomEnabled[12] = false;
    m_roomEnabled[13] = false;
    m_roomEnabled[14] = false;
  }
  State( const State & ) = default;

  State &operator=( const State & ) = default;

  std::vector<bool> m_roomOccupied = std::vector<bool>( 19, false );
  std::vector<bool> m_roomEnabled  = std::vector<bool>( 19, true );
  std::vector<bool> m_amphipodDone = std::vector<bool>( 8, false );
  std::vector<int>  m_amphipodRoom = std::vector<int>( 8, 0 );

  std::uint64_t footprint()
  {
    std::uint64_t result = 0;

    if( m_amphipodRoom[0] < m_amphipodRoom[1] )
    {
      result = ( ( result * 19 ) + m_amphipodRoom[0] ) * 19 + m_amphipodRoom[1];
    }
    else
    {
      result = ( ( result * 19 ) + m_amphipodRoom[1] ) * 19 + m_amphipodRoom[0];
    }
    if( m_amphipodRoom[2] < m_amphipodRoom[3] )
    {
      result = ( ( result * 19 ) + m_amphipodRoom[2] ) * 19 + m_amphipodRoom[3];
    }
    else
    {
      result = ( ( result * 19 ) + m_amphipodRoom[3] ) * 19 + m_amphipodRoom[2];
    }
    if( m_amphipodRoom[4] < m_amphipodRoom[5] )
    {
      result = ( ( result * 19 ) + m_amphipodRoom[4] ) * 19 + m_amphipodRoom[5];
    }
    else
    {
      result = ( ( result * 19 ) + m_amphipodRoom[5] ) * 19 + m_amphipodRoom[4];
    }
    if( m_amphipodRoom[6] < m_amphipodRoom[7] )
    {
      result = ( ( result * 19 ) + m_amphipodRoom[6] ) * 19 + m_amphipodRoom[7];
    }
    else
    {
      result = ( ( result * 19 ) + m_amphipodRoom[7] ) * 19 + m_amphipodRoom[6];
    }

    return result;
  }
};


void printState( std::uint64_t cost, const State &state )
{
  std::string line1( "#############   #############" );
  std::string line2( "#...........#   #...........#" );
  std::string line3( "###.#.#.#.###   ###.#.#.#.###" );
  std::string line4( "  #.#.#.#.#       #.#.#.#.#  " );
  std::string line5( "  #########       #########  " );

  for( int i = 0; i < 8; i++ )
  {
    char c = ( state.m_amphipodDone[i] ? 'A' : 'a' ) + i / 2;
    int pos = state.m_amphipodRoom[i];
    if( pos < 11 )
    {
      line2[pos + 1] = c;
    }
    else if( pos < 15 )
    {
      line3[2 * ( pos - 11 ) + 3] = c;
    }
    else
    {
      line4[2 * ( pos - 15 ) + 3] = c;
    }
  }
  for( int i = 11; i < 15; i++ )
  {
    if( !state.m_roomEnabled[i] )
    {
      line3[2 * ( i - 11 ) + 19] = 'X';
    }
  }

  std::cout << "State with cost " << cost << std::endl;
  std::cout << line1 << std::endl;
  std::cout << line2 << std::endl;
  std::cout << line3 << std::endl;
  std::cout << line4 << std::endl;
  std::cout << line5 << std::endl;
  std::cout << std::endl;
}


std::vector<Amphipod> setupAmphipods()
{
  std::vector<Amphipod> result;

  result.emplace_back( Amphipod( Amber, 1 ) );
  result.emplace_back( Amphipod( Amber, 1 ) );
  result.emplace_back( Amphipod( Bronze, 10 ) );
  result.emplace_back( Amphipod( Bronze, 10 ) );
  result.emplace_back( Amphipod( Copper, 100 ) );
  result.emplace_back( Amphipod( Copper, 100 ) );
  result.emplace_back( Amphipod( Desert, 1000 ) );
  result.emplace_back( Amphipod( Desert, 1000 ) );

  return result;
}

std::vector<Amphipod> setupAmphipodsTwo()
{
  std::vector<Amphipod> result;

  result.emplace_back( Amphipod( Amber, 1 ) );
  result.emplace_back( Amphipod( Amber, 1 ) );
  result.emplace_back( Amphipod( Amber, 1 ) );
  result.emplace_back( Amphipod( Amber, 1 ) );
  result.emplace_back( Amphipod( Bronze, 10 ) );
  result.emplace_back( Amphipod( Bronze, 10 ) );
  result.emplace_back( Amphipod( Bronze, 10 ) );
  result.emplace_back( Amphipod( Bronze, 10 ) );
  result.emplace_back( Amphipod( Copper, 100 ) );
  result.emplace_back( Amphipod( Copper, 100 ) );
  result.emplace_back( Amphipod( Copper, 100 ) );
  result.emplace_back( Amphipod( Copper, 100 ) );
  result.emplace_back( Amphipod( Desert, 1000 ) );
  result.emplace_back( Amphipod( Desert, 1000 ) );
  result.emplace_back( Amphipod( Desert, 1000 ) );
  result.emplace_back( Amphipod( Desert, 1000 ) );

  return result;
}

std::vector<Location> setupLocations()
{
  std::vector<Location> result;

  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Room, Amber, false ) );
  result.emplace_back( Location( Location::Room, Bronze, false ) );
  result.emplace_back( Location( Location::Room, Copper, false ) );
  result.emplace_back( Location( Location::Room, Desert, false ) );
  result.emplace_back( Location( Location::Room, Amber, true ) );
  result.emplace_back( Location( Location::Room, Bronze, true ) );
  result.emplace_back( Location( Location::Room, Copper, true ) );
  result.emplace_back( Location( Location::Room, Desert, true ) );

  return result;
}

std::vector<Location> setupLocationsTwo()
{
  std::vector<Location> result;

  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Passage, NoneAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Hallway, AnyAmphipod, false ) );
  result.emplace_back( Location( Location::Room, Amber, false ) );
  result.emplace_back( Location( Location::Room, Bronze, false ) );
  result.emplace_back( Location( Location::Room, Copper, false ) );
  result.emplace_back( Location( Location::Room, Desert, false ) );
  result.emplace_back( Location( Location::Room, Amber, false ) );
  result.emplace_back( Location( Location::Room, Bronze, false ) );
  result.emplace_back( Location( Location::Room, Copper, false ) );
  result.emplace_back( Location( Location::Room, Desert, false ) );
  result.emplace_back( Location( Location::Room, Amber, false ) );
  result.emplace_back( Location( Location::Room, Bronze, false ) );
  result.emplace_back( Location( Location::Room, Copper, false ) );
  result.emplace_back( Location( Location::Room, Desert, false ) );
  result.emplace_back( Location( Location::Room, Amber, true ) );
  result.emplace_back( Location( Location::Room, Bronze, true ) );
  result.emplace_back( Location( Location::Room, Copper, true ) );
  result.emplace_back( Location( Location::Room, Desert, true ) );

  return result;
}

std::vector<Route> setupRoutes()
{
  std::vector<Route> result;

  result.emplace_back( Route( 0, 11, { 1, 2 } ) );
  result.emplace_back( Route( 0, 15, { 1, 2, 11 } ) );
  result.emplace_back( Route( 0, 12, { 1, 2, 3, 4 } ) );
  result.emplace_back( Route( 0, 16, { 1, 2, 3, 4, 12 } ) );
  result.emplace_back( Route( 0, 13, { 1, 2, 3, 4, 5, 6 } ) );
  result.emplace_back( Route( 0, 17, { 1, 2, 3, 4, 5, 6, 13 } ) );
  result.emplace_back( Route( 0, 14, { 1, 2, 3, 4, 5, 6, 7, 8 } ) );
  result.emplace_back( Route( 0, 18, { 1, 2, 3, 4, 5, 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 1, 11, { 2 } ) );
  result.emplace_back( Route( 1, 15, { 2, 11 } ) );
  result.emplace_back( Route( 1, 12, { 2, 3, 4 } ) );
  result.emplace_back( Route( 1, 16, { 2, 3, 4, 12 } ) );
  result.emplace_back( Route( 1, 13, { 2, 3, 4, 5, 6 } ) );
  result.emplace_back( Route( 1, 17, { 2, 3, 4, 5, 6, 13 } ) );
  result.emplace_back( Route( 1, 14, { 2, 3, 4, 5, 6, 7, 8 } ) );
  result.emplace_back( Route( 1, 18, { 2, 3, 4, 5, 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 3, 11, { 2 } ) );
  result.emplace_back( Route( 3, 15, { 2, 11 } ) );
  result.emplace_back( Route( 3, 12, { 4 } ) );
  result.emplace_back( Route( 3, 16, { 4, 12 } ) );
  result.emplace_back( Route( 3, 13, { 4, 5, 6 } ) );
  result.emplace_back( Route( 3, 17, { 4, 5, 6, 13 } ) );
  result.emplace_back( Route( 3, 14, { 4, 5, 6, 7, 8 } ) );
  result.emplace_back( Route( 3, 18, { 4, 5, 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 5, 11, { 4, 3, 2 } ) );
  result.emplace_back( Route( 5, 15, { 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 5, 12, { 4 } ) );
  result.emplace_back( Route( 5, 16, { 4, 12 } ) );
  result.emplace_back( Route( 5, 13, { 6 } ) );
  result.emplace_back( Route( 5, 17, { 6, 13 } ) );
  result.emplace_back( Route( 5, 14, { 6, 7, 8 } ) );
  result.emplace_back( Route( 5, 18, { 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 7, 11, { 6, 5, 4, 3, 2 } ) );
  result.emplace_back( Route( 7, 15, { 6, 5, 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 7, 12, { 6, 5, 4 } ) );
  result.emplace_back( Route( 7, 16, { 6, 5, 4, 12 } ) );
  result.emplace_back( Route( 7, 13, { 6 } ) );
  result.emplace_back( Route( 7, 17, { 6, 13 } ) );
  result.emplace_back( Route( 7, 14, { 8 } ) );
  result.emplace_back( Route( 7, 18, { 8, 14 } ) );
  result.emplace_back( Route( 9, 11, { 8, 7, 6, 5, 4, 3, 2 } ) );
  result.emplace_back( Route( 9, 15, { 8, 7, 6, 5, 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 9, 12, { 8, 7, 6, 5, 4 } ) );
  result.emplace_back( Route( 9, 16, { 8, 7, 6, 5, 4, 12 } ) );
  result.emplace_back( Route( 9, 13, { 8, 7, 6 } ) );
  result.emplace_back( Route( 9, 17, { 8, 7, 6, 13 } ) );
  result.emplace_back( Route( 9, 14, { 8 } ) );
  result.emplace_back( Route( 9, 18, { 8, 14 } ) );
  result.emplace_back( Route( 10, 11, { 9, 8, 7, 6, 5, 4, 3, 2 } ) );
  result.emplace_back( Route( 10, 15, { 9, 8, 7, 6, 5, 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 10, 12, { 9, 8, 7, 6, 5, 4 } ) );
  result.emplace_back( Route( 10, 16, { 9, 8, 7, 6, 5, 4, 12 } ) );
  result.emplace_back( Route( 10, 13, { 9, 8, 7, 6 } ) );
  result.emplace_back( Route( 10, 17, { 9, 8, 7, 6, 13 } ) );
  result.emplace_back( Route( 10, 14, { 9, 8 } ) );
  result.emplace_back( Route( 10, 18, { 9, 8, 14 } ) );

  int currentCount = result.size();
  for( int i = 0; i < currentCount; i++ )
  {
    std::vector<int> reverseVia( result[i].m_via.size() );
    std::reverse_copy( result[i].m_via.begin(), result[i].m_via.end(), reverseVia.begin() );
    result.emplace_back( Route( result[i].m_end, result[i].m_start, reverseVia ) );
  }

  return result;
}

std::vector<Route> setupRoutesTwo()
{
  std::vector<Route> result;

  result.emplace_back( Route( 0, 11, { 1, 2 } ) );
  result.emplace_back( Route( 0, 15, { 1, 2, 11 } ) );
  result.emplace_back( Route( 0, 19, { 1, 2, 11, 15 } ) );
  result.emplace_back( Route( 0, 23, { 1, 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 0, 12, { 1, 2, 3, 4 } ) );
  result.emplace_back( Route( 0, 16, { 1, 2, 3, 4, 12 } ) );
  result.emplace_back( Route( 0, 20, { 1, 2, 3, 4, 12, 16 } ) );
  result.emplace_back( Route( 0, 24, { 1, 2, 3, 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 0, 13, { 1, 2, 3, 4, 5, 6 } ) );
  result.emplace_back( Route( 0, 17, { 1, 2, 3, 4, 5, 6, 13 } ) );
  result.emplace_back( Route( 0, 21, { 1, 2, 3, 4, 5, 6, 13, 17 } ) );
  result.emplace_back( Route( 0, 25, { 1, 2, 3, 4, 5, 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 0, 14, { 1, 2, 3, 4, 5, 6, 7, 8 } ) );
  result.emplace_back( Route( 0, 18, { 1, 2, 3, 4, 5, 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 0, 22, { 1, 2, 3, 4, 5, 6, 7, 8, 14, 18 } ) );
  result.emplace_back( Route( 0, 26, { 1, 2, 3, 4, 5, 6, 7, 8, 14, 18, 22 } ) );

  result.emplace_back( Route( 1, 11, { 2 } ) );
  result.emplace_back( Route( 1, 15, { 2, 11 } ) );
  result.emplace_back( Route( 1, 19, { 2, 11, 15 } ) );
  result.emplace_back( Route( 1, 23, { 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 1, 12, { 2, 3, 4 } ) );
  result.emplace_back( Route( 1, 16, { 2, 3, 4, 12 } ) );
  result.emplace_back( Route( 1, 20, { 2, 3, 4, 12, 16 } ) );
  result.emplace_back( Route( 1, 24, { 2, 3, 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 1, 13, { 2, 3, 4, 5, 6 } ) );
  result.emplace_back( Route( 1, 17, { 2, 3, 4, 5, 6, 13 } ) );
  result.emplace_back( Route( 1, 21, { 2, 3, 4, 5, 6, 13, 17 } ) );
  result.emplace_back( Route( 1, 25, { 2, 3, 4, 5, 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 1, 14, { 2, 3, 4, 5, 6, 7, 8 } ) );
  result.emplace_back( Route( 1, 18, { 2, 3, 4, 5, 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 1, 22, { 2, 3, 4, 5, 6, 7, 8, 14, 18 } ) );
  result.emplace_back( Route( 1, 26, { 2, 3, 4, 5, 6, 7, 8, 14, 18, 22 } ) );

  result.emplace_back( Route( 3, 11, { 2 } ) );
  result.emplace_back( Route( 3, 15, { 2, 11 } ) );
  result.emplace_back( Route( 3, 19, { 2, 11, 15 } ) );
  result.emplace_back( Route( 3, 23, { 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 3, 12, { 4 } ) );
  result.emplace_back( Route( 3, 16, { 4, 12 } ) );
  result.emplace_back( Route( 3, 20, { 4, 12, 16 } ) );
  result.emplace_back( Route( 3, 24, { 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 3, 13, { 4, 5, 6 } ) );
  result.emplace_back( Route( 3, 17, { 4, 5, 6, 13 } ) );
  result.emplace_back( Route( 3, 21, { 4, 5, 6, 13, 17 } ) );
  result.emplace_back( Route( 3, 25, { 4, 5, 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 3, 14, { 4, 5, 6, 7, 8 } ) );
  result.emplace_back( Route( 3, 18, { 4, 5, 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 3, 22, { 4, 5, 6, 7, 8, 14, 18 } ) );
  result.emplace_back( Route( 3, 26, { 4, 5, 6, 7, 8, 14, 18, 22 } ) );

  result.emplace_back( Route( 5, 11, { 4, 3, 2 } ) );
  result.emplace_back( Route( 5, 15, { 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 5, 19, { 4, 3, 2, 11, 15 } ) );
  result.emplace_back( Route( 5, 23, { 4, 3, 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 5, 12, { 4 } ) );
  result.emplace_back( Route( 5, 16, { 4, 12 } ) );
  result.emplace_back( Route( 5, 20, { 4, 12, 16 } ) );
  result.emplace_back( Route( 5, 24, { 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 5, 13, { 6 } ) );
  result.emplace_back( Route( 5, 17, { 6, 13 } ) );
  result.emplace_back( Route( 5, 21, { 6, 13, 17 } ) );
  result.emplace_back( Route( 5, 25, { 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 5, 14, { 6, 7, 8 } ) );
  result.emplace_back( Route( 5, 18, { 6, 7, 8, 14 } ) );
  result.emplace_back( Route( 5, 22, { 6, 7, 8, 14, 18 } ) );
  result.emplace_back( Route( 5, 26, { 6, 7, 8, 14, 18, 22 } ) );

  result.emplace_back( Route( 7, 11, { 6, 5, 4, 3, 2 } ) );
  result.emplace_back( Route( 7, 15, { 6, 5, 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 7, 19, { 6, 5, 4, 3, 2, 11, 15 } ) );
  result.emplace_back( Route( 7, 23, { 6, 5, 4, 3, 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 7, 12, { 6, 5, 4 } ) );
  result.emplace_back( Route( 7, 16, { 6, 5, 4, 12 } ) );
  result.emplace_back( Route( 7, 20, { 6, 5, 4, 12, 16 } ) );
  result.emplace_back( Route( 7, 24, { 6, 5, 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 7, 13, { 6 } ) );
  result.emplace_back( Route( 7, 17, { 6, 13 } ) );
  result.emplace_back( Route( 7, 21, { 6, 13, 17 } ) );
  result.emplace_back( Route( 7, 25, { 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 7, 14, { 8 } ) );
  result.emplace_back( Route( 7, 18, { 8, 14 } ) );
  result.emplace_back( Route( 7, 22, { 8, 14, 18 } ) );
  result.emplace_back( Route( 7, 26, { 8, 14, 18, 22 } ) );

  result.emplace_back( Route( 9, 11, { 8, 7, 6, 5, 4, 3, 2 } ) );
  result.emplace_back( Route( 9, 15, { 8, 7, 6, 5, 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 9, 19, { 8, 7, 6, 5, 4, 3, 2, 11, 15 } ) );
  result.emplace_back( Route( 9, 23, { 8, 7, 6, 5, 4, 3, 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 9, 12, { 8, 7, 6, 5, 4 } ) );
  result.emplace_back( Route( 9, 16, { 8, 7, 6, 5, 4, 12 } ) );
  result.emplace_back( Route( 9, 20, { 8, 7, 6, 5, 4, 12, 16 } ) );
  result.emplace_back( Route( 9, 24, { 8, 7, 6, 5, 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 9, 13, { 8, 7, 6 } ) );
  result.emplace_back( Route( 9, 17, { 8, 7, 6, 13 } ) );
  result.emplace_back( Route( 9, 21, { 8, 7, 6, 13, 17 } ) );
  result.emplace_back( Route( 9, 25, { 8, 7, 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 9, 14, { 8 } ) );
  result.emplace_back( Route( 9, 18, { 8, 14 } ) );
  result.emplace_back( Route( 9, 22, { 8, 14, 18 } ) );
  result.emplace_back( Route( 9, 26, { 8, 14, 18, 22 } ) );

  result.emplace_back( Route( 10, 11, { 9, 8, 7, 6, 5, 4, 3, 2 } ) );
  result.emplace_back( Route( 10, 15, { 9, 8, 7, 6, 5, 4, 3, 2, 11 } ) );
  result.emplace_back( Route( 10, 19, { 9, 8, 7, 6, 5, 4, 3, 2, 11, 15 } ) );
  result.emplace_back( Route( 10, 23, { 9, 8, 7, 6, 5, 4, 3, 2, 11, 15, 19 } ) );
  result.emplace_back( Route( 10, 12, { 9, 8, 7, 6, 5, 4 } ) );
  result.emplace_back( Route( 10, 16, { 9, 8, 7, 6, 5, 4, 12 } ) );
  result.emplace_back( Route( 10, 20, { 9, 8, 7, 6, 5, 4, 12, 16 } ) );
  result.emplace_back( Route( 10, 24, { 9, 8, 7, 6, 5, 4, 12, 16, 20 } ) );
  result.emplace_back( Route( 10, 13, { 9, 8, 7, 6 } ) );
  result.emplace_back( Route( 10, 17, { 9, 8, 7, 6, 13 } ) );
  result.emplace_back( Route( 10, 21, { 9, 8, 7, 6, 13, 17 } ) );
  result.emplace_back( Route( 10, 25, { 9, 8, 7, 6, 13, 17, 21 } ) );
  result.emplace_back( Route( 10, 14, { 9, 8 } ) );
  result.emplace_back( Route( 10, 18, { 9, 8, 14 } ) );
  result.emplace_back( Route( 10, 22, { 9, 8, 14, 18 } ) );
  result.emplace_back( Route( 10, 26, { 9, 8, 14, 18, 22 } ) );

  int currentCount = result.size();
  for( int i = 0; i < currentCount; i++ )
  {
    std::vector<int> reverseVia( result[i].m_via.size() );
    std::reverse_copy( result[i].m_via.begin(), result[i].m_via.end(), reverseVia.begin() );
    result.emplace_back( Route( result[i].m_end, result[i].m_start, reverseVia ) );
  }

  return result;
}


static std::vector<Amphipod> s_amphipods;
static std::vector<Location> s_locations;
static std::vector<Route>    s_routes;
static std::vector<Amphipod> s_amphipodsTwo;
static std::vector<Location> s_locationsTwo;
static std::vector<Route>    s_routesTwo;


int main()
{
  s_amphipods = setupAmphipods();
  s_locations = setupLocations();
  s_routes = setupRoutes();
  s_amphipodsTwo = setupAmphipodsTwo();
  s_locationsTwo = setupLocationsTwo();
  s_routesTwo = setupRoutesTwo();

  State initialState;
  StateTwo initialStateTwo;

  bool useExampleData = false;

  if( useExampleData )
  {
    // example:
    initialState.m_amphipodRoom[0] = 15;
    initialState.m_amphipodDone[0] = true;
    initialState.m_roomEnabled[11] = true;
    initialState.m_amphipodRoom[1] = 18;
    initialState.m_amphipodRoom[2] = 11;
    initialState.m_amphipodRoom[3] = 13;
    initialState.m_amphipodRoom[4] = 12;
    initialState.m_amphipodRoom[5] = 17;
    initialState.m_amphipodDone[5] = true;
    initialState.m_roomEnabled[13] = true;
    initialState.m_amphipodRoom[6] = 16;
    initialState.m_amphipodRoom[7] = 14;

    initialStateTwo.m_amphipodRoom[0] = 23;
    initialStateTwo.m_amphipodDone[0] = true;
    initialStateTwo.m_roomEnabled[11] = true;
    initialStateTwo.m_roomEnabled[15] = true;
    initialStateTwo.m_roomEnabled[19] = true;
    initialStateTwo.m_amphipodRoom[1] = 21;
    initialStateTwo.m_amphipodRoom[2] = 18;
    initialStateTwo.m_amphipodRoom[3] = 26;
    initialStateTwo.m_amphipodRoom[4] = 11;
    initialStateTwo.m_amphipodRoom[5] = 20;
    initialStateTwo.m_amphipodRoom[6] = 13;
    initialStateTwo.m_amphipodRoom[7] = 17;
    initialStateTwo.m_amphipodRoom[8] = 12;
    initialStateTwo.m_amphipodRoom[9] = 16;
    initialStateTwo.m_amphipodRoom[10] = 25;
    initialStateTwo.m_amphipodDone[10] = true;
    initialStateTwo.m_roomEnabled[13] = true;
    initialStateTwo.m_roomEnabled[17] = true;
    initialStateTwo.m_roomEnabled[21] = true;
    initialStateTwo.m_amphipodRoom[11] = 22;
    initialStateTwo.m_amphipodRoom[12] = 15;
    initialStateTwo.m_amphipodRoom[13] = 19;
    initialStateTwo.m_amphipodRoom[14] = 24;
    initialStateTwo.m_amphipodRoom[15] = 14;
  }
  else
  {
    // input:
    initialState.m_amphipodRoom[0] = 14;
    initialState.m_amphipodRoom[1] = 17;
    initialState.m_amphipodRoom[2] = 12;
    initialState.m_amphipodRoom[3] = 18;
    initialState.m_amphipodRoom[4] = 15;
    initialState.m_amphipodRoom[5] = 16;
    initialState.m_amphipodRoom[6] = 11;
    initialState.m_amphipodRoom[7] = 13;

    initialStateTwo.m_amphipodRoom[0] = 14;
    initialStateTwo.m_amphipodRoom[1] = 18;
    initialStateTwo.m_amphipodRoom[2] = 21;
    initialStateTwo.m_amphipodRoom[3] = 25;
    initialStateTwo.m_amphipodRoom[4] = 12;
    initialStateTwo.m_amphipodRoom[5] = 20;
    initialStateTwo.m_amphipodRoom[6] = 17;
    initialStateTwo.m_amphipodRoom[7] = 26;
    initialStateTwo.m_amphipodRoom[8] = 23;
    initialStateTwo.m_amphipodRoom[9] = 16;
    initialStateTwo.m_amphipodRoom[10] = 24;
    initialStateTwo.m_amphipodRoom[11] = 22;
    initialStateTwo.m_amphipodRoom[12] = 11;
    initialStateTwo.m_amphipodRoom[13] = 15;
    initialStateTwo.m_amphipodRoom[14] = 19;
    initialStateTwo.m_amphipodRoom[15] = 13;
  }

  std::multimap<std::uint64_t, State> openStates;
  std::unordered_map<std::uint64_t, std::uint64_t> knownStates;
  std::uint64_t finalCost = 0;
  std::uint64_t steps = 0;

  std::multimap<std::uint64_t, StateTwo> openStatesTwo;
  std::unordered_map<StateTwoFootprint, std::uint64_t> knownStatesTwo;
  std::uint64_t finalCostTwo = 0;
  std::uint64_t stepsTwo = 0;

  openStates.insert( { 0, initialState } );
  knownStates.insert( { initialState.footprint(), 0 } );

  while( !openStates.empty() )
  {
    auto it = openStates.begin();
    auto [cost, state] = *it;
    openStates.erase( it );

    //if( ( ++steps % 20 ) == 0 )
    //{
    //  std::cout << "Current state after " << steps << " steps:" << std::endl;
    //  printState( cost, state );
    //}
    if( ++steps % 100000 == 0 )
    {
      std::cout << "Current state after " << steps << " steps:" << std::endl;
      printState( cost, state );
    }

    bool done = true;
    for( int i = 0; i < 8; i++ )
    {
      if( state.m_amphipodDone[i] )
      {
        continue;
      }
      done = false;

      for( const auto &route : s_routes )
      {
        if( ( route.m_start != state.m_amphipodRoom[i] ) ||
          state.m_roomOccupied[route.m_end] || !state.m_roomEnabled[route.m_end] )
        {
          continue;
        }
        if( ( s_locations[route.m_end].m_occupantType != AnyAmphipod ) &&
          ( s_locations[route.m_end].m_occupantType != s_amphipods[i].m_type ) )
        {
          continue;
        }
        bool routeBlocked = false;
        for( auto waypoint : route.m_via )
        {
          routeBlocked = routeBlocked || state.m_roomOccupied[waypoint];
        }
        if( routeBlocked )
        {
          continue;
        }

        // move Amphipod
        std::uint64_t nextCost = cost + ( route.m_via.size() + 1 ) * s_amphipods[i].m_stepCost;
        State nextState( state );

        nextState.m_roomOccupied[state.m_amphipodRoom[i]] = false;
        nextState.m_roomOccupied[route.m_end] = true;
        nextState.m_amphipodRoom[i] = route.m_end;
        nextState.m_amphipodDone[i] = ( s_locations[route.m_end].m_occupantType == s_amphipods[i].m_type );

        if( s_locations[route.m_end].m_enableNeighbor )
        {
          nextState.m_roomEnabled[route.m_end - 4] = true;
        }

        //printState( nextCost, nextState );
        auto it = knownStates.find( nextState.footprint() );
        if( ( it == knownStates.end() ) || ( it->second > nextCost ) )
        {
          openStates.insert( { nextCost, nextState } );
          knownStates[nextState.footprint()] = nextCost;
        }
      }
    }
    if( done )
    {
      finalCost = cost;
      break;
    }
  }

  openStatesTwo.insert( { 0, initialStateTwo } );
  knownStatesTwo.insert( { initialStateTwo.footprint(), 0 } );

  while( !openStatesTwo.empty() )
  {
    auto it = openStatesTwo.begin();
    auto [cost, state] = *it;
    openStatesTwo.erase( it );

    //if( ( ++steps % 20 ) == 0 )
    //{
    //  std::cout << "Current state after " << steps << " steps:" << std::endl;
    //  printState( cost, state );
    //}
    if( ++steps % 100000 == 0 )
    {
      std::cout << "Current cost after " << steps << " steps: " << cost << std::endl;
    }

    bool done = true;
    for( int i = 0; i < 16; i++ )
    {
      if( state.m_amphipodDone[i] )
      {
        continue;
      }
      done = false;

      for( const auto &route : s_routesTwo )
      {
        if( ( route.m_start != state.m_amphipodRoom[i] ) ||
          state.m_roomOccupied[route.m_end] || !state.m_roomEnabled[route.m_end] )
        {
          continue;
        }
        if( ( s_locationsTwo[route.m_end].m_occupantType != AnyAmphipod ) &&
          ( s_locationsTwo[route.m_end].m_occupantType != s_amphipodsTwo[i].m_type ) )
        {
          continue;
        }

        // do not move to a slot if the next one is still free
        if( ( route.m_end > 10 ) && ( route.m_end < 19 ) && ( !state.m_roomOccupied[route.m_end + 4] ) )
        {
          continue;
        }
        if( ( route.m_end > 10 ) && ( route.m_end < 15 ) && ( !state.m_roomOccupied[route.m_end + 8] ) )
        {
          continue;
        }

        // do not cross occupied slots
        bool routeBlocked = false;
        for( auto waypoint : route.m_via )
        {
          routeBlocked = routeBlocked || state.m_roomOccupied[waypoint];
        }
        if( routeBlocked )
        {
          continue;
        }

        // move Amphipod
        std::uint64_t nextCost = cost + ( route.m_via.size() + 1 ) * s_amphipodsTwo[i].m_stepCost;
        StateTwo nextState( state );

        nextState.m_roomOccupied[state.m_amphipodRoom[i]] = false;
        nextState.m_roomOccupied[route.m_end] = true;
        nextState.m_amphipodRoom[i] = route.m_end;
        nextState.m_amphipodDone[i] = ( s_locationsTwo[route.m_end].m_occupantType == s_amphipodsTwo[i].m_type );

        if( s_locationsTwo[route.m_end].m_enableNeighbor )
        {
          nextState.m_roomEnabled[route.m_end - 4] = true;
          nextState.m_roomEnabled[route.m_end - 8] = true;
          nextState.m_roomEnabled[route.m_end - 12] = true;
        }

        //printState( nextCost, nextState );
        auto it = knownStatesTwo.find( nextState.footprint() );
        if( ( it == knownStatesTwo.end() ) || ( it->second > nextCost ) )
        {
          openStatesTwo.insert( { nextCost, nextState } );
          knownStatesTwo[nextState.footprint()] = nextCost;
        }
      }
    }
    if( done )
    {
      finalCostTwo = cost;
      break;
    }
  }

  std::cout << "Minimum cost of part one is " << finalCost << std::endl;
  std::cout << "Minimum cost of part two is " << finalCostTwo << std::endl;

  return 0;
}