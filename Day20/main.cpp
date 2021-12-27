
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


static char s_outsideValue = 0;


std::vector<char> convertLine( const std::string &line )
{
  std::vector<char> result;

  for( auto c : line )
  {
    result.push_back( ( c == '#' ) ? 1 : 0 );
  }
  return result;
}


inline char lookupValue( const std::vector<std::vector<char>> &input, int numRows, int numCols, int row, int col )
{
  return ( ( row < 0 ) || ( row >= numRows ) || ( col < 0 ) || ( col >= numCols ) ) ? s_outsideValue : input[row][col];
}


std::vector<std::vector<char>> applyMapping( const std::vector<std::vector<char>> &input,
  const std::vector<char> &lookupPattern )
{
  std::vector<std::vector<char>> result;
  int numRows = input.size();
  int numCols = input[0].size();

  for( int row = -1; row <= numRows; row++ )
  {
    std::vector<char> resultLine;

    for( int col = -1; col <= numCols; col++ )
    {
      int value = 0;
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row - 1, col - 1 );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row - 1, col );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row - 1, col + 1 );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row, col - 1 );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row, col );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row, col + 1 );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row + 1, col - 1 );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row + 1, col );
      value = ( value << 1 ) + lookupValue( input, numRows, numCols, row + 1, col + 1 );
      resultLine.push_back( lookupPattern[value] );
    }
    result.emplace_back( resultLine );
  }

  return result;
}


int countPixel( const std::vector<std::vector<char>> &image )
{
  int count = 0;

  for( const auto &line : image )
  {
    for( auto pixel : line )
    {
      count += (int) pixel;
    }
  }
  return count;
}


std::ostream &printPattern( std::ostream &stream, const std::vector<char> &pattern )
{
  for( auto pixel : pattern )
  {
    stream << ( ( pixel > 0 ) ? '#' : '.' );
  }
  stream << std::endl;
  return stream;
}


std::ostream &printImage( std::ostream &stream, const std::vector<std::vector<char>> &image )
{
  for( const auto &line : image )
  {
    for( auto pixel : line )
    {
      stream << ( ( pixel > 0 ) ? '#' : '.' );
    }
    stream << std::endl;
  }
  return stream;
}


int main()
{
  std::ifstream input( R"(..\Day20\input.txt)" );
  std::string   textLine;
  std::vector<std::vector<char>> image;
  std::vector<char> lookupPattern;

  std::getline( input, textLine );
  lookupPattern = convertLine( textLine );
  std::getline( input, textLine );

  while( std::getline( input, textLine ) )
  {
    image.push_back( convertLine( textLine ) );
  }

  std::cout << "Input image ( " << countPixel( image ) << " pixel set) :" << std::endl;
  //printImage( std::cout, image );
  //std::cout << std::endl;
  //std::cout << "Pattern:" << std::endl;
  //printPattern( std::cout, lookupPattern );
  //std::cout << std::endl;

  for( int round = 0; round < 50; round++ )
  {
    image = applyMapping( image, lookupPattern );
    s_outsideValue = ( s_outsideValue == 0 ) ? lookupPattern[0] : lookupPattern[511];

    std::cout << std::endl;
    std::cout << "next image ( " << countPixel( image ) << " pixel set) :" << std::endl;
    std::cout << "outsideValue: " << ( ( s_outsideValue == 0 ) ? '0' : '1' ) << std::endl;
    //printImage( std::cout, image );
    //std::cout << std::endl;
  }

  return 0;
}