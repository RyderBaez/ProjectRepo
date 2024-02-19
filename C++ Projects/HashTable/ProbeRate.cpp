#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <string>
#include "Hashtable.h"
using namespace std;
string generateStringFromInt(int num) { //generates a string from int taken from test program
	string str = "aaaa";
	for(int i = str.size()-1 ; i >= 0; i--) {
		str[i] += num % 26;
		num /= 26;
	}
	return str;
}
int main(){
    int max = 0;
    int total = 0;
    int num;
    Hashtable<int> hashtable(false, 11);
    for(int i = 0; i < 10000; i++){ //adds unique numbers 
        num = hashtable.add(generateStringFromInt(i), i);
        total += num; //keeps track of the total jumps
        if(max < num){
                max = num;
        }

    }//prints
cout << max << endl;
cout << "Average: " << (total/10000.0) << endl; //ends up being a little less than 10%
}