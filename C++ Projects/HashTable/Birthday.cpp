#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <string>
#include "Hashtable.h"
using namespace std;
string generateStringFromInt(int num) {
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
    for(int i = 0; i < 10000; i++){ //total tests
    Hashtable<int> hashtable(false, 365);
    for(int j = 0; j < 365; j++){ //all number of possible birthdays to be added
        int num = hashtable.add(generateStringFromInt(j), j);
        if(num != 0){ //finds a collision
            total += j;
            //cout << total << endl;
            if(max < j){
                max = j;
            }
            break;
        }
    }
}
cout << "Longest: " << max << endl;
cout << "Average: " << total/10000.0 << endl;//about 14.6 which is accurate
}