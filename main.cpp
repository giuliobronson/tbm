#include <algorithm>
#include <vector>
#include <cstring>
#include <iostream>
#include <fstream>
#include <cctype>
#include <unordered_map>

#define MAX_CONSTS 1000
#define MAX_ID_LEN 50
#define MAX_STR_LEN 250
#define MAX_NUM_LEN 10

using namespace std;

// this is just temporary to test :)
vector<string> inv_enum = {
    "ALIAS", "BOOL", "BREAK", "CHAR", "CONTINUE", "DO", "ELSE", "FALSE", "FUNCTION", "IF", "INT", "STRING", "STRUCT", "TRUE", "VAR", "WHILE",
    "COLON", "SEMI_COLON", "COMMA", "EQUALS", "LEFT_SQUARE", "RIGHT_SQUARE", "LEFT_BRACES", "RIGHT_BRACES", "LEFT_PARENTHESIS", 
    "RIGHT_PARENTHESIS", "AND", "AND_EQUAL", "OR", "OR_EQUAL", "LESS_THAN", "GREATER_THAN", "LESS_OR_EQUAL", "GREATER_OR_EQUAL", "NOT_EQUAL", "EQUAL_EQUAL",
    "PLUS", "PLUS_PLUS", "PLUS_EQUAL", "MINUS", "MINUS_MINUS", "MINUS_EQUAL", "TIMES", "TIMES_EQUAL", "DIVIDE", "DIVIDE_EQUAL", "DOT", 
    "NOT", "XOR", "XOR_EQUAL", "MOD", "MOD_EQUAL", "BIT_AND", "BIT_OR", "BIT_SHIFT_LEFT", "BIT_SHIFT_RIGHT",
    "CHARACTER", "NUMERAL", "STRINGVAL", "ID", "UNKNOWN"
};

