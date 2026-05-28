#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/avl.h"


No* criarNo(char nome[], int pontuacao) {

    No* novo = (No*) malloc(sizeof(No));

    strcpy(novo->nome, nome);
    novo->pontuacao = pontuacao;

    novo->altura = 1;

    novo->esq = NULL;
    novo->dir = NULL;

    return novo;
}

int altura(No* no) {

    if (no == NULL)
        return 0;

    return no->altura;
}

int fatorBalanceamento(No* no) {

    if (no == NULL)
        return 0;

    return altura(no->esq) - altura(no->dir);
}

No* rotacaoDireita(No* y, int* rotacoes) {

    (*rotacoes)++;

    No* x = y->esq;
    No* T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = 1 + (
        altura(y->esq) > altura(y->dir)
        ? altura(y->esq)
        : altura(y->dir)
    );

    x->altura = 1 + (
        altura(x->esq) > altura(x->dir)
        ? altura(x->esq)
        : altura(x->dir)
    );

    return x;
}

No* rotacaoEsquerda(No* x, int* rotacoes) {

    (*rotacoes)++;

    No* y = x->dir;
    No* T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = 1 + (
        altura(x->esq) > altura(x->dir)
        ? altura(x->esq)
        : altura(x->dir)
    );

    y->altura = 1 + (
        altura(y->esq) > altura(y->dir)
        ? altura(y->esq)
        : altura(y->dir)
    );

    return y;
}


No* inserirAVL(No* raiz, char nome[], int pontuacao, int* rotacoes) {

    if (raiz == NULL)
        return criarNo(nome, pontuacao);

    if (pontuacao < raiz->pontuacao)
        raiz->esq = inserirAVL(raiz->esq, nome, pontuacao, rotacoes);

    else if (pontuacao > raiz->pontuacao)
        raiz->dir = inserirAVL(raiz->dir, nome, pontuacao, rotacoes);

    else
        return raiz;

    // atualiza altura
    raiz->altura = 1 + (
        altura(raiz->esq) > altura(raiz->dir)
        ? altura(raiz->esq)
        : altura(raiz->dir)
    );

    // fator
    int balanceamento = fatorBalanceamento(raiz);

    // LL
    if (balanceamento > 1 && pontuacao < raiz->esq->pontuacao)
        return rotacaoDireita(raiz, rotacoes);

    // RR
    if (balanceamento < -1 && pontuacao > raiz->dir->pontuacao)
        return rotacaoEsquerda(raiz, rotacoes);

    // LR
    if (balanceamento > 1 && pontuacao > raiz->esq->pontuacao) {
        raiz->esq = rotacaoEsquerda(raiz->esq, rotacoes);
        return rotacaoDireita(raiz, rotacoes);
    }

    // RL
    if (balanceamento < -1 && pontuacao < raiz->dir->pontuacao) {
        raiz->dir = rotacaoDireita(raiz->dir, rotacoes);
        return rotacaoEsquerda(raiz, rotacoes);
    }

    return raiz;
}

No* buscar(No* raiz, int pontuacao) {

    if (raiz == NULL || raiz->pontuacao == pontuacao)
        return raiz;

    if (pontuacao < raiz->pontuacao)
        return buscar(raiz->esq, pontuacao);

    return buscar(raiz->dir, pontuacao);
}


void imprimirRanking(No* raiz, int* contador) {

    if (raiz == NULL || *contador >= 10)
        return;

    imprimirRanking(raiz->dir, contador);

    printf("%d - %s\n", raiz->pontuacao, raiz->nome);

    (*contador)++;

    imprimirRanking(raiz->esq, contador);
}

// Buscar por nome (O(n) - percorre toda a árvore)
No* buscarPorNome(No* raiz, char nome[]) {

    if (raiz == NULL)
        return NULL;

    if (strcmp(raiz->nome, nome) == 0)
        return raiz;

    No* esq = buscarPorNome(raiz->esq, nome);
    if (esq != NULL)
        return esq;

    return buscarPorNome(raiz->dir, nome);
}

// Validar propriedades AVL
int validarAVL(No* raiz) {

    if (raiz == NULL)
        return 1;

    int bf = fatorBalanceamento(raiz);

    // Verificar se fator de balanceamento está válido
    if (bf < -1 || bf > 1)
        return 0;

    // Validar recursivamente
    return validarAVL(raiz->esq) && validarAVL(raiz->dir);
}

// Encontrar nó com menor pontuação em subárvore
static No* encontrarMinimo(No* raiz) {

    while (raiz->esq != NULL)
        raiz = raiz->esq;

    return raiz;
}

// Remover nó da AVL
No* removerAVL(No* raiz, int pontuacao, int* rotacoes) {

    if (raiz == NULL)
        return NULL;

    if (pontuacao < raiz->pontuacao)
        raiz->esq = removerAVL(raiz->esq, pontuacao, rotacoes);

    else if (pontuacao > raiz->pontuacao)
        raiz->dir = removerAVL(raiz->dir, pontuacao, rotacoes);

    else {
        // Nó encontrado
        // Caso 1: Sem filhos
        if (raiz->esq == NULL && raiz->dir == NULL) {
            free(raiz);
            return NULL;
        }

        // Caso 2: Um filho
        if (raiz->esq == NULL) {
            No* temp = raiz->dir;
            free(raiz);
            return temp;
        }

        if (raiz->dir == NULL) {
            No* temp = raiz->esq;
            free(raiz);
            return temp;
        }

        // Caso 3: Dois filhos
        No* sucessor = encontrarMinimo(raiz->dir);

        raiz->pontuacao = sucessor->pontuacao;
        strcpy(raiz->nome, sucessor->nome);

        raiz->dir = removerAVL(raiz->dir, sucessor->pontuacao, rotacoes);
    }

    if (raiz == NULL)
        return NULL;

    // Atualizar altura
    raiz->altura = 1 + (
        altura(raiz->esq) > altura(raiz->dir)
        ? altura(raiz->esq)
        : altura(raiz->dir)
    );

    // Verificar fator de balanceamento
    int bf = fatorBalanceamento(raiz);

    // Caso LL
    if (bf > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz, rotacoes);

    // Caso LR
    if (bf > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq, rotacoes);
        return rotacaoDireita(raiz, rotacoes);
    }

    // Caso RR
    if (bf < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz, rotacoes);

    // Caso RL
    if (bf < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir, rotacoes);
        return rotacaoEsquerda(raiz, rotacoes);
    }

    return raiz;
}

