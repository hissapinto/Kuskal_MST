#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 256

typedef struct {
      char origem;
      char destino;
      int peso;
} Aresta;

// Vetor que armazena o "pai" de cada vértice.
// Se pai[x] = x, significa que x é o representante (raiz) do seu conjunto.
// Caso contrário, seguimos pai[x] para encontrar a raiz.
int pai[MAX_VERTICES];

// Inicializa a estrutura Union-Find.
// Cada vértice começa sendo pai de si mesmo.
// Isso significa que no início todos os elementos estão em conjuntos separados.
void inicializar_union_find()
{
    for (int i = 0; i < MAX_VERTICES; i++) pai[i] = i;
}

// Busca o representante (raiz) do conjunto ao qual o vértice i pertence.
// Enquanto pai[i] != i significa que ainda não chegamos na raiz.
// Ao final, retornamos o "líder" desse conjunto.
int buscar_raiz(int i)
{
      if (pai[i] != i) pai[i] = buscar_raiz(pai[i]);
      return pai[i];
}

// Une dois conjuntos diferentes.
// Primeiro encontra a raiz de cada conjunto.
// Se forem diferentes, fazemos um apontar para o outro,
// efetivamente unindo os conjuntos.
// Isso garante que futuras chamadas reconhecerão que pertencem ao mesmo grupo.
void unir_conjuntos(int a, int b)
{
      int raiz_a = buscar_raiz(a);
      int raiz_b = buscar_raiz(b);

      // Só une se forem conjuntos diferentes
      if (raiz_a != raiz_b) pai[raiz_b] = raiz_a;
}

// Função usada pelo qsort para ordenar arestas por peso (valor).
// Retorna < 0 se a < b
// Retorna > 0 se a > b
// Retorna 0 se forem iguais
// Isso permite que o Kruskal processe primeiro as arestas menores.
int comparar_arestas(const void *a, const void *b)
{
      Aresta *aresta_a = (Aresta *)a;
      Aresta *aresta_b = (Aresta *)b;

      // Subtração funciona porque:
      // resultado < 0 -> A vem antes de B
      // resultado = 0 -> mesma ordem
      // resultado > 0 -> A vem depois de B
      return (aresta_a->peso - aresta_b->peso);
}

int conta_linhas(FILE *arquivo)
{
      int linhas = 0;
      char buffer[100];

      while (fgets(buffer, sizeof(buffer), arquivo) != NULL) linhas++;

      rewind(arquivo);
      return linhas;
}

void imprime_arestas(const Aresta *vetor, int tam)
{
      for (int i = 0; i < tam; i++)
      {
            printf("Origem: %c | Destino: %c | Peso: %d\n", vetor[i].origem, vetor[i].destino, vetor[i].peso);
      }
}

Aresta* kruskal(Aresta* arestas, int qtd_arestas, int* qtd_resultado)
{
      Aresta* arvore_minima = malloc(qtd_arestas * sizeof(Aresta));
      *qtd_resultado = 0;

      inicializar_union_find();

      for (int i = 0; i < qtd_arestas; i++)
      {
            // Como os vértices são chars, convertemos para int (0–255)
            int u = (unsigned char)arestas[i].origem;
            int v = (unsigned char)arestas[i].destino;

            // Descobrimos as raízes dos dois vértices
            int raiz_u = buscar_raiz(u);
            int raiz_v = buscar_raiz(v);

            // Se as raízes são diferentes, NÃO forma ciclo -> aceita aresta
            if (raiz_u != raiz_v)
            {
                  arvore_minima[*qtd_resultado] = arestas[i];
                  (*qtd_resultado)++;

                  // Une os conjuntos dos vértices
                  unir_conjuntos(raiz_u, raiz_v);
            }
      }

      // Ajusta tamanho real do vetor
      arvore_minima = realloc(arvore_minima, (*qtd_resultado) * sizeof(Aresta));
      return arvore_minima;
}

int main()
{
      FILE *arquivo = fopen("grafos.txt", "r");
      if (arquivo == NULL)
      {
            printf("Erro na leitura do arquivo.\n");
            return -1;
      }

      int qtd_linhas = conta_linhas(arquivo);
      Aresta *arestas = malloc(qtd_linhas * sizeof(Aresta));

      if (arestas == NULL)
      {
            printf("Erro de memoria.\n");
            fclose(arquivo);
            return -1;
      }

      int i = 0;
      while (fscanf(arquivo, " %c %c %d",
                        &arestas[i].origem,
                        &arestas[i].destino,
                        &arestas[i].peso) == 3) {
            i++;
      }
      fclose(arquivo);

      // Ordena por peso
      qsort(arestas, qtd_linhas, sizeof(Aresta), comparar_arestas);

      printf("--- Arestas Ordenadas ---\n");
      imprime_arestas(arestas, qtd_linhas);

      int qtd_resultado = 0;
      Aresta *resultado = kruskal(arestas, qtd_linhas, &qtd_resultado);

      printf("\n--- Arvore Geradora Minima (Kruskal) ---\n");
      imprime_arestas(resultado, qtd_resultado);

      // Soma dos pesos
      int custo_total = 0;
      for (int k = 0; k < qtd_resultado; k++) custo_total += resultado[k].peso;

      printf("\nCusto Total: %d\n", custo_total);

      free(arestas);
      free(resultado);

      return 0;
}
