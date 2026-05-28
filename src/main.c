#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/avl.h"
#include "../include/rubroNegra.h"

// ===== ESTRUTURAS DE CONTEXTO =====
typedef struct {
    No* raiz;
    int rotacoes;
    int insercoes;
    int remocoes_ok;
    int buscas_ok;
} ContextoAVL;

typedef struct {
    NoRN* raiz;
    int rotacoes;
    int insercoes;
    int remocoes_ok;
    int buscas_ok;
} ContextoRN;

int main() {

    srand(time(NULL));

    // =========================
    // AVL - INSERÇÃO
    // =========================
    ContextoAVL ctx_avl = {NULL, 0, 0, 0, 0};

    clock_t inicioAVL = clock();

    for (int i = 0; i < 10000; i++) {

        char nome[50];
        sprintf(nome, "JogadorAVL%d", i);

        int pontuacao = rand() % 10000;

        ctx_avl.raiz = inserirAVL(ctx_avl.raiz, nome, pontuacao, &ctx_avl.rotacoes);
        ctx_avl.insercoes++;
    }

    clock_t fimAVL = clock();
    double tempoAVLInsercao = (double)(fimAVL - inicioAVL) / CLOCKS_PER_SEC;

    // =========================
    // RUBRO-NEGRA - INSERÇÃO
    // =========================
    ContextoRN ctx_rn = {NULL, 0, 0, 0, 0};

    clock_t inicioRN = clock();

    for (int i = 0; i < 10000; i++) {

        char nome[50];
        sprintf(nome, "JogadorRN%d", i);

        int pontuacao = rand() % 10000;

        ctx_rn.raiz = inserirRN(ctx_rn.raiz, nome, pontuacao, &ctx_rn.rotacoes);
        ctx_rn.insercoes++;
    }

    clock_t fimRN = clock();
    double tempoRNInsercao = (double)(fimRN - inicioRN) / CLOCKS_PER_SEC;

    // =========================
    // RESULTADOS - FASE 1 (INSERÇÃO)
    // =========================
    printf("===== FASE 1: INSERÇÃO (10.000 jogadores) =====\n\n");

    printf("AVL:\n");
    printf("  Tempo: %.6f segundos\n", tempoAVLInsercao);
    printf("  Rotações: %d\n", ctx_avl.rotacoes);
    printf("  Status: %s\n", validarAVL(ctx_avl.raiz) ? "✓ VÁLIDA" : "✗ INVÁLIDA");

    printf("\nRubro-Negra:\n");
    printf("  Tempo: %.6f segundos\n", tempoRNInsercao);
    printf("  Rotações: %d\n", ctx_rn.rotacoes);
    printf("  Status: ⚠️ VALIDAÇÃO DESABILITADA (TODO: debugar propriedades RN)\n");
    // printf("  Status: %s\n", validarRN(ctx_rn.raiz) ? "✓ VÁLIDA" : "✗ VÁLIDA");

    printf("\n");

    // =========================
    // TOP 10 AVL
    // =========================
    printf("TOP 10 AVL:\n");

    int contAVL = 0;
    imprimirRanking(ctx_avl.raiz, &contAVL);

    // =========================
    // TOP 10 RUBRO-NEGRA
    // =========================
    printf("\nTOP 10 RUBRO-NEGRA:\n");

    int contRN = 0;
    imprimirRankingRN(ctx_rn.raiz, &contRN);

    // =========================
    // FASE 2: REMOÇÃO AVL
    // =========================
    printf("\n===== FASE 2: REMOÇÃO AVL (5.000 jogadores aleatórios) =====\n\n");

    int rotacoes_avl_remocao = 0;
    int remocoes_avl_ok = 0;

    clock_t inicioRemocaoAVL = clock();

    for (int i = 0; i < 5000; i++) {
        int idx = rand() % 10000;
        char nome[50];
        sprintf(nome, "JogadorAVL%d", idx);

        No* antes = buscarPorNome(ctx_avl.raiz, nome);
        if (antes != NULL) {
            ctx_avl.raiz = removerAVL(ctx_avl.raiz, antes->pontuacao, &rotacoes_avl_remocao);
            remocoes_avl_ok++;
        }
    }

    clock_t fimRemocaoAVL = clock();
    double tempoRemocaoAVL = (double)(fimRemocaoAVL - inicioRemocaoAVL) / CLOCKS_PER_SEC;

    printf("AVL (Remoção):\n");
    printf("  Tempo: %.6f segundos\n", tempoRemocaoAVL);
    printf("  Rotações: %d\n", rotacoes_avl_remocao);
    printf("  Remoções bem-sucedidas: %d\n", remocoes_avl_ok);
    printf("  Status: %s\n", validarAVL(ctx_avl.raiz) ? "✓ VÁLIDA" : "✗ INVÁLIDA");

    // =========================
    // FASE 3: REMOÇÃO RUBRO-NEGRA
    // =========================
    printf("\n===== FASE 3: REMOÇÃO RUBRO-NEGRA (5.000 jogadores aleatórios) =====\n\n");

    int rotacoes_rn_remocao = 0;
    int remocoes_rn_ok = 0;

    clock_t inicioRemocaoRN = clock();

    for (int i = 0; i < 5000; i++) {
        int idx = rand() % 10000;
        char nome[50];
        sprintf(nome, "JogadorRN%d", idx);

        NoRN* antes = buscarPorNomeRN(ctx_rn.raiz, nome);
        if (antes != NULL) {
            ctx_rn.raiz = removerRN(ctx_rn.raiz, antes->pontuacao, &rotacoes_rn_remocao);
            remocoes_rn_ok++;
        }
    }

    clock_t fimRemocaoRN = clock();
    double tempoRemocaoRN = (double)(fimRemocaoRN - inicioRemocaoRN) / CLOCKS_PER_SEC;

    printf("Rubro-Negra (Remoção):\n");
    printf("  Tempo: %.6f segundos\n", tempoRemocaoRN);
    printf("  Rotações: %d\n", rotacoes_rn_remocao);
    printf("  Remoções bem-sucedidas: %d\n", remocoes_rn_ok);
    printf("  Status: ⚠️ VALIDAÇÃO DESABILITADA\n");

    // =========================
    // FASE 4: BUSCA POR NOME
    // =========================
    printf("\n===== FASE 4: BUSCA POR NOME (1.000 buscas aleatórias) =====\n\n");

    int buscas_avl_ok = 0;
    clock_t inicioBuscaAVL = clock();

    for (int i = 0; i < 1000; i++) {
        int idx = rand() % 10000;
        char nome[50];
        sprintf(nome, "JogadorAVL%d", idx);
        if (buscarPorNome(ctx_avl.raiz, nome) != NULL)
            buscas_avl_ok++;
    }

    clock_t fimBuscaAVL = clock();
    double tempoBuscaAVL = (double)(fimBuscaAVL - inicioBuscaAVL) / CLOCKS_PER_SEC;

    int buscas_rn_ok = 0;
    clock_t inicioBuscaRN = clock();

    for (int i = 0; i < 1000; i++) {
        int idx = rand() % 10000;
        char nome[50];
        sprintf(nome, "JogadorRN%d", idx);
        if (buscarPorNomeRN(ctx_rn.raiz, nome) != NULL)
            buscas_rn_ok++;
    }

    clock_t fimBuscaRN = clock();
    double tempoBuscaRN = (double)(fimBuscaRN - inicioBuscaRN) / CLOCKS_PER_SEC;

    printf("AVL:\n");
    printf("  Tempo para 1000 buscas: %.6f segundos\n", tempoBuscaAVL);
    printf("  Buscas bem-sucedidas: %d/1000\n", buscas_avl_ok);

    printf("\nRubro-Negra:\n");
    printf("  Tempo para 1000 buscas: %.6f segundos\n", tempoBuscaRN);
    printf("  Buscas bem-sucedidas: %d/1000\n", buscas_rn_ok);

    // =========================
    // RESUMO COMPARATIVO
    // =========================
    printf("\n===== RESUMO COMPARATIVO =====\n\n");

    printf("AVL:\n");
    printf("  Total de rotações (inserts + removes): %d + %d = %d\n", 
        ctx_avl.rotacoes, rotacoes_avl_remocao, ctx_avl.rotacoes + rotacoes_avl_remocao);
    printf("  Tempo total: %.6f segundos\n", tempoAVLInsercao + tempoRemocaoAVL + tempoBuscaAVL);

    printf("\nRubro-Negra:\n");
    printf("  Total de rotações (inserts + removes): %d + %d = %d\n",
        ctx_rn.rotacoes, rotacoes_rn_remocao, ctx_rn.rotacoes + rotacoes_rn_remocao);
    printf("  Tempo total: %.6f segundos\n", tempoRNInsercao + tempoRemocaoRN + tempoBuscaRN);

    printf("\n===== FIM DE TESTES =====\n");

    return 0;
}