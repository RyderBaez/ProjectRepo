// LetStatement.cpp:
#include "DivStatement.h"
using namespace std;

DivStatement::DivStatement(std::string variableName, std::string value)
        : m_variableName(variableName), m_value(value) {}
DivStatement::~DivStatement() {}

// The LetStatement version of execute() should make two changes to the
// state of the program:
//
//    * set the value of the appropriate variable
//    * increment the program counter
void DivStatement::execute(ProgramState* state, ostream& outf) {
    // TODO
    bool done = false;
    // TODO
    if (!(state->mapfind(m_variableName))) {  // checks if newvalue
        state->mapadd(m_variableName, std::string("0"));
    }
    for (size_t i = 0; i < m_value.size(); i++) {
        if ((m_value[i] - '0' > 9 || m_value[i] - '0' < 0) && !(i == 0 && m_value[i] == '-')) {  // checks for all
                                                                                                 // digits
            if (!(state->mapfind(m_value))) {  // ends if divid by 0
                outf << "Divide by 0 exception" << endl;
                state->jump(state->getnumline() + 1);
                return;
            }
            if (state->mapget(m_value) == 0) {  // ends if divid by 0
                outf << "Divide by 0 exception" << endl;
                state->jump(state->getnumline() + 1);
                return;
            }
            int newval = state->mapget(m_variableName) / state->mapget(m_value);  // if a variable
            // cout << newval << endl;
            state->mapset(m_variableName, newval);
            done = true;
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
            return;
        }
    }
    if (!done) {
        if (stoi(m_value) == 0) {  // ends if divid by 0
            // cout << stoi(m_value) << endl;
            outf << "Divide by 0 exception" << endl;
            state->jump(state->getnumline() + 1);
            return;
        }
        // cout << stoi(m_value) << endl;
        int newval = (state->mapget(m_variableName)) / (stoi(m_value));  // if num divid
        // cout << newval << endl;
        state->mapset(m_variableName, newval);
    }
    state->jump(state->getcurrentline() + 1);  // moves to next line of code
}
