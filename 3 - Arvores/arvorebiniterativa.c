#include <stdio.h>   // Biblioteca padrão para entrada e saída
#include <stdlib.h>  // Biblioteca padrão para alocação de memória

// Estrutura de um nó da árvore binária
typedef struct NoArvore {
    int dado;                  // Valor armazenado no nó
    struct NoArvore* esquerda; // Ponteiro para o filho à esquerda
    struct NoArvore* direita;  // Ponteiro para o filho à direita
} NoArvore;

// Estrutura para um nó da pilha
typedef struct Pilha {
    NoArvore* no;              // Ponteiro para um nó da árvore
    struct Pilha* prox;        // Ponteiro para o próximo elemento na pilha
} Pilha;

// Função para criar um novo nó da árvore
NoArvore* criarNo(int dado) {
    NoArvore* novoNo = (NoArvore*)malloc(sizeof(NoArvore));  // Aloca memória para o novo nó
    if (!novoNo) {  // Verifica se a alocação foi bem-sucedida
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        return NULL;
    }
    novoNo->dado = dado;       // Define o valor do nó
    novoNo->esquerda = NULL;   // Inicializa o ponteiro esquerdo como NULL
    novoNo->direita = NULL;    // Inicializa o ponteiro direito como NULL
    return novoNo;
}

// Função para inserir elementos de um vetor ordenado em uma árvore balanceada
NoArvore* inserirElementos(int vetor[], int inicio, int fim) {
    if (inicio > fim)  // Condição de parada: sublista inválida
        return NULL;

    int meio = (inicio + fim) / 2; // Índice do meio do vetor
    NoArvore* novoNo = criarNo(vetor[meio]); // Cria o nó com o valor do meio

    // Inserção recursiva dos elementos nas subárvores esquerda e direita
    novoNo->esquerda = inserirElementos(vetor, inicio, meio - 1);
    novoNo->direita = inserirElementos(vetor, meio + 1, fim);
    
    return novoNo;
}

// Função para buscar um elemento na árvore binária
NoArvore* buscarElemento(NoArvore* raiz, int dado) {
    if (!raiz || raiz->dado == dado)  // Se a árvore estiver vazia ou o valor for encontrado
        return raiz;

    // Busca recursiva: esquerda se o valor é menor, direita caso contrário
    return (dado < raiz->dado) ? buscarElemento(raiz->esquerda, dado) : buscarElemento(raiz->direita, dado);
}

// Função para encontrar o nó com o menor valor em uma subárvore
NoArvore* encontrarMinimo(NoArvore* raiz) {
    NoArvore* atual = raiz;
    while (atual && atual->esquerda) {  // Vai até o nó mais à esquerda
        atual = atual->esquerda;
    }
    return atual;
}

// Função para excluir um nó da árvore
NoArvore* excluir(NoArvore* raiz, int dado) {
    if (!raiz)  // Se a árvore estiver vazia, retorna NULL
        return NULL;

    if (dado < raiz->dado) {
        raiz->esquerda = excluir(raiz->esquerda, dado);
    } else if (dado > raiz->dado) {
        raiz->direita = excluir(raiz->direita, dado);
    } else {
        // Caso 1 e 2: Nó folha ou com apenas um filho
        if (!raiz->esquerda || !raiz->direita) {
            NoArvore* temp = raiz->esquerda ? raiz->esquerda : raiz->direita;
            free(raiz);  // Libera o nó
            return temp;
        }

        // Caso 3: Nó com dois filhos
        NoArvore* temp = encontrarMinimo(raiz->direita); // Sucessor in-order
        raiz->dado = temp->dado;
        raiz->direita = excluir(raiz->direita, temp->dado);
    }
    return raiz;
}

// Funções auxiliares para operações de pilha
void push(Pilha** topo, NoArvore* no) {
    Pilha* novaPilha = (Pilha*)malloc(sizeof(Pilha));
    novaPilha->no = no;
    novaPilha->prox = *topo;
    *topo = novaPilha;
}

NoArvore* pop(Pilha** topo) {
    if (!*topo) return NULL;
    Pilha* temp = *topo;
    NoArvore* desempilhado = temp->no;
    *topo = temp->prox;
    free(temp);
    return desempilhado;
}

// Funções de travessia da árvore (Recursivas e Iterativas)
void preOrdemRec(NoArvore* raiz) {
    if (raiz) {
        printf("%d ", raiz->dado);
        preOrdemRec(raiz->esquerda);
        preOrdemRec(raiz->direita);
    }
}

void emOrdemRec(NoArvore* raiz) {
    if (raiz) {
        emOrdemRec(raiz->esquerda);
        printf("%d ", raiz->dado);
        emOrdemRec(raiz->direita);
    }
}

void posOrdemRec(NoArvore* raiz) {
    if (raiz) {
        posOrdemRec(raiz->esquerda);
        posOrdemRec(raiz->direita);
        printf("%d ", raiz->dado);
    }
}

void preOrdemIt(NoArvore* raiz) {
    if (!raiz) return;
    
    Pilha* pilha = NULL;
    push(&pilha, raiz);
    
    while (pilha) {
        NoArvore* atual = pop(&pilha);
        printf("%d ", atual->dado);
        
        if (atual->direita) push(&pilha, atual->direita);
        if (atual->esquerda) push(&pilha, atual->esquerda);
    }
}

void emOrdemIt(NoArvore* raiz) {
    Pilha* pilha = NULL;
    NoArvore* atual = raiz;
    
    while (atual || pilha) {
        while (atual) {
            push(&pilha, atual);
            atual = atual->esquerda;
        }
        
        atual = pop(&pilha);
        printf("%d ", atual->dado);
        
        atual = atual->direita;
    }
}

void posOrdemIt(NoArvore* raiz) {
    if (!raiz) return;
    
    Pilha* pilha1 = NULL;
    Pilha* pilha2 = NULL;
    push(&pilha1, raiz);
    
    while (pilha1) {
        NoArvore* atual = pop(&pilha1);
        push(&pilha2, atual);
        
        if (atual->esquerda) push(&pilha1, atual->esquerda);
        if (atual->direita) push(&pilha1, atual->direita);
    }
    
    while (pilha2) {
        NoArvore* atual = pop(&pilha2);
        printf("%d ", atual->dado);
    }
}

// Função principal para testar o código
int main() {
    int vetor[] = {1, 2, 3, 4, 5, 6, 7};
    int n = sizeof(vetor) / sizeof(vetor[0]);
    
    // Cria árvore balanceada a partir do vetor ordenado
    NoArvore* raiz = inserirElementos(vetor, 0, n - 1);
    
    printf("Travessia Pré-Ordem Recursiva: ");
    preOrdemRec(raiz);
    printf("\n");
    
    printf("Travessia Em Ordem Recursiva: ");
    emOrdemRec(raiz);
    printf("\n");
    
    printf("Travessia Pós-Ordem Recursiva: ");
    posOrdemRec(raiz);
    printf("\n");
    
    printf("Travessia Pré-Ordem Iterativa: ");
    preOrdemIt(raiz);
    printf("\n");
    
    printf("Travessia Em Ordem Iterativa: ");
    emOrdemIt(raiz);
    printf("\n");
    
    printf("Travessia Pós-Ordem Iterativa: ");
    posOrdemIt(raiz);
    printf("\n");
    
    return 0;
}
