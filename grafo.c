#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

/* --- Fila para BFS --- */
typedef struct {
    int dados[MAX_VERTICES];
    int frente, tras;
} Fila;

static void fila_init(Fila* f)             { f->frente = f->tras = 0; }
static int  fila_vazia(Fila* f)            { return f->frente == f->tras; }
static void fila_enfileirar(Fila* f, int v){ f->dados[f->tras++] = v; }
static int  fila_desenfileirar(Fila* f)    { return f->dados[f->frente++]; }

void grafo_init(Grafo* g) {
    g->num_vertices = 0;
}

int grafo_adicionar_vertice(Grafo* g, const char* nome, int eh_utilizador) {
    if (g->num_vertices >= MAX_VERTICES) {
        printf("[ERRO] Grafo cheio!\n");
        return -1;
    }
    int idx = g->num_vertices++;
    strncpy(g->vertices[idx].nome, nome, MAX_NOME - 1);
    g->vertices[idx].nome[MAX_NOME - 1] = '\0';
    g->vertices[idx].eh_utilizador = eh_utilizador;
    g->vertices[idx].lista_adj = NULL;
    return idx;
}

void grafo_adicionar_aresta(Grafo* g, int origem, int destino, float distancia) {
    Aresta *a1, *a2;
    if (origem < 0 || origem >= g->num_vertices ||
        destino < 0 || destino >= g->num_vertices) {
        printf("[ERRO] Indice inválido.\n");
        return;
    }
    a1 = (Aresta*)malloc(sizeof(Aresta));
    a1->destino = destino; a1->distancia = distancia;
    a1->prox = g->vertices[origem].lista_adj;
    g->vertices[origem].lista_adj = a1;

    a2 = (Aresta*)malloc(sizeof(Aresta));
    a2->destino = origem; a2->distancia = distancia;
    a2->prox = g->vertices[destino].lista_adj;
    g->vertices[destino].lista_adj = a2;
}

int grafo_indice_por_nome(Grafo* g, const char* nome) {
    int i;
    if (g->num_vertices == 0) return -1;
    for (i = 0; i < g->num_vertices; i++)
        if (strcmp(g->vertices[i].nome, nome) == 0)
            return i;
    return -1;
}

int grafo_bfs_mercado_proximo(Grafo* g, int origem, int* tem_produto) {
    int visitado[MAX_VERTICES] = {0};
    Aresta* a;
    Fila f;
    fila_init(&f);
    visitado[origem] = 1;
    fila_enfileirar(&f, origem);

    printf("\n[BFS] Buscando a partir de: %s\n", g->vertices[origem].nome);

    while (!fila_vazia(&f)) {
        int v = fila_desenfileirar(&f);
        if (!g->vertices[v].eh_utilizador && tem_produto[v]) {
            printf("[BFS] Mercado mais próximo com o produto: %s\n",
                   g->vertices[v].nome);
            return v;
        }
        for (a = g->vertices[v].lista_adj; a != NULL; a = a->prox) {
            if (!visitado[a->destino]) {
                visitado[a->destino] = 1;
                fila_enfileirar(&f, a->destino);
            }
        }
    }
    printf("[BFS] Nenhum mercado próximo possui esse produto.\n");
    return -1;
}

static void dfs_aux(Grafo* g, int v, int* visitado) {
    Aresta* a;
    visitado[v] = 1;
    printf("  -> %s\n", g->vertices[v].nome);
    for (a = g->vertices[v].lista_adj; a != NULL; a = a->prox)
        if (!visitado[a->destino])
            dfs_aux(g, a->destino, visitado);
}

void grafo_dfs(Grafo* g, int inicio) {
    int visitado[MAX_VERTICES] = {0};
    printf("\n[DFS] Explorando rede a partir de: %s\n", g->vertices[inicio].nome);
    dfs_aux(g, inicio, visitado);
    printf("[DFS] Exploração concluída.\n");
}

void grafo_mostrar(Grafo* g) {
    int i;
    Aresta* a;
    if (g->num_vertices == 0) {
        printf("\n=== [!] Nenhum mercado registrado ainda! ===\n");
        return;
    }
    printf("\n=== Rede de Mercados ===\n");
    for (i = 0; i < g->num_vertices; i++) {
        printf("[%d] %s", i, g->vertices[i].nome);
        if (g->vertices[i].eh_utilizador) printf(" (UTILIZADOR)");
        for (a = g->vertices[i].lista_adj; a != NULL; a = a->prox)
            printf("\n      --(%.1f km)--> %s", a->distancia,
                   g->vertices[a->destino].nome);
        printf("\n");
    }
    printf("========================\n");
}

void grafo_destruir(Grafo* g) {
    int i;
    for (i = 0; i < g->num_vertices; i++) {
        Aresta* atual = g->vertices[i].lista_adj;
        while (atual) {
            Aresta* prox = atual->prox;
            free(atual);
            atual = prox;
        }
        g->vertices[i].lista_adj = NULL;
    }
    g->num_vertices = 0;
}
