
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <set>
#include <unordered_map>


struct Digit
{
  explicit Digit( const std::string &digitString )
    : m_length( digitString.size() )
    , m_bits( 0 )
  {
    for( int i = 0; i < m_length; i++ )
    {
      m_bits |= ( 1 << ( digitString[i] - 'a' ) );
    }
  }

  int m_length;
  int m_bits;
};


struct PatternLine
{
  std::vector<Digit> m_input;
  std::vector<Digit> m_output;
};


size_t uniqueDigits( const PatternLine &pattern )
{
  size_t result = 0;

  for( const auto &digit : pattern.m_output )
  {
    size_t numSignals = digit.m_length;

    if( ( numSignals == 2 ) || ( numSignals == 3 ) || ( numSignals == 4 ) || ( numSignals == 7 ) )
    {
      result++;
    }
  }
  return result;
}


int decodePattern( const PatternLine &pattern )
{
  std::vector<int> knownValues( 10, -1 );
  std::unordered_map<int, int> mapping;

  // find 1, 4, 7 and 8
  for( const auto &digit : pattern.m_input )
  {
    switch( digit.m_length )
    {
      case 2:
        knownValues[1] = digit.m_bits;
        mapping[digit.m_bits] = 1;
        break;
      case 3:
        knownValues[7] = digit.m_bits;
        mapping[digit.m_bits] = 7;
        break;
      case 4:
        knownValues[4] = digit.m_bits;
        mapping[digit.m_bits] = 4;
        break;
      case 7:
        knownValues[8] = digit.m_bits;
        mapping[digit.m_bits] = 8;
        break;
      default:
        break;
    }
  }

  // find 0, 6 and 9
  for( const auto &digit : pattern.m_input )
  {
    if( digit.m_length != 6 )
    {
      continue;
    }
    if( ( digit.m_bits & knownValues[4] ) == knownValues[4] )
    {
      knownValues[9] = digit.m_bits;
      mapping[digit.m_bits] = 9;
    }
    else if( ( digit.m_bits & knownValues[1] ) == knownValues[1] )
    {
      knownValues[0] = digit.m_bits;
      mapping[digit.m_bits] = 0;
    }
    else
    {
      knownValues[6] = digit.m_bits;
      mapping[digit.m_bits] = 6;
    }
  }

  // find 2, 3 and 5
  for( const auto &digit : pattern.m_input )
  {
    if( digit.m_length != 5 )
    {
      continue;
    }
    if( ( digit.m_bits & knownValues[1] ) == knownValues[1] )
    {
      knownValues[3] = digit.m_bits;
      mapping[digit.m_bits] = 3;
    }
    else if( ( digit.m_bits & knownValues[6] ) == digit.m_bits )
    {
      knownValues[5] = digit.m_bits;
      mapping[digit.m_bits] = 5;
    }
    else
    {
      knownValues[2] = digit.m_bits;
      mapping[digit.m_bits] = 2;
    }
  }

  // now get the digits and sum up
  int sum = 0;
  for( const auto &digit : pattern.m_output )
  {
    sum = sum * 10 + mapping[digit.m_bits];
  }

  std::cout << "Pattern decodes as " << sum << std::endl;
  
  return sum;
}


int main()
{
  std::ifstream            input( R"(D:\Develop\AdventOfCode2021\Day08\input.txt)" );
  std::string              textLine;
  std::vector<PatternLine> patterns;

  while( std::getline( input, textLine ) )
  {
    std::istringstream ls( textLine );
    PatternLine        pattern;
    std::string        deliminator;

    for( int i = 0; i < 10; i++ )
    {
      std::string digit;
      ls >> digit;
      pattern.m_input.emplace_back( Digit( digit ) );
    }
    ls >> deliminator;
    for( int i = 0; i < 4; i++ )
    {
      std::string digit;
      ls >> digit;
      pattern.m_output.emplace_back( Digit( digit ) );
    }

    patterns.emplace_back( pattern );
  }

  int uniqueDigitSum = 0;
  int outputValueSum = 0;

  for( const auto &pattern : patterns )
  {
    uniqueDigitSum += uniqueDigits( pattern );
    outputValueSum += decodePattern( pattern );
  }

  std::cout << "Found " << uniqueDigitSum << " unique patterns in the output" << std::endl;
  std::cout << "Overall sum of output values is " << outputValueSum << std::endl;

  return 0;
}