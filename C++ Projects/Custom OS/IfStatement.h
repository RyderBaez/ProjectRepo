#ifndef IF_STATEMENT_INCLUDED
#define IF_STATEMENT_INCLUDED

#include "Statement.h"
#include <iostream>

class IfStatement : public Statement {
private:
    std::string m_variableName;
    int m_value;
    std::string m_operator;
    int m_linenum;

public:
    IfStatement(std::string variableName, int value, std::string oper, int linenum);
    ~IfStatement();
    virtual void execute(ProgramState* state, std::ostream& outf);
};

#endif
