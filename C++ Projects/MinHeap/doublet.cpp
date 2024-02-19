#include "MinHeap.h"
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;
void AStar(string startword, string endword, map<string, vector<string>>& adjacent);
int charactersoff(string target, string word);
void lowercase(string& s);
int main(int argc, char* argv[]) {
    string word;
    set<string> words;
    map<string, set<string>> keys;
    map<string, vector<string>> adjacent;
    if (argc < 4) {  // not all arguments given ends with error -1
        return -1;
    }
    ifstream myfile;
    string start = argv[1];
    string stop = argv[2];
    lowercase(start);
    lowercase(stop);
    myfile.open(argv[3]);  // opens file
    int numwords;
    myfile >> numwords;
    for (int i = 0; i < numwords; i++) {  // adds every word to a set to check later and a map to hold adjacents later
        myfile >> word;
        lowercase(word);
        words.insert(word);
        vector<string> newset;  // create new empty vector for each word
        pair<string, vector<string>> newval = make_pair(word, newset);
        adjacent.insert(newval);
    }
    set<string>::iterator it;
    for (it = words.begin(); it != words.end(); ++it) {  // finds all adjacents
        for (size_t i = 0; i < it->size(); i++) {        // not n instead 26(Size)(N)
            for (int j = 0; j < 26; j++) {
                string temp = *it;
                temp[i] = (j + 'a');  // creates all other combinations of the words
                if (words.find(temp) != words.end() && temp != *it) {
                    adjacent[*it].push_back(temp);
                }
            }
        }
    }
    // cout << start << endl;
    // cout << stop << endl;
    AStar(start, stop, adjacent);
    myfile.close();
}
void AStar(string startword, string endword, map<string, vector<string>>& adjacent) {
    int priority;
    int totalmoves = 0;  // keep count
    string word;
    set<string> visited;
    MinHeap<string> heap(2);
    int diff = charactersoff(endword, startword);
    heap.add(startword, diff * startword.size() + diff);  // adds first word

    while (!heap.isEmpty()) {            // while still more to search
        priority = heap.peekpriority();  // pulls next best option to search
        word = heap.peek();
        diff = charactersoff(endword, word);
        if (diff == 0) {  // finds the word!
            cout << (((priority - 1) / (word.size())) + 1) << endl;
            cout << totalmoves << endl;
            return;
        }
        heap.remove();
        if (visited.find(word) == visited.end()) {  // if a new word adds all adjacents to heap
            ++totalmoves;
            visited.insert(word);
            for (size_t i = 0; i < adjacent[word].size(); i++) {  // adds all adjacent words
                int newprior
                        = ((((priority - diff) / (word.size())) + charactersoff(endword, adjacent[word][i]) - diff + 1)
                           * (word.size())) 
                          + charactersoff(endword, adjacent[word][i]); //clang-format did this not me
                heap.add(adjacent[word][i], newprior);  // adds one based off previous prio then redoes math
            }
        }
    }
    cout << "No transformation" << endl;
    cout << totalmoves << endl;  // couldn't find path
}
void lowercase(string& s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = tolower(s.at(i));  // makes all lowercase
    }
}
int charactersoff(string target, string word) {
    int cnt = 0;
    for (size_t i = 0; i < word.size(); i++) {
        if (word[i] != target[i]) {
            ++cnt;  // keeps count of letters off
        }
    }
    return cnt;
}
