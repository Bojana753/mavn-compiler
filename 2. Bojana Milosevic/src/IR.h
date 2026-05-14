/* Autor: Bojana Milosevic Datum: 6. 6. 2024. */
#ifndef __IR__
#define __IR__

#include "Types.h"
using namespace std;

class Label {
public:
	enum LabelType {
		FUNC, LABELA
	};
	LabelType type;
	int position;
	string imeLabele;
	Label() {
		position = -1;
		imeLabele = "";
	}
	Label(int pozL, string ime, LabelType lt) {
		imeLabele = ime;
		position = pozL;
		type = lt;
	}

};

class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};
	string getName() {
		return m_name;
	}

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, int pos, VariableType type) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign) {}
	VariableType getType() {
		return m_type;
	}
	void setName(string s) {
		m_name = s;
	}
	void setPosition(int position) {
		m_position = position;
	}
	void setType(VariableType v) {
		m_type = v;
	}
	int getPosition() {
		return m_position;
	}
	Regs getRegs() {
		return m_assignment;
	}
	void setValue(string value) {
		this->value = value;
	}
	string getValue() {
		return value;
	}
	void setRegs(Regs r) {
		m_assignment = r;
	}
private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	string value;
};


/**
 * This type represents list of variables from program code.
 */
typedef std::list<Variable*> Variables;


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction() : m_position(0), m_type(I_NO_TYPE) {}
	Instruction(int pos, InstructionType type, Variables& dst, Variables& src) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}


	int m_position;
	InstructionType m_type;

	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;
	string labName;
	string labValue;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;
typedef std::vector<vector<int>> InterferenceMatrix;
typedef std::stack<Variable*> SimplificationStack;

#endif




//#ifndef __IR__
//#define __IR__
//
//#include "Types.h"
//
//
///**
// * This class represents one variable from program code.
// */
//class Variable
//{
//public:
//	enum VariableType
//	{
//		MEM_VAR,
//		REG_VAR,
//		NO_TYPE
//	};
//
//	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
//	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}
//
//private:
//	VariableType m_type;
//	std::string m_name;
//	int m_position;
//	Regs m_assignment;
//};
//
//
///**
// * This type represents list of variables from program code.
// */
//typedef std::list<Variable*> Variables;
//
//
///**
// * This class represents one instruction in program code.
// */
//class Instruction
//{
//public:
//	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
//	Instruction (int pos, InstructionType type, Variables& dst, Variables& src) :
//		m_position(pos), m_type(type), m_dst(dst), m_src(src) {}
//
//private:
//	int m_position;
//	InstructionType m_type;
//	
//	Variables m_dst;
//	Variables m_src;
//
//	Variables m_use;
//	Variables m_def;
//	Variables m_in;
//	Variables m_out;
//	std::list<Instruction*> m_succ;
//	std::list<Instruction*> m_pred;
//};
//
//
///**
// * This type represents list of instructions from program code.
// */
//typedef std::list<Instruction*> Instructions;
//
//#endif
