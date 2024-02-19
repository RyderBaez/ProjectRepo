// ProgramState.h
//
// CS 104
//
// The ProgramState class encapsulates the state of an executing Facile
// program.  The state of a BASIC program consists of three parts:
//
//    * The program counter, which specifies the line number of the
//      statement that will execute next.
//    * A map, as explained by the problem writeup.
//    * A stack of integers, which is used to hold return lines for GOSUB
//      statements.
//
// We've provided you a start on this class, but you'll need to add methods
// to it as you go along.  In particular, you'll need methods to access and
// modify the state; these methods will be called primarily by the various
// execute() methods in the Statement subclasses, to allow the execution of
// a statement to change the state of a program.  For example, executing a
// GOTO statement will cause the program counter to be changed.

#ifndef PROGRAM_STATE_INCLUDED
#define PROGRAM_STATE_INCLUDED
#include <iostream>
#include <map>
#include <stack>
#include <string>
class ProgramState {
public:
    ProgramState(int numLines);
    int mapget(std::string value);
    void mapprintall(std::ostream& outf);
    void mapset(std::string value, int newvalue);
    void mapset(std::string value, std::string newvalue);
    bool mapfind(std::string value);
    void jump(int jumppnt);
    void jumpsetreturn(int jumppnt);
    void mapadd(std::string value, std::string num);
    int getreturnpnt();
    int getcurrentline();
    int getnumline();
    // You'll need to add a variety of methods here.  Rather than trying to
    // think of what you'll need to add ahead of time, add them as you find
    // that you need them.

private:
    int m_numLines;
    std::map<std::string, int> values;
    std::stack<int> returns;
    int curline = 1;
};

#endif
