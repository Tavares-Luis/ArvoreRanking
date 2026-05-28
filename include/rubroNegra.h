
#ifndef RUBRONEGRA_H
#define RUBRONEGRA_H

typedef enum {
    VERMELHO,
    PRETO
} Cor;

typedef struct NoRN {

    char nome[50];
    int pontuacao;

    Cor cor;

    struct NoRN* esq;
    struct NoRN* dir;
    struct NoRN* pai;

} NoRN;

NoRN* criarNoRN(char nome[], int pontuacao);
NoRN* inserirRN(NoRN* raiz, char nome[], int pontuacao, int* rotacoes);
void corrigirInsercao(NoRN** raiz, NoRN* novo, int* rotacoes);
NoRN* removerRN(NoRN* raiz, int pontuacao, int* rotacoes);
NoRN* buscarPorNomeRN(NoRN* raiz, char nome[]);
int validarRN(NoRN* raiz);
void imprimirRankingRN(NoRN* raiz, int* contador);
NoRN* rotacaoEsquerdaRN(NoRN* x, int* rotacoes);
NoRN* rotacaoDireitaRN(NoRN* y, int* rotacoes);

#endif //RUBRONEGRA_H
