
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <queue>


struct Position
{
  Position( int row, int col )
    : m_row( row )
    , m_col( col )
  {}

  int m_row;
  int m_col;
};


struct Basin
{
  Basin( int row, int col )
    : m_pos( row, col )
  {}

  void calculateSize( const std::vector<std::vector<int>> &heights )
  {
    int numRows = heights.size();
    int numCols = heights[0].size();

    std::vector<std::vector<bool>> visited( numRows, std::vector<bool>( numCols, false ) );
    std::queue<Position> toVisit;
    int size = 0;

    toVisit.push( m_pos );
    while( !toVisit.empty() )
    {
      auto pos = toVisit.front();
      toVisit.pop();
      if( visited[pos.m_row][pos.m_col] )
      {
        continue;
      }

      visited[pos.m_row][pos.m_col] = true;
      size++;

      if( ( pos.m_row > 0 ) && ( heights[pos.m_row - 1][pos.m_col] < 9 ) && !visited[pos.m_row - 1][pos.m_col] )
      {
        toVisit.push( Position( pos.m_row - 1, pos.m_col ) );
      }
      if( ( pos.m_row < numRows - 1 ) && ( heights[pos.m_row + 1][pos.m_col] < 9 ) && !visited[pos.m_row + 1][pos.m_col] )
      {
        toVisit.push( Position( pos.m_row + 1, pos.m_col ) );
      }
      if( ( pos.m_col > 0 ) && ( heights[pos.m_row][pos.m_col - 1] < 9 ) && !visited[pos.m_row][pos.m_col - 1] )
      {
        toVisit.push( Position( pos.m_row, pos.m_col - 1 ) );
      }
      if( ( pos.m_col < numCols - 1 ) && ( heights[pos.m_row][pos.m_col + 1] < 9 ) && !visited[pos.m_row][pos.m_col + 1] )
      {
        toVisit.push( Position( pos.m_row, pos.m_col + 1 ) );
      }
    }

    //std::cout << "Basin at ( " << m_pos.m_col << " ; " << m_pos.m_row << " ) has size " << size << std::endl;

    m_size = size;
  }

  Position m_pos;
  int      m_size = 0;
};


int main()
{
  std::ifstream                  input( R"(..\Day09\input.txt)" );
  std::string                    textLine;
  std::vector<std::vector<int>>  heights;
  std::vector<std::vector<bool>> isMinimum;

  while( std::getline( input, textLine ) )
  {
    std::vector<int>  profile;
    std::vector<bool> isMinProfile;

    for( auto pos : textLine )
    {
      profile.push_back( pos - '0' );
      isMinProfile.push_back( true );
    }
    heights.emplace_back( profile );
    isMinimum.emplace_back( isMinProfile );
  }

  int numRows = heights.size();
  int numCols = heights[0].size();

  for( int row = 0; row < numRows - 1; row++ )
  {
    for( int col = 0; col < numCols; col++ )
    {
      if( heights[row + 1][col] <= heights[row][col] )
      {
        isMinimum[row][col] = false;
      }
      if( heights[row][col] <= heights[row + 1][col] )
      {
        isMinimum[row + 1][col] = false;
      }
    }
  }
  for( int row = 0; row < numRows; row++ )
  {
    for( int col = 0; col < numCols - 1; col++ )
    {
      if( heights[row][col + 1] <= heights[row][col] )
      {
        isMinimum[row][col] = false;
      }
      if( heights[row][col] <= heights[row][col + 1] )
      {
        isMinimum[row][col + 1] = false;
      }
    }
  }
  
  std::vector<Basin> foundBasins;

  int minimumCount = 0;
  int minimumSum = 0;
  for( int row = 0; row < numRows; row++ )
  {
    for( int col = 0; col < numCols; col++ )
    {
      if( isMinimum[row][col] )
      {
        minimumCount++;
        minimumSum += heights[row][col] + 1;
        foundBasins.emplace_back( Basin( row, col ) );
      }
    }
  }

  for( auto &basin : foundBasins )
  {
    basin.calculateSize( heights );
  }

  std::sort( foundBasins.begin(), foundBasins.end(), []( const Basin &a, const Basin &b ) {
    return ( a.m_size > b.m_size );
  } );

  int multipliedSizes = foundBasins[0].m_size * foundBasins[1].m_size * foundBasins[2].m_size;

  //for( auto &basin : foundBasins )
  //{
  //  std::cout << "Basin at ( " << basin.m_pos.m_col << " ; " << basin.m_pos.m_row << " ) has size " << basin.m_size << std::endl;
  //}

  std::cout << "Found " << minimumCount << " minima with a total sum of " << minimumSum << std::endl;
  std::cout << "Multiplied sizes of largest three basins is " << multipliedSizes << std::endl;

  return 0;
}