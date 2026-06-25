#ifndef MAIN_H
#define MAIN_H

#include "trie.h"
#include "grafo.h"

// Estrutura dinâmica (Lista Ligada) para os utilizadores/administradores
typedef struct Usuario {
    char username[50];
    char password[50];
    struct Usuario *proximo;
} Usuario;

// Estrutura para associar dinamicamente cada vértice do Grafo (Mercado) à sua própria Trie
typedef struct CatalogoMercado {
    int id_vertice;
    trie *raiz_produtos;
    struct CatalogoMercado *proximo;
} CatalogoMercado;

// Protótipos das funções auxiliares de gestão
void cadastrar_usuario(Usuario **lista, const char *user, const char *pass);
bool fazer_login(Usuario *lista, const char *user, const char *pass);
void associar_trie_ao_mercado(CatalogoMercado **lista, int id_vertice);
trie* obter_trie_do_mercado(CatalogoMercado *lista, int id_vertice);
void libertar_catalogos(CatalogoMercado *lista);

#endif
