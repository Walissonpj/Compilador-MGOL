#include <bits/stdc++.h>
using namespace std;

struct no
{
	string lexema;
	string token;
	string tipo;
	no(string lexema="", string token="", string tipo="-"): lexema(lexema), token(token), tipo(tipo){}
};


class Lexico{
	private:
		map<int, map<char, int> > AFD;
		map<int, string> finalStates;
		map<string, no> tableOfSymbol;
		int line;
		int column;
		int pos;
		string text;
		string fname;
		ifstream inFile;
		
	public:
		
		void ReadAllText(){
			text = "";
			inFile.open(fname, ios::in);
			string aux;
			while( getline(inFile, aux)){
				text = text + aux + '\n';
			}
			text += '$';
		}
		void constructAFD(){
			// todos as transicoes a partir do estado 1
			AFD[1][' '] = 1;
			AFD[1]['\t'] = 1;
			AFD[1]['\n'] = 1;
			for( int i = '0'; i <= '9'; i++)
				AFD[1][i] = 2;
			
			AFD[1]['"'] = 8;
			for( char i = 'a'; i <= 'z'; i++){
				AFD[1][i] = 10;
				AFD[1][toupper(i)] = 10;
			}
			AFD[1]['$'] = 11;
			AFD[1]['{'] = 12;
			AFD[1]['<'] = 14;
			AFD[1]['='] = 16;
			AFD[1]['>'] = 17;
			AFD[1]['/'] = 18;
			AFD[1]['+'] = 18;
			AFD[1]['-'] = 18;
			AFD[1]['*'] = 18;
			AFD[1]['('] = 19;
			AFD[1][')'] = 20;
			AFD[1][';'] = 21;

			// transicoes a partir do estado 2

			for( char i = '0'; i <= '9'; i++)
				AFD[2][i] = 2;
			AFD[2]['.'] = 3;
			AFD[2]['E'] = 5;

			// transicoes a partir do estado 3
			for( char i = '0'; i <= '9'; i++)
				AFD[3][i] = 4;
			
			// transicoes a partir do estado 4

			for( char i = '0'; i <= '9'; i++)
				AFD[4][i] = 4;

			// transicoes a partir do estado 5

			for( char i = '0'; i <= '9'; i++)
				AFD[5][i] = 7;
			AFD[5]['+'] = 6;
			AFD[5]['-'] = 6;

			// transicoes a partir do estado 6

			for( char i = '0'; i <= '9'; i++)
				AFD[6][i] = 7;
			
			// transicoes a partir do estado 7

			for( char i = '0'; i <= '9'; i++)
				AFD[7][i] = 7;
			
			// transicoes a partir do estado 8
			for( char i = 32; i < 127; i++){
				if( i != '"')
					AFD[8][i] = 8;
			}
			AFD[8]['"'] = 9;

			// transicoes a partir do estado 9

			// transicoes a partir do estado 10

			for( char i = '0'; i <= '9'; i++)
				AFD[10][i] = 10;
			for (char i = 'a'; i <= 'z'; ++i)
			{
				AFD[10][i] = 10;
				AFD[10][toupper(i)] = 10;
			}
			
			AFD[10]['_'] = 10;

			// transicao a partir do estado 11

			// transicao a partir do estado 12
			for( char i = 32; i < 127; i++)
				if( i != '}')
					AFD[12][i] = 12;

			AFD[12]['}'] = 13;

			// transicoes a partir do estado 13

			// transicoes a partir dp estado 14
			AFD[14]['-'] = 15;
			AFD[14]['>'] = 16;
			AFD[14]['='] = 16;

			// transicoes a partir do estado 15

			// transicoes a partir do estado 16

			// transicoes a partir do estado 17

			AFD[17]['='] = 16;

			// transicoes a partir do estado 18
			// transicoes a partir do estado 19
			// transicoes a partir do estado 20
			// transicoes a partir do estado 21


			// estados finais

			finalStates[2] = "Num";
			finalStates[4] = "Num";
			finalStates[7] = "Num";
			finalStates[9] = "Literal";
			finalStates[10] = "Id";
			finalStates[11] = "EOF";
			finalStates[13] = "Comentario";
			finalStates[14] = "OPR";
			finalStates[15] = "RCB";
			finalStates[16] = "OPR";
			finalStates[17] = "OPR";
			finalStates[18] = "OPM";
			finalStates[19] = "AB_P";
			finalStates[20] = "FC_P";
			finalStates[21] = "PT_V";

			// palavras chave

			tableOfSymbol["inicio"] = no("inicio", "inicio");
			tableOfSymbol["varinicio"] = no("varinicio", "varinicio");
			tableOfSymbol["varfim"] = no("varfim", "varfim");
			tableOfSymbol["escreva"] = no("escreva", "escreva");
			tableOfSymbol["leia"] = no("leia", "leia");
			tableOfSymbol["se"] = no("se", "se");
			tableOfSymbol["entao"] = no("entao", "entao");
			tableOfSymbol["fimse"] = no("fimse", "fimse");
			tableOfSymbol["fim"] = no("fim", "fim");
			tableOfSymbol["inteiro"] = no("inteiro", "inteiro");
			tableOfSymbol["lit"] = no("lit", "lit");
			tableOfSymbol["real"] = no("real", "real");

		}

		Lexico(string fname, map<string, no>& tableOfSymbol){
			this->line = 1;
			this->column = 1;
			this->fname = fname;
			this->pos = 0;
			this->tableOfSymbol = tableOfSymbol;
			ReadAllText();
			constructAFD();
		}

		no nextToken(){
			string lexema = "";
			int u=1;
			while(pos < (int)text.size()){
				if( AFD[u].count(text[pos])){
					if( text[pos] != ' ' && text[pos] != '\t' && text[pos] != '\n')
						lexema += text[pos];
					if( text[pos] == '\n'){
						line++;
						column = 1;
					}
					u = AFD[u][text[pos]];
					pos++;
					column++;
				}else
					break;
			}
			if( finalStates.count(u)){
				no newtoken(lexema, finalStates[u]);
				if( newtoken.token == "Id"){
					if( !tableOfSymbol.count(lexema))
						tableOfSymbol[lexema] = newtoken;
				}
				return newtoken;
			}else{
				if( lexema == ""){
					pos++;
					column++;
				}
				no newtoken( "", "ERRO");
				return newtoken;
			}
		}
		int getColumn(){
			return column;
		}
		int getLine(){
			return line;
		}
};