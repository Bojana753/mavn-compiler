/* Autor: Bojana Milosevic Datum: 6. 6. 2024. */

#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;
SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin())
{
	labelNum = 0;
	registerNum = 0;
}


bool SyntaxAnalysis::Do()
{
	currentToken = getNextToken();

	// TO DO: Call function for the starting non-terminal symbol
	Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}


void SyntaxAnalysis::printTokenInfo(Token token)
{
	cout << setw(20) << left << token.tokenTypeToString(token.getType());
	cout << setw(25) << right << token.getValue() << endl;
}


void SyntaxAnalysis::eat(TokenType t)
{
	if (currentToken.getType() == T_END_OF_FILE) {
		return;
	}
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			cout << currentToken.getValue() << endl;
			currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;

			throw runtime_error("syntax failed");
		}
	}
}


Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == lexicalAnalysis.getTokenList().end())
		throw runtime_error("End of input file reached");
	return *tokenIterator++;
}

void SyntaxAnalysis::S()
{
	TokenType tipTokena = currentToken.getType();
	if (tipTokena == T_MEM) {
		eat(T_MEM);
		Variable* var = new Variable(currentToken.getValue(), -1, Variable::VariableType::MEM_VAR);
		eat(T_M_ID);
		var->setValue(currentToken.getValue());
		eat(T_NUM);

		if (varExistcheck(var->getName())) {
			errorFound = true;
			throw runtime_error("promenjiva postoji ");
		}
		variables.push_back(var);

	}
	else if (tipTokena == T_FUNC) {
		eat(T_FUNC);
		if (varExistcheck(currentToken.getValue()))
		{
			errorFound = true;
			throw runtime_error("funk pogresna");
		}
		
		Label* label = new Label(labelNum, currentToken.getValue(), Label::LabelType::FUNC);
		eat(T_ID);
		labels.push_back(label);
	}
	else if (tipTokena == T_REG) {
		eat(T_REG);



		Variable* var = new Variable(currentToken.getValue(), registerNum, Variable::VariableType::REG_VAR);

		if (varExistcheck(var->getName()))
		{
			errorFound = true;
			throw runtime_error("registar pogresan");
		}

		eat(T_R_ID);
		variables.push_back(var);
		registerNum++;
	}
	else if (tipTokena == T_ID) {
		Label* lab = new Label(labelNum, currentToken.getValue(), Label::LabelType::LABELA);

		if (labExistcheck(lab->imeLabele))
		{
			errorFound = true;
			throw runtime_error("labela pogresna");
		}

		eat(T_ID);
		eat(T_COL);
		labels.push_back(lab);
		E();

	}
	else {
		E();
	}
}

void SyntaxAnalysis::Q()
{
	S();
	eat(T_SEMI_COL);
	L();
}




void SyntaxAnalysis::L()
{
	if (currentToken.getType() == T_END_OF_FILE) {
		eat(T_END_OF_FILE);
	}
	else {
		Q();
	}
}

//funkcija za proveravanje postojanja labele sa zadatim imenom
bool SyntaxAnalysis::labExistcheck(string imeM) {

	for (auto it = labels.begin(); it != labels.end(); it++)
	{
		if ((*it)->imeLabele == imeM)
		{
			return true;
		}
	}

	return false;
}

//funkcija za proveravanje postojanja promenjive sa zadatim imenom
bool SyntaxAnalysis::varExistcheck(string imeM) {

	for (auto it = variables.begin(); it != variables.end(); it++)
	{
		if ((*it)->getName() == imeM)
		{
			return true;
		}
	}

	return false;
}

