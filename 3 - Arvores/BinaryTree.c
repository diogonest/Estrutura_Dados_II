// DIOGO OLIVEIRA SANTOS - 202311226

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct typeno { // Definição da Struct do nó
    int chave;
    struct typeno *esq; // Ponteiro que aponta para o nó à esquerda
    struct typeno *dir; // Ponteiro que aponta para o nó à direita 
    struct typeno *pai; // É utilizado no momento de exclusão, para facilitar o percurso pelos nós
};


struct typeno *build(int valor) { // Função para criar um novo nó
    struct typeno *novo = (struct typeno *)malloc(sizeof(struct typeno));
    if (novo == NULL) {
        printf("Erro: Falha ao alocar memória para o novo nó.\n");
        exit(-1);
    }
    novo->chave = valor;
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}


void add(struct typeno **arvore, int valor) { // Função para adicionar um valor à árvore
    if (*arvore == NULL) {
        *arvore = build(valor);
    } else {
        if (valor < (*arvore)->chave) {
            add(&((*arvore)->esq), valor); // Se o valor a ser inserido for menor que o atual, é inserido à esquerda
        } else {
            add(&((*arvore)->dir), valor); // Se o valor a ser inserido for maior que o atual, é inserido à direita
        }
    }
}

struct typeno *find(struct typeno *arvore, int valor){ // Função para encontrar um nó com base em sua chave

    if(arvore == NULL){
        printf("Empty\n");
        return NULL;
    }

    if(valor > arvore->chave){
        return find(arvore->dir, valor);
    }

    else if(valor < arvore->chave){
        return find(arvore->esq, valor);
    }

    else{
        return arvore;
    }

}


void remover(struct typeno **raiz, int chave) {// Função para remover um nó
    struct typeno *no = find(*raiz, chave);
    if (no == NULL) return;

    if (no->esq == NULL && no->dir == NULL) { // Caso 1: Nó é uma folha (não tem filhos)
        if (no->pai != NULL) {
            if (no->pai->esq == no) {
                no->pai->esq = NULL;
            } else {
                no->pai->dir = NULL;
            }
        } else {
            *raiz = NULL; // Se for o único nó na árvore
        }
        free(no);
    }
   
    else if (no->esq == NULL && no->dir != NULL) {  // Caso 2: Nó tem apenas um filho à direita
        if (no->pai != NULL) {
            if (no->pai->esq == no) {
                no->pai->esq = no->dir;
            } else {
                no->pai->dir = no->dir;
            }
        } else {
            *raiz = no->dir; // Se o nó for a raiz
        }
        no->dir->pai = no->pai;
        free(no);
    }
    
    else if (no->esq != NULL && no->dir == NULL) { // Caso 3: Nó tem apenas um filho à esquerda
        if (no->pai != NULL) {
            if (no->pai->esq == no) {
                no->pai->esq = no->esq;
            } else {
                no->pai->dir = no->esq;
            }
        } else {
            *raiz = no->esq; // Se o nó for a raiz
        }
        no->esq->pai = no->pai;
        free(no);
    }
    
    else { // Caso 4: Nó tem dois filhos
        struct typeno *sucessor = no->dir; // Encontrar o sucessor (o menor nó na subárvore direita)
        while (sucessor->esq != NULL) {
            sucessor = sucessor->esq;
        }
        no->chave = sucessor->chave;
        remover(&no->dir, sucessor->chave); // Remover o sucessor
    }
}



void imprimeNo(int a, int b){ // (REFERÊNCIA DA WEB) Função para imprimir um nó precedido por espaços vazios

    for (int i = 0; i < b; i++)
        printf("   ");
    printf("%i\n", a);
}


void imprimeArvore(struct typeno *arvore, int b){ // (REFERÊNCIA DA WEB) Função para exibir a árvore no formato esquerda-raiz-direita segundo Sedgewick 
    if (arvore == NULL){
        return;
    }
    imprimeArvore(arvore->dir, b + 1);
    imprimeNo(arvore->chave, b); 
    imprimeArvore(arvore->esq, b + 1);
}

int main() {
    struct typeno *arvore = NULL;

    // Adicionando valores à árvore
    add(&arvore, 12);
    add(&arvore, 10);
    add(&arvore, 15);
    add(&arvore, 6);
    add(&arvore, 14);
    add(&arvore, 11);
    add(&arvore, 20);
    add(&arvore, 16);
    add(&arvore, 1);
    add(&arvore, 8);


    imprimeArvore(arvore, 0); // Imprimir a árvore

    remover(&arvore, 6); // Removendo nó da árvore
    remover(&arvore, 1); 
    return 0;
}


