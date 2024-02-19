// LetStatement.cpp:
#include "AddStatement.h"
using namespace std;

AddStatement::AddStatement(std::string variableName, std::string value)
        : m_variableName(variableName), m_value(value) {}
AddStatement::~AddStatement() {}

// The LetStatement version of execute() should make two changes to the
// state of the program:
//
//    * set the value of the appropriate variable
//    * increment the program counter
void AddStatement::execute(ProgramState* state, ostream& outf) {
    // TODO
    bool done = false;
    if (!(state->mapfind(m_variableName))) {  // if not in the map add
        // cout << m_variableName << " " << m_value << endl;
        std::string s = "0";
        state->mapadd(m_variableName, std::string("0"));
        // cout << state->mapget(m_variableName) << endl;
    }
    // TODO
    for (size_t i = 0; i < m_value.size(); i++) {  // checks if val is in map
        if ((m_value[i] - '0' > 9 || m_value[i] - '0' < 0) && !(i == 0 && m_value[i] == '-')) {
            int newval = (state->mapget(m_variableName)) + (state->mapget(m_value));  // adds
            // cout << newval << endl;
            state->mapset(m_variableName, newval);
            done = true;
            state->jump(state->getcurrentline() + 1);  // moves to next line of code
            return;
        }
    }
    if (!done) {
        int newval = (state->mapget(m_variableName)) + (stoi(m_value));  // adds
        // cout << newval << " " << stoi(m_value) << endl;
        state->mapset(m_variableName, newval);
    }
    state->jump(state->getcurrentline() + 1);  // moves to next line of code
}
