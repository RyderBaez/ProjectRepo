#include "company.hpp"

#include <iostream>
using namespace std;
// Initializes the tracker with n students and their 1-person companies.
CompanyTracker::CompanyTracker(int n) {
    numCompanies = n;
    companies = new Company*[numCompanies];
    for (int i = 0; i < numCompanies; ++i)
        companies[i] = new Company();
}

// Deallocates all dynamically allocated memory.
CompanyTracker::~CompanyTracker() {
    // your implementation goes here
    for (int i = 0; i < numCompanies; i++) {
        while (companies[i]->parent != nullptr) {
            split(i);  // uses split to remove all higher classes to isolate each student
        }
        delete companies[i];  // removes each student after each connections
    }
    delete companies;  // deletes overall
}

void CompanyTracker::merge(int i, int j) {
    // your implementation goes here
    if (i >= numCompanies || j >= numCompanies || i < 0 || j < 0) {
    } else {
        Company* stud1 = findLargest(i);
        Company* stud2 = findLargest(j);                  // finds largest company both apart of
        if (stud1 != stud2) {                             // checks not the same
            Company* parent = new Company(stud1, stud2);  // merges with new dynamically allocated company
            stud1->parent = parent;                       // sets it to the previous companies parents
            stud2->parent = parent;
        }
    }
}

void CompanyTracker::split(int i) {
    if (i >= numCompanies || i < 0) {  // checks for a bad index
    } else if (companies[i]->parent == nullptr) {
    }  // checks if the company has 1 person
    else {
        // your implementation goes here
        Company* stud1 = findLargest(i);  // gets largest company
        stud1->merge1->parent = nullptr;  // splits
        stud1->merge2->parent = nullptr;
        delete stud1;  // deletes largest
    }
}

bool CompanyTracker::inSameCompany(int i, int j) {
    if (i >= numCompanies || j >= numCompanies) {  // checks for bad indexs
        return false;
    }
    if (i < 0 || j < 0) {  // checks for bad indexs
        return false;
    }
    // your implementation goes here
    Company* stud1 = companies[i];
    Company* stud2 = companies[j];
    while ((stud1->parent) != nullptr) {  // gets the largest company that student 1 is apart of
        stud1 = stud1->parent;
    }
    while ((stud2->parent) != nullptr) {  // gets the largest company that student 2 is apart of
        stud2 = stud2->parent;
    }
    if (stud1 == stud2) {  // checks if it is the same company
        return true;
    } else {
        return false;
    }
}
Company* CompanyTracker::findLargest(int i) {
    Company* stud = companies[i];
    while (stud->parent != nullptr) {  // gets the largest company that student 1 is apart of
        stud = stud->parent;
    }
    return stud;
}
