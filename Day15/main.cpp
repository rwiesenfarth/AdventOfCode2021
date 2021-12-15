
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


struct GridPosition
{
  GridPosition( int cost )
    : m_cost( cost )
  { }

  enum Direction
  {
    Unknown, Top, Left, Right, Bottom, End
  };

  int       m_cost;
  Direction m_direction = Unknown;
  int       m_pathCost = 0;
};


struct PathPoint
{
  int m_row = 0;
  int m_col = 0;
};


std::vector<std::vector<GridPosition>> growGrid( const std::vector<std::vector<GridPosition>> &grid )
{
  auto numRowsIn = grid.size();
  auto numColsIn = grid[0].size();
  auto numRowsOut = 5 * numRowsIn;
  auto numColsOut = 5 * numColsIn;

  std::vector<std::vector<GridPosition>> result( numRowsOut, std::vector<GridPosition>( numColsOut, GridPosition( 0 ) ) );

  for( int row = 0; row < numRowsOut; row++ )
  {
    auto inRow = row % numRowsIn;
    auto addRow = row / numRowsIn;

    for( int col = 0; col < numColsOut; col++ )
    {
      auto inCol = col % numColsIn;
      auto addCol = col / numColsIn;
      auto newCost = grid[inRow][inCol].m_cost + addRow + addCol;

      result[row][col].m_cost = ( newCost <= 9 ) ? newCost : newCost - 9;
    }
  }

  return result;
}


int main()
{
  std::ifstream                          input( R"(D:\Develop\AdventOfCode2021\Day15\input.txt)" );
  std::string                            textLine;
  std::vector<std::vector<GridPosition>> grid;
  std::multimap<int, PathPoint>          partialPaths;

  while( std::getline( input, textLine ) )
  {
    std::vector<GridPosition> gridLine;

    for( const auto &c : textLine )
    {
      gridLine.emplace_back( GridPosition( c - '0' ) );
    }
    grid.emplace_back( gridLine );
  }

  grid = growGrid( grid );

  int numRows = grid.size();
  int numCols = grid[0].size();

  int currentCost = grid[numRows - 1][numCols - 1].m_cost;
  grid[numRows - 1][numCols - 1].m_direction = GridPosition::End;
  grid[numRows - 1][numCols - 1].m_pathCost  = currentCost;

  partialPaths.insert( { currentCost, PathPoint{ numRows - 1, numCols - 1 } } );

  while( !partialPaths.empty() )
  {
    auto it = partialPaths.begin();
    auto [cost, node] = *it;
    partialPaths.erase( it );

    if( ( node.m_row == 0 ) && ( node.m_col == 0 ) )
    {
      break;
    }
    const auto &thisNode = grid[node.m_row][node.m_col];

    if( node.m_row > 0 )
    {
      auto &nextNode = grid[node.m_row - 1][node.m_col];

      if( ( nextNode.m_direction == GridPosition::Unknown ) ||
        ( thisNode.m_pathCost + nextNode.m_cost < nextNode.m_pathCost ) )
      {
        nextNode.m_direction = GridPosition::Bottom;
        nextNode.m_pathCost = thisNode.m_pathCost + nextNode.m_cost;
        partialPaths.insert( { nextNode.m_pathCost, PathPoint{ node.m_row - 1, node.m_col } } );
      }
    }
    if( node.m_row < numRows - 1 )
    {
      auto &nextNode = grid[node.m_row + 1][node.m_col];

      if( ( nextNode.m_direction == GridPosition::Unknown ) ||
        ( thisNode.m_pathCost + nextNode.m_cost < nextNode.m_pathCost ) )
      {
        nextNode.m_direction = GridPosition::Top;
        nextNode.m_pathCost = thisNode.m_pathCost + nextNode.m_cost;
        partialPaths.insert( { nextNode.m_pathCost, PathPoint{ node.m_row + 1, node.m_col } } );
      }
    }
    if( node.m_col > 0 )
    {
      auto &nextNode = grid[node.m_row][node.m_col - 1];

      if( ( nextNode.m_direction == GridPosition::Unknown ) ||
        ( thisNode.m_pathCost + nextNode.m_cost < nextNode.m_pathCost ) )
      {
        nextNode.m_direction = GridPosition::Right;
        nextNode.m_pathCost = thisNode.m_pathCost + nextNode.m_cost;
        partialPaths.insert( { nextNode.m_pathCost, PathPoint{ node.m_row, node.m_col - 1 } } );
      }
    }
    if( node.m_col < numCols - 1 )
    {
      auto &nextNode = grid[node.m_row][node.m_col + 1];

      if( ( nextNode.m_direction == GridPosition::Unknown ) ||
        ( thisNode.m_pathCost + nextNode.m_cost < nextNode.m_pathCost ) )
      {
        nextNode.m_direction = GridPosition::Left;
        nextNode.m_pathCost = thisNode.m_pathCost + nextNode.m_cost;
        partialPaths.insert( { nextNode.m_pathCost, PathPoint{ node.m_row, node.m_col + 1 } } );
      }
    }
  }

  const auto &start = grid[0][0];
  std::cout << "The total risk is " << start.m_pathCost - start.m_cost << std::endl;

  return 0;
}