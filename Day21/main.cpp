
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


static constexpr int s_winningScore = 1000;
static std::vector<int> s_resultCounts = { 0, 0, 0, 1, 3, 6, 7, 6, 3, 1 };


struct Die
{
  Die( int faces )
    : m_faces( faces )
  { }

  int roll()
  {
    int result = m_state + 1;

    m_state = ( m_state + 1 ) % m_faces;
    m_rolls++;

    //std::cout << "  die rolled " << result << std::endl;

    return result;
  }

  int numRolls()
  {
    return m_rolls;
  }

  int m_faces;
  int m_state = 0;
  int m_rolls = 0;
};


struct Pawn
{
  Pawn( int startingSpace, int boardSize = 10 )
    : m_position( startingSpace - 1 )
    , m_boardSize( boardSize )
  { }

  bool move( Die &die )
  {
    int steps = die.roll() + die.roll() + die.roll();

    m_position = ( m_position + steps ) % m_boardSize;
    m_score += m_position + 1;

    //std::cout << "moved to " << m_position + 1 << ", score " << m_score << std::endl;

    return ( m_score >= s_winningScore );
  }

  int m_position;
  int m_boardSize;
  int m_score = 0;
};


struct DiracState
{
  DiracState( int playerOneStart, int playerTwoStart, int winningScore = 21, int boardSize = 10 )
    : m_winningScore( winningScore )
    , m_boardSize( boardSize )
    , m_states( std::vector<std::uint64_t>( winningScore * boardSize * winningScore * boardSize, 0 ) )
  {
    m_states[stateIndex( 0, playerOneStart - 1, 0, playerTwoStart - 1 )] = 1;
  }

  int stateIndex( int playerOneScore, int playerOnePosition, int playerTwoScore, int playerTwoPosition )
  {
    return ( ( playerOneScore * m_boardSize + playerOnePosition ) * m_winningScore +
      playerTwoScore ) * m_boardSize + playerTwoPosition;
  }

  int playerOneScore( int stateIndex )
  {
    return stateIndex / ( m_boardSize * m_winningScore * m_boardSize );
  }

  int playerOnePosition( int stateIndex )
  {
    return ( stateIndex / ( m_winningScore * m_boardSize ) ) % m_boardSize;
  }

  int playerTwoScore( int stateIndex )
  {
    return ( stateIndex / m_boardSize ) % m_winningScore;
  }

  int playerTwoPosition( int stateIndex )
  {
    return stateIndex % m_boardSize;
  }

  bool playerOneMove()
  {
    std::vector<std::uint64_t> newStates( m_states.size(), 0 );
    bool foundStates = false;

    //std::cout << "Player one moves..." << std::endl;

    for( int index = 0; index < m_states.size(); index++ )
    {
      if( m_states[index] == 0 )
      {
        continue;
      }
      int scoreOne = playerOneScore( index );
      int positionOne = playerOnePosition( index );
      int scoreTwo = playerTwoScore( index );
      int positionTwo = playerTwoPosition( index );
      std::uint64_t count = m_states[index];

      //std::cout << "  [ " << positionOne << " ; " << scoreOne << " --- " <<
      //  positionTwo << " ; " << scoreTwo << " ] : count " << count << std::endl;

      for( int rollResult = 3; rollResult < 10; rollResult++ )
      {
        std::uint64_t newCount = count * s_resultCounts[rollResult];
        int newPositionOne = ( positionOne + rollResult ) % m_boardSize;
        int newScoreOne = scoreOne + newPositionOne + 1;
        if( newScoreOne >= m_winningScore )
        {
          m_playerOneWins += newCount;
          //std::cout << "    => player one wins increased by " << newCount;
        }
        else
        {
          foundStates = true;
          int newIndex = stateIndex( newScoreOne, newPositionOne, scoreTwo, positionTwo );
          newStates[newIndex] += newCount;
          //std::cout << "    => [ " << newPositionOne << " ; " << newScoreOne << " --- " <<
          //  positionTwo << " ; " << scoreTwo << " ] : add " << newCount << std::endl;
        }
      }
    }
    m_states = newStates;
    return foundStates;
  }

  bool playerTwoMove()
  {
    std::vector<std::uint64_t> newStates( m_states.size(), 0 );
    bool foundStates = false;

    //std::cout << "Player two moves..." << std::endl;

    for( int index = 0; index < m_states.size(); index++ )
    {
      if( m_states[index] == 0 )
      {
        continue;
      }
      int scoreOne = playerOneScore( index );
      int positionOne = playerOnePosition( index );
      int scoreTwo = playerTwoScore( index );
      int positionTwo = playerTwoPosition( index );
      std::uint64_t count = m_states[index];

      //std::cout << "  [ " << positionOne << " ; " << scoreOne << " --- " <<
      //  positionTwo << " ; " << scoreTwo << " ] : count " << count << std::endl;

      for( int rollResult = 3; rollResult < 10; rollResult++ )
      {
        std::uint64_t newCount = count * s_resultCounts[rollResult];
        int newPositionTwo = ( positionTwo + rollResult ) % m_boardSize;
        int newScoreTwo = scoreTwo + newPositionTwo + 1;
        if( newScoreTwo >= m_winningScore )
        {
          m_playerTwoWins += newCount;
          //std::cout << "    => player two wins increased by " << newCount;
        }
        else
        {
          foundStates = true;
          int newIndex = stateIndex( scoreOne, positionOne, newScoreTwo, newPositionTwo );
          newStates[newIndex] += newCount;
          //std::cout << "    => [ " << positionOne << " ; " << scoreOne << " --- " <<
          //  newPositionTwo << " ; " << newScoreTwo << " ] : add " << newCount << std::endl;
        }
      }
    }
    m_states = newStates;
    return foundStates;
  }

  int m_winningScore;
  int m_boardSize;
  std::vector<std::uint64_t> m_states;
  std::uint64_t m_playerOneWins = 0;
  std::uint64_t m_playerTwoWins = 0;
};


int main()
{
  Die               die( 100 );
  //std::vector<Pawn> players = { Pawn( 4 ), Pawn( 8 ) };
  std::vector<Pawn> players = { Pawn( 1 ), Pawn( 10 ) };
  bool              gameEnded = false;
  //int               totalMoves = 0;

  while( !gameEnded )
  {
    for( auto &player : players )
    {
      if( player.move( die ) )
      {
        gameEnded = true;
        break;
      }
    }
  }

  int minScore = s_winningScore;
  for( const auto &player : players )
  {
    if( player.m_score < minScore )
    {
      minScore = player.m_score;
    }
  }

  std::cout << "minimumScore is " << minScore << ", die was rolled " << die.m_rolls << " times, product is " <<
    minScore * die.m_rolls << std::endl;

  //DiracState state( 4, 8 );
  DiracState state( 1, 10 );

  for( int i = 0; true; i++ )
  {
    if( !state.playerOneMove() )
    {
      break;
    }
    if( !state.playerTwoMove() )
    {
      break;
    }
    std::cout << "round " << i << " standing " << state.m_playerOneWins << " : " << state.m_playerTwoWins << std::endl;
  }

  std::cout << "Player one wins " << state.m_playerOneWins << std::endl;
  std::cout << "Player two wins " << state.m_playerTwoWins << std::endl;
}