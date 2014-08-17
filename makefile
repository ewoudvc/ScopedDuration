

# Yes C++14! Wake up it's 2014
# Backport available on request ...
CXXFLAGS = -Wall -Wextra -std=c++1y -O2

RECIPE = $(LINK.cc) $^ -o $@

default: demo

ScopedDurationRecorder.o : ScopedDurationRecorder.hpp ScopedDuration.hpp
demo.o: ScopedDurationRecorder.o
demo: demo.o ScopedDurationRecorder.o
	$(RECIPE)
