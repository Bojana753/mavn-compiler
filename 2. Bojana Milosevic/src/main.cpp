/* Autor: Bojana Milosevic Datum: 6. 6. 2024. */
#include <iostream>
#include <exception>
#include "LexicalAnalysis.h"
#include "IR.h"
#include "LivenessAnalysis.h"

#include "Ispis.h"

using namespace std;


int main()
{
	try
	{
		std::string fileName = ".\\..\\examples\\simple1.mavn";

		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}
		bool done;
		SyntaxAnalysis syntaxAn(lex);
		done = syntaxAn.Do();

		if (done)
		{
			cout << "Syntax analysis finished successfully!" << endl;
		}
		else
		{
			throw runtime_error("\nException! Syntax analysis failed!\n");
		}

		syntaxAn.sucingpredCreate();
		livenessAnalysis(syntaxAn.getInstructions());

		ispis(syntaxAn);
		livenessIspis(syntaxAn);
		int n;
		cout << "Enter key" << endl;
		cin >> n;

	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		return 1;
	}

	return 0;
}
