
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


struct Orientation
{
  Orientation() = default;

  enum Axis { UAxis, VAxis, WAxis };

  Axis m_xAxis = UAxis;
  Axis m_yAxis = VAxis;
  Axis m_zAxis = WAxis;
  bool m_xPositive = true;
  bool m_yPositive = true;
  bool m_zPositive = true;

  static const char axisChar( Axis axis )
  {
    return ( axis == UAxis ) ? 'U' : ( axis == VAxis ) ? 'V' : 'W';
  }

  static const std::vector<Orientation> &permutations()
  {
    static std::vector<Orientation> s_permutations;

    if( s_permutations.empty() )
    {
      std::vector<Orientation> v1;

      Orientation orientation;
      orientation.m_xAxis = UAxis;
      orientation.m_yAxis = VAxis;
      orientation.m_zAxis = WAxis;
      v1.push_back( orientation );
      orientation.m_yAxis = WAxis;
      orientation.m_zAxis = VAxis;
      v1.push_back( orientation );
      orientation.m_xAxis = VAxis;
      orientation.m_yAxis = UAxis;
      orientation.m_zAxis = WAxis;
      v1.push_back( orientation );
      orientation.m_yAxis = WAxis;
      orientation.m_zAxis = UAxis;
      v1.push_back( orientation );
      orientation.m_xAxis = WAxis;
      orientation.m_yAxis = UAxis;
      orientation.m_zAxis = VAxis;
      v1.push_back( orientation );
      orientation.m_yAxis = VAxis;
      orientation.m_zAxis = UAxis;
      v1.push_back( orientation );

      for( auto orientation : v1 )
      {
        orientation.m_xPositive = true;
        orientation.m_yPositive = true;
        orientation.m_zPositive = true;
        s_permutations.push_back( orientation );
        orientation.m_xPositive = true;
        orientation.m_yPositive = false;
        orientation.m_zPositive = false;
        s_permutations.push_back( orientation );
        orientation.m_xPositive = false;
        orientation.m_yPositive = true;
        orientation.m_zPositive = false;
        s_permutations.push_back( orientation );
        orientation.m_xPositive = false;
        orientation.m_yPositive = false;
        orientation.m_zPositive = true;
        s_permutations.push_back( orientation );

        orientation.m_xPositive = false;
        orientation.m_yPositive = true;
        orientation.m_zPositive = true;
        s_permutations.push_back( orientation );
        orientation.m_xPositive = true;
        orientation.m_yPositive = false;
        orientation.m_zPositive = true;
        s_permutations.push_back( orientation );
        orientation.m_xPositive = true;
        orientation.m_yPositive = true;
        orientation.m_zPositive = false;
        s_permutations.push_back( orientation );
        orientation.m_xPositive = false;
        orientation.m_yPositive = false;
        orientation.m_zPositive = false;
        s_permutations.push_back( orientation );
      }
    }
    return s_permutations;
  }
};


struct InteriorPosition
{
  InteriorPosition() = default;
  InteriorPosition( int u, int v, int w )
    : m_u( u )
    , m_v( v )
    , m_w( w )
  {}

  int m_u = 0;
  int m_v = 0;
  int m_w = 0;
};


struct ExteriorPosition
{
  ExteriorPosition() = default;
  ExteriorPosition( const ExteriorPosition & ) = default;
  ExteriorPosition( int x, int y, int z )
    : m_x( x )
    , m_y( y )
    , m_z( z )
  { }
  ExteriorPosition( const InteriorPosition &interior, const Orientation &orientation )
    : ExteriorPosition()
  {
    m_x = ( orientation.m_xAxis == Orientation::UAxis ) ? interior.m_u :
      ( orientation.m_xAxis == Orientation::VAxis ) ? interior.m_v : interior.m_w;
    m_y = ( orientation.m_yAxis == Orientation::UAxis ) ? interior.m_u :
      ( orientation.m_yAxis == Orientation::VAxis ) ? interior.m_v : interior.m_w;
    m_z = ( orientation.m_zAxis == Orientation::UAxis ) ? interior.m_u :
      ( orientation.m_zAxis == Orientation::VAxis ) ? interior.m_v : interior.m_w;
    if( !orientation.m_xPositive )
    {
      m_x = -m_x;
    }
    if( !orientation.m_yPositive )
    {
      m_y = -m_y;
    }
    if( !orientation.m_zPositive )
    {
      m_z = -m_z;
    }
  }

