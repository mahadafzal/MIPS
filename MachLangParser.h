#ifndef __MACHLANGPARSER_H__
#define __MACHLANGPARSER_H__

using namespace std;

#include <iostream>
#include <fstream>
#include "Instruction.h"
#include "RegisterTable.h"
#include "OpcodeTable.h"
#include <math.h>
#include <vector>
#include <sstream>
#include <stdlib.h>

/* This class reads in a machine language representation of a MIPS
 * program.  If the file is correctly formatted (i.e. every line
 * contains a properly formatted 32 bit machine language/binary
 * representation of a supported MIPS instruction, this
 * class will retain a list of Instructions (one for each machine
 * language instruction from the file).  This list of Instructions
 * can be iterated through.
 */
class MachLangParser{
public:

  // Specify a text file containing machine language/binary representations
  // of MIPS instructions.  This constructor checks the correctness of 
  // each line of the file to see if it is a 32 bit binary sequence that
  // represents a supported MIPS instruction.  If it does, a list of
  // Instructions is created and stored internally.  If it does not, then
  // the class will indicate that the file is incorrectly formatted.
  //
  MachLangParser(string input_file);

  // Destructor
  ~MachLangParser();

  // Returns true if the file specified was a valid/correct MIPS instruction
  // file.  Otherwise, returns false.
  bool isFormatCorrect() { return myFormatCorrect; };
  // Returns true if the line of the file has size of 32 bits and all of them
  // are 0's and 1's.

  // Iterator that returns the next Instruction in the list of Instructions.
  Instruction getNextInstruction();

private:
  vector<Instruction> myInstructions;      // list of Instructions
  int myIndex;                             // iterator index
  bool myFormatCorrect;                     

  RegisterTable registers;                 // encodings for registers
  OpcodeTable opcodes;                     // encodings of opcodes

  // Helper integers and strings

  string rs_bin;                           // String representation of rs binary field
  string rt_bin;                           // String representation of rt binary field
  string rd_bin;                           // String representation of rd binary field
  string imm_bin;                          // String representation of immediate binary field

  int rs_Dec;                              // Decimal representation of rs register
  int rt_Dec;                              // Decimal representation of rt register
  int rd_Dec;                              // Decimal representation of rd register
  int imm_Dec;                             // Decimal representation of immediate field

  // Constants

  const int thirtyTwo = 32;
  const int multiplier = 4;
  const string offset = "00";

  const int opc_start = 0;
  const int opc_length = 6;
  const int func_start = 26;
  const int func_length = 6;
  const int reg_length = 5;

  const int rs_startR = 6;
  const int rt_startR = 11;
  const int rd_startR = 16;
  const int imm_startR = 21;

  const int imm_startJ = 6;
  const int imm_lengthJ = 26;

  const int rs_startI = 6;
  const int rt_startI = 11;
  const int imm_startI = 16;
  const int imm_lengthI = 16;
  
  // Decomposes a line of 32 bit machine instruction into opcode field, funct field, and number of operands
  void getTokens(string line, string &opcode, string &funct, int &num_operands);

  void getRTokens(string line);

  void getJTokens(string line);

  void getBranchITokens(string line);

  void getITokens(string line);

  // Given an Opcode and number of operands, breaks apart the fields in the Opcode template accordingly and
  // stores them in an Instruction instance. 
  bool getOperands(Instruction &i, Opcode o, int operand_count);

  // Given a valid 32 bit MIPS instruction, returns a string representing the MIPS assembly representation
  // of that instruction.
  string decode(Instruction i);

  string decodeR(Instruction i);

  string decodeJ(Instruction i);

  string decodeI(Instruction i);

  // Given an integer, returns the hexademical string representation. 
  string toHex(int dec);

  // Given a binary string representation, assumes it is in two's complement and returns the decimal
  // representation of the binary number. 
  int convertBinToInt(string bin);

};

#endif
