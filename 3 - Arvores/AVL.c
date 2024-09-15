#include <stdio.h>
#include <stdlib.h>

// DIOGO OLIVEIRA SANTOS - 202311226

//---------- ÁRVORE BINÁRIA ---------- 

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


typedef struct typenoAVL{

    int info;
    int altura; // altura da subárvore
    struct typeno *esq;
    struct typeno *dir;

}typenoAVL;

//---------- FUNÇÕES AUXILIARES ----------

int altura(typenoAVL* no){ 

    if(no == NULL){ // se o nó não existir, a altura da árvore é -1
        return -1;
    }
    else{
        return no->altura;
    }
}

int fatorBalanceamento(typenoAVL* no){

    return abs(altura(no->esq) - altura(no->dir)); 
    /* o sinal do FB determinará apenas qual rotação será aplicada.
    Neste caso, precisaremos apenas do módulo.  */
}

//---------- ROTAÇÕES ----------

void rotSimDir(ArvAVL *raiz){ // ponteiro de ponteiro para referenciar a raíz

    struct typenoAVL *no;

    no = (*raiz)->esq; // auxiliar para o lado esquerdo da raíz
    (*raiz)->esq = no->dir;
    no->dir = *raiz;

    (*raiz)->altura = fmax(altura((*raiz)->esq), altura((*raiz)->dir)) +1;

    no->altura = fmax(altura(no->esq), (*raiz)->altura) + 1;

    *raiz = no;
}

void rotSimEsq(ArvAVL *raiz){

    struct typenoAVL *no;

    no = (*raiz)->dir; //auxiliar para o lado direito da raíz 
    (*raiz)->dir = no->esq;
    no->esq = (*raiz);

    (*raiz)-> altura = fmax(altura((*raiz)->esq), altura((*raiz)->dir) + 1);

    no->altura = fmax(altura(no->dir), ((*raiz)->altura) + 1);

    (*raiz) = no;

}

int insere_ArvAVL(ArvAVL *raiz, int valor){
    int res;
    if(*raiz == NULL){//árvore vazia ou nó folha
        struct typenoAVL *novo;
        novo = (struct typenoAVL*)malloc(sizeof(struct typenoAVL));
        if(novo == NULL)
            return 0;

        novo->info = valor;
        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }

    struct typenoAVL *atual = *raiz;
    if(valor < atual->info){
        if((res = insere_ArvAVL(&(atual->esq), valor)) == 1){
            if(fatorBalanceamento_NO(atual) >= 2){
                if(valor < (*raiz)->esq->chave ){
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
    }else{
        if(valor > atual->info){
            if((res = insere_ArvAVL(&(atual->dir), valor)) == 1){
                if(fatorBalanceamento_NO(atual) >= 2){
                    if((*raiz)->dir->info < valor){
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }else{
            printf("Valor duplicado!!\n");
            return 0;
        }
    }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return res;
}

int remove_ArvAVL(ArvAVL *raiz, int valor){
	if(*raiz == NULL){// valor não existe
	    printf("valor não existe!!\n");
	    return 0;
	}

    int res;
	if(valor < (*raiz)->info){
	    if((res = remove_ArvAVL(&(*raiz)->esq,valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->dir->esq) <= altura_NO((*raiz)->dir->dir))
                    RotacaoRR(raiz);
                else
                    RotacaoRL(raiz);
            }
	    }
	}

	if((*raiz)->info < valor){
	    if((res = remove_ArvAVL(&(*raiz)->dir, valor)) == 1){
            if(fatorBalanceamento_NO(*raiz) >= 2){
                if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq) )
                    RotacaoLL(raiz);
                else
                    RotacaoLR(raiz);
            }
	    }
	}

	if((*raiz)->info == valor){
	    if(((*raiz)->esq == NULL || (*raiz)->dir == NULL)){// nó tem 1 filho ou nenhum
			struct typenoAVL *oldNode = (*raiz);
			if((*raiz)->esq != NULL)
                *raiz = (*raiz)->esq;
            else
                *raiz = (*raiz)->dir;
			free(oldNode);
		}else { // nó tem 2 filhos
			struct typenoAVL* temp = procuraMenor((*raiz)->dir);
			(*raiz)->info = temp->info;
			remove_ArvAVL(&(*raiz)->dir, (*raiz)->info);
            if(fatorBalanceamento_NO(*raiz) >= 2){
				if(altura_NO((*raiz)->esq->dir) <= altura_NO((*raiz)->esq->esq))
					RotacaoLL(raiz);
				else
					RotacaoLR(raiz);
			}
		}
		if (*raiz != NULL)
            (*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;
		return 1;
	}

	(*raiz)->altura = maior(altura_NO((*raiz)->esq),altura_NO((*raiz)->dir)) + 1;

	return res;
}

