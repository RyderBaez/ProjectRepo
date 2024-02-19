// LetStatement.h
//
// CS 104
//
// The LetStatement class encapsulates a LET statement in a BASIC program.
// A LET statement looks like this:
//
//     LET A 3
//
// Primarily, it consists of two things: a variable name and an integer value.
// So, a LetStatement object contains a string and an integer, which is the
// important information contained in a LET statement.
//
// You'll need to similarly define subclasses for each of the other kinds of
// statements in a BASIC program.

#ifndef DIV_STATEMENT_INCLUDED
#define DIV_STATEMENT_INCLUDED

#include "ProgramState.h"
#include "Statement.h"

class DivStatement : public Statement {
private:
    std::string m_variableName;
    std::string m_value;

public:
    DivStatement(std::string variableName, std::string value);
    ~DivStatement();
    virtual void execute(ProgramState* state, std::ostream& outf);
};

#endif
