
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


enum BraceType
{
  Square, Brace, Paren, Angle
};


int main()
{
  std::ifstream              input( R"(D:\Develop\AdventOfCode2021\Day10\input.txt)" );
  std::string                textLine;
  int                        totalError = 0;
  std::vector<std::uint64_t> completionScores;

  while( std::getline( input, textLine ) )
  {
    std::stack<BraceType> chunkStack;
    int lineError       = 0;
    std::uint64_t completionScore = 0;

    for( auto pos : textLine )
    {
      switch( pos )
      {
        case '[':
          chunkStack.push( Square );
          break;
        case ']':
          if( chunkStack.top() != Square )
          {
            lineError = 57;
          }
          chunkStack.pop();
          break;
        case '{':
          chunkStack.push( Brace );
          break;
        case '}':
          if( chunkStack.top() != Brace )
          {
            lineError = 1197;
          }
          chunkStack.pop();
          break;
        case '(':
          chunkStack.push( Paren );
          break;
        case ')':
          if( chunkStack.top() != Paren )
          {
            lineError = 3;
          }
          chunkStack.pop();
          break;
        case '<':
          chunkStack.push( Angle );
          break;
        case '>':
          if( chunkStack.top() != Angle )
          {
            lineError = 25137;
          }
          chunkStack.pop();
          break;
        default:
          break;
      }
      if( lineError > 0 )
      {
        break;
      }
    }
    if( lineError == 0 )
    {
      while( !chunkStack.empty() )
      {
        switch( chunkStack.top() )
        {
          case Square:
            completionScore = completionScore * 5 + 2;
            break;
          case Brace:
            completionScore = completionScore * 5 + 3;
            break;
          case Paren:
            completionScore = completionScore * 5 + 1;
            break;
          case Angle:
            completionScore = completionScore * 5 + 4;
            break;
          default:
            break;
        }
        chunkStack.pop();
      }
      completionScores.push_back( completionScore );

      //std::cout << "Line completion score: " << completionScore << std::endl;
    }
    else
    {
      totalError += lineError;
    }
  }

  std::sort( completionScores.begin(), completionScores.end() );

  std::cout << "The total syntax error score is " << totalError << std::endl;
  std::cout << "The middle completion score is " << completionScores[completionScores.size() / 2] << std::endl;

  return 0;
}