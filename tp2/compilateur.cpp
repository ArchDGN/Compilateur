//  A compiler from a very simple Pascal-like structured language LL(k)
//  to 64-bit 80x86 Assembly langage
//  Copyright (C) 2019 Pierre Jourlin
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

// Build with "make compilateur"


#include <string>
#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

char current, lookedAhead;                // Current char    
int NLookedAhead=0;

void ReadChar(void){
    if(NLookedAhead>0){
        current=lookedAhead;    // Char has already been read
        NLookedAhead--;
    }
    else
        // Read character and skip spaces until 
        // non space character is read
        while(cin.get(current) && (current==' '||current=='\t'||current=='\n'));
}

void LookAhead(void){
    while(cin.get(lookedAhead) && (lookedAhead==' '||lookedAhead=='\t'||lookedAhead=='\n'));
    NLookedAhead++;
}

void Error(string s){
	cerr<< s << endl;
	exit(-1);
}

void Letter(void)
{
	if (current >= 'a' && current <= 'z') {
		cout << "\tpush " << current << endl;
		ReadChar();
	} else {
		Error("Mauvaise entrez Nop");
	}
}

// SimpleExpression := Term {AdditiveOperator Term}
// Term := Digit | "(" SimpleExpression ")"
// AdditiveOperator := "+" | "-"
// Digit := "0"|"1"|"2"|"3"|"4"|"5"|"6"|"7"|"8"|"9"

void AdditiveOperator(void){
	if(current=='+'||current=='-')
		ReadChar();
	else if (current == '|') {
		LookAhead();
		if (lookedAhead == '|') {
			ReadChar();
			ReadChar();
		} else {
			Error("| mais pas de deuxieme |");
		}
	}
	else
		Error("Opérateur additif attendu");	   // Additive operator expected
}


void Number(void){
	string number = "";

	// Il faut au moins un chiffre valide
	if((current<'0')||(current>'9'))
		Error("Chiffre attendu");		   // Digit expected

	// Ensuite on recupere tout les chiffre suivant pour former le nombre
	while (current >= '0' && current <= '9') {
		number+=current;
		ReadChar();
	}

	cout << "\tpush $"<<number<<endl;
}

void SimpleExpression(void);			// Called by Term() and calls Term()
void Factor();
void MutliplicationEtOperation();
void Expression();

void Term(void)
{
	char temp;
	Factor();

	while (current == '*' || current == '/' || current == '%' || current == '&')
	{
		temp = current;
		MutliplicationEtOperation();
		Factor();

		cout << "\tpop %rbx" << endl;
        cout << "\tpop %rax" << endl;

		if (temp == '*') {
			cout << "\timulq %rbx, %rax" << endl;
		} else if (temp == '/') {
			cout << "\tcqto" << endl;
            cout << "\tidivq %rbx" << endl;
		} else if (temp == '%') {
			cout << "\tcqto" << endl;
            cout << "\tidivq %rbx" << endl;
            cout << "\tmovq %rdx, %rax" << endl;
		} else if (temp == '&') {
			cout << "\tandq %rbx, %rax" << endl;
		}

		cout << "\tpush %rax" << endl;
	}
}

void Factor()
{
	if (current == '(') {
		ReadChar();
		Expression();
		if (current != ')')
			Error("Expression Invalide");
		else
			ReadChar();
	} else if (current == '!') {
		ReadChar();
		Factor();

		// on recupere, on inverse les bits, on remet
		cout << "\tpop %rax" << endl;
        cout << "\tnotq %rax" << endl;
        cout << "\tpush %rax" << endl;
	} else if (current >= 'a' && current <= 'z') {
		Letter();
	} else if (current >= '0' && current <= '9') {
		Number();
	} else {
		Error("Nop invalide");
	}
}


void MutliplicationEtOperation()
{
	if (current == '*' || current == '/' || current == '%')
		ReadChar();
	else if (current == '&') {
		LookAhead();
		if (lookedAhead == '&') {
			ReadChar();
			ReadChar();
		} else
			Error("& devrait etre suivit d'un deuxieme & mais la non donc error");
	} else 
		Error("Operateur attendu");
}


