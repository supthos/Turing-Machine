// Turing Machine.cpp : 
// Reed Axewielder

#include <vector>
#include <bitset>
#include <iostream>

enum TMInst
{
    FE, TE, ST, LT, RT, WE, RD, RN, LD, CL, ED, NG = -1
};

class TuringMachine {
public:
	TuringMachine() {
	}
	~TuringMachine() {}
	TuringMachine(std::vector<std::string> program) {
		LoadAndRun(program);
	}
	TuringMachine(std::string program) {
		LoadAndRun(program);
	}

	template <typename T>
	void LoadAndRun(T program) {
		unsigned ld = Load(program);
		if (ld > 0 && states.size() > 0 && ld <= states.size()) {
			unsigned se = unsigned(states.size()) - ld;
			Run(states[se]);	
		}
	}
	void Reset() {
		Start();
		End();
	}
private:
	std::vector<std::string> states {};
	short head = 0;
	unsigned state = 0;
	unsigned count = 0;
	std::vector<unsigned> instnum {};
	std::vector<unsigned> previous {};
	unsigned long long Rtape[512]{};
	unsigned long long Ltape[512]{};

	unsigned Load(std::string program) { 
		unsigned count = 0;
		std::string prog = "";
		for (auto it = program.begin(); it != program.end(); ++it) {
			if (*it != LD) {
				prog += (*it);
			}
			else {
				if (!prog.empty() && prog != "") {
					states.push_back(prog);
					count++;
				}
				prog = "";
			}
		}
		if (!prog.empty() && prog != "") {
			states.push_back(prog);
			count++;
		}
		return count;
	}
	unsigned Load(std::vector<std::string> program) { 
		unsigned count = 0;
		for (std::string& s : program) {
			count += Load(s);
		}
		return count;
	}
	void Call(unsigned state) {
		if (state < states.size()) {
			previous.push_back(this->state);
			instnum.push_back(count);
			this->state = state;

			Run(states[state]);
			
			this->state = previous.back();
			previous.pop_back();
			count = instnum.back();
			instnum.pop_back();
		}
		else std::cerr << "State not in memory";
	}
	void Left() { head--; }
	void Right() { head++; }
	void Write(bool a) {
		if (head >= 0){
			unsigned tape = head / 64;
			if (a == true) Rtape[tape] = Rtape[tape] | (1ULL << (head % 64));
			else Rtape[tape] = Rtape[tape] & ~(1ULL << (head % 64));
		}
		else if(head<0) {
			unsigned tape = abs((head + 1) / 64);
			if (a == true) Ltape[tape] = Ltape[tape] | (1ULL << ((abs((head + 1) % 64))));
			else Ltape[tape] = Ltape[tape] & ~(1ULL << (abs((head + 1) % 64)));
		}
	}
	bool Read() {
		if (head >= 0) {
			unsigned tape = head / 64;
			unsigned long long val = Rtape[tape] & (1ULL << (head % 64));
			if (val == 0) return false;
			else return true;
		}
		else if (head < 0) {
			unsigned tape = abs((head + 1) / 64);
			unsigned long long val = Ltape[tape] & (1ULL << (abs((head + 1) % 64)));
			if (val == 0) return false;
			else return true;
		}
	}
	void Start() {
		head = state = count = 0;
		for (unsigned u = 0; u < 512; u++) {
			Ltape[u] = 0;
			Rtape[u] = 0;
		}

		states.clear();
		instnum.clear();
		previous.clear();

	}
	void End() {
		std::string mess = "";
		if (head >= 0) {
			for (int i = 0; i < 63 - (head % 64); i++) {
				mess += " ";
			}
			mess += "_";

		}

		if (head < 0) {
			for (int i = -1; i > -64 - ((head+1)%64) ; i--) {
				mess += " ";
			}
			mess += "^";
		}


		std::cout << "State of the machine: \n"
			<< std::bitset<64>(Ltape[(abs((head + 1) / 64))]) << std::endl
			<< mess<< std::endl
			<< std::bitset<64>(Rtape[(head/64)]) << std::endl;
				std::cout << "Head: " << head << std::endl;
		std::cout << "State: " << state << std::endl;
		std::cout << "Count: " << count << std::endl;
		std::cout << "Trail: ";

		for (auto a : previous) {
			std::cout << a << " ";
		}
		std::cout << "\n"
			<< "Route: ";
		for (auto a : instnum) {
			std::cout << a << " ";
		}
		std::cout << "\n"
			<< "States: " << states.size() << std::endl;
	}

	void Run(std::string Program) {
		count = 0;
		std::string prgrm = "";
		for (std::string::iterator it = (Program.begin()+count); it < Program.end(); ++it) {
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
					if (it != Program.end() && *it == char(int(TE))) {
						++it;
						count++;
						if (Read() == true)
							Call(int(*it));
						else {
							++it;
							count++;
							if (it != Program.end() && *it != char(int(NG)))
								Call(int(*it));
						}

					}
					else if (it != Program.end() && *it == char(int(FE))) {
						++it;
						count++;
						if (Read() == false)
							Call(int(*it));
						else {
							++it;
							count++;
							if (it != Program.end() && *it != char(int(NG)))
								Call(int(*it));
						}
					}
					if (it == Program.end()) {
						break; break;
					}
				}
				case char(int(WE)) : // Write TE or FE at tape head.
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
					std::string prog (it+1, Program.end());

					if (!prog.empty() && prog != "") {
						Load(prog);
					}
					break;
				}

				case char(int(RN)) : // Run the following program until the end.
				{
					prgrm.clear();
					prgrm = "";
					++it;
					count++;
					while (it != Program.end() && (*it) != char(int(ED))) {
						prgrm += (*it);
						++it;
						count++;
					}
					if ((*it) == char(int(ED))) {
						prgrm += (*it);
						++it;
						count++;
					}
				
					new TuringMachine(prgrm);
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
	int p[] = {ST,WE,1,LT,WE,1,LT,WE,1,ED};
	for (int i : p) {
		program += char(i);
	}

	TuringMachine* Alpha = new TuringMachine(program);

	Alpha->Reset();

	program.clear();
	program = "";
	int r[] = { LD, WE, TE, RT, CL, FE, 1, NG, RT, WE, FE, ED, LD, WE, TE, LT, WE, FE, ED };
	for (int i : r) {
		program += char(i);
	}

	Alpha->LoadAndRun(program);
	TuringMachine Beth(program);

	return 0;
}
