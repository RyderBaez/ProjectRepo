// IfStatement.cpp:
#include "IfStatement.h"

IfStatement::IfStatement(std::string variableName, int value, std::string oper, int linenum)
        : m_variableName(variableName), m_value(value), m_operator(oper), m_linenum(linenum) {}
IfStatement::~IfStatement() {}

void IfStatement::execute(ProgramState* state, std::ostream& outf) {
    if (!(state->mapfind(m_variableName))) {  // checks if not established
        // cout << m_variableName << " " << m_value << endl;
        state->mapadd(m_variableName, std::string("0"));
    }
    if (m_operator == "=" || m_operator == "==") {  // equals statement
        if (state->mapget(m_variableName) == m_value) {
            if (m_linenum > state->getnumline()) {  // out of bounds
                outf << "Illegal jump instruction" << std::endl;
            }
            state->jump(m_linenum);  // goes to the index corresponding with the line number
        } else {

            state->jump(state->getcurrentline() + 1);  // moves to next line of code
        }
    } else if (m_operator == ">") {  // greater than
        if (state->mapget(m_variableName) > m_value) {
            if (m_linenum > state->getnumline()) {  // illegal jump
                outf << "Illegal jump instruction" << std::endl;
            }
            state->jump(m_linenum);  // goes to the index corresponding with the line number
        } else {
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
        }
    } else if (m_operator == "<") {  // less than
        if (state->mapget(m_variableName) < m_value) {
            if (m_linenum > state->getnumline()) {  // out of bounds
                outf << "Illegal jump instruction" << std::endl;
            }
            state->jump(m_linenum);  // goes to the index corresponding with the line number
        } else {
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
        }
    } else if (m_operator == ">=") {  // greater than or equal
        if (state->mapget(m_variableName) >= m_value) {
            if (m_linenum > state->getnumline()) {  // illegal jump
                outf << "Illegal jump instruction" << std::endl;
            }
            state->jump(m_linenum);  // goes to the index corresponding with the line number
        } else {
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
        }
    } else if (m_operator == "<=") {  // less than or equal
        if (state->mapget(m_variableName) <= m_value) {
            if (m_linenum > state->getnumline()) {  // illegal jump
                outf << "Illegal jump instruction" << std::endl;
            }
            state->jump(m_linenum);  // goes to the index corresponding with the line number
        } else {
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
        }
    } else if (m_operator == "<>") {  // greater than or less than
        if (state->mapget(m_variableName) < m_value || state->mapget(m_variableName) > m_value) {
            if (m_linenum > state->getnumline()) {  // illegal jump
                outf << "Illegal jump instruction" << std::endl;
            }
            state->jump(m_linenum);  // goes to the index corresponding with the line number
        } else {
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
        }
    }
}
