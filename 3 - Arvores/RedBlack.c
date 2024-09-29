#include <stdio.h>
#include <stdlib.h>

#define VERMELHO 0  // Constante para a cor vermelha
#define PRETO 1     // Constante para a cor preta

// Estrutura de um nó da árvore Red-Black
struct No {
    int valor;              // Valor armazenado no nó
    int cor;                // Cor do nó: VERMELHO ou PRETO
    struct No *esquerda;    // Ponteiro para o nó filho à esquerda
    struct No *direita;     // Ponteiro para o nó filho à direita
    struct No *pai;         // Ponteiro para o nó pai
};

typedef struct No No;

/**
 * Função para criar um novo nó.
 * O novo nó é inicialmente vermelho e não possui filhos nem pai.
 * 
 * @param valor Valor a ser armazenado no nó
 * @return Ponteiro para o novo nó criado
 */
No *criarNo(int valor) {
    No *novoNo = (No *)malloc(sizeof(No));
    novoNo->valor = valor;
    novoNo->cor = VERMELHO;
    novoNo->esquerda = novoNo->direita = novoNo->pai = NULL;
    return novoNo;
}

/**
 * Função para realizar rotação à esquerda.
 * Isso é necessário para reestruturar a árvore Red-Black e manter suas propriedades.
 * 
 * @param raiz Ponteiro para a raiz da árvore
 * @param x Nó em que a rotação será realizada
 */
void rotacaoEsquerda(No **raiz, No *x) {
    No *y = x->direita;
    x->direita = y->esquerda;
    if (y->esquerda != NULL)
        y->esquerda->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->esquerda)
        x->pai->esquerda = y;
    else
        x->pai->direita = y;
    y->esquerda = x;
    x->pai = y;
}

/**
 * Função para realizar rotação à direita.
 * Isso é necessário para reestruturar a árvore Red-Black e manter suas propriedades.
 * 
 * @param raiz Ponteiro para a raiz da árvore
 * @param x Nó em que a rotação será realizada
 */
void rotacaoDireita(No **raiz, No *x) {
    No *y = x->esquerda;
    x->esquerda = y->direita;
    if (y->direita != NULL)
        y->direita->pai = x;
    y->pai = x->pai;
    if (x->pai == NULL)
        *raiz = y;
    else if (x == x->pai->direita)
        x->pai->direita = y;
    else
        x->pai->esquerda = y;
    y->direita = x;
    x->pai = y;
}

/**
 * Função para corrigir as violações das propriedades da árvore Red-Black após uma inserção.
 * A função mantém o equilíbrio da árvore, garantindo que todas as propriedades da Red-Black sejam preservadas.
 * 
 * @param raiz Ponteiro para a raiz da árvore
 * @param z Nó recém-inserido que pode causar uma violação
 */
void corrigirViolacao(No **raiz, No *z) {
    while (z != *raiz && z->pai->cor == VERMELHO) {
        No *y;
        if (z->pai == z->pai->pai->esquerda) {
            y = z->pai->pai->direita;
        } else {
            y = z->pai->pai->esquerda;
        }

        // Caso 1: Tio é vermelho, recolorir e continuar subindo na árvore
        if (y != NULL && y->cor == VERMELHO) {
            z->pai->cor = PRETO;
            y->cor = PRETO;
            z->pai->pai->cor = VERMELHO;
            z = z->pai->pai;
        } else {
            // Caso 2 e 3: Reestruturar a árvore com rotações e recoloração
            if (z->pai == z->pai->pai->esquerda) {
                if (z == z->pai->direita) {
                    z = z->pai;
                    rotacaoEsquerda(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoDireita(raiz, z->pai->pai);
            } else {
                if (z == z->pai->esquerda) {
                    z = z->pai;
                    rotacaoDireita(raiz, z);
                }
                z->pai->cor = PRETO;
                z->pai->pai->cor = VERMELHO;
                rotacaoEsquerda(raiz, z->pai->pai);
            }
        }
    }
    (*raiz)->cor = PRETO;
}

/**
 * Função para inserir um novo nó na árvore Red-Black.
 * O nó é inserido de maneira semelhante à inserção em uma árvore binária de busca,
 * mas com correção de violações das propriedades Red-Black.
 * 
 * @param raiz Ponteiro para a raiz da árvore
 * @param valor Valor a ser inserido na árvore
 */
void inserir(No **raiz, int valor) {
    No *z = criarNo(valor);
    No *y = NULL;
    No *x = *raiz;

    // Busca a posição correta para o novo nó
    while (x != NULL) {
        y = x;
        if (z->valor < x->valor)
            x = x->esquerda;
        else
            x = x->direita;
    }

    // Define o pai do novo nó
    z->pai = y;
    if (y == NULL)
        *raiz = z;
    else if (z->valor < y->valor)
        y->esquerda = z;
    else
        y->direita = z;

    // Corrige as possíveis violações das propriedades Red-Black
    corrigirViolacao(raiz, z);
}

/**
 * Função para imprimir a árvore Red-Black em ordem (esquerda-raiz-direita).
 * 
 * @param raiz Ponteiro para o nó raiz da árvore
 */
void emOrdem(No *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);
        printf("%d %s\n", raiz->valor, raiz->cor == VERMELHO ? "RED" : "BLK");
        emOrdem(raiz->direita);
    }
}

/**
 * Função para imprimir a árvore Red-Black no formato esquerda-raiz-direita,
 * com uma indentação proporcional à profundidade do nó. Exibe a cor do nó.
 * 
 * @param raiz Ponteiro para o nó raiz da árvore
 * @param b Nível de profundidade atual para definir a indentação
 */
void imprimeArvoreRB(No *raiz, int b) {
    if (raiz != NULL) {
        imprimeArvoreRB(raiz->direita, b + 1);

        // Imprime o valor do nó com indentação adequada ao nível
        for (int i = 0; i < b; i++)
            printf("       ");
        if (raiz->cor == VERMELHO)
            printf("\033[31m%d\033[0m\n\n", raiz->valor);  // Vermelho
        else
            printf("%d\n\n", raiz->valor);  // Preto

        imprimeArvoreRB(raiz->esquerda, b + 1);
    }
}

/**
 * Função principal para inserir valores de exemplo na árvore Red-Black e exibir a árvore resultante.
 */
int main() {
    No *raiz = NULL;
    int vetor[] = {12, 31, 20, 17, 11, 8, 3, 24, 15, 33};
    int tam = sizeof(vetor) / sizeof(vetor[0]);

    // Insere os valores no vetor na árvore Red-Black
    for (int i = 0; i < tam; i++)
        inserir(&raiz, vetor[i]);

    // Imprime a árvore Red-Black
    printf("Árvore Red-Black: \n");
    imprimeArvoreRB(raiz, 3);

    return 0;
}
