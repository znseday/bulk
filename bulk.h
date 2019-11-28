#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <iterator>
#include <algorithm>
#include <memory>
//#include <chrono>
//#include <ratio>
//#include <thread>
#include <ctime>


using namespace std;

// __FUNCSIG__ is for VS, but Qt (mingw) works with __PRETTY_FUNCTION__
#if ((defined WIN32) || (defined WIN64)) && (defined _MSC_VER)
//#define MY_P_FUNC __FUNCSIG__
#else
#define MY_P_FUNC __PRETTY_FUNCTION__
#endif

#if ((defined NDEBUG) || (defined _NDEBUG))
#define MY_DEBUG_ONLY(x)
#else
#define MY_DEBUG_ONLY(x) (x)
#endif

void TestFile(const char *file_name);
//-----------------------------------------------

//enum class OutputType
//{
//    Console,
//    LocalFile
//    //std::string ToString() {return ((int)Console) ? std::string("Console") : std::string("LocalFile");} // doesn't work (((
//};
//-----------------------------------------------


class Observer
{
private:
//    size_t N = 3;
//    bool BracketOpen = false;
//    std::vector<std::string> cmds;
public:

    virtual void Do(/*OutputType out_, */const std::vector<std::string> &cmds, /*std::chrono::time_point<std::chrono::high_resolution_clock>*/ time_t t) = 0;
    virtual ~Observer() = default;

    //void (Observer::*ActiveHandler)(std::string &&) = &Observer::StateCommand;

//    void StateLine(std::string &&str)
//    {
//        if (str == "{")
//        {
//            ActiveHandler = &Observer::StateOpenBlock;
//            (this->*ActiveHandler)(std::move(str));
//        }
//    }
//    void StateOpenBlock( [[maybe_unused]] std::string &&str )
//    {
//        cmds.clear();
//        ActiveHandler = &Observer::StateCommand;
//    }
//    void StateCommand(std::string &&str)
//    {
//        if (cmds.empty() && str != "{")
//        {
//            cmds.push_back(str); // emplace_back
//        }
//        if (cmds.size() == N && !BracketOpen)
//        {
//            Do()
//        }
//        if (str == "{")
//        {
//            ActiveHandler = &Observer::StateOpenBlock;
//            (this->*ActiveHandler)(std::move(str));
//        }
//        else if (str == "}")
//            ActiveHandler = &Observer::StateExec;
//        else
//        {
//            cmds.push_back(str); // emplace_back
//            if(cmds.size()==N)
//            {
//                ActiveHandler = &Observer::StateExec;
//            }
//        }
//    }
//    void StateExec(std::string &&str)
//    {
//        ActiveHandler = &Observer::StateCommand;
//        (this->*ActiveHandler)(std::move(str));
//    }

};
//-----------------------------------------------

//class Commands;
//void Commands::subscribe(Observer *obs); // doesn't work((( How to do forward declaration for methods?

class Commands
{
private:
    //OutputType output;
    std::vector<Observer *> subs;

    size_t N = 3;
    size_t BracketOpenLevel = 0;
    std::vector<std::string> cmds;

    //std::chrono::time_point<std::chrono::high_resolution_clock> timeFirst;
    time_t timeFirst;

public:

    Commands(size_t _N) : N(_N) {}

    void subscribe(Observer *obs)
    {
        subs.push_back(obs);
    }

    void AnalyzeCommand(const std::string &str)
    {
        if (str != "{" && str != "}")
        {
            if (cmds.empty())
                timeFirst = std::time(nullptr);
                //timeFirst = std::chrono::high_resolution_clock::now();

            cmds.push_back(str); // emplace_back
        }
        if (cmds.size() == N && !BracketOpenLevel)
        {
            ExecForAllSubs();
            //cmds.clear();
        }
        if (str == "{")
        {
            BracketOpenLevel++;

            if (BracketOpenLevel == 1 && !cmds.empty())
                ExecForAllSubs();

        }
        if (str == "}")
        {
            BracketOpenLevel--;
            if (BracketOpenLevel == 0 && !cmds.empty())
            {
                ExecForAllSubs();
                //cmds.clear();

            }
        }

    }

    void ExecForAllSubs(/*const std::vector<std::string> &_cmds*/)
    {
        if (!cmds.empty() && BracketOpenLevel == 0)
        {
            for (auto s : subs)
                s->Do(cmds, timeFirst);

            cmds.clear();
        }
    }
};
//-----------------------------------------------

class ConsoleObserver : public Observer
{
public:

    ConsoleObserver(Commands *_cmds)
    {
        _cmds->subscribe(this);
    }

    void Do(const std::vector<std::string> &cmds, [[maybe_unused]]/* std::chrono::time_point<std::chrono::high_resolution_clock>*/ time_t t) override
    {
        //std::cout << MY_P_FUNC << std::endl;
        std::cout << "bulk: ";
        size_t cmds_size = cmds.size();
        for (size_t i = 0; i < cmds_size; i++)
            std::cout << cmds[i] << (  (i<(cmds_size-1)) ? ", " : "\n");
    }
};
//-----------------------------------------------

class LocalFileObserver : public Observer
{
public:
    LocalFileObserver(Commands *_cmds)
    {
        _cmds->subscribe(this);
    }

    void Do(const std::vector<std::string> &cmds, /*std::chrono::time_point<std::chrono::high_resolution_clock>*/ time_t t) override
    {
        //std::cout << MY_P_FUNC << std::endl;
        //size_t tInSecs = std::chrono::duration_cast<std::chrono::milliseconds>(t).count(); // doesn't work. Why ???

        ofstream f( std::string("bulk") + std::to_string(t) + std::string(".log") );

        size_t cmds_size = cmds.size();
        for (size_t i = 0; i < cmds_size; i++)
            f << cmds[i] << std::endl;

        f.close();
    }
};
//-----------------------------------------------







