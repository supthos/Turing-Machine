// Turing Machine.cpp 
// Reed Axewielder
// 2025 Copyright AXEWIELDER GOODS
// GMDA
#include"TuringMachine.h"

int main()
{

    std::cout << "Turing Machine\n";
	std::cout << "2025 Copyright AXEWIELDER GOODS\n"
		<< "New York, NY, USA\n";

	std::string program = "";
	char p[] = {ST,WE,1,LT,WE,1,LT,WE,1,ED};
	for (char i : p) {
		program += i;
	}

	TuringMachine* Alpha = new TuringMachine(program);

	Alpha->Reset();

	program.clear();
	program = "";
	char r[] = { LD, LT, LT, WE, TE, RT, RT, WE, TE, RT, CL, FE, 1, NG, RT, WE, FE, ED, LD, WE, TE, LT, WE, FE, ED };
	for (int i : r) {
		program += i;
	}

	Alpha->LoadAndRun(program);
	TuringMachine Beth(program);

	return 0;
}
