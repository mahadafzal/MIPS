# MIPS
A C++ Simulation of the MIPS Processor which converts binary machine language instructions to MIPS Instructions.

Overview: The program reads in a MIPS machine instruction file as a command line argument which would contain a 32 bit instruction per line. If the file is formatted according to the conditions specified, the program decodes the 32 bit representation of the MIPS instructions and prints the corresponding MIPS assembly instructions. 

The program consists of four classes - MachLangParser, Instruction, OpcodeTable, and RegisterTable - along with a driver file named DisASM.cpp.

The program responds to an incorrect file and if the overall format is correct, specifies which line is incorrect. If the format of the whole file is wrong, it will terminate and prompt the user that it is incorrect. If no file is specified, then again the user will be prompted to provide one.

Description of Files:

OpcodeTable: The class contains a template for storing information of different types of instructions. The information is stored in an enum and have different fields including opcode field, function field, name, and so on. Methods are provided for implementation and access to this information. Adding new instructions would be simple as most conditions are handled and the convention would have to be followed in the enum to add information for another instruction.

RegisterTable: This class stores information about the valid registers and their names for MIPS ISA. It corresponds integers to register names and to their register numbers. Methods are made available for access to both the names and numbers of the registers.

Instruction: For a specific instruction, this class holds information according to the template designated for it. Given the existence or absence of fields, an Instruction instance would hold information for opcode field, registers, immediate fields, and function fields. They can be accessed using the getter methods present in the class. 

MachLangParser: This is where the program reads in a file and checks it for correctness. Once that is established, it iterates through the lines of the instruction, converting them to assembly instructions and then providing a decoded assembly instruction for each 32 bit representation. 

Organization: 

The driver file will pass a name of a file to the MachLangParser class. After checking that a file is correct, the MachLangParser class utilizes the RegisterTable class and the OpcodeTable class to determine if only valid representations have been passed. The opcode field and func field are the main pieces of information used to determine which instruction the line represents, after which an Opcode is found and the instruction formed. Additionally, the helper methods in the RegisterTable and OpcodeTable class help allow the MachLangParser class to compare the tokens it breaks down to the information stored, and then return a string representation according to the alignments and fields of the instructions. The MachLangParser class will, for each line, break down the 32 bit instruction into various fields, create an instance of Instruction and store the values it receives according to the specific instruction, aligning a string according to the positions of the field and returning a string assembly representation. It will iterate through line by line, checking for correctness of each line, and repeating the process.
