#include "company.hpp"
#include <iostream>
using namespace std;

int main(int argc, char const* argv[]) {
    CompanyTracker Test(10);  // creates a test tracker
    Test.merge(1, 2);         // test merges
    Test.merge(1, 4);
    Test.merge(3, 4);
    Test.merge(2, 7);
    Test.merge(1, 200);  // should do nothing
    Test.merge(-1, 4);   // should do nothing
    Test.merge(1, 4);    // should do nothing
    Test.merge(1, 1);    // should do nothing

    cout << Test.inSameCompany(1, 1);  // test same company
    cout << Test.inSameCompany(1, 2);
    cout << Test.inSameCompany(2, 3);
    cout << Test.inSameCompany(1, 5);
    cout << Test.inSameCompany(1, 7);
    cout << Test.inSameCompany(1, 9);
    cout << Test.inSameCompany(1, -2);
    cout << Test.inSameCompany(1, 20) << endl;

    Test.split(1);
    Test.split(-1);
    Test.split(10);
    Test.split(4);
    Test.split(2);
    Test.split(200);

    cout << Test.inSameCompany(1, 1);  // repeats same test after split
    cout << Test.inSameCompany(1, 2);
    cout << Test.inSameCompany(2, 3);
    cout << Test.inSameCompany(1, 5);
    cout << Test.inSameCompany(1, 7);
    cout << Test.inSameCompany(1, 9);
    cout << Test.inSameCompany(1, -2);
    cout << Test.inSameCompany(1, 20) << endl;

    return 0;
}
