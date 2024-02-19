#include "avlbst.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace std;
void schedule(stringstream& ss);
bool schedulehelper(
        stringstream& ss,
        int numexams,
        int numstud,
        int numslots,
        bool** filled,
        int curstud,
        AVLTree<string, int>* parse);
int main(int argc, char* argv[]) {
    if (argc > 2) {
        return -1;
    }  // reads file and then finds a valid schedule
    ifstream inFile;
    inFile.open(argv[1]);
    stringstream ss;
    ss << inFile.rdbuf();
    inFile.close();
    try {
        schedule(ss);
    } catch (int) {
        cout << "error" << endl;
    }
}

void schedule(stringstream& ss) {
    int numexams, numstud, numslots;
    ss >> numexams;
    ss >> numstud;
    ss >> numslots;
    bool** possible = new bool*[numstud];
    for (size_t i = 0; i < size_t(numstud); i++) {
        possible[i] = new bool[numslots];
    }
    for (size_t i = 0; i < size_t(numstud); i++) {
        for (size_t j = 0; j < size_t(numslots); j++) {
            possible[i][j] = true;
        }
    }
    AVLTree<string, int>* parse;
    bool complete = schedulehelper(ss, numexams, numstud, numslots, possible, -1, parse);
    if (!complete) {
        cout << "No valid solution." << endl;
    }
}
bool schedulehelper(
        stringstream& ss,
        int numexams,
        int numstud,
        int numslots,
        bool** filled,
        int curstud,
        AVLTree<string, int>* parse) {
    string val;
    ss >> val;  // inputs the next test
    if (ss.fail()) {
        return true;
    }
    if (val[0] > '9' || val[0] < '0') {  // if it is a name skip and go to next row
        curstud++;
        ss >> val;
    }

    if ((parse->find(val)) == (parse)->end()) {  // checks if already in the AVLtree
        for (size_t i = 0; i < size_t(numslots); i++) {
            if (filled[curstud][i]) {  // if not can be at any time
                filled[curstud][i] = false;
                parse->insert(pair<string, int>(val, i));
                bool success = schedulehelper(
                        ss, numexams, numstud, numslots, filled, curstud, parse);  // must return true all the way
                if (success) {
                    cout << val << " " << i << endl;
                    return true;
                }
                filled[curstud][i] = true;
            } else {
                return false;
            }
        }
        stringstream ssrepaired;
        ssrepaired << val << ss.rdbuf();
        ss.swap(ssrepaired);
    } else {  // put at the time it is already at
        BinarySearchTree<string, int>::iterator curval = parse->find(val);
        size_t curpos = curval->second;
        if (filled[curstud][curpos]) {
            filled[curstud][curpos] = false;
            parse->insert(pair<string, int>(val, curpos));
            bool success = schedulehelper(ss, numexams, numstud, numslots, filled, curstud, parse);
            if (success) {  // lets the one who put it in print it
                return true;
            }
            filled[curstud][curpos] = true;  // frees up that spot
            stringstream ssrepaired;
            ssrepaired << val << ss.rdbuf();
            ss.swap(ssrepaired);
            return false;  // otherwise fails
        } else {
            stringstream ssrepaired;
            ssrepaired << val << ss.rdbuf();
            ss.swap(ssrepaired);
            return false;
        }
    }
    return false;
}