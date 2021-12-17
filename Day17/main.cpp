
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


struct Target
{
  int m_minX;
  int m_maxX;
  int m_minY;
  int m_maxY;
};


int minXHitting( const Target &target )
{
  int x = 2;

  while( ( x * ( x + 1 ) ) / 2 < target.m_minX )
    x++;
  return x;
}


std::vector<int> xHitting( const Target &target, int xStart )
{
  std::vector<int> result;

  for( int xTest = xStart; xTest <= target.m_maxX; xTest++ )
  {
    for( int x = 0, xVelocity = xTest; ( x <= target.m_maxX ) && ( xVelocity >= 0 ); x += xVelocity, xVelocity-- )
    {
      if( x >= target.m_minX )
      {
        result.push_back( xTest );
        break;
      }
    }
  }

  return result;
}


int maxHeightForPair( const Target &target, int xVelocity, int yVelocity )
{
  int  height = 0;
  bool hit = false;

  int x = 0;
  int dx = xVelocity;
  int y = 0;
  int dy = yVelocity;
  while( ( x <= target.m_maxX ) && ( y >= target.m_minY ) )
  {
    if( ( x >= target.m_minX ) && ( y <= target.m_maxY ) )
    {
      hit = true;
    }
    if( y > height )
    {
      height = y;
    }

    x += xVelocity;
    y += yVelocity;
    if( xVelocity > 0 )
    {
      xVelocity--;
    }
    yVelocity--;
  }

  return hit ? height : 0;
}


bool pairHit( const Target &target, int xVelocity, int yVelocity )
{
  bool hit = false;

  int x = 0;
  int dx = xVelocity;
  int y = 0;
  int dy = yVelocity;
  while( ( x <= target.m_maxX ) && ( y >= target.m_minY ) )
  {
    if( ( x >= target.m_minX ) && ( y <= target.m_maxY ) )
    {
      hit = true;
      break;
    }

    x += xVelocity;
    y += yVelocity;
    if( xVelocity > 0 )
    {
      xVelocity--;
    }
    yVelocity--;
  }

  return hit;
}


int maxHeightForX( const Target &target, int xVelocity )
{
  int height = 0;

  for( int yVelocity = 0; yVelocity < 10000; yVelocity++ )
  {
    int h = maxHeightForPair( target, xVelocity, yVelocity );
    if( h > height )
    {
      height = h;
    }
  }
  return height;
}


int hitCountForX( const Target &target, int xVelocity )
{
  int hitCount = 0;

  for( int yVelocity = target.m_minY; yVelocity < 10000; yVelocity++ )
  {
    if( pairHit( target, xVelocity, yVelocity ) )
    {
      hitCount++;
    }
  }
  return hitCount;
}


int main()
{
  //Target target{ 20, 30, -10, -5 }; // example
  Target target{ 88, 125, -157, -103 }; // input

  int xStart = minXHitting( target );
  std::cout << "Minimum X value : " << xStart << std::endl;;

  auto xRange = xHitting( target, xStart );
  int maxHeight = 0;
  int hitCount = 0;
  for( auto x : xRange )
  {
    int height = maxHeightForX( target, x );
    int hits = hitCountForX( target, x );
    std::cout << "x = " << x << " : maxHeight " << height << ", hit count " << hits << std::endl;
    if( height > maxHeight )
    {
      maxHeight = height;
    }
    hitCount += hits;
  }

  std::cout << std::endl;
  std::cout << "Maximum overall height is " << maxHeight << std::endl;
  std::cout << "Total hit count is " << hitCount << std::endl;

  return 0;
}