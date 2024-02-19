// Interpreter.cpp
#include "AddStatement.h"
#include "DivStatement.h"
#include "EndStatement.h"
#include "IfStatement.h"
#include "JumpStatement.h"
#include "JumpSubStatement.h"
#include "LetStatement.h"
#include "MultStatement.h"
#include "PrintAllStatement.h"
#include "PrintStatement.h"
#include "ReturnStatement.h"
#include "Statement.h"
#include "SubStatement.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// parseProgram() takes a filename as a parameter, opens and reads the
// contents of the file, and returns an vector of pointers to Statement.
void parseProgram(istream& inf, vector<Statement*>& program);

// parseLine() takes a line from the input file and returns a Statement
// pointer of the appropriate type.  This will be a handy method to call
// within your parseProgram() method.
Statement* parseLine(string line);

// interpretProgram() reads a program from the given input stream
// and interprets it, writing any output to the given output stream.
// Note:  you are required to implement this function!
void interpretProgram(istream& inf, ostream& outf);

int main() {
    cout << "Enter BASIC program file name: ";
    string filename;
    getline(cin, filename);
    ifstream infile(filename.c_str());
    if (!infile) {
        cout << "Cannot open " << filename << "!" << endl;
        return 1;
    }
    interpretProgram(infile, cout);
}

void parseProgram(istream& inf, vector<Statement*>& program) {
    program.push_back(NULL);

    string line;
    while (!inf.eof()) {
        getline(inf, line);
        program.push_back(parseLine(line));
    }
}

Statement* parseLine(string line) {
    Statement* statement;
    stringstream ss;
    string type;
    string var;
    string val;
    int integer;
    int integer2;  // only used for if

    ss << line;
    ss >> type;
    ss >> type;

    // cout << line << " " << type << endl;
    // cout << type << endl;
    if (type == "LET") {
        ss >> var;
        ss >> val;
        // cout << var << " " << val << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "LET" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new LetStatement(var, val);
    } else if (type == "PRINT") {
        ss >> var;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "PRINT" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new PrintStatement(var);
    } else if (type == "PRINTALL") {
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "PRINTALL" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new PrintAllStatement();
    } else if (type == "JUMP") {
        ss >> integer;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "LET" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new JumpStatement(integer);
    } else if (type == "ADD") {
        ss >> var;
        ss >> val;
        // cout << var << " " << val << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "ADD" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new AddStatement(var, val);
    } else if (type == "SUB") {
        ss >> var;
        ss >> val;
        // cout << var << " " << val << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "SUB" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new SubStatement(var, val);
    } else if (type == "MULT") {
        ss >> var;
        ss >> val;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "MULT" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new MultStatement(var, val);
    } else if (type == "DIV") {
        ss >> var;
        ss >> val;
        // cout << var << " " << val << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "DIV" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new DivStatement(var, val);
    } else if (type == "GOTO") {
        ss >> integer;
        // cout << integer << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "GOTO" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new JumpStatement(integer);
    } else if (type == "IF") {
        ss >> var;
        ss >> val;
        ss >> integer;

        ss >> type;  // eats the THEN that is typed here
        ss >> integer2;
        // cout << var << " " << integer << " " << val << " " << type << " " << integer2 <<endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "IF" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new IfStatement(var, integer, val, integer2);
    } else if (type == "GOSUB") {
        ss >> integer;
        // cout << integer << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "GOSUB" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new JumpSubStatement(integer);
    } else if (type == "RETURN") {
        // cout << type << endl;
        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "RETURN" will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new ReturnStatement();
    } else if (type == "END" || type == ".") {

        // Note:  Because the project spec states that we can assume the file
        //	  contains a syntactically legal program, we know that
        //	  any line that begins with "END" or "." will be followed by a space
        //	  and then a variable and then an integer value.
        statement = new EndStatement();
    }
    // Incomplete;  TODO:  Finish this function!

    return statement;
}

void interpretProgram(istream& inf, ostream& outf) {
    // Statement * statement = new LetStatement("test", "5");
    // Statement * statement2 = new PrintStatement("test");
    vector<Statement*> program;
    parseProgram(inf, program);
    ProgramState state = ProgramState(program.size());
    // statement->execute(&state, outf);
    // statement2->execute(&state, outf);
    size_t i = 1;
    while (i < program.size()) {  // executes
        (program.at(i))->execute(&state, outf);
        i = state.getcurrentline();
    }
    for (size_t j = 1; j < program.size(); j++) {  // deletes all of the dynamically allocated
        delete program[j];
    }
    // Incomplete;  TODO:  Finish this function!
}
