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

#ifndef ScopedDuration_HPP
#define ScopedDuration_HPP

#include <chrono>
#include <string>


// --------------------------------------------------------------------------------
typedef std::chrono::time_point<std::chrono::system_clock> ChronoSysClkTimePoint;

struct ChronoTimePointDifferenceSeconds
{
    double operator()(ChronoSysClkTimePoint start,
                      ChronoSysClkTimePoint end) {
        std::chrono::duration<double> elapsed_seconds = end-start;
        return elapsed_seconds.count();
    }
};
// --------------------------------------------------------------------------------
/**
 * @brief Root class of the ScopedDuration "library"
 * Uses automatic storage duration to time the duration of a scope.
 * Needs helper functions from you to deal with time. 
 *
 * If you want to use types that need less template parameters and makes use of C++11 std::chrono, 
 * See <insert header>
 *
 * @todo Move all the chrono stuff out of this header, so it stays C++03 compatible
 */
template<typename SubmitFunction,
         typename NowTimeFunction = decltype(&std::chrono::system_clock::now), //pass as function pointer ... in a typedef
         typename TimeDifferenceSecondsFunction = ChronoTimePointDifferenceSeconds,
         typename TimePoint = ChronoSysClkTimePoint>
class ScopedDurationT
{
public:
    ScopedDurationT(SubmitFunction sf = SubmitFunction(), 
                    NowTimeFunction now = std::chrono::system_clock::now, 
                    TimeDifferenceSecondsFunction diff = TimeDifferenceSecondsFunction()) :
        submit(sf),
        now(now),
        diff(diff),
        start(now())
        {}

    ScopedDurationT(ScopedDurationT&& d) : 
        submit(std::move(d.submit)),
        now(std::move(d.now)),
        diff(std::move(d.diff)),
        start(now())  // Reset time, don't need to take into account the housekeeping
        {}

    ~ScopedDurationT(){
        submit(diff(start, now()));
    }
    
private:
    SubmitFunction submit;
    NowTimeFunction now;
    TimeDifferenceSecondsFunction diff;
    TimePoint start;
};
// --------------------------------------------------------------------------------
#endif // ScopedDuration_HPP
