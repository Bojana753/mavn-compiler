#pragma once

#include "LexicalAnalysis.h"
#include "Token.h"
#include "IR.h"

class SyntaxAnalysis
{
public:
	
	list<Variable*> getVars() {
		return variables;
	}
	list<Label*> getLabs() {
		return labels;
	}
	list<Instruction*> getInstructions() {
		return instructions;
	}
	Instruction* nadjiInstrukciju(int pos);
	void sucingpredCreate();
	/**
	* Constructor
	*/
	SyntaxAnalysis(LexicalAnalysis& lex);

	/**
	* Method which performs lexical analysis
	*/
	Variable* nadjiVariable(string);
	Label* nadjiLabelu(string);
	bool Do();
private:
	bool varExistcheck(string);
	/**
	* Prints the error message, and token that caused the syntax error
	*/
	void printSyntaxError(Token token);

	/**
	* Prints the token info
	*/
	void printTokenInfo(Token token);

	bool labExistcheck(string);
	/**
	* Eats the current token if its type is "t"
	* otherwise reports syntax error
	*
	* param[in] - t - the expected token type
	*/
	void eat(TokenType t);

	/**
	* Returns the next token from the token list
	*/
	Token getNextToken();

	/**
	 * Nonterminal Q
	 */
	void Q();

	/**
	 * Nonterminal S
	 */
	void S();

	/**
	 * Nonterminal L
	 */
	void L();

	/**
	 * Nonterminal E
	 */
	void E();

	/**
	* Reference to lexical analysis module
	*/
	LexicalAnalysis& lexicalAnalysis;

	/**
	* Syntax error indicator
	*/
	bool errorFound;

	int labelNum;
	int registerNum;
	list<Label*> labels;
	list<Variable*> variables;
	list<Instruction*> instructions;
	TokenList::iterator tokenIterator;

	
	Token currentToken;
};