  ExteriorPosition &operator=( const ExteriorPosition & ) = default;

  bool operator==( const ExteriorPosition &other ) const
  {
    return ( m_x == other.m_x ) && ( m_y == other.m_y ) && ( m_z == other.m_z );
  }

  size_t hash() const
  {
    return ( (size_t)( m_x + 65536 ) << 40 ) + ( (size_t)( m_y + 65536 ) << 20 ) + (size_t)( m_z + 65536 );
  }

  int m_x = 0;
  int m_y = 0;
  int m_z = 0;
};


ExteriorPosition operator+( const ExteriorPosition &pos1, const ExteriorPosition &pos2 )
{
  ExteriorPosition result( pos1 );
  result.m_x += pos2.m_x;
  result.m_y += pos2.m_y;
  result.m_z += pos2.m_z;
  return result;
}


ExteriorPosition operator-( const ExteriorPosition &pos )
{
  ExteriorPosition result;
  result.m_x = -pos.m_x;
  result.m_y = -pos.m_y;
  result.m_z = -pos.m_z;
  return result;
}

std::ostream &operator<<( std::ostream &stream, const Orientation &ori )
{
  stream << "[ " << ( ori.m_xPositive ? '+' : '-' ) << ori.axisChar( ori.m_xAxis ) << " ; " <<
    ( ori.m_yPositive ? '+' : '-' ) << ori.axisChar( ori.m_yAxis ) << " ; " <<
    ( ori.m_zPositive ? '+' : '-' ) << ori.axisChar( ori.m_zAxis ) << " ]";
  return stream;
}


std::ostream &operator<<( std::ostream &stream, const ExteriorPosition &extPos )
{
  stream << "( X = " << extPos.m_x << " ; Y = " << extPos.m_y << " ; Z = " << extPos.m_z << " )";
  return stream;
}


std::ostream &operator<<( std::ostream &stream, const InteriorPosition &intPos )
{
  stream << "( U = " << intPos.m_u << " ; V = " << intPos.m_v << " ; W = " << intPos.m_w << " )";
  return stream;
}


struct ExteriorDistance
{
  ExteriorDistance() = default;
  ExteriorDistance( const ExteriorDistance & ) = default;
  ExteriorDistance( const ExteriorPosition &p1, const ExteriorPosition &p2 )
    : m_dx( p2.m_x - p1.m_x )
    , m_dy( p2.m_y - p1.m_y )
    , m_dz( p2.m_z - p1.m_z )
  { }

  ExteriorDistance &operator=( const ExteriorDistance & ) = default;

  size_t hash() const
  {
    return ( (size_t)( m_dx + 65536 ) << 40 ) + ( (size_t)( m_dy + 65536 ) << 20 ) + (size_t)( m_dz + 65536 );
  }

  int m_dx = 0;
  int m_dy = 0;
  int m_dz = 0;
};


std::ostream &operator<<( std::ostream &stream, const ExteriorDistance &dist )
{
  stream << "( dx = " << dist.m_dx << " ; dy = " << dist.m_dy << " ; dz = " << dist.m_dz << " )";
  return stream;
}


ExteriorPosition operator+( const ExteriorPosition &pos, const ExteriorDistance &dist )
{
  ExteriorPosition result( pos );
  result.m_x += dist.m_dx;
  result.m_y += dist.m_dy;
  result.m_z += dist.m_dz;
  return result;
}



