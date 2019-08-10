#ifndef _REGISTERTABLE_H
#define _REGISTERTABLE_H

#include <string>

using namespace std;


typedef int Register;
const int NumRegisters = 32;

// Each register has a number and a string name
struct RegisterEntry{
  string name;
  Register number;
};

/* This class stores information about the valid register names for MIPS.
 */
class RegisterTable{
public:
  // Constructs and initializes the valid register names and numbers
  RegisterTable();

  // Given an int representing a MIPS register operand, returns the number associated
  // with that register.  If string is not a valid register, returns NumRegisters.
  Register getNum(int reg);

  // Given an int representing a MIPS register operand, returns the name associated
  // with that register. If string is not a valid register, returns NULL.
  string getName(int regi);

private:
  RegisterEntry myRegisters[64];
  const string null = "";

};


#endif
