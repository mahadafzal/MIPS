#include "MachLangParser.h"
#include <string>
#include <bitset>


MachLangParser::MachLangParser(string input_file)
// Specify a text file containing MIPS assembly instructions. Function
// checks syntactic correctness of file and creates a list of Instructions.
{
  Instruction i;
  myFormatCorrect = true;
  int instructionCount = 0;

  ifstream in;
  in.open(input_file.c_str());

  if(in.bad()){

    myFormatCorrect = false;
  }

  else{

    string line;

    while( getline(in, line)){

      instructionCount++;

      // checks if line is longer than 32 bits
      if ((int)line.size() != thirtyTwo){

        myFormatCorrect = false;

        cout << "The instruction at line " + to_string(instructionCount) + " is longer than 32 bits." << endl;
        exit(1);
      }

      // checks if there are strings other than 1 or 0
      for (int j = 1; j < (int)line.size(); j++){

        if (line.at(j) == '0' || line.at(j) == '1'){

          continue;
        }

        else{

          myFormatCorrect = false;

          cout << "The 32 bit instruction at line " + to_string(instructionCount) + " is an incorrect binary instruction." << endl;

          exit(1);
        }
      }

      // set encoding for each line to print 32 bit instruction adjacent to the assembly instruction
      i.setEncoding(line);

      string opcode("");
      string funct("");
      int operand_count = 0;

      getTokens(line, opcode, funct, operand_count);

      Opcode o = opcodes.getOpcode(opcode, funct);

      // checks if the opcode provided is valid
      if (o == UNDEFINED){

        // invalid opcode specified
        cout << "The opcode field specified at line " + to_string(instructionCount) + " is invalid." << endl;
        myFormatCorrect = false;
        exit(1);
      }

      bool success = getOperands(i, o, operand_count);

      if(!success){

        myFormatCorrect = false;
        break;
      }

      string decoding = decode(i);
      i.setAssembly(decoding);
      myInstructions.push_back(i);
    }
  }

  myIndex = 0;
}

MachLangParser::~MachLangParser() 
// Destructor
{

}

Instruction MachLangParser::getNextInstruction()
// Iterator that returns the next Instruction in the list of Instructions.
{
  if(myIndex < (int)(myInstructions.size())){

    myIndex++;
    return myInstructions[myIndex-1];
  }

  Instruction i;
  return i;

}

void MachLangParser::getTokens(string line, string &opcode, string &funct, int &numOperands) 
// Given a line, opcode, function field, and numOperands, breaks the line down according to instruction
// types and converts the respective fields to integer representations. 
{

  // get binary string representation of opcode field and function fields
  opcode = line.substr(opc_start, opc_length);
  funct = line.substr(func_start, func_length);

  Opcode myOpcode = opcodes.getOpcode(opcode, funct);

  if (opcodes.getInstType(myOpcode) == RTYPE){

    getRTokens(line);
  }

  if (opcodes.getInstType(myOpcode) == JTYPE){

    getJTokens(line);
  }

  if (opcodes.getInstType(myOpcode) == ITYPE){

    if (opcodes.getName(myOpcode) == "beq"){

      getBranchITokens(line);
    }

    else{

      getITokens(line);
    }
  }
}

void MachLangParser::getRTokens(string line)
// Given a line, breaks the string into respective binary fields and converts them to decimal. 
{
  rs_bin = line.substr(rs_startR, reg_length);
  rt_bin = line.substr(rt_startR, reg_length);
  rd_bin = line.substr(rd_startR, reg_length);
  imm_bin = line.substr(imm_startR, reg_length);

  bitset<5> rs_value (rs_bin);
  bitset<5> rt_value (rt_bin);
  bitset<5> rd_value (rd_bin);
  bitset<5> imm_value (imm_bin);

  rs_Dec = rs_value.to_ulong();
  rt_Dec = rt_value.to_ulong();
  rd_Dec = rd_value.to_ulong();
  imm_Dec = imm_value.to_ulong();
}

void MachLangParser::getJTokens(string line)
// Given a line, breaks the string into respective binary fields and converts them to decimal. 
{
  string imm_bin = line.substr(imm_startJ,imm_lengthJ) + offset;
  bitset<26> imm_value (imm_bin);

  imm_Dec = (imm_value.to_ulong())*multiplier;
}

