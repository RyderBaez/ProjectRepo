#include "ProgramState.h"

#include <iostream>
#include <sstream>
ProgramState::ProgramState(int numlines) { m_numLines = numlines; }
int ProgramState::mapget(std::string value) {
    return values[value];  // returns value
}
void ProgramState::mapprintall(std::ostream& outf) {
    for (std::map<std::string, int>::iterator iter = values.begin(); iter != values.end(); ++iter) {
        outf << iter->first << " " << iter->second << std::endl;  // sends all to file
    }
}
void ProgramState::mapset(std::string value, int newvalue) { values[value] = newvalue; }
void ProgramState::mapset(std::string value, std::string newvalue) {
    for (size_t i = 0; i < newvalue.size(); i++) {
        if ((newvalue[i] - '0' > 9 || newvalue[i] - '0' < 0) && !(i == 0 && newvalue[i] == '-')) {
            values[value] = values[newvalue];  // checks if another value
            break;
        }
    }
    values[value] = stoi(newvalue);  // uses number
}
bool ProgramState::mapfind(std::string value) {
    if (values.find(value) == values.end())  // if not in return false
        return false;
    return true;  // otherwise return true
}
void ProgramState::mapadd(std::string value, std::string num) {
    for (size_t i = 0; i < num.size(); i++) {
        if ((num[i] - '0' > 9 || num[i] - '0' < 0) && !((i == 0) && num[i] == '-')) {  // checks if a int
            // std::cout << num << std::endl;
            if (mapfind(num)) {
                values.insert(std::make_pair(value, values[num]));  // adds value from map
            } else {
                values.insert(std::make_pair(value, 0));  // makes new value
            }
            return;
        }
    }
    // std::cout << stoi(num) << std::endl;
    values.insert(std::make_pair(value, stoi(num)));  // converts to an int and adds
}
void ProgramState::jump(int jumppnt) {
    curline = jumppnt;  // jumps
}
void ProgramState::jumpsetreturn(int jumppnt) {
    returns.push(curline + 1);  // adds a return pnt
    curline = jumppnt;
}
int ProgramState::getreturnpnt() {
    if (!returns.empty()) {
        int returnpnt = returns.top();  // jumps back to return pnt
        // std::cout << returnpnt << std::endl;
        returns.pop();
        return returnpnt;
    }
    return m_numLines + 1;  // ends if nothing to return to when return called
}
int ProgramState::getcurrentline() { return curline; }
int ProgramState::getnumline() { return m_numLines; }