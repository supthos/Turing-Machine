// Turing Machine.cpp : 
// Reed Axewielder
#include <vector>
#include <bitset>
#include <iostream>
enum TMInst
{
    FE, TE, ST, LT, RT, WT, RD, RN, LD, CL, ED, NG = -1
};
class TuringMachine {
public:
	TuringMachine() {
		Start();
	}
	~TuringMachine() {}
	TuringMachine(std::vector<std::string> program) {
		Start();
		for (auto& i: program)
			Load(i);
		Run(states[0]);
	}
	TuringMachine(std::string program) {
		Start();
		Run(program);
	}
private:
	std::vector<std::string> states;
	int head = 0;
	unsigned state = 0;
	unsigned long long Rtape = 0;
	unsigned long long Ltape = 0;

	void Load(std::string program) { states.push_back(program); }
	void Load(std::vector<std::string> program) { states = program; }
	void Call(unsigned state) {
		this->state = state;
		if (state < states.size()) {
			Run(states[state]);
		}
	}
	void Left() { head--; }
	void Right() { head++; }
	void Write(bool a) {
		if (head >= 0){
			if (a == true) Rtape = Rtape | (1ULL << head);
			else Rtape = Rtape & ~(1ULL << head);
		}
		else if(head<0) {
			if (a == true) Ltape = Ltape | (1ULL << (-head-1));
			else Ltape = Ltape & ~(1ULL << (-head-1));
		}
	}
	bool Read() {
		if (head >= 0) {
			if ((Rtape & (1ULL << head)) == 0) return false;
			else return true;
		}
		if (head < 0) {
			if ((Ltape & (1ULL << (-head))) == 0) return false;
			else return true;
		}
	}
	void Start() {
		head = Ltape = Rtape = 0;
	}
	void End() {
		std::cout << "State of the machine: \n"
			<< std::bitset<64>(Ltape) << std::endl
			<< std::bitset<64>(Rtape) << std::endl;
		std::cout << "Head: " << head << std::endl;
		std::cout << "State: " << state << std::endl;
	}
	void Run(std::string Program) {
		std::string prgrm = "";
		for (std::string::iterator it = Program.begin(); it != Program.end(); ++it) {
			char c = *it;
			
			switch (c)
			{
				case char(int(ST)) : // Start. Initialize to zero.
					Start();
					break;
				case char(int(LT)) : // Move tape head to the left.
					Left();
					break;

				case char(int(RT)) : // Move tape head to the right.
					Right();
					break;

					case char(int(CL)) : // Conditional Call
					{

						++it;
						if (it != Program.end() && *it == char(TE)) {
							++it;
							if (Read() == true)
								Call(int(*it));
							else {
								++it;
								if (it != Program.end() && *it != char(NG))
									Call(int(*it));
							}

						}
						else if (it != Program.end() && *it == char(FE)) {
							++it;
							if (Read() == false)
								Call(int(*it));
							else {
								++it;
								if (it != Program.end() && *it != char(NG))
									Call(int(*it));
							}
						}
						if (it == Program.end()) {
							break; break;
						}
					}
				case char(int(WT)) : // Write TE or FE at tape head.
					if ((it + 1) == Program.end()) { break; break; }
					if (*(it + 1) == char(TE)) Write(true);
					if (*(it + 1) == char(FE)) Write(false);
					++it;
					break;
				case char(int(NG)) : // Do nothing.
					break;
				case char(int(RD)) : // Read the value at the tape head.
					Read();
					break;
				case char(int(LD)) : // Load a machine state program.
				{
					std::string prog = "";
					while ((it + 1) != Program.end() || *(it + 1) != LD) {
						++it;
						prog += (*it);
					}
					states.push_back(prog);
				}

				case char(int(RN)) : // Run the following program until the end.
				{
					prgrm.clear();
					++it;
					while (it != Program.end() && (*it) != char(ED)) {
						prgrm += (*it);
						++it;
					}
					if ((*it) == char(ED)) {
						prgrm += (*it);
						++it;
					}
					TuringMachine(prgrm);
					if (it == Program.end()) { break; break; }
					break;
				}
				case char(int(ED)) : // End the program and print the machine state.
					End();
					break;

				default:
					break;
			}
		}
	}
};



int main()
{

    std::cout << "Turing Machine\n";
	std::cout << "2025 Copyright AXEWIELDER GOODS\n";

	std::string program = "";
	int p[] = {ST,WT,1,LT,WT,1,LT,WT,1,ED};
	for (int i : p) {
		program += char(i);
	}

	TuringMachine Alpha(program);

    std::cout << (1ULL << 60) << std::endl;
}
