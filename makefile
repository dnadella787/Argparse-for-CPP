
default: args.cpp args.h main.cpp
	g++ parser.cpp args.cpp main.cpp -o ./arg 
