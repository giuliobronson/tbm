#include<algorithm>
#include<vector>
#include<string>
#include<iostream>
#include<unordered_map>

#define MAX_CONSTS 1000

using namespace std;

typedef enum {
    // Palavras Reservadas
    ALIAS, BOOL, BREAK, CHAR, CONTINUE, DO, ELSE, FALSE, FUNCTION, IF, INT, STRING, STRUCT, TRUE, VAR, WHILE,

    // Símbolos
    COLON, SEMI_COLON, COMMA, EQUALS, LEFT_SQUARE, RIGHT_SQUARE, LEFT_BRACES, RIGHT_BRACES, LEFT_PARENTHESIS, 
    RIGHT_PARENTHESIS, AND, OR, LESS_THAN, GREATER_THAN, LESS_OR_EQUAL, GREATER_OR_EQUAL, NOT_EQUAL, EQUAL_EQUAL,
    PLUS, MINUS, TIMES, DIVIDE, DOT, NOT,

    // Tokens Regulares
    CHARACTER, NUMERAL, STRINGVAL, ID,

    // Token Desconhecido
    UNKNOWN
} t_token;

// typedef struct {
//     unsigned char type; // 0-char, 1-int, 2-string
//     union {
//         char cVal;
//         int  nVal;
//         string sVal;
//     };
// } t_const;

// // Vetor de constantes lidas
// t_const vConsts[MAX_CONSTS];
// int nNumConsts = 0;

// Caractere lido
char nextChar = '\x20';

// Tokens principal e secundário
t_token token;
int token2nd;

// Função para verificar tabela de palavras-chave
t_token searchKeyWord(string name) {
    static vector<string> keyWordTable = {
        "alias", "bool", "break", "char", "continue", "do", "else", "false", 
        "function", "if", "int", "string", "struct", "true", "var", "while"
    };

    // Verifica se a palavra existe na tabela de palavras-chave
    auto it = lower_bound(keyWordTable.begin(), keyWordTable.end(), name);
    if(it != keyWordTable.end() && *it == name) {
        return (t_token)(it - keyWordTable.begin());
    } else {
        return ID;
    }
}

// Função para inserir e buscar identificadores na tabela de nomes
int searchName(string name) {
    static unordered_map<string, int> identifierTable;
    static int currentIndex = 0;

    // Verifica se o identificador já está na tabela
    auto it = identifierTable.find(name);
    if (it != identifierTable.end()) {
        // Retorna o token secundário já existente
        return it->second;
    } else {
        // Adiciona um novo identificador na tabela com o próximo token secundário
        identifierTable[name] = currentIndex;
        return currentIndex++;
    }
}

// Funções para inclusões de constantes de cada tipo. Retornam a posição em que foram inseridas
int addCharConst(char c);

int addIntConst(int n);

int addStringConst(string s);

// Funções para a recuperação do valor de uma constante a partir de sua posição
char getCharConst(int n);

int getIntConst(int n);

string getStringConst(int n);

// Função para a leitura do arquivo de entrada
char readChar(void);

// Função que implementa o autômato finito do analisador léxico
t_token nextToken(void);

int main() {
    cout << searchName("test1") << endl;
    cout << searchName("test2") << endl;
    cout << searchName("test1") << endl;
    return 0;
}