namespace std
{
template <>
class hash <ExteriorPosition>
{
  public:
  size_t operator()( const ExteriorPosition &position ) const
  {
    return position.hash();
  }
};


template <>
class hash <ExteriorDistance>
{
  public:
  size_t operator()( const ExteriorDistance &distance ) const
  {
    return distance.hash();
  }
};
}


struct Scanner
{
  Scanner() = default;
  Scanner( const Scanner & ) = default;

  Scanner &operator=( const Scanner & ) = default;

  bool                          m_oriented = false;
  Orientation                   m_orientation;
  ExteriorPosition              m_position;
  std::vector<InteriorPosition> m_beaconPositions;

  std::unordered_map<std::size_t, std::pair<ExteriorPosition, ExteriorPosition>> beaconDistances() const
  {
    std::vector<ExteriorPosition> positions;

    for( const auto &pos : m_beaconPositions )
    {
      positions.emplace_back( ExteriorPosition( pos, m_orientation ) );
    }

    std::unordered_map<std::size_t, std::pair<ExteriorPosition, ExteriorPosition>> result;

    for( int i = 0; i < positions.size(); i++ )
    {
      for( int j = 0; j < positions.size(); j++ )
      {
        if( i == j )
        {
          continue;
        }
        result.insert( { ExteriorDistance( positions[i], positions[j] ).hash() , { positions[i], positions[j] } } );
      }
    }
    return result;
  }

  int matches( const Scanner &other ) const
  {
    auto hashesOne = beaconDistances();
    auto hashesTwo = other.beaconDistances();
    int result = 0;

    for( const auto &[hash, _] : hashesOne )
    {
      if( hashesTwo.find( hash ) != hashesTwo.end() )
      {
        result++;
      }
    }
    return result;
  }

  bool estimatePosition( const Scanner &other )
  {
    std::unordered_map<size_t, ExteriorDistance> distances;
    std::unordered_map<size_t, int> counts;

    for( const auto &left : other.m_beaconPositions )
    {
      auto p1 = other.m_position + ExteriorPosition( left, other.m_orientation );
      for( const auto &right : m_beaconPositions )
      {
        auto p2 = m_position + ExteriorPosition( right, m_orientation );
        auto d = ExteriorDistance( p1, p2 );

        distances[d.hash()] = d;
        counts[d.hash()]++;
      }
    }

    //std::cout << "found " << distances.size() << " different distances" << std::endl;

    ExteriorDistance maxDist;
    int maxCount = 0;
    for( const auto &[hash, count] : counts )
    {
      if( count > maxCount )
      {
        maxCount = count;
        maxDist = distances[hash];
      }
    }

    //std::cout << "maximum is " << maxDist << " with a count of " << maxCount << std::endl;
    if( maxCount < 12 )
    {
      return false;
    }

    m_position = ExteriorPosition( -maxDist.m_dx, -maxDist.m_dy, -maxDist.m_dz );
    m_oriented = true;
    return true;
  }

  //bool estimatePosition( const Scanner &other )
  //{
  //  auto hashesOne = beaconDistances();
  //  auto hashesTwo = other.beaconDistances();
  //  std::unordered_map<size_t, ExteriorPosition> positions;
  //  std::unordered_map<size_t, int> counts;
  //  bool result = false;

  //  for( const auto &[hash, positionPair] : hashesOne )
  //  {
  //    //for( auto it = hashesTwo.find( hash ); it != hashesTwo.end(); ++it )
  //    //{
  //    //  auto p1 = positionPair.first;
  //    //  auto p2 = it->second.first;

  //    //  ////auto pos = other.m_position + ExteriorDistance( p2, p1 );
  //    //  //auto pos = -( other.m_position + ExteriorDistance( p2, p1 ) );
  //    //  auto pos = -( -other.m_position + ExteriorDistance( p2, p1 ) );
  //    //  positions[pos.hash()] = pos;
  //    //  counts[pos.hash()]++;
  //    //}
  //    auto it = hashesTwo.find( hash );
  //    if( it != hashesTwo.end() )
  //    {
  //      auto p1 = positionPair.first;
  //      auto p2 = it->second.first;

