
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


static constexpr int s_boardSize = 5;


struct BingoEntry
{
  bool m_drawn = false;
  int  m_value = 0;

  explicit BingoEntry( int value )
    : m_value( value )
  { }
};


struct BingoBoard
{
  BingoBoard( std::ifstream &input )
  {
    m_entries.resize( s_boardSize );
    m_rowHits.resize( s_boardSize, 0 );
    m_colHits.resize( s_boardSize, 0 );
    for( auto &row : m_entries )
    {
      for( int col = 0; col < s_boardSize; col++ )
      {
        int value;
        input >> value;
        row.emplace_back( BingoEntry( value ) );
      }
    }
  }

  bool markAndCheckWin( int value )
  {
    for( int r = 0; r < s_boardSize; r ++ )
    {
      auto &row = m_entries[r];

      for( int c = 0; c < s_boardSize; c++ )
      {
        auto &col = row[c];

        if( col.m_value == value )
        {
          col.m_drawn = true;
          m_rowHits[r]++;
          m_colHits[c]++;
        }
      }
    }
    for( int i = 0; i < s_boardSize; i++ )
    {
      if( ( m_rowHits[i] == s_boardSize ) || ( m_colHits[i] == s_boardSize ) )
      {
        m_hasWon = true;
        break;
      }
    }
    return m_hasWon;
  }

  int sumUnmarked()
  {
    int sum = 0;

    for( const auto &row : m_entries )
    {
      for( const auto &col : row )
      {
        if( !col.m_drawn )
        {
          sum += col.m_value;
        }
      }
    }
    return sum;
  }

  std::vector<std::vector<BingoEntry>> m_entries;
  std::vector<int>                     m_rowHits;
  std::vector<int>                     m_colHits;
  bool                                 m_hasWon = false;
};


int main()
{
  std::ifstream           input( R"(..\Day04\input.txt)" );
  std::string             drawnLine;
  std::vector<int>        drawnValues;
  std::vector<BingoBoard> boards;

  std::getline( input, drawnLine );
  while( true )
  {
    drawnValues.push_back( std::stoi( drawnLine ) );

    auto pos = drawnLine.find( ',' );
    if( pos == std::string::npos )
    {
      break;
    }
    else
    {
      drawnLine = drawnLine.substr( pos + 1 );
    }
  }

  while( !input.eof() )
  {
    boards.emplace_back( BingoBoard( input ) );
  }

  std::cout << "Read " << drawnValues.size() << " values and " << boards.size() << " boards." << std::endl;

  int  finalScore = 0;
  bool isFirst    = true;
  for( auto value : drawnValues )
  {
    for( auto &board : boards )
    {
      if( !board.m_hasWon && board.markAndCheckWin( value ) )
      {
        finalScore = value * board.sumUnmarked();
        if( isFirst )
        {
          std::cout << "Final score of first board won is " << finalScore << std::endl;
          isFirst = false;
        }
      }
    }
  }

  std::cout << "Final score of last board won is " << finalScore << std::endl;

  return 0;
}