#include<algorithm>
#include<vector>
#include<cstring>
#include<iostream>
#include<fstream>
#include<cctype>
#include<unordered_map>

#define MAX_CONSTS 1000
#define MAX_ID_LEN 50

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

typedef struct {
    unsigned char type; // 0-char, 1-int, 2-string
    union {
        char cVal;
        int nVal;
        char *sVal;
    } _;
} t_const;

// Vetor de constantes lidas
t_const vConsts[MAX_CONSTS];
int nNumConsts = 0;

ifstream inputFile;

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
    if(it != identifierTable.end()) {
        // Retorna o token secundário já existente
        return it->second;
    } else {
        // Adiciona um novo identificador na tabela com o próximo token secundário
        identifierTable[name] = currentIndex;
        return currentIndex++;
    }
}

// Funções para inclusões de constantes de cada tipo. Retornam a posição em que foram inseridas
int addCharConst(char c) {
    if(nNumConsts >= MAX_CONSTS) {
        cerr << "Erro: Excedido o limite máximo de constantes." << endl;
        return -1; // Retorna -1 em caso de erro
    }
    vConsts[nNumConsts].type = 0; // 0 para char
    vConsts[nNumConsts]._.cVal = c;
    return nNumConsts++;
}

int addIntConst(int n) {
    if(nNumConsts >= MAX_CONSTS) {
        cerr << "Erro: Excedido o limite máximo de constantes." << endl;
        return -1; // Retorna -1 em caso de erro
    }
    vConsts[nNumConsts].type = 1; // 1 para int
    vConsts[nNumConsts]._.nVal = n;
    return nNumConsts++;
}

int addStringConst(char *s) {
    if(nNumConsts >= MAX_CONSTS) {
        cerr << "Erro: Excedido o limite máximo de constantes." << endl;
        return -1; // Retorna -1 em caso de erro
    }
    vConsts[nNumConsts].type = 2; // 2 para string
    strcpy(vConsts[nNumConsts]._.sVal, s);
    return nNumConsts++;
}

// Funções para a recuperação do valor de uma constante a partir de sua posição
char getCharConst(int n) {
    if(n > nNumConsts) {
        cerr << "Erro: Excedido o número de constantes." << endl;
        return -1; // Retorna -1 em caso de erro
    }
    return vConsts[n]._.cVal;
}

int getIntConst(int n) {
    if(n > nNumConsts) {
        cerr << "Erro: Excedido o número de constantes." << endl;
        return -1; // Retorna -1 em caso de erro
    }
    return vConsts[n]._.nVal;
}

string getStringConst(int n) {
    if(n > nNumConsts) {
        cerr << "Erro: Excedido o número de constantes." << endl;
        return ""; // Retorna -1 em caso de erro
    }
    return vConsts[n]._.sVal;
}

// Função para inicializar a leitura do arquivo
void openLexerFile(const string& filename) {
    inputFile.open(filename);
    if(!inputFile) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        exit(1); // Encerra o programa em caso de erro na abertura do arquivo
    }
}

// Função para fechar o arquivo
void closeLexerFile() {
    if(inputFile.is_open()) {
        inputFile.close();
    }
}

// Função para a leitura do arquivo de entrada
char readChar() {
    char c;
    if(inputFile.get(c)) { // Lê um caractere do arquivo
        return c;
    } else {
        return EOF; // Retorna EOF se atingir o fim do arquivo
    }
}

// Função que implementa o autômato finito do analisador léxico
t_token nextToken(void) {
    while(isspace(nextChar)) {
        nextChar = readChar();
    }
    if(isalpha(nextChar)) {
        char text[MAX_ID_LEN + 1];
        int i = 0;
        do {
            text[i++] = nextChar;
            nextChar = readChar();
        } while(isalnum(nextChar) || nextChar == '_');
        text[i] = '\0';
        token = searchKeyWord(text);
        if(token == ID) {
            token2nd = searchName(text);
        }
    }
    return token;
}

int main() {
    openLexerFile("main.tbm");
    while(nextChar) {
        nextToken();
    }
    closeLexerFile();

    return 0;
}