
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


typedef std::int64_t register_t;

const char *c_registerStrings[4] = { "W", "X", "Y", "Z" };

struct AluState
{
  AluState() = default;
  AluState( const AluState & ) = default;

  AluState &operator=( const AluState & ) = default;

  register_t m_registers[4] = { 0, 0, 0, 0 };
  int        m_pc = 0;
};


class Operator
{
  public:
  virtual bool isInputOperator() const
  {
    return false;
  }

  virtual bool apply( AluState & ) const
  {
    return false;
  }

  virtual bool input( AluState &, register_t ) const
  {
    return false;
  }

  virtual void print() const = 0;

  protected:
  Operator( int destinationRegister)
    : m_destinationRegister( destinationRegister )
  { }

  int m_destinationRegister = 0;
};


class InputOperator : public Operator
{
  public:
  InputOperator( int destinationRegister )
    : Operator( destinationRegister )
  { }

  bool isInputOperator() const override
  {
    return true;
  }

  bool input( AluState &state, register_t value ) const override
  {
    state.m_registers[m_destinationRegister] = value;
    return true;
  }

  void print() const override
  {
    std::cout << "INP  " << c_registerStrings[m_destinationRegister] << std::endl;
  }
};


class AddOperator : public Operator
{
  public:
  AddOperator( int destinationRegister, int sourceRegister )
    : Operator( destinationRegister )
    , m_sourceRegister( sourceRegister )
  { }

  bool apply( AluState &state ) const override
  {
    state.m_registers[m_destinationRegister] += state.m_registers[m_sourceRegister];
    return true;
  }

  void print() const override
  {
    std::cout << "ADD  " << c_registerStrings[m_destinationRegister] << "  " <<
      c_registerStrings[m_sourceRegister] << std::endl;
  }

  private:
  int m_sourceRegister;
};


class AddImmediateOperator : public Operator
{
  public:
  AddImmediateOperator( int destinationRegister, register_t value )
    : Operator( destinationRegister )
    , m_value( value )
  {}

  bool apply( AluState &state ) const override
  {
    state.m_registers[m_destinationRegister] += m_value;
    return true;
  }

  void print() const override
  {
    std::cout << "ADD  " << c_registerStrings[m_destinationRegister] << "  " << m_value << std::endl;
  }

  private:
  register_t m_value;
};


class MulOperator : public Operator
{
  public:
  MulOperator( int destinationRegister, int sourceRegister )
    : Operator( destinationRegister )
    , m_sourceRegister( sourceRegister )
  {}

  bool apply( AluState &state ) const override
  {
    state.m_registers[m_destinationRegister] *= state.m_registers[m_sourceRegister];
    return true;
  }

  void print() const override
  {
    std::cout << "MUL  " << c_registerStrings[m_destinationRegister] << "  " <<
      c_registerStrings[m_sourceRegister] << std::endl;
  }

  private:
  int m_sourceRegister;
};


class MulImmediateOperator : public Operator
{
  public:
  MulImmediateOperator( int destinationRegister, register_t value )
    : Operator( destinationRegister )
    , m_value( value )
  {}

  bool apply( AluState &state ) const override
  {
    state.m_registers[m_destinationRegister] *= m_value;
    return true;
  }

  void print() const override
  {
    std::cout << "MUL  " << c_registerStrings[m_destinationRegister] << "  " << m_value << std::endl;
  }

  private:
  register_t m_value;
};


class DivOperator : public Operator
{
  public:
  DivOperator( int destinationRegister, int sourceRegister )
    : Operator( destinationRegister )
    , m_sourceRegister( sourceRegister )
  {}

  bool apply( AluState &state ) const override
  {
    if( state.m_registers[m_sourceRegister] == 0 )
    {
      return false;
    }
    state.m_registers[m_destinationRegister] /= state.m_registers[m_sourceRegister];
    return true;
  }

  void print() const override
  {
    std::cout << "DIV  " << c_registerStrings[m_destinationRegister] << "  " <<
      c_registerStrings[m_sourceRegister] << std::endl;
  }

