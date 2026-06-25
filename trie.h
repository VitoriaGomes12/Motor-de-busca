#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#define MAX 128

typedef struct Notrie {
    bool fim;
    struct Notrie *filhos[MAX];
} trie;

/* Carrega produtos do dataset.txt para a Trie */
void carregarProdutos(trie *raiz);

/* Normaliza: remove acentos e converte para minusculas */
void normalizar(char palavra[]);

/* Cria um novo no da Trie */
trie *criarNo();

/* Insere uma palavra na Trie */
void inserir(trie *raiz, char palavra[]);

/* Pesquisa uma palavra exacta na Trie. Devolve true se existir */
bool pesquisar(trie *raiz, char palavra[]);

/* Lista todos os produtos armazenados na Trie */
void listar(trie *raiz, char palavra[], int posicao);

/* Autocomplete: lista todos os produtos com o prefixo dado */
void autocomplete(trie *raiz, char prefixo[]);

/* Remove uma palavra da Trie */
bool remover(trie *raiz, char palavra[], int posicao);

/* Edita (remove antiga e insere nova) */
void editar(trie *raiz, char antiga[], char nova[]);

#endif
