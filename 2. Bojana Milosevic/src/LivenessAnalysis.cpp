/* Autor: Bojana Milosevic Datum: 6. 6. 2024. */

#include "LivenessAnalysis.h"

//pomocna funkcija za proveravanje postojanja promenjive u listi promenivih
bool postojiVar(Variable* promenljiva, Variables skupPromenljivih)
{
	for (auto iter = skupPromenljivih.begin(); iter != skupPromenljivih.end(); ++iter)
	{
		if ((*iter)->getName() == promenljiva->getName())
		{
			return true;
		}
	}
	return false;
}
//nadji sve promenjive koje se nalaze u izlaz ali ne i u definise
Variables filterVariables(const Variables& izlaz, const Variables& definise)
{
	Variables pomoc;
	for (const auto& var : izlaz)
	{
		if (!postojiVar(var, definise))
		{
			pomoc.push_back(var);
		}
	}
	return pomoc;
}

void livenessAnalysis(Instructions instrukcije)
{
	bool promenjeno = true;

	while (promenjeno)
	{
		promenjeno = false;
		//iteriraj kroz intrukcije od nazad
		Instructions::reverse_iterator revIter = instrukcije.rbegin();
		for (; revIter != instrukcije.rend(); revIter++) {
			Variables& izlazPre = (*revIter)->m_out;
			Variables& ulazPre = (*revIter)->m_in;
			Variables izlaz;
			Variables ulaz;

			//nadji sve in-ove naslednika trenutne instrukcije
			Instructions& naslednici = (*revIter)->m_succ;
			Instructions::iterator naslIter = naslednici.begin();
			for (; naslIter != naslednici.end(); naslIter++) {
				Variables& ulazNasl = (*naslIter)->m_in;
				izlaz.insert(izlaz.end(), ulazNasl.begin(), ulazNasl.end());
			}
			izlaz.sort();
			izlaz.unique();

			//pomoc = izlaz bez def
			Variables pomoc = filterVariables(izlaz, (*revIter)->m_def);

			// uniraj use i out - def(pomoc)
			ulaz = (*revIter)->m_use;
			ulaz.insert(ulaz.end(), pomoc.begin(), pomoc.end());
			ulaz.sort();
			ulaz.unique();

			if (ulaz != ulazPre || izlaz != izlazPre) {
				promenjeno = true;
			}

			ulazPre = ulaz;
			izlazPre = izlaz;
		}
	}
}
