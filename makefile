
default: arg.cpp parser.cpp main.cpp
	g++ -std=c++1z arg.cpp parser.cpp main.cpp -o ./arg 