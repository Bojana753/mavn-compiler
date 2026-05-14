# MAVN Compiler

> **MAVN** (MIPS Assembler Visokog Nivoa) — prevodilac sa višeg MIPS asemblerskog jezika na standardni MIPS 32-bit asemblerski jezik.

Ispitni rad iz predmeta **Osnovi paralelnog programiranja i softverski alati**  
Fakultet tehničkih nauka, Univerzitet u Novom Sadu, jun 2024.

---

## O projektu

MAVN jezik uvodi koncept **registarskih promenljivih**, što programerima omogućava da pišu instrukcije koristeći promenljive umesto konkretnih registara. Prevodilac automatski dodeljuje resurse (registre `$t0`–`$t3`) i generiše validan MIPS 32-bit asemblerski kod.

### Podržane instrukcije

| Instrukcija | Opis |
|---|---|
| `add` | Sabiranje |
| `addi` | Sabiranje sa konstantom |
| `sub` | Oduzimanje |
| `div` | Deljenje |
| `or` | Logičko ILI |
| `la` | Učitavanje adrese u registar |
| `li` | Učitavanje konstante u registar |
| `lw` | Čitanje memorijske reči |
| `sw` | Upis memorijske reči |
| `b` | Bezuslovni skok |
| `bltz` | Skok ako je registar < 0 |
| `bgtz` | Skok ako je registar > 0 |
| `nop` | Bez operacije |

---

## Arhitektura

Prevodilac se sastoji od tri faze:

```
.mavn fajl  →  Leksička analiza  →  Sintaksna analiza  →  Analiza životnog veka  →  .s fajl
```

### Komponente

- **LexicalAnalysis** — tokenizacija ulaznog koda pomoću konačnog automata (`FiniteStateMachine`)
- **SyntaxAnalysis** — provera gramatičkih pravila MAVN jezika
- **LivenessAnalysis** — analiza životnog veka promenljivih, optimizacija dodele registara
- **IR** — međureprezentacija (`Variable`, `Instruction`, `Label`)
- **Constants** — konstante korišćene u celom programu

### Gramatika MAVN jezika

```
Q → S ; L
S → _mem mid num  |  _reg rid  |  _func id  |  id: E  |  E
L → eof  |  Q
E → add rid, rid, rid  |  addi rid, rid, num  |  sub rid, rid, rid
  | la rid, mid       |  lw rid, num(rid)    |  li rid, num
  | sw rid, num(rid)  |  b id               |  bltz rid, id  |  nop
```

---

## Rezultati prevođenja

### Generisani MIPS asemblerski kod (`simple.s`)

![simple.s output](https://i.imgur.com/Oq57Jv5.png)

### Analiza životnog veka promenljivih (`simpleLiveness.s`)

![simpleLiveness.s](https://i.imgur.com/UXtOwnN.png)

---

## Pokretanje projekta

### Preduslovi

- Windows OS
- [Visual Studio 2022](https://visualstudio.microsoft.com/) sa instaliranim **Desktop development with C++** radnim opterećenjem

### Koraci

1. Kloniraj repozitorijum:
```bash
git clone https://github.com/Bojana753/mavn-compiler.git
```

2. Otvori fajl `2. Bojana Milosevic/src/LexicalAnalysis.sln` u Visual Studio-u

3. Pokreni aplikaciju pritiskom na **F5** ili dugme **Start**

4. U konzoli unesi putanju do ulaznog `.mavn` fajla i željenog izlaznog `.s` fajla

### Testni primeri

Gotovi primeri se nalaze u folderu `2. Bojana Milosevic/examples/`:

| Ulaz | Izlaz |
|---|---|
| `simple.mavn` | `simple.s`, `simpleLiveness.s` |
| `simple1.mavn` | `simple1.s`, `simpleAnalisys1.s` |
| `simpleError.mavn` | primer detekcije grešaka |
| `multiply.mavn` | `multiply.s`, `multiplyLiveness.s` |

---

## Struktura projekta

```
2. Bojana Milosevic/
├── src/
│   ├── main.cpp
│   ├── LexicalAnalysis.cpp / .h
│   ├── SyntaxAnalysis.cpp / .h
│   ├── LivenessAnalysis.cpp / .h
│   ├── FiniteStateMachine.cpp / .h
│   ├── Token.cpp / .h
│   ├── Ispis.cpp / .h
│   ├── IR.h
│   ├── Types.h
│   ├── Constants.h
│   └── LexicalAnalysis.sln / .vcxproj
├── examples/
│   ├── simple.mavn / simple.s / simpleLiveness.s
│   ├── simple1.mavn / simple1.s / simpleAnalisys1.s
│   ├── simpleError.mavn
│   └── multiply.mavn / multiply.s / multiplyLiveness.s
└── doc/
    ├── dokumentacija.pdf
    └── MIPS-instructions.pdf
```

---

## Tehnologije

- **C++** (Visual Studio 2022, MSVC)
- MIPS 32-bit arhitektura

---

## Autor

Bojana Milošević  
Predmet: Osnovi paralelnog programiranja i softverski alati  
Fakultet tehničkih nauka, Novi Sad
