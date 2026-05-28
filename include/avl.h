
#ifndef BST_H
#define BST_H

typedef struct No {

    char nome[50];
    int pontuacao;
    int altura;

    struct No* esq;
    struct No* dir;

} No;

No* inserirAVL(No* raiz, char nome[], int pontuacao, int* rotacoes);
No* criarNo(char nome[], int pontuacao);
int altura(No* no);
int fatorBalanceamento(No* no);
No* rotacaoDireita(No* y, int* rotacoes);
No* rotacaoEsquerda(No* x, int* rotacoes);
No* removerAVL(No* raiz, int pontuacao, int* rotacoes);
No* buscarPorNome(No* raiz, char nome[]);
int validarAVL(No* raiz);
No* buscar(No* raiz, int pontuacao);
void imprimirRanking(No* raiz, int* contador);

#endif //BST_H
