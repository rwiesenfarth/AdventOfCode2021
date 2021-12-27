
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


struct Cave
{
  Cave() = default;
  Cave( const std::string &name )
    : m_name( name )
    , m_isSmall( name[0] >= 'a' )
    , m_isEnd( name == "end" )
  { }

  std::string m_name    = "(invalid)";
  bool        m_isSmall = false;
  bool        m_isEnd   = false;
};


struct Path
{
  Path()
  {
    m_visitedSmallCaves.insert( "start" );
    m_caves.emplace_back( "start" );
  }
  Path( const Path & ) = default;

  Path &operator=( const Path & ) = default;

  std::unordered_set<std::string> m_visitedSmallCaves;
  std::vector<std::string>        m_caves;
  std::string                     m_smallCaveTwice;
};


struct Connection
{
  Connection( const std::string &one, const std::string &two )
    : m_one( one )
    , m_two( two )
  { }

  std::string m_one;
  std::string m_two;
};


int main()
{
  std::ifstream                         input( R"(..\Day12\input.txt)" );
  std::string                           textLine;
  std::unordered_map<std::string, Cave> caves;
  std::vector<Connection>               connections;
  std::vector<Path>                     pathsToVisit;
  std::vector<Path>                     pathsToContinue;
  std::vector<Path>                     pathsComplete;

  while( std::getline( input, textLine ) )
  {
    auto pos = textLine.find( '-' );
    Cave one( textLine.substr( 0, pos ) );
    Cave two( textLine.substr( pos + 1 ) );

    caves.insert( { one.m_name, one } );
    caves.insert( { two.m_name, two } );
    if( two.m_name != "start" )
    {
      connections.emplace_back( one.m_name, two.m_name );
    }
    if( one.m_name != "start" )
    {
      connections.emplace_back( two.m_name, one.m_name );
    }
  }

  pathsToVisit.push_back( Path() );

  bool allowOneSmallCaveTwice = true;

  while( !pathsToVisit.empty() )
  {
    for( auto path : pathsToVisit )
    {
      auto caveName = path.m_caves.back();

      for( const auto &connection : connections )
      {
        auto it = path.m_visitedSmallCaves.find( connection.m_two );
        if( ( connection.m_one == caveName ) &&
          ( ( it == path.m_visitedSmallCaves.end() ) || ( allowOneSmallCaveTwice && path.m_smallCaveTwice.empty() ) ) )
        {
          const auto &cave     = caves[connection.m_two];
          auto        nextPath = path;

          if( it != path.m_visitedSmallCaves.end() )
          {
            nextPath.m_smallCaveTwice = connection.m_two;
          }
          nextPath.m_caves.push_back( connection.m_two );
          if( cave.m_isSmall )
          {
            nextPath.m_visitedSmallCaves.insert( cave.m_name );
          }
          if( cave.m_isEnd )
          {
            pathsComplete.emplace_back( nextPath );

            //for( const auto &step : nextPath.m_caves )
            //{
            //  std::cout << step << ',';
            //}
            //std::cout << std::endl;
          }
          else
          {
            pathsToContinue.emplace_back( nextPath );
          }
        }
      }
    }
    pathsToVisit = std::move( pathsToContinue );
  }

  std::cout << "Found " << pathsComplete.size() << " paths in total" << std::endl;

  return 0;
}