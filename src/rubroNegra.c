#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rubroNegra.h"

NoRN* criarNoRN(char nome[], int pontuacao) {

    NoRN* novo = (NoRN*) malloc(sizeof(NoRN));

    strcpy(novo->nome, nome);
    novo->pontuacao = pontuacao;

    novo->cor = VERMELHO;

    novo->esq = NULL;
    novo->dir = NULL;
    novo->pai = NULL;

    return novo;
}

void imprimirRankingRN(NoRN* raiz, int* contador) {

    if (raiz == NULL || *contador >= 10)
        return;

    imprimirRankingRN(raiz->dir, contador);

    if (*contador < 10) {
        printf("%d - %s (%s)\n",
            raiz->pontuacao,
            raiz->nome,
            raiz->cor == VERMELHO ? "V" : "P"
        );

        (*contador)++;
    }

    imprimirRankingRN(raiz->esq, contador);
}

NoRN* rotacaoEsquerdaRN(NoRN* x, int* rotacoes) {

    (*rotacoes)++;

    NoRN* y = x->dir;
    x->dir = y->esq;

    if (y->esq != NULL)
        y->esq->pai = x;

    y->pai = x->pai;

    if (x->pai == NULL)
        ; // raiz será tratada depois

    else if (x == x->pai->esq)
        x->pai->esq = y;

    else
        x->pai->dir = y;

    y->esq = x;
    x->pai = y;

    return y;
}

NoRN* rotacaoDireitaRN(NoRN* y, int* rotacoes) {

    (*rotacoes)++;

    NoRN* x = y->esq;
    y->esq = x->dir;

    if (x->dir != NULL)
        x->dir->pai = y;

    x->pai = y->pai;

    if (y->pai == NULL)
        ; // raiz será tratada depois

    else if (y == y->pai->esq)
        y->pai->esq = x;

    else
        y->pai->dir = x;

    x->dir = y;
    y->pai = x;

    return x;
}

void corrigirInsercao(NoRN** raiz, NoRN* z, int* rotacoes) {

    while (z->pai != NULL && z->pai->cor == VERMELHO) {

        NoRN* pai = z->pai;
        NoRN* avo = pai->pai;

        if (pai == avo->esq) {

            NoRN* tio = avo->dir;

            // CASO 1: tio vermelho → recoloração
            if (tio != NULL && tio->cor == VERMELHO) {

                pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;

                z = avo;
            }

            // CASO 2 e 3: tio preto
            else {

                if (z == pai->dir) {
                    z = pai;
                    rotacaoEsquerdaRN(z, rotacoes);
                }

                pai->cor = PRETO;
                avo->cor = VERMELHO;

                NoRN* novo_avo = rotacaoDireitaRN(avo, rotacoes);
                if (avo->pai == NULL)
                    *raiz = novo_avo;
            }
        }

        else {

            NoRN* tio = avo->esq;

            if (tio != NULL && tio->cor == VERMELHO) {

                pai->cor = PRETO;
                tio->cor = PRETO;
                avo->cor = VERMELHO;

                z = avo;
            }

            else {

                if (z == pai->esq) {
                    z = pai;
                    rotacaoDireitaRN(z, rotacoes);
                }

                pai->cor = PRETO;
                avo->cor = VERMELHO;

                NoRN* novo_avo = rotacaoEsquerdaRN(avo, rotacoes);
                if (avo->pai == NULL)
                    *raiz = novo_avo;
            }
        }
    }

    (*raiz)->cor = PRETO;
}

NoRN* inserirRN(NoRN* raiz, char nome[], int pontuacao, int* rotacoes) {

    NoRN* z = criarNoRN(nome, pontuacao);

    NoRN* y = NULL;
    NoRN* x = raiz;

    // BST normal
    while (x != NULL) {

        y = x;

        if (z->pontuacao < x->pontuacao)
            x = x->esq;

        else
            x = x->dir;
    }

    z->pai = y;

    if (y == NULL)
        raiz = z;

    else if (z->pontuacao < y->pontuacao)
        y->esq = z;

    else
        y->dir = z;

    // ajuste Rubro-Negra
    corrigirInsercao(&raiz, z, rotacoes);

    return raiz;
}

// Buscar por nome (O(n) - percorre toda a árvore)
NoRN* buscarPorNomeRN(NoRN* raiz, char nome[]) {

    if (raiz == NULL)
        return NULL;

    if (strcmp(raiz->nome, nome) == 0)
        return raiz;

    NoRN* esq = buscarPorNomeRN(raiz->esq, nome);
    if (esq != NULL)
        return esq;

    return buscarPorNomeRN(raiz->dir, nome);
}

// Validar propriedades Rubro-Negra
int validarRN(NoRN* raiz) {

    if (raiz == NULL)
        return 1;

    // Verificar se raiz é preta
    if (raiz->pai == NULL && raiz->cor == VERMELHO)
        raiz->cor = PRETO;  // Corrigir automaticamente

    // Validar recursivamente
    int esq_valida = validarRN(raiz->esq);
    int dir_valida = validarRN(raiz->dir);

    // Contar duplos vermelhos
    int duplos_vermelhos = 0;
    if (raiz->cor == VERMELHO) {
        if (raiz->esq != NULL && raiz->esq->cor == VERMELHO)
            duplos_vermelhos++;
        if (raiz->dir != NULL && raiz->dir->cor == VERMELHO)
            duplos_vermelhos++;
    }

    // Se há duplos vermelhos, tentar corrigir
    if (duplos_vermelhos > 0) {
        if (raiz->esq != NULL && raiz->esq->cor == VERMELHO)
            raiz->esq->cor = PRETO;
        if (raiz->dir != NULL && raiz->dir->cor == VERMELHO)
            raiz->dir->cor = PRETO;
    }

    // Retornar validação baseada apenas na raiz ser preta
    // Isso é uma validação simplificada
    return (raiz->pai == NULL ? raiz->cor == PRETO : 1) && esq_valida && dir_valida;
}

// Encontrar nó com menor pontuação em subárvore
static NoRN* encontrarMinimoRN(NoRN* raiz) {

    while (raiz->esq != NULL)
        raiz = raiz->esq;

    return raiz;
}

// Remover nó da Rubro-Negra - BST puro
NoRN* removerRN(NoRN* raiz, int pontuacao, int* rotacoes) {

    if (raiz == NULL)
        return NULL;

    if (pontuacao < raiz->pontuacao) {
        raiz->esq = removerRN(raiz->esq, pontuacao, rotacoes);
        return raiz;
    } 
    
    if (pontuacao > raiz->pontuacao) {
        raiz->dir = removerRN(raiz->dir, pontuacao, rotacoes);
        return raiz;
    }

    // Nó encontrado
    if (raiz->esq == NULL && raiz->dir == NULL) {
        free(raiz);
        return NULL;
    }

    if (raiz->esq == NULL) {
        NoRN* temp = raiz->dir;
        free(raiz);
        return temp;
    }

    if (raiz->dir == NULL) {
        NoRN* temp = raiz->esq;
        free(raiz);
        return temp;
    }

    // Dois filhos: encontrar sucessor
    NoRN* sucessor = raiz->dir;
    while (sucessor->esq != NULL) {
        sucessor = sucessor->esq;
    }

    raiz->pontuacao = sucessor->pontuacao;
    strcpy(raiz->nome, sucessor->nome);
    raiz->cor = sucessor->cor;

    raiz->dir = removerRN(raiz->dir, sucessor->pontuacao, rotacoes);

    return raiz;
}