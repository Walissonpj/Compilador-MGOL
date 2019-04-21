#include "lexico.cpp"

int main(int argc, char const *argv[])
{
	map<string, no> tableOfSymbol;
	Lexico lexicalAnalyzer(argv[1], tableOfSymbol);
	int i = 0;
	no token = lexicalAnalyzer.nextToken();
	while(token.token != "EOF"){
		
		if(token.token == "ERRO"){
			cout << "Erro line: " << lexicalAnalyzer.getLine() << " column: "<< lexicalAnalyzer.getColumn() << endl;
		}
		token = lexicalAnalyzer.nextToken();
	}
	return 0;
}