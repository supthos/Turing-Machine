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
	}
	~TuringMachine() {}
	TuringMachine(std::vector<std::string> program) {
		for (auto& i: program)
			Load(i);
		Run(states[0]);
	}
	TuringMachine(std::string program) {
		Run(program);
	}
private:
	std::vector<std::string> states {};
	int head = 0;
	unsigned state = 0;
	unsigned count = 0;
	std::vector<unsigned> instnum {};
	std::vector<unsigned> previous {};
	unsigned long long Rtape = 0;
	unsigned long long Ltape = 0;

	void Load(std::string program) { states.push_back(program); }
	void Load(std::vector<std::string> program) { states = program; }
	void Call(unsigned state, unsigned inst) {
		previous.push_back(this->state);
		instnum.push_back(inst);
		this->state = state;
		if (state < states.size()) {
			Run(states[state]);
		}
		else std::cerr << "State not in memory";
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
		head = Ltape = Rtape = state = count = 0;
		states.clear();
		
		previous.clear();
	}
	void End() {
		std::cout << "State of the machine: \n"
			<< std::bitset<64>(Ltape) << std::endl
			<< std::bitset<64>(Rtape) << std::endl;
		std::cout << "Head: " << head << std::endl;
		std::cout << "State: " << state << std::endl;
		std::cout << "Count: " << count << std::endl;

		if (!previous.empty() && !instnum.empty()) {
			state = previous.back();
			previous.pop_back();
			Run(states[state], instnum.back() + 1); // must make sure this +1 is necessary.
			instnum.pop_back();
		}

	}
	void Run(std::string program) {
		Run(program, 0);

	}
	void Run(std::string Program, unsigned bytes) {
		std::string prgrm = "";
		for (std::string::iterator it = (Program.begin()+bytes); it < Program.end(); ++it) {
			char c = *it;
			count++;
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
					count++;
					if (it != Program.end() && *it == char(TE)) {
						++it;
						count++;
						if (Read() == true)
							Call(int(*it),count);
						else {
							++it;
							count++;
							if (it != Program.end() && *it != char(NG))
								Call(int(*it), count);
						}

					}
					else if (it != Program.end() && *it == char(FE)) {
						++it;
						count++;
						if (Read() == false)
							Call(int(*it), count);
						else {
							++it;
							count++;
							if (it != Program.end() && *it != char(NG))
								Call(int(*it), count);
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
					count++;
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
						count++;
						prog += (*it);
					}
					Load(prog);
				}

				case char(int(RN)) : // Run the following program until the end.
				{
					prgrm.clear();
					++it;
					count++;
					while (it != Program.end() && (*it) != char(ED)) {
						prgrm += (*it);
						++it;
						count++;
					}
					if ((*it) == char(ED)) {
						prgrm += (*it);
						++it;
						count++;
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
	std::cout << "2025 Copyright AXEWIELDER GOODS\n"
		<< "New York, NY, USA\n";

	std::string program = "";
	int p[] = {ST,WT,1,LT,WT,1,LT,WT,1,ED};
	for (int i : p) {
		program += char(i);
	}

	TuringMachine Alpha(program);

    std::cout << (1ULL << 60) << std::endl;
}