  //      ////auto pos = other.m_position + ExteriorDistance( p2, p1 );
  //      //auto pos = -( other.m_position + ExteriorDistance( p2, p1 ) );
  //      auto pos = -( -other.m_position + ExteriorDistance( p2, p1 ) );
  //      positions[pos.hash()] = pos;
  //      counts[pos.hash()]++;
  //    }
  //  }
  //  int maxCount = 0;
  //  size_t maxHash = 0;
  //  for( const auto &[hash, count] : counts )
  //  {
  //    //std::cout << "  estimated position " << positions[hash] << " with count " << count << std::endl;
  //    if( count > maxCount )
  //    {
  //      maxCount = count;
  //      maxHash = hash;
  //    }
  //  }
  //  std::cout << "  final position " << positions[maxHash] << " with count " << maxCount << std::endl;
  //  if( maxCount > 0 )
  //  {
  //    m_position = positions[maxHash];
  //    result = true;
  //  }
  //  return result;
  //}

  void addBeacons( std::unordered_map<std::size_t, ExteriorPosition> &beacons ) const
  {
    if( !m_oriented )
    {
      return;
    }

    for( const auto &beacon : m_beaconPositions )
    {
      auto pos = m_position + ExteriorPosition( beacon, m_orientation );
      beacons[pos.hash()] = pos;
    }
  }
};


std::ostream &operator<<( std::ostream &stream, const Scanner &scanner )
{
  stream << "Scanner at ";
  if( scanner.m_oriented )
  {
    stream << scanner.m_position << " with orientation " << scanner.m_orientation;
  }
  else
  {
    stream << "unknown position";
  }
  stream << " with " << scanner.m_beaconPositions.size() << " beacon positions";
  for( const auto &position : scanner.m_beaconPositions )
  {
    std::cout << std::endl << "    " << position << " -> " <<
      scanner.m_position + ExteriorPosition( position, scanner.m_orientation );
  }
  return stream;
}



bool addToNetwork( const Scanner &scannerOne, Scanner &scannerTwo,
  std::unordered_map<std::size_t, ExteriorPosition> &beacons )
{
  int     maxCount = 0;
  Scanner bestCandidate( scannerTwo );
  bool    positionFound = false;

  for( const auto &ori : Orientation::permutations() )
  {
    Scanner candidate( scannerTwo );
    candidate.m_orientation = ori;
    auto count = scannerOne.matches( candidate );
    //if( count > 0 )
    //{
    //  std::cout << "orientation " << ori << " results in " << count << " matches" << std::endl;
    //}
    if( count >= maxCount )
    {
      maxCount = count;
      bestCandidate.m_orientation = ori;

      if( bestCandidate.estimatePosition( scannerOne ) )
      {
        positionFound = true;
        break;
      }
    }
  }
  if( maxCount < 12 )
  {
    return false;
  }

  //std::cout << "Best candidate is " << bestCandidate << std::endl;

  //if( !bestCandidate.estimatePosition( scannerOne ) )
  if( !positionFound )
  {
    return false;
  }

  bool failedBeacon = false;
  for( const auto &[_, position] : beacons )
  {
    ExteriorDistance dist( position, bestCandidate.m_position );
    if( ( dist.m_dx < -1000 ) || ( dist.m_dx > 1000 ) || ( dist.m_dy < -1000 ) || ( dist.m_dy > 1000 ) ||
      ( dist.m_dz < -1000 ) || ( dist.m_dz > 1000 ) )
    {
      continue;
    }
    //std::cout << "searching beacon " << position << std::endl;

    bool found = false;
    for( const auto &beacon : bestCandidate.m_beaconPositions )
    {
      auto pos = bestCandidate.m_position + ExteriorPosition( beacon, bestCandidate.m_orientation );
      //std::cout << "  testing " << beacon << " -> " << pos << std::endl;
      if( pos == position )
      {
        //std::cout << "    found" << std::endl;
        found = true;
        break;
      }
    }
    if( !found )
    {
      //std::cout << "FAILED" << std::endl;
      failedBeacon = true;
      break;
    }
  }

  if( failedBeacon )
  {
    return false;
  }

  bestCandidate.m_oriented = true;

  for( const auto &beacon : bestCandidate.m_beaconPositions )
  {
    auto pos = bestCandidate.m_position + ExteriorPosition( beacon, bestCandidate.m_orientation );
    beacons[pos.hash()] = pos;
  }

  scannerTwo = bestCandidate;

  //std::cout << "Added scanner " << scannerTwo << std::endl;
  std::cout << "Added scanner at position " << scannerTwo.m_position << std::endl;
  std::cout << "Number of beacons is now " << beacons.size() << std::endl;
  return true;
}


