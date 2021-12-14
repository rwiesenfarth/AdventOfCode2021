
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>


struct Rule
{
  std::string m_match;
  char        m_insert;

  Rule( const std::string &line )
  {
    m_match = line.substr( 0, 2 );;
    m_insert = line[6];
  }
};


std::string insertion( const std::string &input, const std::vector<Rule> &rules )
{
  std::string result;

  result.reserve( input.size() * 2 );

  for( int i = 0; i < input.size() - 1; i++ )
  {
    char matchingInsert = '-';

    for( const auto &rule : rules )
    {
      if( input.substr( i , 2 ) == rule.m_match )
      {
        matchingInsert = rule.m_insert;
        break;
      }
    }
    result.push_back( input[i] );
    if( matchingInsert != '-' )
    {
      result.push_back( matchingInsert );
    }
  }
  result.push_back( input.back() );
  result.shrink_to_fit();

  //std::cout << "Insertion result is " << result << std::endl;

  return result;
}


std::unordered_map<std::string, std::uint64_t> insertion( const std::unordered_map<std::string, std::uint64_t> &input,
  const std::vector<Rule> &rules )
{
  std::unordered_map<std::string, std::uint64_t> result;

  for( const auto &[pattern, count] : input )
  {
    bool matched = false;
    for( const auto &rule : rules )
    {
      if( pattern == rule.m_match )
      {
        result[std::string( { pattern[0], rule.m_insert } )] += count;
        result[std::string( { rule.m_insert, pattern[1] } )] += count;
        matched = true;
        break;
      }
    }
    if( !matched )
    {
      result[pattern] += count;
    }
  }

  //std::cout << "Pairs/count after insertion:" << std::endl;
  //for( const auto &[pattern, count] : result )
  //{
  //  std::cout << pattern << " : " << count << std::endl;
  //}

  return result;
}


int main()
{
  std::ifstream                                  input( R"(D:\Develop\AdventOfCode2021\Day14\input.txt)" );
  std::string                                    pattern;
  std::string                                    textLine;
  std::vector<Rule>                              rules;
  std::unordered_map<std::string, std::uint64_t> pairCount;
  char                                           lastChar;

  std::getline( input, pattern );
  for( int i = 0; i < pattern.size() - 1; i++ )
  {
    pairCount[pattern.substr( i, 2 )]++;
  }
  lastChar = pattern.back();

  std::getline( input, textLine );

  while( std::getline( input, textLine ) )
  {
    rules.emplace_back( Rule( textLine ) );
  }

  for( int i = 0; i < 10; i++ )
  {
    pattern = insertion( pattern, rules );
  }

  for( int i = 0; i < 40; i++ )
  {
    pairCount = insertion( pairCount, rules );
  }

  std::unordered_map<char, std::uint64_t> quantities;
  std::uint64_t finalLength = 0;
  for( const auto &[pattern, count] : pairCount )
  {
    quantities[pattern[0]] += count;
    quantities[pattern[1]] += count;
    finalLength += count;
  }
  for( auto &[_, count] : quantities )
  {
    count /= 2;
  }
  quantities[lastChar]++;
  finalLength++;

  //for( const auto &c : pattern )
  //{
  //  quantities[c]++;
  //}

  std::uint64_t minCount = finalLength;
  std::uint64_t maxCount = 0;
  for( const auto &[_, quantity] : quantities )
  {
    if( quantity < minCount )
    {
      minCount = quantity;
    }
    if( quantity > maxCount )
    {
      maxCount = quantity;
    }
  }

  std::cout << "length is " << finalLength << ", min count is " << minCount << ", max count is " << maxCount <<
    ", difference is " << maxCount - minCount << std::endl;

  return 0;
}