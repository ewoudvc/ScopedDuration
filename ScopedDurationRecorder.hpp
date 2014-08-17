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

#ifndef ScopedDurationRecorder_HPP
#define ScopedDurationRecorder_HPP

#include "ScopedDuration.hpp"
#include <string>
#include <map>
#include <vector>

//--------------------------------------------------------------------------------
#include <iostream>

/**
 * @brief cout Print functor for ScopedDurationData
 * Prints whatever data to cout ...
 * Better provide data cout can handle, or write an overload
 *
 * @todo perhaps move this to it's own header, as it requires <iostream> extra
 * On the other hand, then it can't be used as default to ScopedDurationData::PrettyPrintResults
 */
template<typename T>
struct ScopedDurationDataCout
{
    void operator()(const T& data){
        std::cout << data;
    }
};
//--------------------------------------------------------------------------------
/**
 * @brief Very dump functor to keep track of submitted durations for a given tag
 * Also provides PrettyPrintResults with some basic print options
 * You can use the public recordedDurations if you need options beyond this
 */
struct ScopedDurationData
{
    static std::map<std::string, std::vector<double>> recordedDurations;

    enum class PrintOption 
    {
        defaultPrint,
        sortAscending,
        sortDesending,
    };

    ScopedDurationData(const std::string& tag) :
        tag(tag) {}

    ScopedDurationData(const ScopedDurationData& d) :
        tag(d.tag)
        {}

    ScopedDurationData(ScopedDurationData&& data) :
        tag(std::move(data.tag)) {}

    /**
     * @brief Prints pretty results by in default mode to an instande of whatever PrintFunction type you provide
     * Should you want to pass a function pointer (or lambda without capture list), you can do so, but you're obliged to also be explicit about the print options
     */
    // template<typename PrintFunction=ScopedDurationDataCout<double>>
    // void PrettyPrintResults(PrintOption option=PrintOption::defaultPrint, PrintFunction print=PrintFunction()){

    // }

    void operator()(double seconds){
        recordedDurations[tag].push_back(seconds);
    }

private:
    std::string tag;
};




/**
 * @brief The actual ScopedDurationRecorder type definition
 */
typedef ScopedDurationT<ScopedDurationData> ScopedDurationRecorder;

/**
 * @brief 
 */
inline ScopedDurationRecorder make_ScopedDurationRecorder(const std::string& tag)
{
    return ScopedDurationRecorder(ScopedDurationData(tag));
}



#endif // ScopedDurationRecorder_HPP