  private:
  int m_sourceRegister;
};


class DivImmediateOperator : public Operator
{
  public:
  DivImmediateOperator( int destinationRegister, register_t value )
    : Operator( destinationRegister )
    , m_value( value )
  {}

  bool apply( AluState &state ) const override
  {
    if( m_value == 0 )
    {
      return false;
    }
    state.m_registers[m_destinationRegister] /= m_value;
    return true;
  }

  void print() const override
  {
    std::cout << "DIV  " << c_registerStrings[m_destinationRegister] << "  " << m_value << std::endl;
  }

  private:
  register_t m_value;
};


class ModOperator : public Operator
{
  public:
  ModOperator( int destinationRegister, int sourceRegister )
    : Operator( destinationRegister )
    , m_sourceRegister( sourceRegister )
  {}

  bool apply( AluState &state ) const override
  {
    if( ( state.m_registers[m_destinationRegister] < 0 ) || ( state.m_registers[m_sourceRegister] <= 0 ) )
    {
      return false;
    }
    state.m_registers[m_destinationRegister] %= state.m_registers[m_sourceRegister];
    return true;
  }

  void print() const override
  {
    std::cout << "MOD  " << c_registerStrings[m_destinationRegister] << "  " <<
      c_registerStrings[m_sourceRegister] << std::endl;
  }

  private:
  int m_sourceRegister;
};


class ModImmediateOperator : public Operator
{
  public:
  ModImmediateOperator( int destinationRegister, register_t value )
    : Operator( destinationRegister )
    , m_value( value )
  {}

  bool apply( AluState &state ) const override
  {
    if( ( state.m_registers[m_destinationRegister] < 0 ) || ( m_value <= 0 ) )
    {
      return false;
    }
    state.m_registers[m_destinationRegister] %= m_value;
    return true;
  }

  void print() const override
  {
    std::cout << "MOD  " << c_registerStrings[m_destinationRegister] << "  " << m_value << std::endl;
  }

  private:
  register_t m_value;
};


class EqlOperator : public Operator
{
  public:
  EqlOperator( int destinationRegister, int sourceRegister )
    : Operator( destinationRegister )
    , m_sourceRegister( sourceRegister )
  {}

  bool apply( AluState &state ) const override
  {
    state.m_registers[m_destinationRegister] = 
      ( state.m_registers[m_destinationRegister] == state.m_registers[m_sourceRegister] ) ? 1 : 0;
    return true;
  }

  void print() const override
  {
    std::cout << "EQL  " << c_registerStrings[m_destinationRegister] << "  " <<
      c_registerStrings[m_sourceRegister] << std::endl;
  }

  private:
  int m_sourceRegister;
};


class EqlImmediateOperator : public Operator
{
  public:
  EqlImmediateOperator( int destinationRegister, register_t value )
    : Operator( destinationRegister )
    , m_value( value )
  {}

  bool apply( AluState &state ) const override
  {
    state.m_registers[m_destinationRegister] =( state.m_registers[m_destinationRegister] == m_value ) ? 1 : 0;
    return true;
  }

  void print() const override
  {
    std::cout << "EQL  " << c_registerStrings[m_destinationRegister] << "  " << m_value << std::endl;
  }

  private:
  register_t m_value;
};


