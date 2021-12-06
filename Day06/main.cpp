
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


static constexpr int s_maxAge = 8;
static constexpr int s_days = 256;


int main()
{
  std::ifstream       input( R"(D:\Develop\AdventOfCode2021\Day06\input.txt)" );
  std::string         textLine;
  std::vector<size_t> fishOfAge( s_maxAge + 1, 0 );

  std::getline( input, textLine );
  while( true )
  {
    fishOfAge[std::stoi( textLine )]++;

    auto pos = textLine.find( ',' );
    if( pos == std::string::npos )
    {
      break;
    }
    else
    {
      textLine = textLine.substr( pos + 1 );
    }
  }

  for( int day = 0; day < s_days; day++ )
  {
    size_t fishOfAge0 = fishOfAge[0];

    for( int newAge = 0; newAge < s_maxAge; newAge++ )
    {
      fishOfAge[newAge] = fishOfAge[newAge + 1];
    }
    fishOfAge[6] += fishOfAge0;
    fishOfAge[8] = fishOfAge0;
  }

  size_t fishTotal = 0;
  for( int age = 0; age <= s_maxAge; age++ )
  {
    fishTotal += fishOfAge[age];
  }

  std::cout << "After " << s_days << " days, there are " << fishTotal << " lanternfishes" << std::endl;

  return 0;
}