void MachLangParser::getBranchITokens(string line)
// Given a line, breaks the string into respective binary fields and converts them to decimal. 
{
  string rs_bin = line.substr(rs_startI,reg_length);
  string rt_bin = line.substr(rt_startI,reg_length);
  string imm_bin = line.substr(imm_startI,imm_lengthI) + offset;

  bitset<5> rs_value (rs_bin);
  bitset<5> rt_value (rt_bin);

  rs_Dec = rs_value.to_ulong();
  rt_Dec = rt_value.to_ulong();
  imm_Dec = (convertBinToInt(imm_bin))*multiplier;
}

void MachLangParser::getITokens(string line)
// Given a line, breaks the string into respective binary fields and converts them to decimal. 
{
  string rs_bin = line.substr(rs_startI,reg_length);
  string rt_bin = line.substr(rt_startI,reg_length);
  string imm_bin = line.substr(imm_startI,imm_lengthI);

  bitset<5> rs_value (rs_bin);
  bitset<5> rt_value (rt_bin);

  rs_Dec = rs_value.to_ulong();
  rt_Dec = rt_value.to_ulong();
  imm_Dec = convertBinToInt(imm_bin);
}

bool MachLangParser::getOperands(Instruction &i, Opcode o, int operand_count)
// Given an instruction, Opcode and number of operands, breaks apart the fields in the Opcode template accordingly and
// sets them in an Instruction instance. 
{
  int rs, rt, rd;
  rs = rt = rd = NumRegisters;
  int imm = 0;

  // get positions and type according to the Opcode 
  int rs_p = opcodes.RSposition(o);
  int rt_p = opcodes.RTposition(o);
  int rd_p = opcodes.RDposition(o);
  int imm_p = opcodes.IMMposition(o);

  InstType insType = opcodes.getInstType(o);

  // Break the token down differently according to the type. Checks if register position is non-existent. 
  if (insType == RTYPE){

    if (rs_p != -1){

      rs = registers.getNum(rs_Dec);
      if (rs == NumRegisters)
        return false;
    }

    if (rt_p != -1){

      rt = registers.getNum(rt_Dec);
      if (rt == NumRegisters)
        return false;
    }
    
    if (rd_p != -1){

      rd = registers.getNum(rd_Dec);
      if (rd == NumRegisters)
        return false;
    }

    if (imm_p != -1){

      imm = imm_Dec;
    }
  }

  if (insType == JTYPE){

    imm = imm_Dec;
  }

  if (insType == ITYPE){

    if (rs_p != -1){

      rs = registers.getNum(rs_Dec);
      if ( rs == NumRegisters)
        return false;
    }

    if (rt_p != -1){

      rt = registers.getNum(rt_Dec);
      if ( rt == NumRegisters)
        return false;
    }

    if (imm_p != -1){

      imm = imm_Dec;
    }
  } 

  // store the values in an Instruction instance 
  i.setValues(o, rs, rt, rd, imm);

  return true;
}

string MachLangParser::decode(Instruction i)
// Given a valid 32 bit MIPS instruction, calls the respective decode method according to 
// instruction type and returns the assembly representation in the form of a string.
{
  // Opcode table for reference 
  OpcodeTable* table = new OpcodeTable;
  Opcode myOpcode = i.getOpcode();

  if (table-> getInstType(myOpcode) == RTYPE){

    return decodeR(i);
  }

  if (table-> getInstType(myOpcode) == JTYPE){

    return decodeJ(i);
  }

  if (table-> getInstType(myOpcode) == ITYPE){

    return decodeI(i);
  }

  else{

    //Undefined
  }

  return "";
}

