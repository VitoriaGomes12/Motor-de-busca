#ifndef GRAFO_H
#define GRAFO_H

#define MAX_VERTICES 100
#define MAX_NOME     50

/* Aresta da lista de adjacência */
typedef struct Aresta {
    int    destino;
    float  distancia;
    struct Aresta* prox;
} Aresta;

/* Vertice: mercado ou utilizador */
typedef struct {
    char    nome[MAX_NOME];
    int     eh_utilizador;
    Aresta* lista_adj;
} Vertice;

/* Grafo principal */
typedef struct {
    Vertice vertices[MAX_VERTICES];
    int     num_vertices;
} Grafo;

void grafo_init(Grafo* g);
int  grafo_adicionar_vertice(Grafo* g, const char* nome, int eh_utilizador);
void grafo_adicionar_aresta(Grafo* g, int origem, int destino, float distancia);
int  grafo_indice_por_nome(Grafo* g, const char* nome);
int  grafo_bfs_mercado_proximo(Grafo* g, int origem, int* tem_produto);
void grafo_dfs(Grafo* g, int inicio);
void grafo_mostrar(Grafo* g);
void grafo_destruir(Grafo* g);

#endif