void addInstruction( std::vector<Operator*> &program, const std::string &line )
{
  std::string command = line.substr( 0, 3 );
  int         destination = line[4] - 'w';

  if( command == "inp" )
  {
    program.push_back( new InputOperator( destination ) );
    return;
  }

  bool isImmediate;
  register_t value = 0;
  int source = 0;

  if( line[6] < 'w' )
  {
    isImmediate = true;
    value = std::stoi( line.substr( 6 ) );
  }
  else
  {
    isImmediate = false;
    source = line[6] - 'w';
  }

  if( command == "add" )
  {
    if( isImmediate )
    {
      program.push_back( new AddImmediateOperator( destination, value ) );
    }
    else
    {
      program.push_back( new AddOperator( destination, source ) );
    }
  }
  if( command == "mul" )
  {
    if( isImmediate )
    {
      program.push_back( new MulImmediateOperator( destination, value ) );
    }
    else
    {
      program.push_back( new MulOperator( destination, source ) );
    }
  }
  if( command == "div" )
  {
    if( isImmediate )
    {
      program.push_back( new DivImmediateOperator( destination, value ) );
    }
    else
    {
      program.push_back( new DivOperator( destination, source ) );
    }
  }
  if( command == "mod" )
  {
    if( isImmediate )
    {
      program.push_back( new ModImmediateOperator( destination, value ) );
    }
    else
    {
      program.push_back( new ModOperator( destination, source ) );
    }
  }
  if( command == "eql" )
  {
    if( isImmediate )
    {
      program.push_back( new EqlImmediateOperator( destination, value ) );
    }
    else
    {
      program.push_back( new EqlOperator( destination, source ) );
    }
  }
}


//
// UNUSED
//
// Does a brute force search on 22.876.792.454.961 combinations...
//
void runProgram( const std::vector<Operator *> &program, AluState &state, std::uint64_t &inputData )
{
  //std::cout << "entered loop at pc = " << state.m_pc << " with input data " << inputData << std::endl;

  while( state.m_pc < program.size() )
  {
    auto instruction = program[state.m_pc++];

    if( instruction->isInputOperator() )
    {
      std::uint64_t savedInput = inputData;

      for( int i = 9; i > 0; --i )
      {
        AluState childState( state );
        inputData = 10 * savedInput + i;

        instruction->input( childState, i );
        runProgram( program, childState, inputData );
        if( childState.m_registers[3] == 0 )
        {
          std::cout << "Found result at calling pc = " << state.m_pc <<
            " , calling input data " << savedInput << std::endl;
          state = childState;
          break;
        }
        else
        {
          inputData = savedInput;
        }
      }
    }
    else
    {
      if( !instruction->apply( state ) )
      {
        std::cout << "ERROR: pc = " << state.m_pc << ", reg = [ " << state.m_registers[0] << " , " <<
          state.m_registers[1] << " , " << state.m_registers[2] << " , " << state.m_registers[3] << " ]" << std::endl;
        std::cout << "  instruction: ";
        instruction->print();
      }
    }
  }
}


void runProgramWithInput( const std::vector<Operator *> &program,const std::string &input )
{
  //std::cout << "entered loop at pc = " << state.m_pc << " with input data " << inputData << std::endl;
  AluState state;
  int inputIndex = 0;

  while( state.m_pc < program.size() )
  {
    auto instruction = program[state.m_pc++];

    if( instruction->isInputOperator() )
    {
      instruction->input( state, input[inputIndex++] - '0' );
    }
    else
    {
      if( !instruction->apply( state ) )
      {
        std::cout << "ERROR: pc = " << state.m_pc << ", reg = [ " << state.m_registers[0] << " , " <<
          state.m_registers[1] << " , " << state.m_registers[2] << " , " << state.m_registers[3] << " ]" << std::endl;
        std::cout << "  instruction: ";
        instruction->print();
      }
    }
  }
  std::cout << "result: for input " << input << ": reg = [ " << state.m_registers[0] << " , " <<
    state.m_registers[1] << " , " << state.m_registers[2] << " , " << state.m_registers[3] << " ]" << std::endl;
}


int main()
{
  std::ifstream          input( R"(..\Day24\input.txt)" );
  std::string            textLine;
  std::vector<Operator*> program;

  while( std::getline( input, textLine ) )
  {
    addInstruction( program, textLine );
  }

  //for( const auto &instruction : program )
  //{
  //  instruction->print();
  //}

  // largest number (see analyzedInput.txt for relations)
  runProgramWithInput( program, "95299897999897");

  // smallest number (see analyzedInput.txt for relations)
  runProgramWithInput( program, "31111121382151" );

  for( auto instruction : program )
  {
    delete instruction;
  }
  return 0;
}