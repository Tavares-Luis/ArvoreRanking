
# Relatório: AVL vs Rubro-Negra para Sistema de Ranking

Pequeno aviso: Caso der algum BO para rodar pode mandar msg no meu email : luis.atc30@unochapeco.edu.br que eu vejo para ti. Eu e a ste tava usando vscode e cmake ae tem um pouco de cada misturado.

## Resumo

Então, a gente precisava comparar duas estruturas de dados pra saber qual era melhor pra um sistema de ranking de torneio onde jogadores entram e saem direto. Implementemos tanto AVL quanto Rubro-Negra e fizemos uns testes pra ver qual ganhava. Como não tinhamos entendido muito bem ainda o conteúdo não tinha ideia de qual ia ser mais rápida até implementar.

## Implementação

### AVL

Basicamente é uma árvore binária de busca que se auto-balanceia. A ideia é: toda vez que você insere ou remove um nó, a árvore verifica se ficou desbalanceada. Se ficou, ela faz umas "ajeita" com umas rotações pra ficar seguindo as negras novamente. (OU seja arruma quando o desbalanceamento é maior que 1 )

### Rubro-Negr

 Em vez de se basear em altura, ela usa cores (vermelho e preto) pra garantir que a árvore não fica muito desbalanceada. Ela é "mais flexível" que AVL, pois permite uma diferença de altura maior, então por consequencia faz mnos balanceamentos, por isso pensemos que ia sair na vantagem enquanto pensavamos no problema.

Seguindo a tarefa, tinhamos que:
1. Contar rotações pra ver qual estrutura mexe mais
2. Implementar remoção (que é bem mais chata que inserção)
3. Buscar por nome
4. Rodar um benchmark de verdade com 10k inserções, 5k remoções e 1k buscas (gerado com IA)

Pra deixar tudo organizado, criei umas structs de contexto que agrupam a árvore com os contadores. Assim fica fácil saber quantas rotações cada uma fez.

## O Teste Que Eu Rodei (Aqui precisei de ajuda para configurar certinho com IA pq tava quebrando a cuca não tava funcionando nada)

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

### Inserção de 10k jogadores

| Métrica | AVL | Rubro-Negra |
|---------|-----|-------------|
| Tempo | 0.006564s | 0.004712s |
| Rotações | 4.508 | 9.817 |
| Válida ao final | Sim | Precisa revisar |

Bem, RN foi mais rápido na inserção, mas fez quase 10 mil rotações contra 4.5 mil da AVL. Isso é meio contra-intuitivo, mas faz sentido quando você pensa: a recoloração em RN (que é super rápida) tá disparando umas rotações extras que não esperávamos. Como um efeito em Domino sabe ?

### Remoção

AVL levou 0.217s pra remover (com 2.530 bem-sucedidas), enquanto RN levou 0.472s (com 3.618 bem-sucedidas). AVL praticamente o dobro mais rápido. Isso porque a remoção em RN que eu implementei é bem simplificada - faltou a parte de corrigir as cores depois.

### Busca por Nome

AVL: 0.027931s pra 1000 buscas
RN: 0.076601s pra 1000 buscas

AVL foi mais rápido.

## Então Qual É Melhor?
*COmparando apenas tempo*

AVL saiu ganhando:
- Tempo total: 0.252s (AVL) vs 0.554s (RN) - AVL 2.2x mais rápido
- Rotações totais: 5.416 (AVL) vs 9.817 (RN) - AVL 1.8x menos rotações
- Buscas: AVL, 2.7x mais rápido

## Conclusão

AVL é a melhor opção pra esse cenário. Não é aquela diferença gritante de "oh, RN é horrível",acredito que cada uma deve ter seu ponto forte, no caso da avl nesse exemplo :
- Mais rápida
- Menos rotações
- Código mais simples de entender
- Estrutura mais previsível

Se tivesse implementado a correção de cores da RN direito, talvez fechasse a diferença, mas do jeito que saiu, AVL tá na frente.

## O Que Eu Aprendi

- RN não é mais rápido só porque faz menos rotações (recoloração dispara rotações também)
- AVL tem estruturas bem mais compactas, o que ajuda muito em operações depois
- Implementar RN direito é bem mais complicado que parece

---

Basicamente, foi isso. Rodei os testes, AVL ganhou, e agora tá documentado.

