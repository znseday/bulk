#include <iostream>
#include <vector>
#include <map>
#include <iterator>
#include <algorithm>
#include <cassert>

#include "bulk.h"

using namespace std;

int main()
{
#if (defined WIN32) || (defined WIN64)
    cout << "Homework bulk" << endl << endl;    // for debugging
    cout << MY_P_FUNC << endl;                  // for debugging
#else
    // some
#endif

    MY_DEBUG_ONLY(cout << "Homework bulk (DEBUG detected)" << endl);

    cout << "Hello, bulk!" << endl;

    TestBasic();

    return 0;
}

