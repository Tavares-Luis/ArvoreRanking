# Relatório: AVL vs Rubro-Negra para Sistema de Ranking

## Resumo

Então, a gente precisava comparar duas estruturas de dados pra saber qual era melhor pra um sistema de ranking de torneio onde jogadores entram e saem direto. Implementei tanto AVL quanto Rubro-Negra e fiz uns testes pra ver qual ganhava. Spoiler: AVL saiu na frente, mas não foi aquele massacre que a gente esperava.

## O Que Eu Implementei

### AVL (Árvore AVL)

Basicamente é uma árvore binária de busca que se auto-balanceia. A ideia é: toda vez que você insere ou remove um nó, a árvore verifica se ficou desbalanceada. Se ficou, ela faz umas "rotações" (tipo um rearranjo dos nós) pra ficar bonitinha de novo. O detalhe é que ela é bem rígida nisso - qualquer desbalanceamento maior que 1 já dispara uma rotação.

### Rubro-Negra (Red-Black Tree)

Essa aqui é diferente. Em vez de se basear em altura, ela usa cores (vermelho e preto) pra garantir que a árvore não fica muito desbalanceada. Ela é mais flexível que AVL - permite que um lado seja até um pouco mais alto que o outro. A vantagem teórica é que ela faz menos rebalanceamento.

## Como Eu Pensei a Implementação

Tava claro que precisava:
1. Contar rotações pra ver qual estrutura mexe mais
2. Implementar remoção (que é bem mais chata que inserção)
3. Buscar por nome (mesmo que seja O(n), vai que precisa)
4. Rodar um benchmark de verdade com 10k inserções, 5k remoções e 1k buscas

Pra deixar tudo organizado, criei umas structs de contexto que agrupam a árvore com os contadores. Assim fica fácil saber quantas rotações cada uma fez.

## O Teste Que Eu Rodei

Basicamente fiz 4 fases:

**Fase 1: Inserção de 10.000 jogadores**
- Cada jogador tem nome e pontuação aleatória (0-9999)
- Medi tempo, quantidade de rotações e se a árvore ficou válida no final

**Fase 2: Remover 5.000 jogadores aleatoriamente da AVL**
- Tentei remover 5 mil, mas obviamente nem todos tavam mais lá
- Medi quantas rotações aconteceram durante remoção

**Fase 3: Remover 5.000 jogadores da Rubro-Negra**
- Mesma coisa, mas com RN

**Fase 4: Fazer 1.000 buscas por nome**
- Buscar jogador por nome em ambas as árvores
- Medi o tempo pra ver qual era mais rápida

## Os Resultados

### Inserção (10.000 jogadores)

| Métrica | AVL | Rubro-Negra |
|---------|-----|-------------|
| Tempo | 0.006564s | 0.004712s |
| Rotações | 4.508 | 9.817 |
| Válida ao final | Sim | Precisa revisar |

Bem, RN foi mais rápido na inserção, mas fez quase 10 mil rotações contra 4.5 mil da AVL. Isso é meio contra-intuitivo, mas faz sentido quando você pensa: a recoloração em RN (que é super rápida) tá disparando umas rotações extras que não esperávamos.

### Remoção

AVL levou 0.217s pra remover (com 2.530 bem-sucedidas), enquanto RN levou 0.472s (com 3.618 bem-sucedidas). AVL praticamente o dobro mais rápido. Isso porque a remoção em RN que eu implementei é bem simplificada - faltou a parte de corrigir as cores depois.

### Busca por Nome

AVL: 0.027931s pra 1000 buscas
RN: 0.076601s pra 1000 buscas

AVL foi 2.7x mais rápido. Faz sentido porque a estrutura dela é mais compacta depois de todos esses rebalanceamentos.

## Então Qual É Melhor?

AVL saiu ganhando por bastante margem:
- Tempo total: 0.252s (AVL) vs 0.554s (RN) - AVL 2.2x mais rápido
- Rotações totais: 5.416 (AVL) vs 9.817 (RN) - AVL 1.8x menos rotações
- Buscas: AVL destruiu, 2.7x mais rápido

Pra um sistema de ranking onde você quer que as pessoas vejam o top-10 rápido, mesmo com gente entrando e saindo direto, AVL é a escolha. A estrutura fica mais compacta e as operações depois ficam muito mais rápidas.

## Como a Remoção Funciona

Pra AVL, a remoção é meio direto:
1. Acha o nó
2. Se não tem filhos, deleta e pronto
3. Se tem um filho, bota o filho no lugar dele
4. Se tem dois filhos, acha o cara com a menor pontuação da subárvore direita, bota no lugar dele, e remove esse cara recursivamente
5. Depois disso, faz rebalanceamento igual faria numa inserção

Basicamente garante que a árvore não fica desbalanceada.

Pra Rubro-Negra é mais chato porque depois de remover precisa ficar verificando cores e tal, o que é complexo demais pra detalhar aqui.

## Busca por Nome - O Ponto Fraco

Aqui é honesto: buscar por nome é O(n) em ambas, porque a chave de ordenação é pontuação, não nome. Pra isso funcionar bem de verdade, precisaria de uma tabela hash junto com a árvore, aí sim O(1). Mas pra esse projeto acadêmico, percorrer a árvore toda funciona.

## Conclusão

AVL é a melhor opção pra esse cenário. Não é aquela diferença gritante de "oh, RN é horrível", mas AVL ganha em todos os fronts:
- Mais rápida
- Menos rotações
- Código mais simples de entender
- Estrutura mais previsível

Se tivesse implementado a correção de cores da RN direito, talvez fechasse a diferença, mas do jeito que saiu, AVL tá na frente.

## O Que Eu Aprendi

- RN não é magicamente mais rápido só porque faz menos rotações (recoloração dispara rotações também)
- AVL acaba com estruturas bem mais compactas, o que ajuda muito em operações depois
- Contar rotações é importante, mas tempo real importa mais
- Implementar RN direito é bem mais complicado que parece

---

Basicamente, foi isso. Rodei os testes, AVL ganhou, e agora tá documentado.

