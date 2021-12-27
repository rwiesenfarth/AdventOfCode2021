
#include <fstream>
#include <iostream>
#include <vector>


void splitList( const std::vector<int> &input, int bitNumber, std::vector<int> &ones, std::vector<int> &zeroes )
{
  int mask = 1 << bitNumber;

  for( auto value : input )
  {
    if( value & mask )
    {
      ones.push_back( value );
    }
    else
    {
      zeroes.push_back( value );
    }
  }
}


int oxygenGeneratorRating( const std::vector<int> &input, int bitCount )
{
  std::vector<int> currentInput = input;
  int              currentBit   = bitCount;

  do
  {
    std::vector<int> ones;
    std::vector<int> zeroes;

    currentBit--;
    splitList( currentInput, currentBit, ones, zeroes );
    if( ones.size() >= zeroes.size() )
    {
      currentInput = ones;
    }
    else
    {
      currentInput = zeroes;
    }
  }
  while( currentInput.size() > 1 );

  std::cout << "Oxygen generator rating = " << currentInput[0] << std::endl;

  return currentInput[0];
}


int co2ScrubberRating( const std::vector<int> &input, int bitCount )
{
  std::vector<int> currentInput = input;
  int              currentBit = bitCount;

  do
  {
    std::vector<int> ones;
    std::vector<int> zeroes;

    currentBit--;
    splitList( currentInput, currentBit, ones, zeroes );
    if( zeroes.size() <= ones.size() )
    {
      currentInput = zeroes;
    }
    else
    {
      currentInput = ones;
    }
  }
  while( currentInput.size() > 1 );

  std::cout << "CO2 scrubber rating = " << currentInput[0] << std::endl;

  return currentInput[0];
}


int main()
{
  std::ifstream    input( R"(..\Day03\input.txt)" );
  std::string      line;
  std::vector<int> bitsSet;
  std::vector<int> values;
  int              lineCount = 0;

  while( !input.eof() )
  {
    input >> line;
    lineCount++;
    //std::cout << "read: '" << line << "'" << std::endl;

    if( bitsSet.empty() )
    {
      bitsSet.resize( line.size(), 0 );
    }

    int value = 0;
    for( int i = 0; i < line.size(); i++ )
    {
      value <<= 1;
      if( line[i] == '1' )
      {
        value++;
        bitsSet[i]++;
      }
    }
    values.push_back( value );
  }

  int gamma   = 0;
  int epsilon = 0;

  for( auto ones : bitsSet )
  {
    gamma   <<= 1;
    epsilon <<= 1;

    if( ones > lineCount / 2 )
    {
      gamma++;
    }
    else
    {
      epsilon++;
    }
  }

  std::cout << "result: gamma = " << gamma << ", epsilon = " << epsilon << ", power consumption = " <<
    gamma * epsilon << std::endl;

  auto oxy = oxygenGeneratorRating( values, bitsSet.size() );
  auto co2 = co2ScrubberRating( values, bitsSet.size() );

  std::cout << "life support rating = " << oxy * co2 << std::endl;

  return 0;
}