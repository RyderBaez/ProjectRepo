#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
using namespace std;
long mod_exp(long base, long exponent, long mod);
long mod_exp(long long base, long exponent, long mod);
long EuclideanGCD(long p, long q);
long ExtendedEuclideanGCD(long e, long L);
void lowercase(string& s);
int main(int argc, char *argv[]) {
    if(argc < 3){
        cout << "need to give p and q values" << endl;
        return -1; //returns 
    }
    long p = atol(argv[1]);
    long q = atol(argv[2]);
    long L = (p-1) * (q-1) / EuclideanGCD(p-1, q-1); //calculate values
    long e = 65537;
    if(e >= L){ //terminate with error
       cout << "error L less than e" << endl;
       return -1; 
    }
    long d = ExtendedEuclideanGCD(e, L);
    if(d == -1){ //terminate with error
        return -1;
    }
    string fullline;
    string command;
    string filename;
    string outfile;
    string message;
    long n;
    stringstream ss;
    while(command != "exit"){ //continues while not the final command
        cout << "enter a command: exit, encode, or decode" << endl;
        cin >> command;
        lowercase(command); //get original command in lowercase
        if(command == "exit"){
            //end
        }
        else if(command == "encrypt"){
            cin >> outfile; //gets follow up values
            cin >> n;
            ofstream ofile(outfile.c_str());
            if (!ofile) { //checks for valid file
                cout << "Cannot open " << outfile << "!" << endl;
            }
            else if(n < 27){ //checks for valid N
                cout << "Error: invalid N" << endl;
            }
            else{
                int remainder = 0; //for smaller than x short strings
                message = "";
                getline(cin, fullline); //gets whole message
                ss.str(string()); //clears anything left over in the string stream
                ss.clear();
                ss << fullline;
                ss >> filename;
                message += filename; //takes parts to message to convert
                while(!ss.eof()){
                    ss >> filename;
                    message += " " + filename;
                }
                lowercase(message); //converts to lowercase
                string encoded;
                long x = 1 + log(n/27)/log(100);
                for(size_t i = 0; i < (message.size()/x); i++){ //goes through ever letter for a set x
                    for(int j = 0; j < x; j++){
                        if(i*x + j < message.size()){
                            if(message.at(i*x + j) != ' '){
                            if(int(char(message[i*x + j]) - ('a' - 1)) < 10){
                                encoded += "0";
                            }
                            encoded += to_string(int(char(message[i*x + j]) - ('a' - 1)));
                            //cout << int(char(message[i*x + j]) - ('a' - 1)) << endl;
                        }
                        else{ //" "
                          encoded += "00";  
                        }
                        }
                        else{//extra at the end
                            encoded += "00";
                        }
                    }
                    ofile << mod_exp(stol(encoded), e, n);
                    ofile << ' ';
                    encoded = "";
                    remainder = i + 1;
                }
                if((message.size()) - (x * remainder) > 0){ //checks if extra end character
                    
                     for(int j = 0; j < x; j++){ //encrypt them
                        if(size_t(j) + (x*remainder) < message.size()){
                            if(message.at(x*remainder+j) != ' '){
                            if(int(char(message[x*remainder+j]) - ('a' - 1)) < 10){
                                encoded += "0";
                            }
                            encoded += to_string(int(char(message[x*remainder+j]) - ('a' - 1)));
                            //cout << int(char(message[x*remainder+j]) - ('a' - 1)) << endl;
                        }
                        else{ //space
                          encoded += "00";  
                        }
                        }
                        else{//extra
                            encoded += "00";
                        }
                    }
                    ofile << mod_exp(stol(encoded), e, n); //put in file
                    ofile << ' ';
                    encoded = "";
                } 
                ofile.close(); //close the file
            }
        }
        else if (command == "decrypt"){
            getline(cin, fullline); //gets the full line
            ss.str(string()); //clears anything left over in the string stream
            ss.clear();
            ss << fullline; //string stream it
            ss >> filename;
            ss >> outfile;
            ifstream infile(filename.c_str());
            ofstream ofile(outfile.c_str());
            if (!infile) { //bad file
                cout << "Cannot open " << filename << "!" << endl;
            }
            else if (!ofile) { //badfile
                cout << "Cannot open " << outfile << "!" << endl;
            }
            else{
                size_t lettercount = 0;
                n = q * p;
                long long decode;
                infile >> decode; //pulls from file
                while(!infile.eof()){
                    long M = mod_exp(decode, d, n); //finds M
                    string values = to_string(M); //string it
                    if(values.size() > lettercount){ //checks letter count
                        lettercount = values.size();
                    }
                    if(values.size() % 2 == 1){ //checks even for leading zeros like 7
                        values = '0' + values;
                        if(values.size() > lettercount){
                            lettercount = values.size();
                        }
                    }
                    while(lettercount > values.size()){ //makes sure to check for missing leading zeros
                        values = '0' + values;
                    }
                    for(size_t i = 0; i < (values.size()); i+=2){
                        string ch = values.substr(i,2); //uses all groups of 2
                        if (ch == "00"){
                            ofile << ' '; 
                        }
                        else{
                            ofile << char(stoi(ch) - 1 + int('a'));
                        }
                    }
                    infile >> decode; //gets next set
                }
                infile.close();
                ofile.close();
            }
        }
        else{ //bad command
            cout << "invalid command" << endl;
        }
    }
    return 0;
    
}

long mod_exp(long base, long exponent, long mod) {
    long temp = 1;
    long long iter = base % mod; //base of modded
    while(exponent > 0){
        if((exponent) % 2){//check if important
            temp =(temp * iter) % mod;//add it to final value
        }
        iter = (iter * iter) % mod;//increase
        exponent = exponent / 2;
    } 
    return temp;
}
long mod_exp(long long base, long exponent, long mod) {
    long temp = 1;
    long long iter = base % mod;
    while(exponent > 0){
        if((exponent) % 2){//check if important
            temp =(temp * iter) % mod;//add it to final value
        }
        iter = (iter * iter) % mod;//increase
        exponent = exponent / 2;
    } 
    return temp;
}
long EuclideanGCD(long p, long q){
    long mod = p % q;
    if(mod == 0){//finds when divids through
        return q;
    }
    if(q == 1){ // 1
        return q;
    }
    return EuclideanGCD(q, mod); //mod down to final value
}
long ExtendedEuclideanGCD(long e, long L){//follows the form in the code
 long t, old_t, r, old_r, quotient, temp;
 t = 1;
 old_t = 0;
 r = e;
 old_r = L;
 while(r != 0){
    quotient = old_r / r;
    temp = r;
    r = old_r - quotient * r;
    old_r = temp;
    temp = t;
    t = old_t - quotient * t;
    old_t = temp;
 }
 long d = old_t;
 while(d < 0){//matches d to range
    d += e*L;
 }
 while ( d > e*L){
    d -= e*L;
 }
if(old_r != 1){
    cout << "bad choice of p and q" << endl;
    return -1; 
}
 return d;
}
void lowercase(string& s) {
    for (size_t i = 0; i < s.size(); i++) {
        s[i] = tolower(s.at(i));  // makes all lowercase
    }
}