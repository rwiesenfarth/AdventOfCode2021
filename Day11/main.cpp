
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_map>
#include <queue>
#include <stack>


int doStep( std::vector<std::vector<int>> &octopuses )
{
  int  numRows       = octopuses.size();
  int  numCols       = octopuses[0].size();
  int  flashes       = 0;
  bool changeOccured = false;

  for( auto &octopusRow : octopuses )
  {
    for( auto &octopus : octopusRow )
    {
      octopus++;
    }
  }

  do
  {
    changeOccured = false;
    for( int row = 0; row < numRows; row++ )
    {
      for( int col = 0; col < numCols; col++ )
      {
        if( octopuses[row][col] > 9 )
        {
          changeOccured = true;
          flashes++;
          octopuses[row][col] = 0;
          for( int r = row - 1; r <= row + 1; r++ )
          {
            if( ( r < 0 ) || ( r >= numRows ) )
            {
              continue;
            }

            for( int c = col - 1; c <= col + 1; c++ )
            {
              if( ( c < 0 ) || ( c >= numCols ) )
              {
                continue;
              }
              if( octopuses[r][c] > 0 )
              {
                octopuses[r][c]++;
              }
            }
          }
        }
      }
    }
  }
  while( changeOccured );

  return flashes;
}


bool allZero( const std::vector<std::vector<int>> &octopuses )
{
  for( const auto &octopusRow : octopuses )
  {
    for( const auto &octopus : octopusRow )
    {
      if( octopus > 0 )
      {
        return false;
      }
    }
  }
  return true;
}


void dumpOctopuses( const std::vector<std::vector<int>> &octopuses )
{
  for( const auto &octopusRow : octopuses )
  {
    for( const auto &octopus : octopusRow )
    {
      std::cout << octopus;
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


int main()
{
  std::ifstream                 input( R"(D:\Develop\AdventOfCode2021\Day11\input.txt)" );
  std::string                   textLine;
  std::vector<std::vector<int>> octopuses;

  while( std::getline( input, textLine ) )
  {
    std::vector<int> octopusRow;

    for( auto pos : textLine )
    {
      octopusRow.push_back( pos - '0' );
    }
    octopuses.emplace_back( octopusRow );
  }

  int totalFlashes = 0;
  int step = 0;
  int numSteps = 100;
  bool allZeroFound = false;

  //std::cout << "Before any steps:" << std::endl;
  //dumpOctopuses( octopuses );
  do
  {
    int flashesInStep = doStep( octopuses );
    //if( ( step >= 192 ) && ( step < 196 ) )
    //{
    //  std::cout << "Aftes step " << step + 1 << ":" << std::endl;
    //  dumpOctopuses( octopuses );
    //}
    if( step < numSteps )
    {
      totalFlashes += flashesInStep;
    }

    step++;
    allZeroFound = allZero( octopuses );
  }
  while( !allZeroFound );

  std::cout << "After " << numSteps << " steps there were " << totalFlashes << " flashes" << std::endl;
  std::cout << "First all-zero step was at step " << step << std::endl;

  return 0;
}