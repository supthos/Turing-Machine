// TuringMachine.h
// Reed Axewielder
// 2025 Copyright AXEWIELDER GOODS
// GMDA
#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <valarray>
#include <string>

enum TMInst: char
{
	FE, TE, ST, LT, RT, WE, RD, RN, LD, CL, ED, NG = -1
};

template<typename T>
using TMTape = std::valarray<T>;

template <typename T>
TMTape<T> MakeTape(const unsigned long long& k) {
	return std::valarray<T>(T(false), std::pow(2, double(k)));
}

class TuringMachine {
public:
	TuringMachine() {
		NewTape(16);
	}
	~TuringMachine() {}

	TuringMachine(TMTape<bool>& tape) {
		Tape = tape;
		zero = Tape.size() / 2;
		order = std::log2(zero) + 1;
	}

	// turing machine with tape of 2^n squares
	TuringMachine(const unsigned long long& n) {
		NewTape(n);
	}

	template <typename T>
	TuringMachine(T program) {
		LoadAndRun(program);
	}

	template <typename T>
	TuringMachine(TMTape<bool>& tape, T program) {
		TuringMachine(tape);
		LoadAndRun(program);
	}

	template <typename T>
	bool LoadAndRun(T program) {
		unsigned ld = Load(program);
		if (ld > 0 && states.size() > 0 && ld <= states.size()) {
			unsigned se = unsigned(states.size()) - ld;
			return Run(states[se]);
		}
		else return false;
	}

	void Reset() {
		Start();
		End();
	}

protected:
	std::vector<std::string> states{};
	long long head = 0;
	const unsigned long long infimum = 0;
	unsigned long long order = 16;
	unsigned long long zero = 1024*64 / 2;
	TMTape<bool> Tape = MakeTape<bool>(order);
	unsigned state = 0;
	unsigned count = 0;
	std::vector<unsigned> instnum{};
	std::vector<unsigned> previous{};
	
	void NewTape() {
		Tape = MakeTape<bool>(order);
	}
	//new tape with 2^n squares
	void NewTape(unsigned long long n) {
		Tape = MakeTape<bool>(n);
		zero = Tape.size() / 2;
		order = n;
	}
	void MoreTape() {
		TMTape<bool> VTape = MakeTape<bool>(order+1);

		if (VTape.size() == 2*Tape.size()) {
			for (unsigned i = 0; i < Tape.size(); i++) {
				VTape[i + zero] = Tape[i];
			}
			Tape = VTape;
			zero = Tape.size() / 2;
			++order;
		}
		else {
			std::cerr << "No more tape available. Sorry.\n"
				<< "Womp, womp...\n";
		}
	}
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
	bool Call(unsigned state) {
		bool retval = false;
		if (state < states.size()) {
			previous.push_back(this->state);
			instnum.push_back(count);
			this->state = state;

			retval = Run(states[state]);

			this->state = previous.back();
			previous.pop_back();
			count = instnum.back();
			instnum.pop_back();
		}
		else std::cerr << "State not in memory";
		return retval;
	}
	void Left() {
		if (--head < -(long long(zero))) {
			MoreTape();
		}
	}
	void Right() {
		if (++head >= long long(zero)) {
			MoreTape();
		}
	}
	void Write(bool a) {
		Tape[head + (zero)+1] = a;
	}
	bool Read() {
		return Tape[head + (zero)+1] ;
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

	bool Run(std::string Program) {
		bool retval = true;
		count = 0;
		std::string prgrm = "";
		for (std::string::iterator it = (Program.begin() + count); it < Program.end(); ++it) {
			char c = *it;
			count++;
			switch (c)
			{
			case ST: // Start. Initialize to zero.
			{
				std::string number = "";
					
				while (it + 1 != Program.end() && *(it+1) != NG) {
					++it;
					++count;
					number += *it;
				}

				if (number == "" && std::stoi(number)>=0) {
					Start(std::stoi(number));
				}
				else {
					Start();
				}
				break;
			}
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
						retval = Call(int(*it));
					else {
						++it;
						count++;
						if (it != Program.end() && *it != NG)
							retval = Call(int(*it));
					}

				}
				else if (it != Program.end() && *it == FE) {
					++it;
					count++;
					if (Read() == false)
						retval = Call(int(*it));
					else {
						++it;
						count++;
						if (it != Program.end() && *it != NG)
							retval = Call(int(*it));
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

				TuringMachine* TM = new TuringMachine();
				retval = TM->LoadAndRun(prgrm);
				delete(TM);
				if (it == Program.end()) { break; break; }
				break;
			}
			case ED: // End the program and print the machine state.
				End();

				break;

			default:
				retval = false;
				break;
			}
		}
		return retval;
	}
};
