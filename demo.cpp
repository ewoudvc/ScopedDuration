/*
  The MIT License (MIT)

  Copyright (c) 2014 Ewoud Van Craeynest <ewoudvc@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <string>
//#include <chrono>
#include <iostream>
//#include <map>
//#include <vector>
#include <numeric>
#include <sstream>
#include <functional>

#include <boost/algorithm/string/join.hpp>

#include "ScopedDurationRecorder.hpp"

using std::cout;
using std::endl;

using std::string;
string original("Hello");

//--------------------------------------------------------------------------------
/**
 * @brief Concatenates the same string a huge number of times using string::operator+
 */
void operatorPlus()
{
    auto sd = make_ScopedDurationRecorder("operatorPlus");
    string s;
    for(int i=0; i<100000; ++i)
        s = s + original; 
}
//--------------------------------------------------------------------------------
void operatorPlusEquals()
{
    auto sd = make_ScopedDurationRecorder("operatorPlusEquals");
    string s;
    //size_t cap = s.capacity();

    for(int i=0; i<100000; ++i)
    {
        s += original;    
        
        //if (cap!=s.capacity()) cout << (cap = s.capacity()) << endl;
    }
}
//--------------------------------------------------------------------------------
void boostJoin()
{
    using boost::algorithm::join;
    using std::vector;

    auto sd = make_ScopedDurationRecorder("boostJoin");
    vector<string> strings; // No preallocation
    for(int i=0; i<100000; ++i)
        strings.push_back(original);    
    auto s = join(strings, "");
}
//--------------------------------------------------------------------------------
void boostJoinPreallocate()
{
    using boost::algorithm::join;
    using std::vector;

    auto sd = make_ScopedDurationRecorder("boostJoinPreallocate");
    vector<string> strings; 
    strings.reserve(100000);  // Preallocation!!
    for(int i=0; i<100000; ++i)
        strings.push_back(original);    
    auto s = join(strings, "");
}
//--------------------------------------------------------------------------------
void stringStream()
{
    using std::ostringstream;

    auto sd = make_ScopedDurationRecorder("stringStream");
    ostringstream oss;
    for(int i=0; i<100000; ++i)
        oss << original;

    string s = oss.str();
}
//--------------------------------------------------------------------------------
template<class T>
void printTimings(T times, const char* name, bool verbose=false)
{
    auto it = times.find(name);
    auto average = std::accumulate(begin(it->second), end(it->second), 0.0) / it->second.size();
    cout << name <<" takes on average: " << average << " seconds" <<endl;  

    int i=0;
    if (verbose)
        for (auto x : times[name])
            cout << i++ << ": " << x << " seconds" <<endl; 
}
//--------------------------------------------------------------------------------
// auto make_durationlogger(const string& tag)
// {
//     return ScopedDurationT<void(*)(double,const string&)>(tag, [](double seconds, const string& tag)
//                                                           { 
//                                                               cout << "Duration for " << tag << " :" << seconds << endl;
//                                                           });
// }
//--------------------------------------------------------------------------------
int main()
{
    using std::map;
    using std::vector;

    // To demo other possibilities of ScopedDuration
    // Using a function functor, demoing decltype
    std::function<void(double)> logf= [](double seconds){ cout << "Duration for 'main' " << " :" << seconds << endl; };
    ScopedDurationT<decltype(logf)> sd(logf); // Just log time, your functor should take care of tagging and whatnot

    // Using a lambda as a function pointer
    ScopedDurationT<void(*)(double)> sf([](double seconds)
                                        { 
                                            cout << "Duration for 'main2'" << " :" << seconds << endl;
                                        });
    
    // Using another function to produce a ScopedDuration object coupled to a logger function
    //auto sm = make_durationlogger("Main3"); // Rely on RVO to eliminate temporaries

    for (int i=0; i<5; ++i)
    {
        operatorPlus();
        operatorPlusEquals();
        boostJoin();
        boostJoinPreallocate();
        stringStream();
    }

    // Refactor into ScopedDurationData::PrintResults() taking a print functor/functionptr?
    printTimings(ScopedDurationData::recordedDurations, "operatorPlusEquals");
    printTimings(ScopedDurationData::recordedDurations, "operatorPlus");
    printTimings(ScopedDurationData::recordedDurations, "boostJoin");
    printTimings(ScopedDurationData::recordedDurations, "boostJoinPreallocate");
    printTimings(ScopedDurationData::recordedDurations, "stringStream");

}
//--------------------------------------------------------------------------------
