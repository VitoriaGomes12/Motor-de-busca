#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "trie.h"

void carregarProdutos(trie *raiz) {
    FILE *fp;
    char palavras[100];

    fp = fopen("dataset.txt", "r");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro dataset.txt\n");
        return;
    }

    while (fgets(palavras, sizeof(palavras), fp) != NULL) {
        palavras[strcspn(palavras, "\n\r")] = '\0';
        if (strlen(palavras) > 0)
            inserir(raiz, palavras);
    }

    fclose(fp);
}

void normalizar(char palavra[]) {
    char resultado[200];
    int i = 0, j = 0;
    int tam = strlen(palavra);

    while (i < tam) {
        unsigned char c = (unsigned char)palavra[i];

        if (c < 128) {
            resultado[j++] = tolower(c);
            i++;
        } else if (c == 0xC3 && i + 1 < tam) {
            unsigned char c2 = (unsigned char)palavra[i + 1];
            switch (c2) {
                case 0xA0: case 0xA1: case 0xA2: case 0xA3:
                case 0x80: case 0x81: case 0x82: case 0x83:
                    resultado[j++] = 'a'; break;
                case 0xA7: case 0x87:
                    resultado[j++] = 'c'; break;
                case 0xA8: case 0xA9: case 0xAA:
                case 0x88: case 0x89: case 0x8A:
                    resultado[j++] = 'e'; break;
                case 0xAC: case 0xAD:
                case 0x8C: case 0x8D:
                    resultado[j++] = 'i'; break;
                case 0xB2: case 0xB3: case 0xB4: case 0xB5:
                case 0x92: case 0x93: case 0x94: case 0x95:
                    resultado[j++] = 'o'; break;
                case 0xB9: case 0xBA:
                case 0x99: case 0x9A:
                    resultado[j++] = 'u'; break;
                default:
                    resultado[j++] = '?'; break;
            }
            i += 2;
        } else {
            i++;
        }
    }
    resultado[j] = '\0';
    strcpy(palavra, resultado);
}

trie *criarNo() {
    int i;
    trie *novono = (trie*)malloc(sizeof(trie));
    if (novono == NULL) {
        printf("ERRO DE ALOCAÇÃO\n");
        return NULL;
    }
    for (i = 0; i < MAX; i++)
        novono->filhos[i] = NULL;
    novono->fim = false;
    return novono;
}

void inserir(trie *raiz, char palavra[]) {
    int i, indice;
    normalizar(palavra);
    int tam = strlen(palavra);
    trie *root = raiz;
    for (i = 0; i < tam; i++) {
        indice = (unsigned char)palavra[i];
        if (root->filhos[indice] == NULL)
            root->filhos[indice] = criarNo();
        root = root->filhos[indice];
    }
    root->fim = true;
}

bool pesquisar(trie *raiz, char palavra[]) {
    int i, indice;
    normalizar(palavra);
    if (raiz == NULL) return false;
    int tam = strlen(palavra);
    trie *root = raiz;
    for (i = 0; i < tam; i++) {
        indice = (unsigned char)palavra[i];
        if (root->filhos[indice] == NULL) return false;
        root = root->filhos[indice];
    }
    return root->fim;
}

void listar(trie *raiz, char palavra[], int posicao) {
    int i;
    if (raiz == NULL) return;
    if (raiz->fim == true) {
        palavra[posicao] = '\0';
        printf("  %s\n", palavra);
    }
    for (i = 0; i < MAX; i++) {
        if (raiz->filhos[i] != NULL) {
            palavra[posicao] = (char)i;
            listar(raiz->filhos[i], palavra, posicao + 1);
        }
    }
}

void autocomplete(trie *raiz, char prefixo[]) {
    int i, indice;
    normalizar(prefixo);
    if (raiz == NULL) {
        printf("RAIZ VAZIA\n");
        return;
    }
    int tam = strlen(prefixo);
    trie *root = raiz;
    for (i = 0; i < tam; i++) {
        indice = (unsigned char)prefixo[i];
        if (root->filhos[indice] == NULL) {
            printf("  Nenhum produto com esse prefixo.\n");
            return;
        }
        root = root->filhos[indice];
    }
    listar(root, prefixo, tam);
}

bool remover(trie *raiz, char palavra[], int posicao) {
    int i;
    if (raiz == NULL) return false;

    if (posicao < (int)strlen(palavra)) {
        int indice = (unsigned char)palavra[posicao];
        bool inutil = remover(raiz->filhos[indice], palavra, posicao + 1);
        if (inutil) {
            free(raiz->filhos[indice]);
            raiz->filhos[indice] = NULL;
        }
    } else {
        if (raiz->fim == true)
            raiz->fim = false;
    }

    for (i = 0; i < MAX; i++) {
        if (raiz->filhos[i] != NULL)
            return false;
    }
    return raiz->fim == false;
}

void editar(trie *raiz, char antiga[], char nova[]) {
    char temp[100];
    strncpy(temp, antiga, 99);
    normalizar(temp);

    if (!pesquisar(raiz, temp)) {
        printf("  Produto '%s' não existe.\n", antiga);
        return;
    }
    remover(raiz, temp, 0);
    inserir(raiz, nova);
    printf("  Produto actualizado para '%s'.\n", nova);
}
