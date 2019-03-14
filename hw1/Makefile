all: 
	g++ -g main.cpp myclass.cpp -o main

judge:
	g++ -std=c++11 -O2 main.cpp myclass.cpp -o main

merge:
	cat myclass.hpp > merge.cpp
	tail -n +2 myclass.cpp >> merge.cpp
	tail -n +2 main.cpp >> merge.cpp

sol2: sol2.cpp
	g++ -g -Wall sol2.cpp -o sol2

clean:
	rm -f *~ main
