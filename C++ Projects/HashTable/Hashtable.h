
#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H

#include <string>
#include <math.h>
#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>
template<typename T>
class Hashtable {
    public:
    Hashtable(bool debug = false, unsigned int size = 11);
    ~Hashtable();
    int add(std::string k, const T& val);
    const T& lookup (std::string k) const;
    void reportAll (std::ostream & out) const;
    int hash (std::string k) const;
    private:
        int r[5];
        bool deb;
        std::vector<std::string> Hashkeys;
        std::vector<T> Hashvalues;
        int sizes[17] = {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877, 205759, 411527, 823117};
        int totalsizes = 17;
        int numinserted = 0;
        void resize ();
};



template<typename T>
Hashtable<T>::Hashtable(bool debug, unsigned int size){
        deb = debug;
        //std::cout << size << std::endl;
        if(debug == true){
             r[0] = 983132572; 
             r[1] = 1468777056; 
             r[2] = 552714139; 
             r[3] = 984953261; 
             r[4] = 261934300;
        }
        else{//sets rand values
            r[0] = std::rand() % (size);
            r[1] = std::rand() % (size);
            r[2] = std::rand() % (size);
            r[3] = std::rand() % (size);
            r[4] = std::rand() % (size);
        }
        Hashkeys = std::vector<std::string>(size);
        Hashvalues = std::vector<T>(size);
        for(size_t i = 0; i < size; i++){
            Hashkeys[i] = ""; //sets them to keyless
        }

}

template<typename T>
Hashtable<T>::~Hashtable (){}
template<typename T>
int Hashtable<T>::add(std::string k, const T& val){
        numinserted++;//keeps track of the values in the hashtable
        if(numinserted * 2 >= int(Hashkeys.size())){
            resize();
        }
        int local = hash(k);
        int quad = 0;
        while(Hashkeys[(local + quad * quad)%Hashkeys.size()] != ""){
            //if the hash table slot is taken quadratic probe
            quad++;
        }
        Hashkeys[(local + quad * quad)%Hashkeys.size()] = k;
        Hashvalues[(local + quad * quad)%Hashkeys.size()] = val;
        return quad;
}
template<typename T>
const T& Hashtable<T>::lookup(std::string k) const{
        int size = Hashkeys.size();
        int local = hash(k);
        int quad = 0;
        while((Hashkeys[(local + quad * quad)%Hashkeys.size()] != "") && (Hashkeys[(local + quad * quad)%Hashkeys.size()] != k) && (quad < size)){
            quad++;//quadratic probes until it finds the given key
        }
        return Hashvalues[(local + quad * quad)%Hashkeys.size()];//returns the found value
}
template<typename T>
void Hashtable<T>::reportAll(std::ostream& out) const{
        for(size_t i = 0; i < Hashkeys.size(); i++){
            if(Hashkeys[i] != ""){//print values with something
                out << Hashkeys[i] << " " << Hashvalues[i] << std::endl; 
            }
        }
}
template<typename T>
int Hashtable<T>::hash(std::string k) const{
        int len = k.size() - 1;
        int hash = 0; //used for intermediate value then the total
        int placement = 4;
        int hashcomp[5];
        for(int i = 0; i < 5; i++){
            hashcomp[i] = 0;
        }
        for(int i = len; i >= 0; i-=6){//gets sets of 6
        for(int j = 0; j < 6; j++){//goes through the sets
           if(i - j < 0){
            hash += 0;
           }
           else{
            hash += (int(k[i-j] - 'a' + 1) * pow(27, j));//convert char to int then power
           }
        }
        hashcomp[placement] = hash;
        hash = 0;
        placement--;
        }
        for(int i = 0; i < 5; i++){
            hash += ((long long)r[i] * (long long)(hashcomp[i])) % Hashkeys.size(); //total it
        }
        //std::cout << hash % Hashkeys.size() << std::endl;
        return (hash % Hashkeys.size());
}
template<typename T>
void Hashtable<T>::resize(){
        numinserted = 1;
        std::vector<std::string> Tempkeys = Hashkeys;
        std::vector<T> Tempvalues = Hashvalues;//saves old arrays
        int size = Tempkeys.size();
        for(int i = 0; i < totalsizes; i++){
            if(sizes[i] > size){
                size = sizes[i];
                //searches for new size
                break;
            }
        }
        if(deb == true){
             r[0] = 983132572; 
             r[1] = 1468777056; 
             r[2] = 552714139; 
             r[3] = 984953261; 
             r[4] = 261934300;
        }
        else{//creates new rand values
            r[0] = std::rand() % (size);
            r[1] = std::rand() % (size);
            r[2] = std::rand() % (size);
            r[3] = std::rand() % (size);
            r[4] = std::rand() % (size);
        }
        Hashkeys = std::vector<std::string>(size);
        Hashvalues = std::vector<T>(size);
        for(int i = 0; i < size; i++){
            Hashkeys[i] = ""; //sets them to keyless
        }
        for(size_t i = 0; i < Tempkeys.size(); i++){
            if(Tempkeys[i] != ""){//readds all lost values
                add(Tempkeys[i], Tempvalues[i]);
            }
        }
}

#endif