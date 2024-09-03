
Um programa é formado por uma Lista de Declarações Externas (**LDE**):

  

**P** -> **LDE**

**LDE** -> **LDE**  **DE** | **DE**

  

Uma Declaração Externa (**DE**) é uma Declaração de Função (**DF**) ou uma Declaração de Tipo (**DT**):

  

**DE** -> **DF** | **DT**

  

Um tipo pode ser a palavra 'int', 'char', 'bool', 'string' ou um **ID** representando o tipo previamente declarado:

  

**T** -> 'int' | 'char' | 'bool' | 'string' | **ID**

  

Uma Declaração de Tipo (**DT**) pode ser uma declaração de um tipo vetor, um tipo estrutura ou um tipo sinônimo:

  

**DT** -> **T**'\[' **NUM** '\]' **ID**
| 'struct' **ID** '{' **DC** '}'
| 'alias' **T** **ID**

**DC** -> **DC** ';' **T**  **LI**
| **T** **LI**

  

Uma Declaração de Função (**DF**) é formada pelo Tipo (**T**) de retorno, seguido pela palavra ‘function’, seguida pelo nome da função (**ID**), seguida pela Lista de Parâmetros (**LP**) entre parênteses, seguida pelo Bloco (**B**):

  

**DF** -> **T** 'function' **ID** '(' **LP** ')' **B**

**LP** -> **LP** ',' **T**  **ID**
| **T** **ID**

  

Um Bloco (**B**) é delimitado por chave e contém uma Lista de Declaração de Variáveis (**LDV**) seguida por uma Lista de Statements (**LS**) ou Comandos:

  

**B** -> '{' **LDV**  **LS** '}'

**LDV** -> **LDV**  **DV** | **DV**

**LS** -> **LS**  **S** | **S**

  

Uma Declaração de Variáveis (**DV**) é formada pela palavra ‘var’, seguida pelo Tipo (**T**), seguido por uma Lista de Identificadores (**LI**), separados por ‘,’ e com um ‘;’ ao final.

  

**DV** -> 'var' **T**  **LI** ';'

**LI** -> **LI** ',' **ID** | **ID**

  

Um Statement (**S**) pode ser um comando de seleção, repetição, um bloco, uma atribuição ou um comando de controle de fluxo de repetição (‘break’ ou ‘continue’):

  

**S** -> 'if' '(' **E** ')' **S**
| 'if' '(' **E** ')' **S** 'else' **S**
| 'while' '(' **E** ')' **S**
| 'do' S while '(' **E** ')' ';'
| **B**
| **LV** '=' **E** ';'
| **LV** '+=' **E** ';'
| **LV** '-=' **E** ';'
| **LV** '*=' **E** ';'
| **LV** '/=' **E** ';'
| **LV** '%=' **E** ';'
| **LV** '<<=' **E** ';'
| **LV** '>>=' **E** ';'
| **LV** '|=' **E** ';'
| **LV** '&=' **E** ';'
| **LV** '^=' **E** ';'

| 'break' ';'

| 'continue' ';'

  

Uma Expressão (**E**) pode ser composta por operadores lógicos, relacionais ou aritméticos, além de permitir o acesso aos componentes dos tipos agregados e da atribuição de valores:


**E** -> **E** '&&' **M**
| **E** '||' **M**
| **M**

**M** -> **M** '%' **L**
| **L**

**L** -> **L** ‘<’ **R**
| **L** '>'**R**
| **L** '<=' **R**
| **L** '>=' **R**
| **L** '\==' **R**
| **L** '!=' **R**
| **R**

**R** -> **R** '+' **Y**
| **R** '-' **Y**
| **Y**


**Y** -> **Y** '\*' **Z**
 	| **Y** '/' **Z**
	| **Z**

**Z** -> **Z** '&' **F**
| **Z** '|' **F**
| **Z** '^' **F**
| **Z** '<<' **F**
| **Z** '>>' **F**
| **F**

**F** -> **LV**
| '(' **E** ')'
| **ID** '(' **LE** ')'
| '-' **F**
| '!' **F**
| **TRUE**
| **FALSE**
| **CHR**
| **STR**
| **NUM**

**LE** -> **LE** ',' **E**
| **E**

**LV** -> **LV** '.' **ID**
| **LV** '\[' **E** '\]'
| **ID**

  

**ID** -> Id

  **TRUE** -> 'true'

**FALSE** -> 'false'

**CHR** -> c

**STR** -> s

**NUM** -> n