void SyntaxAnalysis::E()
{

	if (currentToken.getType() == T_SUB) {
		//pronadji sve promenjive sa zadatim imenom
		eat(T_SUB);
		Variable* foundDestVar = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar2 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		src.push_back(foundSrcVar2);
		dst.push_back(foundDestVar);
		//napravi instrukciju sa svim datim informacijama
		Instruction* inst = new Instruction(labelNum, I_SUB, dst, src);

		//formiram use i def zbog liveness analisys
		inst->m_def.push_back(foundDestVar);
		inst->m_use.push_back(foundSrcVar1);

		inst->m_use.push_back(foundSrcVar2);
		instructions.push_back(inst);
		labelNum++;
	}
	else if (currentToken.getType() == T_ADD) {
		eat(T_ADD);
		Variable* foundDestVar = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar2 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		src.push_back(foundSrcVar2);
		dst.push_back(foundDestVar);
		Instruction* inst = new Instruction(labelNum, I_ADD, dst, src);
		inst->m_def.push_back(foundDestVar);

		inst->m_use.push_back(foundSrcVar1);

		inst->m_use.push_back(foundSrcVar2);

		instructions.push_back(inst);
		labelNum++;
	}
	else if (currentToken.getType() == T_LW) {
		eat(T_LW);
		Variable* foundDestVar = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);

		string number = currentToken.getValue();
		eat(T_NUM);
		eat(T_L_PARENT);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_R_PARENT);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		dst.push_back(foundDestVar);
		Instruction* inst = new Instruction(labelNum, I_LW, dst, src);
		inst->labValue = number;
		inst->m_def.push_back(foundDestVar);
		inst->m_use.push_back(foundSrcVar1);

		instructions.push_back(inst);
		labelNum++;
	}
	else if (currentToken.getType() == T_LA) {
		eat(T_LA);
		Variable* foundDestVar = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_M_ID);

		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		dst.push_back(foundDestVar);
		Instruction* inst = new Instruction(labelNum, I_LA, dst, src);

		inst->m_def.push_back(foundDestVar);
		inst->m_use.push_back(foundSrcVar1);

		instructions.push_back(inst);
		labelNum++;
	}
	else if (currentToken.getType() == T_B) {
		eat(T_B);
		Label* lbl = nadjiLabelu(currentToken.getValue());
		eat(T_ID);
		list<Variable*> src;
		list<Variable*> dst;
		Instruction* inst = new Instruction(labelNum, I_B, dst, src);
		inst->labName = lbl->imeLabele;
		instructions.push_back(inst);
		labelNum++;

	}
	else if (currentToken.getType() == T_BLTZ) {
		eat(T_BLTZ);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Label* lbl = nadjiLabelu(currentToken.getValue());
		eat(T_ID);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		Instruction* inst = new Instruction(labelNum, I_BLTZ, dst, src);
		inst->labName = lbl->imeLabele;

		inst->m_use.push_back(foundSrcVar1);
		instructions.push_back(inst);
		labelNum++;
	}

	///
	else if (currentToken.getType() == T_DIV) {
		eat(T_DIV);
		Variable* foundDestVar = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar2 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		src.push_back(foundSrcVar2);
		dst.push_back(foundDestVar);
		Instruction* inst = new Instruction(labelNum, I_DIV, dst, src);
		inst->m_def.push_back(foundDestVar);

		inst->m_use.push_back(foundSrcVar1);

		inst->m_use.push_back(foundSrcVar2);

		instructions.push_back(inst);
		labelNum++;
	}

	else if (currentToken.getType() == T_OR) {
		eat(T_OR);
		Variable* foundDestVar = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* foundSrcVar2 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		src.push_back(foundSrcVar2);
		dst.push_back(foundDestVar);
		Instruction* inst = new Instruction(labelNum, I_OR, dst, src);

		inst->m_def.push_back(foundDestVar);
		inst->m_use.push_back(foundSrcVar1);
		inst->m_use.push_back(foundSrcVar2);

		instructions.push_back(inst);
		labelNum++;
	}

	else if (currentToken.getType() == T_BGTZ) {
		eat(T_BGTZ);
		Variable* foundSrcVar1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Label* lbl = nadjiLabelu(currentToken.getValue());
		eat(T_ID);
		list<Variable*> src;
		list<Variable*> dst;
		src.push_back(foundSrcVar1);
		Instruction* inst = new Instruction(labelNum, I_BGTZ, dst, src);
		inst->labName = lbl->imeLabele;

		inst->m_use.push_back(foundSrcVar1);
		instructions.push_back(inst);
		labelNum++;
	}
	else if (currentToken.getType() == T_ADDI) {
		eat(T_ADDI);
		Variable* destinationVariable = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		Variable* sourceVariable1 = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		string number = currentToken.getValue();
		eat(T_NUM);
		list<Variable*> sourceVariables;
		list<Variable*> destinationVariables;
		sourceVariables.push_back(sourceVariable1);
		destinationVariables.push_back(destinationVariable);
		Instruction* instruction = new Instruction(labelNum, I_ADDI, destinationVariables, sourceVariables);

		instruction->m_def.push_back(destinationVariable);
		instruction->m_use.push_back(sourceVariable1);
		instruction->labValue = number;
		instructions.push_back(instruction);
		labelNum++;
	}
	else if (currentToken.getType() == T_SW) {
		eat(T_SW);
		Variable* destinationVariable = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);

	string number = currentToken.getValue();
		eat(T_NUM);
	eat(T_L_PARENT);
		Variable* sourceVariable1 = nadjiVariable(currentToken.getValue());
	eat(T_R_ID);
	eat(T_R_PARENT);
		list<Variable*> sourceVariables;
		list<Variable*> destinationVariables;
		sourceVariables.push_back(sourceVariable1);
		destinationVariables.push_back(destinationVariable);
		Instruction* instruction = new Instruction(labelNum, I_SW, destinationVariables, sourceVariables);
	instruction->labValue = number;
		instruction->m_def.push_back(destinationVariable);
		instruction->m_use.push_back(sourceVariable1);
		instructions.push_back(instruction);
		labelNum++;
	}
	else if (currentToken.getType() == T_LI) {
		eat(T_LI);
		Variable* destinationVariable = nadjiVariable(currentToken.getValue());
		eat(T_R_ID);
		eat(T_COMMA);
		string number = currentToken.getValue();
		eat(T_NUM);

		list<Variable*> sourceVariables;
		list<Variable*> destinationVariables;
		destinationVariables.push_back(destinationVariable);

		Instruction* instruction = new Instruction(labelNum, I_LI, destinationVariables, sourceVariables);

		instruction->m_def.push_back(destinationVariable);
	instruction->labValue = number;
		instructions.push_back(instruction);
		labelNum++;
	}
	else if (currentToken.getType() == T_NOP) {
		eat(T_NOP);

		list<Variable*> sourceVariables;
		list<Variable*> destinationVariables;
		Instruction* instruction = new Instruction(labelNum, I_NOP, destinationVariables, sourceVariables);

		instructions.push_back(instruction);
		labelNum++;
	}
}

