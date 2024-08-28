// DIOGO OLIVEIRA SANTOS - 202311226

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct typeno{ // definição nó

    int chave;
    struct typeno *esq;
    struct typeno *dir;
    struct typeno *pai;
};


struct typeno *build(int valor){ // criação 

    struct typeno *novo = (struct typeno *)malloc(sizeof(struct typeno));
        if (novo == NULL)
        {
            printf("Erro: Falha ao alocar memória para o novo nó.\n");
            exit(-1);
        }
        novo->chave = valor;
        novo->esq = NULL;
        novo->dir = NULL;
        novo->pai = NULL;
        return novo;

}

struct typeno *add(struct typeno *arvore, int valor){


    if(arvore == NULL){
        return build(valor);
    }

    else{
        if(valor < arvore->chave){

            if(arvore->esq == NULL){
                struct typeno *novo = add(arvore->esq, valor);
                arvore->esq = novo;
                novo->pai = arvore;
            }
        }
        else{
            if(arvore->dir == NULL){
                struct typeno *novo = add(arvore->dir, valor);
                arvore->dir = novo;
                novo->pai = arvore;
            }
        }
    }
    return arvore;

}

void remover(struct typeno **raiz, struct typeno *no) {
    if (no == NULL) return;

    // Caso 1: Nó é uma folha (não tem filhos)
    if (no->esq == NULL && no->dir == NULL) {
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

    // Caso 2: Nó tem apenas um filho à direita
    else if (no->esq == NULL && no->dir != NULL) {
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

    // Caso 3: Nó tem apenas um filho à esquerda
    else if (no->esq != NULL && no->dir == NULL) {
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

    // Caso 4: Nó tem dois filhos
    else {
        // Encontrar o sucessor (o menor nó na subárvore direita)
        struct typeno *sucessor = no->dir;
        while (sucessor->esq != NULL) {
            sucessor = sucessor->esq;
        }
        no->chave = sucessor->chave;
        remover(raiz, sucessor);
    }
}


struct typeno *find(struct typeno *arvore, int valor){

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

void imprimeArvore(struct typeno *arvore){

    printf("1 - Em ordem \n 2 - Pré ordem \n 3 - Pós ordem");
    printf("Por favor, digite um número: ");
    int choice = scanf("%d", &choice);

    switch(choice){

        case 1:
            em_ordem(arvore);

        //case 2: 
            // executar print em pré ordem

        //case 3: 
            // executar print em pós ordem
    }

}

void em_ordem(struct typeno *arvore){
    
    while(arvore->esq != NULL){
        arvore = arvore->esq;
        printf("%d\n",arvore->chave);
        printf("%d\n",arvore->pai->chave);
        if(arvore->pai->dir != NULL) printf("%d\n",arvore->pai->dir->chave);
    };

}

int main(){

    struct typeno *arvore = NULL;
    add(arvore, 12);
    add(arvore, 10);
    add(arvore, 15);
    add(arvore, 6);
    add(arvore, 14);
    add(arvore, 11);
    add(arvore, 20);
    add(arvore, 16);
    add(arvore, 1); 
    add(arvore, 8);

    imprimeArvore(arvore);

    return 0;
}

int main();

