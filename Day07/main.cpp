
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>


static constexpr int s_maxAge = 8;
static constexpr int s_days = 256;


int fuelForPosition( const std::vector<int> &crabPositions, int targetPosition )
{
  int sum = 0;

  for( auto position : crabPositions )
  {
    sum += std::abs( targetPosition - position );
  }

  std::cout << "fuel at position " << targetPosition << ": " << sum << std::endl;

  return sum;
}


int sumupFuelForPosition( const std::vector<int> &crabPositions, int targetPosition )
{
  int sum = 0;

  for( auto position : crabPositions )
  {
    int diff = std::abs( targetPosition - position );
    sum += ( diff * ( diff + 1 ) ) / 2;
  }

  std::cout << "sumup fuel at position " << targetPosition << ": " << sum << std::endl;

  return sum;
}


void checkAllValues( const std::vector<int> crabPositions, int maxPosition )
{
  int minIndex = 0;
  int minFuel = maxPosition * crabPositions.size();

  for( int position = 0; position <= maxPosition; position++ )
  {
    int fuel = fuelForPosition( crabPositions, position );

    if( fuel < minFuel )
    {
      minIndex = position;
      minFuel = fuel;
    }
  }

  std::cout << "minimum fuel at position " << minIndex << ": " << minFuel << std::endl;
}


void checkAllSumupValues( const std::vector<int> crabPositions, int maxPosition )
{
  int minIndex = 0;
  int minFuel = maxPosition * maxPosition * crabPositions.size() / 2;

  for( int position = 0; position <= maxPosition; position++ )
  {
    int fuel = sumupFuelForPosition( crabPositions, position );

    if( fuel < minFuel )
    {
      minIndex = position;
      minFuel = fuel;
    }
  }

  std::cout << "minimum sumup fuel at position " << minIndex << ": " << minFuel << std::endl;
}


int main()
{
  std::ifstream    input( R"(..\Day07\input.txt)" );
  std::string      textLine;
  std::vector<int> crabPositions;
  int              maxPosition = 0;

  std::getline( input, textLine );
  while( true )
  {
    int position = std::stoi( textLine );

    if( position > maxPosition )
    {
      maxPosition = position;
    }
    crabPositions.push_back( position );

    auto pos = textLine.find( ',' );
    if( pos == std::string::npos )
    {
      break;
    }
    else
    {
      textLine = textLine.substr( pos + 1 );
    }
  }

  checkAllValues( crabPositions, maxPosition );

  std::sort( crabPositions.begin(), crabPositions.end() );
  size_t medianIndex = crabPositions.size() / 2;

  int medianFuel = fuelForPosition( crabPositions, crabPositions[medianIndex] );

  std::cout << "fuel at low median position " << crabPositions[medianIndex] << ": " << medianFuel << std::endl;

  medianIndex++;
  medianFuel = fuelForPosition( crabPositions, crabPositions[medianIndex] );
  std::cout << "fuel at high median position " << crabPositions[medianIndex] << ": " << medianFuel << std::endl;

  checkAllSumupValues( crabPositions, maxPosition );

  return 0;
}