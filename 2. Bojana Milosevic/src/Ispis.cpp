/* Autor: Bojana Milosevic Datum: 6. 6. 2024. */

#include "Ispis.h"

// Funkcija koja generise MIPS asembler kod na osnovu sintaksne analize
void ispis(SyntaxAnalysis sintaks) {
    // Moguci registri
    string registri[4] = { "$t0", "$t1", "$t2", "$t3" };

    // Otvaranje fajla za upis
    ofstream fajl;
    fajl.open(".\\..\\examples\\simple1.s");

    // Dobijanje liste instrukcija, labela i promenljivih iz objekta sintaks
    list<Instruction*> instrukcije = sintaks.getInstructions();
    list<Label*> labele = sintaks.getLabs();
    list<Variable*> promenljive = sintaks.getVars();
    Label* glavnaFunkcija = *(labele.begin());

    // Upis globalne funkcije
    fajl << ".global ";
    fajl << glavnaFunkcija->imeLabele;
    fajl << endl;
    fajl << endl;

    // Pocetak sekcije sa podacima
    fajl << ".data" << endl;

    // Iteriranje kroz promenljive i upisivanje memorijskih promenljivih
    for (auto it = promenljive.begin(); it != promenljive.end(); it++) {
        if ((*it)->getType() == Variable::MEM_VAR) {
            fajl << (*it)->getName() << ":\t.word " << (*it)->getValue() << endl;
        }
    }

    // Pocetak sekcije sa kodom
    fajl << endl << ".text" << endl;

    // Iteriranje kroz instrukcije i upisivanje u fajl
    for (auto instIter = instrukcije.begin(); instIter != instrukcije.end(); instIter++) {
        // Provera da li postoji labela na trenutnoj poziciji instrukcije
        for (auto lblIter = labele.begin(); lblIter != labele.end(); lblIter++) {
            if ((*lblIter)->position == (*instIter)->m_position) {
                fajl << (*lblIter)->imeLabele << ":" << endl;
            }
        }

        // Upisivanje instrukcija u odgovarajucem MIPS formatu
        if ((*instIter)->m_type == I_ADD) {
            fajl << "add \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.back()->getRegs()] << endl;
        }
        else if ((*instIter)->m_type == I_ADDI) {
            fajl << "addi \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << (*instIter)->labValue << endl;
        }
        else if ((*instIter)->m_type == I_LA) {
            fajl << "la \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << (*instIter)->m_src.front()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_LW) {
            fajl << "lw \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << (*instIter)->labValue << "(" << registri[(*instIter)->m_src.front()->getRegs()] << ")" << endl;
        }
        else if ((*instIter)->m_type == I_B) {
            fajl << "b \t" << (*instIter)->labName << endl;
        }
        else if ((*instIter)->m_type == I_LI) {
            fajl << "li \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << (*instIter)->labValue << endl;
        }
        else if ((*instIter)->m_type == I_BLTZ) {
            fajl << "bltz \t" << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << (*instIter)->labName << endl;
        }
        else if ((*instIter)->m_type == I_SUB) {
            fajl << "sub \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.back()->getRegs()] << endl;
        }
        else if ((*instIter)->m_type == I_SW) {
            fajl << "sw \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << "(" << (*instIter)->labValue << ")"
                << registri[(*instIter)->m_src.back()->getRegs()] << endl;
        }
        else if ((*instIter)->m_type == I_NOP) {
            fajl << "nop \t" << endl;
        }
        else if ((*instIter)->m_type == I_DIV) {
            fajl << "div \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.back()->getRegs()] << endl;
        }
        else if ((*instIter)->m_type == I_OR) {
            fajl << "or \t" << registri[(*instIter)->m_dst.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << registri[(*instIter)->m_src.back()->getRegs()] << endl;
        }
        else if ((*instIter)->m_type == I_BGTZ) {
            fajl << "bgtz \t" << registri[(*instIter)->m_src.front()->getRegs()] << ", "
                << (*instIter)->labName << endl;
        }
    }
    // Zatvaranje fajla
    fajl.close();
}

// Funkcija koja generise izlazni fajl sa analizom zivosti promenljivih
void livenessIspis(SyntaxAnalysis sintaks) {
    // Otvaranje fajla za upis
    ofstream fajl;
    fajl.open(".\\..\\examples\\simpleAnalisys1.s");

    // Dobijanje liste instrukcija iz objekta sintaks
    Instructions instrukcije = sintaks.getInstructions();

    // Iteriranje kroz instrukcije i upisivanje analize zivosti u fajl
    for (auto instIter = instrukcije.begin(); instIter != instrukcije.end(); instIter++) {
        Instruction* ins = (*instIter);

        // Upisivanje ulaznih promenljivih za trenutnu instrukciju
        fajl << "\t\t\tIN:" << endl;
        for (auto varIter = ins->m_in.begin(); varIter != ins->m_in.end(); varIter++) {
            fajl << "\t\t\t" << (*varIter)->getName() << endl;
        }

        // Upisivanje instrukcije u formatu citljivom za analizu
        if ((*instIter)->m_type == I_ADD) {
            fajl << "add \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->m_src.back()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_ADDI) {
            fajl << "addi \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->labValue << endl;
        }
        else if ((*instIter)->m_type == I_LA) {
            fajl << "la \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->m_src.front()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_LW) {
            fajl << "lw \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->labValue << "(" << (*instIter)->m_src.front()->getName() << ")" << endl;
        }
        else if ((*instIter)->m_type == I_B) {
            fajl << "b \t" << (*instIter)->labName << endl;
        }
        else if ((*instIter)->m_type == I_LI) {
            fajl << "li \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->labValue << endl;
        }
        else if ((*instIter)->m_type == I_BLTZ) {
            fajl << "bltz \t" << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->labName << endl;
        }
        else if ((*instIter)->m_type == I_SUB) {
            fajl << "sub \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->m_src.back()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_SW) {
            fajl << "sw \t" << (*instIter)->m_dst.front()->getName() << ", "
                << "(" << (*instIter)->labValue << ")"
                << (*instIter)->m_src.back()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_NOP) {
            fajl << "nop \t" << endl;
        }
        else if ((*instIter)->m_type == I_DIV) {
            fajl << "div \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->m_src.back()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_OR) {
            fajl << "or \t" << (*instIter)->m_dst.front()->getName() << ", "
                << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->m_src.back()->getName() << endl;
        }
        else if ((*instIter)->m_type == I_BGTZ) {
            fajl << "bgtz \t" << (*instIter)->m_src.front()->getName() << ", "
                << (*instIter)->labName << endl;
        }

        // Upisivanje izlaznih promenljivih za trenutnu instrukciju
        fajl << "\t\t\tOUT:" << endl;
        for (auto varIter = ins->m_out.begin(); varIter != ins->m_out.end(); varIter++) {
            fajl << "\t\t\t" << (*varIter)->getName() << endl;
        }
        fajl << endl;
        fajl << endl;
    }
    // Zatvaranje fajla
    fajl.close();
}