typedef enum {
    // Palavras Reservadas
    ALIAS, BOOL, BREAK, CHAR, CONTINUE, DO, ELSE, FALSE, FUNCTION, IF, INT, STRING, STRUCT, TRUE, VAR, WHILE,

    // Símbolos
    COLON, SEMI_COLON, COMMA, EQUALS, LEFT_SQUARE, RIGHT_SQUARE, LEFT_BRACES, RIGHT_BRACES, LEFT_PARENTHESIS, 
    RIGHT_PARENTHESIS, AND, AND_EQUAL, OR, OR_EQUAL, LESS_THAN, GREATER_THAN, LESS_OR_EQUAL, GREATER_OR_EQUAL, NOT_EQUAL, EQUAL_EQUAL,
    PLUS, PLUS_EQUAL, MINUS, MINUS_EQUAL, TIMES, TIMES_EQUAL, DIVIDE, DIVIDE_EQUAL, DOT, 
    NOT, XOR, XOR_EQUAL, MOD, MOD_EQUAL, BIT_AND, BIT_OR, BIT_SHIFT_LEFT, BIT_SHIFT_RIGHT,

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
ofstream outputFile;

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

bool isRegularToken(t_token token) {
    return token >= CHARACTER && token <= ID;
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

void openOutputFile(const string& filename) {
    outputFile.open(filename);
    if(!outputFile) {
        cerr << "Erro ao abrir o arquivo: " << filename << endl;
        exit(1); // Encerra o programa em caso de erro na abertura do arquivo
    }
}

void closeOutputFile() {
    if(outputFile.is_open()) {
        outputFile.close();
    }
}

void writeToken(const t_token token) {
    if(outputFile.is_open()) {
        outputFile << inv_enum[token] << ", " << (isRegularToken(token) ? to_string(token2nd) : "") << ";" << endl;
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

int t = 0;
// Função que implementa o autômato finito do analisador léxico
t_token nextToken(void) {
    while(isspace(nextChar)) {
        nextChar = readChar();
    }
    t++;
    if(t > 50) {
        exit(1);
    }
    cout << nextChar;
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
    } else if( isdigit( nextChar ) ) {
        char numeral[MAX_NUM_LEN + 1];
        int i = 0;
        do {
            numeral[i++] = nextChar;
            nextChar = readChar();
            while( nextChar == '_' || nextChar == '\'' ) { // because we are 8_000 cool
                nextChar = readChar();
                continue;
            }
        } while( isdigit( nextChar ) );
        numeral[i] = '\0';
        token = NUMERAL;
        int n = 0, j = 0;
        while( numeral[j++] != '\0' )
            n = n * 10 + numeral[j] - '0';
        token2nd = addIntConst( n );
    } else if( nextChar == '"' ) {
        char string[ MAX_STR_LEN + 1 ];
        int i = 1;
        do {
            string[ i++ ] = nextChar;
            nextChar = readChar();
        } while( nextChar != '"' );
        string[ i++ ] = '"';
        string[ i ] = '\0';
        token = STRINGVAL;
        token2nd = addStringConst( string );
    } else switch( nextChar ) {
        case ':':
            token = COLON;
            nextChar = readChar();
            break;
        case ';':
            token = SEMI_COLON;
            nextChar = readChar();
            break;
        case ',':
            token = COMMA;
            nextChar = readChar();
            break;
        case '=':
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = EQUAL_EQUAL;
                nextChar = readChar();
            } else {
                token = EQUALS;
            }
            break;
        case '[':
            token = LEFT_SQUARE;
            nextChar = readChar();
            break;
        case ']':
            token = RIGHT_SQUARE;
            nextChar = readChar();
            break;
        case '{':
            token = LEFT_BRACES;
            nextChar = readChar();
            break;
        case '}':
            token = RIGHT_BRACES;
            nextChar = readChar();
            break;
        case '(':
            token = LEFT_PARENTHESIS;
            nextChar = readChar();
            break;
        case ')':
            token = RIGHT_PARENTHESIS;
            nextChar = readChar();
            break;
        case '&':
            token = BIT_AND;
            nextChar = readChar();
            if( nextChar == '&' ) {
                token = AND;
                nextChar = readChar();
            } else if( nextChar == '=' ) {
                token = AND_EQUAL;
                nextChar = readChar();
            }
            break;
        case '|':
            token = BIT_OR;
            nextChar = readChar();
            if( nextChar == '|' ) {
                token = OR;
                nextChar = readChar();
            } else if( nextChar == '=' ) {
                token = OR_EQUAL;
                nextChar = readChar();
            }
            break;
        case '<':
            token = LESS_THAN;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = LESS_OR_EQUAL;
                nextChar = readChar();
            } else if( nextChar == '<' ) {
                token = BIT_SHIFT_LEFT;
                nextChar = readChar();
            }
            break;
        case '>': 
            token = GREATER_THAN;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = GREATER_OR_EQUAL;
                nextChar = readChar();
            } else if( nextChar == '>' ) {
                token = BIT_SHIFT_RIGHT;
                nextChar = readChar();
            }
            break;
        case '!':
            token = NOT;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = NOT_EQUAL;
                nextChar = readChar();
            }
            break;
        case '+':
            token = PLUS;
            nextChar = readChar();
            if( nextChar == '=') {
                token = PLUS_EQUAL;
                nextChar = readChar();
            }
            break;
        case '-': 
            token = MINUS;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = MINUS_EQUAL;
                nextChar = readChar();
            }
            break;
        case '*':
            token = TIMES;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = TIMES_EQUAL;
                nextChar = readChar();
            }
            break;
        case '/':   
            token = DIVIDE;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = DIVIDE_EQUAL;
                nextChar = readChar();
            }
            break;
        case '.':
            token = DOT;
            nextChar = readChar();
            break;
        case '\'':
            nextChar = readChar();
            token = CHARACTER;
            token2nd = addCharConst(nextChar);
            readChar();
            nextChar = readChar();
            break;
        case '^':
            token = XOR;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = XOR_EQUAL;
                nextChar = readChar();
            }
            break;
        case '%':
            token = MOD;
            nextChar = readChar();
            if( nextChar == '=' ) {
                token = MOD_EQUAL;
                nextChar = readChar();
            }
            break;
        default:
            token = UNKNOWN;
            nextChar = readChar();
            break;
    } 
    return token;
}

int main() {
    openLexerFile("main.tbm");
    openOutputFile("output.txt");
    while(nextChar != EOF) {
        t_token token = nextToken();
        writeToken(token);
    }
    closeLexerFile();
    closeOutputFile();
    return 0;
}