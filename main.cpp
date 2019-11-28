#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <cassert>
#include <chrono>
#include <thread>

#include "bulk.h"

using namespace std;

int main(int argc, const char **argv)
{
    #if (defined WIN32) || (defined WIN64)
        cout << MY_P_FUNC << endl;                  // for debugging
        ifstream i_stream = ifstream("my_own_test.txt");
        if (!i_stream)
        {
            cout << "My error: the input file not found" << endl;
            exit(0);
        }
    #else
        i_stream = cin;
    #endif

    MY_DEBUG_ONLY(cout << "Homework bulk (DEBUG detected)" << endl);

    #if (defined WIN32) || (defined WIN64)
        cout << "Tests on local machine:" << endl;
        TestFile("test1.txt");
        TestFile("test2.txt");
        TestFile("test3.txt");
        TestFile("test4.txt");
    #else
        // some
    #endif

    Commands cmds( (argc<2) ? 3 : static_cast<size_t>(atoi(argv[1])) );

    LocalFileObserver LocalFileObs(&cmds);
    ConsoleObserver   ConsoleObs(&cmds);

    string line;
    while (getline(i_stream, line))
    {
        #if (defined WIN32) || (defined WIN64)
            std::this_thread::sleep_for(1.0s);
            cout << line << endl; // just echo
        #else
            // nothing
        #endif

        cmds.AnalyzeCommand(line);
    }
    cmds.ExecForAllSubs();

    return 0;
}

