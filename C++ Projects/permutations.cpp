#include <iostream>
#include <string>
using namespace std;
void permutationshelper(string a, string b);
void permutations(string in) {
    if(in == ""){
        return;
    }
    for (size_t i = 0; i < in.length(); i++) {
        if (i == 0) {  // special case for pulling first letter
            permutationshelper((in.substr(1, in.length() - 1)), (in.substr(i, 1)));
        } else if (i == in.length() - 1) {  // special cast for pulling last letter
            permutationshelper((in.substr(0, in.length() - 1)), (in.substr(i, 1)));
        } else {  // pull middle letter
            permutationshelper((in.substr(0, i) + in.substr(i + 1, in.length() - 1 - i)), (in.substr(i, 1)));
        }
    }
}

void permutationshelper(string a, string b) {  // a = remainning character //b = currently contructed permutation
    if (a.length() == 1) {                     // if final letter in original word print
        cout << (b + a) << endl;
    } else {
        for (size_t i = 0; i < a.length(); i++) {
            if (i == 0) {  // special removal for first
                permutationshelper((a.substr(1, a.length() - 1)), (b + a.at(i)));
            } else if (i == a.length() - 1) {  // special removal for last
                permutationshelper((a.substr(0, a.length() - 1)), (b + a.at(i)));
            } else {  // removal for all middle character
                permutationshelper((a.substr(0, i) + a.substr(i + 1, a.length() - 1 - i)), (b + a.at(i)));
            }
        }
    }
}