//nadji promenjivu po imenu
Variable* SyntaxAnalysis::nadjiVariable(string ime) {
	for (auto variterator = variables.begin(); variterator != variables.end(); variterator++) {
		if ((*variterator)->getName() == ime) {
			return (*variterator);
		}
	}
	errorFound = true;
	throw runtime_error("error");
	return NULL;
}

//pronadji labelu po imenu
Label* SyntaxAnalysis::nadjiLabelu(string name) {
	for (auto variterator = labels.begin(); variterator != labels.end(); variterator++) {
		if ((*variterator)->imeLabele == name) {
			return (*variterator);
		}
	}
	errorFound = true;
	throw runtime_error("error");
	return NULL;
}

//nadji instrukciju po poziciji
Instruction* SyntaxAnalysis::nadjiInstrukciju(int pos) {
	for (auto it = instructions.begin(); it != instructions.end(); it++) {
		if ((*it)->m_position == pos) {
			return (*it);
		}
	}
	return NULL;
}

//konektuj dve instrukcije
void connectInstructions(Instruction* previous, Instruction* next) {
	previous->m_succ.push_back(next);
	next->m_pred.push_back(previous);
}
void SyntaxAnalysis::sucingpredCreate() {
	// za svaku instrukciju nadji sledeceg i prethodnog
	for (auto it = instructions.begin(); it != instructions.end(); it++) {
		// posebni slucajevi(skokovi)
		if ((*it)->m_type == I_B || (*it)->m_type == I_BLTZ || (*it)->m_type == I_BGTZ) {
			Label* label = nadjiLabelu((*it)->labName);
			Instruction* ins = nadjiInstrukciju(label->position);
			connectInstructions(*it, ins);	
		}
		//bezuslovni skok ne moze da ide na labelu ispod njega
		if ((*it)->m_type != I_B) {
			list<Instruction*>::iterator it1 = it;
			it1++;
			if (it1 != instructions.end()) {
				connectInstructions(*it, *it1);
			}
		}
	}
}