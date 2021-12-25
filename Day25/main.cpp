
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


enum PositionState
{
  Empty,
  EastMovingSeaCucumber,
  SouthMovingSeaCucumber
};


void print( const std::vector<std::vector<PositionState>> &seaFloor )
{
  for( const auto &row : seaFloor )
  {
    for( const auto &pos : row )
    {
      std::cout << ( ( pos == SouthMovingSeaCucumber ) ? 'v' : ( pos == EastMovingSeaCucumber ) ? '>' : '.' );
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}


bool moveEastOneStep( std::vector<std::vector<PositionState>> &seaFloor )
{
  bool                                    movementHappened = false;
  size_t                                  rows = seaFloor.size();
  size_t                                  columns = seaFloor[0].size();
  std::vector<std::vector<PositionState>> result( rows, std::vector<PositionState>( columns , Empty ) );

  for( int row = 0; row < rows; row++ )
  {
    for( int column = 0; column < columns; column++ )
    {
      int  nextColumn = ( column + 1 ) % columns;
      auto spot = seaFloor[row][column];

      if( ( spot == EastMovingSeaCucumber ) && ( seaFloor[row][nextColumn] == Empty ) )
      {
        result[row][column] = Empty;
        result[row][nextColumn] = EastMovingSeaCucumber;
        movementHappened = true;
      }
      else if( result[row][column] == Empty )
      {
        result[row][column] = spot;
      }
    }
  }
  seaFloor = result;

  return movementHappened;
}


bool moveSouthOneStep( std::vector<std::vector<PositionState>> &seaFloor )
{
  bool                                    movementHappened = false;
  size_t                                  rows = seaFloor.size();
  size_t                                  columns = seaFloor[0].size();
  std::vector<std::vector<PositionState>> result( rows, std::vector<PositionState>( columns, Empty ) );

  for( int row = 0; row < rows; row++ )
  {
    int nextRow = ( row + 1 ) % rows;

    for( int column = 0; column < columns; column++ )
    {
      auto spot = seaFloor[row][column];

      if( ( spot == SouthMovingSeaCucumber ) && ( seaFloor[nextRow][column] == Empty ) )
      {
        result[row][column] = Empty;
        result[nextRow][column] = SouthMovingSeaCucumber;
        movementHappened = true;
      }
      else if( result[row][column] == Empty )
      {
        result[row][column] = spot;
      }
    }
  }
  seaFloor = result;

  return movementHappened;
}


bool moveOneStep( std::vector<std::vector<PositionState>> &seaFloor )
{
  bool movedEast = moveEastOneStep( seaFloor );
  bool movedSouth = moveSouthOneStep( seaFloor );

  return movedEast || movedSouth;
}


int main()
{
  std::ifstream                           input( R"(..\Day25\input.txt)" );
  std::string                             textLine;
  std::vector<std::vector<PositionState>> seaFloor;

  while( std::getline( input, textLine ) )
  {
    std::vector<PositionState> row;

    for( auto spot : textLine )
    {
      row.push_back( ( spot == 'v' ) ? SouthMovingSeaCucumber : ( spot == '>' ) ? EastMovingSeaCucumber : Empty );
    }
    seaFloor.push_back( row );
  }

  //std::cout << "Initial state:" << std::endl;
  //print( seaFloor );

  int step = 1;
  for( ; moveOneStep( seaFloor ); step++ )
  {
    //print( seaFloor );
  }

  //std::cout << "Final state:" << std::endl;
  //print( seaFloor );

  std::cout << "Steps needed to reach final state: " << step << std::endl;

  return 0;
}