void Expression(void)
{
	SimpleExpression();

	string instruction_set;

	switch (current) {
		case '=':
			LookAhead();
            if (lookedAhead == '=') {
                ReadChar();
            } else {
                Error("'==' attendu");
            }
            ReadChar();
			SimpleExpression();

			cout << "\tpop %rbx"<<endl;	// get first operand
			cout << "\tpop %rax"<<endl;	// get second operand
			cout << "\tcmpq %rbx, %rax"<<endl;	// fait la comparaison
			cout << "\tsete %al" <<endl;	// set if equal sur les 8bits
			cout << "\tmovzbq %al, %rax" <<endl;	// balai sur les 64
			cout << "\tnegq %rax" << endl;
			cout << "\tpush %rax" << endl; // on met en haut de la pile

			break;
		case '<' :

			LookAhead();

			if (lookedAhead == '=') {
				instruction_set = "\tsetle %al"; // (Set if Less or Equal) : Inférieur ou égal (<=)
				ReadChar();
			} else {
				instruction_set = "\tsetl %al"; // (Set if Less) : Strictement inférieur (<)
			}
			ReadChar();
			SimpleExpression();

			cout << "\tpop %rbx" << endl; // get first operand
            cout << "\tpop %rax" << endl; // get second operand
            cout << "\tcmpq %rbx, %rax" << endl; // fait la comparaison
            
            cout << instruction_set << endl;
            
            cout << "\tmovzbq %al, %rax" << endl; // balai sur les 64
			cout << "\tnegq %rax" << endl;
            cout << "\tpush %rax" << endl; // on met en haut de la pile
			break;
		case '>':
			LookAhead();

			if (lookedAhead == '=') {
				instruction_set = "\tsetge %al"; // setge %al (Set if Greater or Equal) : Supérieur ou égal (>=)
				ReadChar();
			} else {
				instruction_set = "\tsetg %al"; // setg %al (Set if Greater) : Strictement supérieur (>)
			}
			ReadChar();
			SimpleExpression();

			cout << "\tpop %rbx" << endl;
            cout << "\tpop %rax" << endl;
            cout << "\tcmpq %rbx, %rax" << endl;
            
            cout << instruction_set << endl;
            
            cout << "\tmovzbq %al, %rax" << endl;
			cout << "\tnegq %rax" << endl;
            cout << "\tpush %rax" << endl;
			break;

		case '!':
            LookAhead();
            if (lookedAhead == '=') {
                ReadChar();
            } else {
                Error("'!=' attendu");
            }
            ReadChar();
            SimpleExpression();

            cout << "\tpop %rbx" << endl;
            cout << "\tpop %rax" << endl;
            cout << "\tcmpq %rbx, %rax" << endl;
            cout << "\tsetne %al" << endl; // Set Not Equal
            cout << "\tmovzbq %al, %rax" << endl;
            cout << "\tnegq %rax" << endl;
            cout << "\tpush %rax" << endl;
            break;
		default:
			return;
	}
}


void SimpleExpression(void){
	char adop;
	Term();
	while(current=='+'||current=='-' || current=='|'){
		adop=current;		// Save operator in local variable
		AdditiveOperator();
		Term();
		cout << "\tpop %rbx"<<endl;	// get first operand
		cout << "\tpop %rax"<<endl;	// get second operand
		if(adop == '+')
			cout << "\taddq	%rbx, %rax"<<endl;	// add both operands
		else if (adop == '-')
			cout << "\tsubq	%rbx, %rax"<<endl;	// substract both operands
		else if (adop == '|')
			cout << "\torq %rbx, %rax" << endl; // OU logique bit-à-bit
		cout << "\tpush %rax"<<endl;			// store result
	}

}

void DeclarerPartie()
{
	if (current == '[') {
		cout << "\t.data" << endl;
		ReadChar();

		while (current != ']') {
			if (current >= 'a' && current <= 'z') {
				cout << current << ":\t.quad 0" << endl;
				ReadChar();

				if (current == ',')
					ReadChar();
			} else {
				Error("Les var global doivent etre des lettres entre 'a' et 'z'");
			}
		}

		ReadChar();
	}
}


void Assignement()
{
    if (current >= 'a' && current <= 'z') {
        char temp = current;
        ReadChar();

        if (current == '=') {
            ReadChar();
            Expression();
            cout << "\tpop " << temp << endl;
        } else {
            Error("assigment incorrect");
        }
    } else {
        Error("Une variable était attendue à gauche du '='");
    }
}


void AssignementAvance()
{
	Assignement();

	while(current == ';') {
		ReadChar();
		Assignement();
	}

	if (current != '.')
		Error("Pas de . final !!!");
}

// ./compilateur < test.p > test.s && gcc -ggdb -no-pie -fno-pie test.s -o test
// ./test && echo $?

int main(void){	// First version : Source code on standard input and assembly code on standard output
	// Header for gcc assembler / linker
	cout << "\t\t\t# This code was produced by the CERI Compiler"<<endl;

	// Let's proceed to the analysis and code production
	ReadChar();

    if (current == '[')
		DeclarerPartie();

	cout << "\t.text" << endl;
    cout << "\t.globl main" << endl;
    cout << "main:" << endl;
    cout << "\tmovq %rsp, %rbp" << endl;

	AssignementAvance();
    
	// fin
    cout << "\tmovq %rbp, %rsp" << endl;
    cout << "\tret" << endl;
    return 0;

}