string MachLangParser::decodeR(Instruction i)
// Given an instruction, returns the assembly instruction representation in the form of a string.
{

  OpcodeTable* table = new OpcodeTable;
  Opcode myOpcode = i.getOpcode();

  string DisASM = "";
  DisASM += (table-> getName(myOpcode) + "\t");

  for(int w = 0; w < table-> numOperands(myOpcode); w++){

    if(w == table-> RDposition(myOpcode)){

      DisASM += "$";
      DisASM += to_string(i.getRD());
    }

    if(w == table-> IMMposition(myOpcode)){

      DisASM += to_string(i.getImmediate());
    }

    if(w == table-> RSposition(myOpcode)){

      DisASM += "$";
      DisASM += to_string(i.getRS());
    }

    if(w == table-> RTposition(myOpcode)){

      DisASM += "$";
      DisASM += to_string(i.getRT());
    }

      // adding commas iteratively
    if(w < table-> numOperands(myOpcode)-1){

      DisASM += ", ";
    }
  }

  return DisASM;
}

string MachLangParser::decodeJ(Instruction i)
// Given an instruction, returns the assembly instruction representation in the form of a string.
{

  string DisASM = "";

  OpcodeTable* table = new OpcodeTable;
  Opcode myOpcode = i.getOpcode();

  DisASM += (table-> getName(myOpcode) + "\t");

  DisASM += toHex(i.getImmediate());

  return DisASM;
}

string MachLangParser::decodeI(Instruction i)
// Given an instruction, returns the assembly instruction representation in the form of a string.
{
  string DisASM = "";

  OpcodeTable* table = new OpcodeTable;
  Opcode myOpcode = i.getOpcode();

  DisASM += (table-> getName(myOpcode) + "\t");

  // checks if it is a load instruction 
  if (table-> IMMposition(myOpcode) == 1){

    for(int w = 0; w < table-> numOperands(myOpcode); w++){

      if(w == table-> RSposition(myOpcode)){

        DisASM += "(";
        DisASM += "$";
        DisASM += to_string(i.getRS());
        DisASM += ")";
      }

      if(w == table-> RTposition(myOpcode)){

        DisASM += "$";
        DisASM += to_string(i.getRT());
        DisASM += ", ";
      }

      if(w == table-> IMMposition(myOpcode)){

        DisASM += to_string(i.getImmediate());
      }
    } 
  }

  else if(table-> IMMposition(myOpcode) == 2){

    for(int w = 0; w < table-> numOperands(myOpcode); w++){

      if(w == table-> RSposition(myOpcode)){

        DisASM += "$";
        DisASM += to_string(i.getRS());
      }

      if(w == table-> RTposition(myOpcode)){

        DisASM += "$";
        DisASM += to_string(i.getRT());
      }
      if(w == table-> IMMposition(myOpcode)){

          // if it is branch equal to instruction, convert imm value to hexadecimal before adding to string
        if (table-> getName(myOpcode) == "beq"){

          DisASM += toHex(i.getImmediate());
        }

        else{

          DisASM += to_string(i.getImmediate());
        }
      }

        // add commas iteratively
      if(w < table-> numOperands(myOpcode)-1){

        DisASM += ", ";
      }
    }
  }

  return DisASM;
}

string MachLangParser::toHex(int dec)
// Given an integer, returns the Hexademical string representation. 
{
  stringstream strstream;
  strstream<<"0x"<<hex<<dec;
  string cnvt(strstream.str());
  return cnvt;
}

int MachLangParser::convertBinToInt(string bin)
// Given a binary string representation, assumes it is in two's complement and returns the decimal
// representation of the binary number. 
{
  // result to be returned
  int result = 0;

  // magnitude for decimal representation. Simply returns the integer if the number is positive.
  int magnitude = 0;

  // if positive value, do not go through process of negative number conversion
  if (bin.at(0) == '0'){

    bitset<16> mag_value (bin);
    result = mag_value.to_ulong();
    return result;
  }

  // convert from two's complement to decimal representation
  else{

    // flip bits 
    for(int i = 0; i < (int)bin.length(); i++){

      if (bin.at(i) == '1'){

        bin.at(i) = '0';
      }

      else if(bin.at(i) == '0'){

        bin.at(i) = '1';
      }
    } 

    int count = 0;

     // add 1
    for(int j = (int)bin.length() - 1; j > count; j--){

     if (bin.at(j) == '0'){

       bin.at(j) = '1';

       break;
     }
     else{

      bin.at(j) = '0';
    } 

    count++;
  }

     // convert to negative int in order to return 

  bitset<16> dec_bin (bin);

  magnitude = dec_bin.to_ulong();

  result = (-1)*magnitude;
}

return result;
}
