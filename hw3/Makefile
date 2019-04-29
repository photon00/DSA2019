all:
	g++ -g -std=c++11 main.cpp state.cpp -o main

judge:
	g++ -std=c++11 -O2 main.cpp state.cpp -o main

merge:
	cp state.hpp merge.cpp
	tail -n +2 state.cpp >> merge.cpp
	tail -n +6 main.cpp >> merge.cpp

clean:
	rm main