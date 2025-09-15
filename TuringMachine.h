// TuringMachine.h
// Reed Axewielder
// 2025 Copyright AXEWIELDER GOODS
// GMDA
#pragma once
#include <vector>
#include <iostream>
#include <valarray>

enum TMInst: char
{
	FE, TE, ST, LT, RT, WE, RD, RN, LD, CL, ED, NG = -1
};

class TuringMachine {
public:
	TuringMachine() {
	}
	~TuringMachine() {}

	TuringMachine(std::valarray<bool>& tape) {
		Tape = tape;
		VASize = tape.size();
		zero = VASize / 2;
	}

	TuringMachine(const unsigned long long& n) {
		NewTape(n);
	}

	template <typename T>
	TuringMachine(T program) {
		LoadAndRun(program);
	}

	template <typename T>
	TuringMachine(std::valarray<bool>& tape, T program) {
		TuringMachine(tape);
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

protected:
	std::vector<std::string> states{};
	long long head = 0;
	const unsigned long long infimum = 0;
	size_t VASize = 1024*64;
	unsigned long long zero = VASize / 2;
	unsigned state = 0;
	unsigned count = 0;
	std::vector<unsigned> instnum{};
	std::vector<unsigned> previous{};
	
	std::valarray<bool> Tape = std::valarray<bool>(false, VASize);

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
	void Left() { 
		if (--head < -(long long(zero))) {
			zero = VASize;
			VASize = 2 * VASize;
			
			std::valarray<bool> VTape = std::valarray<bool>(false, VASize); 
			for (unsigned i = 0; i < zero; i++) {
				VTape[i + zero] = Tape[i];
			}
			Tape = VTape;
		}
	}
	void Right() { 
		if (++head >= long long(zero)) {
			zero = VASize;
			VASize = 2 * VASize;

			std::valarray<bool> VTape = std::valarray<bool>(false, VASize);
			for (unsigned i = 0; i < zero; i++) {
				VTape[i + zero] = Tape[i];
			}
			Tape = VTape;
		}
	}
	void Write(bool a) {
		if (a == true) Tape[head+(zero)+1] = (Tape[head + (zero)+1] || true);
		else Tape[head + (zero)+1] = (Tape[head + (zero)+1] && false);
	}
	bool Read() {
		return Tape[head + (zero)+1] ;
	}
	void NewTape(unsigned long long n) {
		VASize = unsigned long long(std::pow(2, n));
		zero = VASize / 2;
		Tape = std::valarray<bool>(false, VASize);
	}
	void Start(unsigned long long n) {
		head = state = count = 0;

		NewTape(n);

		states.clear();
		instnum.clear();
		previous.clear();
	}
	void Start() {
		head = state = count = 0;

		Tape = false;

		states.clear();
		instnum.clear();
		previous.clear();

	}
	void End() {
		std::string mess = "";
		std::string lt = "", rt = "";
		long long block = head / 64;
		for (unsigned i = 0; i < 64; i++) {
			if (Tape[(zero/64 + block - 1) * 64 + i + 1] == false) lt += "0";
			else lt += "1";
			if (Tape[(zero/64 + block + 1) * 64 - i] == false) rt += "0";
			else rt += "1";
		}

		if (head >= 0) {
			for (int i = 0; i < 63 - (head % 64); i++) {
				mess += " ";
			}
			mess += "_";
		}
		else if (head < 0) {
			for (int i = -1; i > -64 - ((head + 1) % 64); i--) {
				mess += " ";
			}
			mess += "^";
		}


		std::cout << "State of the machine: \n"
			<< lt << std::endl
			<< mess << std::endl
			<< rt << std::endl;
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
		for (std::string::iterator it = (Program.begin() + count); it < Program.end(); ++it) {
			char c = *it;
			count++;
			switch (c)
			{
			case ST: // Start. Initialize to zero.
				Start();
				break;
			case LT: // Move tape head to the left.
				Left();
				break;

			case RT: // Move tape head to the right.
				Right();
				break;

			case CL: // Conditional Call
			{

				++it;
				count++;
				if (it != Program.end() && *it == TE) {
					++it;
					count++;
					if (Read() == true)
						Call(int(*it));
					else {
						++it;
						count++;
						if (it != Program.end() && *it != NG)
							Call(int(*it));
					}

				}
				else if (it != Program.end() && *it == FE) {
					++it;
					count++;
					if (Read() == false)
						Call(int(*it));
					else {
						++it;
						count++;
						if (it != Program.end() && *it != NG)
							Call(int(*it));
					}
				}
				if (it == Program.end()) {
					break; break;
				}
				break;
			}
			case WE: // Write TE or FE at tape head.
				if ((it + 1) == Program.end()) { break; break; }
				if (*(it + 1) == TE) Write(true);
				if (*(it + 1) == FE) Write(false);
				++it;
				count++;
				break;
			case NG: // Do nothing.
				break;
			case RD: // Read the value at the tape head.
				Read();
				break;
			case LD: // Load a machine state program.
			{
				std::string prog(it + 1, Program.end());

				if (!prog.empty() && prog != "") {
					Load(prog);
				}
				break;
			}

			case RN: // Run the following program until the end.
			{
				prgrm.clear();
				prgrm = "";
				++it;
				count++;
				while (it != Program.end() && (*it) != ED) {
					prgrm += (*it);
					++it;
					count++;
				}
				if ((*it) == ED) {
					prgrm += (*it);
					++it;
					count++;
				}

				new TuringMachine(prgrm);
				if (it == Program.end()) { break; break; }
				break;
			}
			case ED: // End the program and print the machine state.
				End();

				break;

			default:
				break;
			}
		}
	}
};