int main()
{
  //InteriorPosition testInterior( 2, 4, 8 );

  //for( const auto &ori : Orientation::permutations() )
  //{
  //  ExteriorPosition ext( testInterior, ori );

  //  std::cout << ori << " : " << ext << std::endl;
  //}

  std::ifstream input( R"(D:\Develop\AdventOfCode2021\Day19\input.txt)" );
  std::string   textLine;
  std::vector<Scanner> scanners;
  Scanner currentScanner;
  std::unordered_map<std::size_t, ExteriorPosition> beacons;

  while( std::getline( input, textLine ) )
  {
    if( textLine.empty() )
    {
      scanners.push_back( currentScanner );
      continue;
    }
    if( textLine.substr( 0, 3 ) == "---" )
    {
      currentScanner = Scanner();
      continue;
    }
    InteriorPosition position;
    position.m_u = std::stoi( textLine );
    auto pos = textLine.find( ',' );
    position.m_v = std::stoi( textLine.substr( pos + 1 ) );
    pos = textLine.find( ',', pos + 1 );
    position.m_w = std::stoi( textLine.substr( pos + 1 ) );
    currentScanner.m_beaconPositions.emplace_back( position );
  }
  scanners.push_back( currentScanner );

  scanners[0].m_oriented = true;
  scanners[0].addBeacons( beacons );

  //for( const auto &[_, pos] : beacons )
  //{
  //  std::cout << "beacon at " << pos << std::endl;
  //}

  //for( const auto &scanner : scanners )
  //{
  //  std::cout << scanner << std::endl;
  //}

  bool didChanges;
  do
  {
    didChanges = false;
    for( int i = 0; i < scanners.size(); i++ )
    {
      if( !scanners[i].m_oriented )
      {
        continue;
      }
      for( int j = 0; j < scanners.size(); j++ )
      {
        if( scanners[j].m_oriented )
        {
          continue;
        }
        if( addToNetwork( scanners[i], scanners[j], beacons ) )
        {
          didChanges = true;
        }
        else
        {
          std::cout << "addToNetwork( " << i << ", " << j << ",...) did not succeed" << std::endl;
        }
      }
    }
  }
  while( didChanges );

  //if( addToNetwork( scanners[0], scanners[1], beacons ) )
  //{
  //  std::cout << "Added to network: " << scanners[1] << std::endl;
  //}

  std::cout << "Scanner positions:" << std::endl;
  for( const auto &scanner : scanners )
  {
    std::cout << "  " << scanner.m_position << std::endl;
  }

  int maxDist = 0;
  for( int i = 0; i < scanners.size(); i++ )
  {
    auto p1 = scanners[i].m_position;
    for( int j = 0; j < scanners.size(); j++ )
    {
      auto p2 = scanners[j].m_position;
      int dist = abs( p1.m_x - p2.m_x ) + abs( p1.m_y - p2.m_y ) + abs( p1.m_z - p2.m_z );
      if( dist > maxDist )
      {
        maxDist = dist;
      }
    }
  }

  std::cout << "Maximum manhattan distance is " << maxDist << std::endl;

  return 0;
}