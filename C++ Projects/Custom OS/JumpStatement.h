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

#ifndef JUMP_STATEMENT_INCLUDED
#define JUMP_STATEMENT_INCLUDED

#include "ProgramState.h"
#include "Statement.h"

class JumpStatement : public Statement {
private:
    int value;

public:
    JumpStatement(int value);
    ~JumpStatement();
    virtual void execute(ProgramState* state, std::ostream& outf);
};

#endif
