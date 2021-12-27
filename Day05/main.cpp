
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


struct Line
{
  int m_x1 = 0;
  int m_y1 = 0;
  int m_x2 = 0;
  int m_y2 = 0;
};


struct Grid
{
  Grid( int xSize, int ySize )
    : m_values( ySize, std::vector<int>( xSize, 0 ) )
  {}

  void addHorizontalOrVerticalLines( const std::vector<Line> &lines )
  {
    for( const auto &line : lines )
    {
      if( line.m_x1 == line.m_x2 )
      {
        int start = std::min( line.m_y1, line.m_y2 );
        int end   = std::max( line.m_y1, line.m_y2 );

        for( int i = start; i <= end; i++ )
        {
          m_values[i][line.m_x1]++;
        }
      }
      else if( line.m_y1 == line.m_y2 )
      {
        int start = std::min( line.m_x1, line.m_x2 );
        int end = std::max( line.m_x1, line.m_x2 );

        for( int i = start; i <= end; i++ )
        {
          m_values[line.m_y1][i]++;
        }
      }
    }
  }

  void addDiagonalLines( const std::vector<Line> &lines )
  {
    for( const auto &line : lines )
    {
      if( std::abs( line.m_x1 - line.m_x2 ) == std::abs( line.m_y1 - line.m_y2 ) )
      {
        int count = std::abs( line.m_x1 - line.m_x2 );
        int dx = ( line.m_x1 < line.m_x2 ) ? 1 : -1;
        int dy = ( line.m_y1 < line.m_y2 ) ? 1 : -1;

        for( int i = 0; i <= count; i++ )
        {
          m_values[line.m_y1 + i * dy][line.m_x1 + i * dx]++;
        }
      }
    }
  }
  
  int countDangerousAreas()
  {
    int count = 0;

    for( const auto &row : m_values )
    {
      for( const auto &col : row )
      {
        if( col > 1 )
        {
          count++;
        }
      }
    }
    return count;
  }

  std::vector<std::vector<int>> m_values;
};


int main()
{
  std::ifstream     input( R"(..\Day05\input.txt)" );
  std::vector<Line> lines;
  std::string       textLine;
  int               xMax = 0;
  int               yMax = 0;

  while( std::getline( input, textLine ) )
  {
    Line line;

    line.m_x1 = std::stoi( textLine );
    if( line.m_x1 > xMax )
    {
      xMax = line.m_x1;
    }
    textLine = textLine.substr( textLine.find( ',' ) + 1 );
    line.m_y1 = std::stoi( textLine );
    if( line.m_y1 > yMax )
    {
      yMax = line.m_y1;
    }
    textLine = textLine.substr( textLine.find( '>' ) + 2 );
    line.m_x2 = std::stoi( textLine );
    if( line.m_x2 > xMax )
    {
      xMax = line.m_x2;
    }
    textLine = textLine.substr( textLine.find( ',' ) + 1 );
    line.m_y2 = std::stoi( textLine );
    if( line.m_y2 > yMax )
    {
      yMax = line.m_y2;
    }

    lines.emplace_back( line );
  }

  Grid grid( xMax + 1, yMax + 1 );

  grid.addHorizontalOrVerticalLines( lines );

  std::cout << "First number of dangerous areas: " << grid.countDangerousAreas() << std::endl;

  grid.addDiagonalLines( lines );

  std::cout << "Second number of dangerous areas: " << grid.countDangerousAreas() << std::endl;

  return 0;
}