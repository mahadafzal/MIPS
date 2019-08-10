DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS= -DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ -std=c++11 $(CFLAGS) -c $<


DisASM: DisASM.o Instruction.o OpcodeTable.o RegisterTable.o MachLangParser.o
	g++ -o DisASM DisASM.o OpcodeTable.o MachLangParser.o RegisterTable.o Instruction.o

MachLangParser.o: MachLangParser.h OpcodeTable.h RegisterTable.h Instruction.h 

Instruction.o: OpcodeTable.h RegisterTable.h Instruction.h 

OpcodeTable.o: OpcodeTable.h 

RegisterTable.o: RegisterTable.h  

clean:
	/bin/rm -f DisASM *.o core


