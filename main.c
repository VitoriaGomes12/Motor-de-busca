#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

// Inicializa a lista de administradores e adiciona um padrão
void cadastrar_usuario(Usuario **lista, const char *user, const char *pass) {
    Usuario *novo = (Usuario*)malloc(sizeof(Usuario));
    if (novo == NULL) return;
    strcpy(novo->username, user);
    strcpy(novo->password, pass);
    novo->proximo = *lista;
    *lista = novo;
}

// Validação de login simples na lista ligada
bool fazer_login(Usuario *lista, const char *user, const char *pass) {
    Usuario *atual = lista;
    while (atual != NULL) {
        if (strcmp(atual->username, user) == 0 && strcmp(atual->password, pass) == 0) {
            return true;
        }
        atual = atual->proximo;
    }
    return false;
}

// Cria e associa uma árvore Trie dinamicamente a um vértice de Mercado
void associar_trie_ao_mercado(CatalogoMercado **lista, int id_vertice) {
    CatalogoMercado *novo = (CatalogoMercado*)malloc(sizeof(CatalogoMercado));
    if (novo == NULL) return;
    novo->id_vertice = id_vertice;
    novo->raiz_produtos = criarNo(); // Usa a função do teu colega no trie.c
    novo->proximo = *lista;
    *lista = novo;
}

// Procura e devolve a árvore Trie correspondente ao ID do mercado
trie* obter_trie_do_mercado(CatalogoMercado *lista, int id_vertice) {
    CatalogoMercado *atual = lista;
    while (atual != NULL) {
        if (atual->id_vertice == id_vertice) {
            return atual->raiz_produtos;
        }
        atual = atual->proximo;
    }
    return NULL;
}

// Limpa a memória das tries alocadas dinamicamente
void libertar_catalogos(CatalogoMercado *lista) {
    CatalogoMercado *atual = lista;
    while (atual != NULL) {
        CatalogoMercado *aux = atual->proximo;
        // Se tivesses uma função de destruir_trie, chamava-se aqui.
        free(atual);
        atual = aux;
    }
}

void escrever_trie(FILE *f, trie *no, char *buf, int pos)
{
    int i;
    if (no == NULL) return;
    if (no->fim) {
        buf[pos] = '\0';
        fprintf(f, "%s\n", buf);
    }
    for (i = 0; i < MAX; i++) {
        if (no->filhos[i] != NULL) {
            buf[pos] = (char)i;
            escrever_trie(f, no->filhos[i], buf, pos + 1);
        }
    }
}

void escolher_ficheiro_windows(char *caminho_out, int tamanho)
{
    /* Usa o comando PowerShell para abrir o explorador de ficheiros */
    FILE *pipe = popen(
        "powershell -Command \""
        "Add-Type -AssemblyName System.Windows.Forms; "
        "$f = New-Object System.Windows.Forms.OpenFileDialog; "
        "$f.Filter = 'Ficheiros de texto (*.txt)|*.txt|Todos (*.*)|*.*'; "
        "if ($f.ShowDialog() -eq 'OK') { Write-Output $f.FileName }\"",
        "r");

    caminho_out[0] = '\0';
    if (pipe == NULL) {
        printf("[!] Erro ao abrir dialogo de ficheiro.\n");
        return;
    }
    if (fgets(caminho_out, tamanho, pipe) != NULL)
        caminho_out[strcspn(caminho_out, "\r\n")] = '\0';
    pclose(pipe);
}


// Interface do Administrador
void menuAdmin(Grafo *g, CatalogoMercado **catalogos, Usuario **usuarios) {
    int opcao, v_origem, v_destino;
    float dist;
    char nome[MAX_NOME], produto[100], user[50], pass[50];
    trie *raiz_mercado;

    do {
        printf("\n=================================");
        printf("\n     PAINEL ADMINISTRATIVO       ");
        printf("\n=================================");
        printf("\n1. Adicionar Mercado (Vertice)");
        printf("\n2. Conectar Mercados (Aresta)");
        printf("\n3. Inserir Produto num Mercado");
        printf("\n4. Remover Produto de um Mercado");
        printf("\n5. Mostrar Rede de Mercados");
       printf("\n6. Cadastrar Novo Administrador");
    printf("\n7. Adicionar Localizacao para Clientes");
    printf("\n8. Carregar Produtos de Ficheiro para um Mercado");
    printf("\n9. Listar Produtos de um Mercado (consola)");
    printf("\n10. Exportar Produtos de um Mercado para Ficheiro");
    printf("\n0. Terminar Sessao");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer

        switch(opcao) {
            case 1:
                printf("\nIntroduza o nome do Mercado/Localizacao: ");
                fgets(nome, sizeof(nome), stdin); nome[strcspn(nome, "\n")] = '\0';
                int idx = grafo_adicionar_vertice(g, nome, 0); // 0 = Mercado/Local
                if (idx != -1) {
                    associar_trie_ao_mercado(catalogos, idx);
                    printf("-> Mercado '%s' adicionado com sucesso (ID: %d)!\n", nome, idx);
                }
                break;

            case 2:
                printf("\nIntroduza o ID do Mercado de Origem: ");
                scanf("%d", &v_origem);
                printf("Introduza o ID do Mercado de Destino: ");
                scanf("%d", &v_destino);
                printf("Introduza a distancia (em km): ");
                scanf("%f", &dist);
                grafo_adicionar_aresta(g, v_origem, v_destino, dist);
                printf("-> Conexao estabelecida!\n");
                break;

            case 3:
                printf("\nIntroduza o ID do Mercado: ");
                scanf("%d", &v_origem);
                getchar();
                raiz_mercado = obter_trie_do_mercado(*catalogos, v_origem);
                if (raiz_mercado != NULL) {
                    printf("Nome do produto: ");
                    fgets(produto, sizeof(produto), stdin); produto[strcspn(produto, "\n")] = '\0';
                    inserir(raiz_mercado, produto);
                    printf("-> Produto inserido na Trie deste mercado.\n");
                } else {
                    printf("[!] Mercado nao encontrado ou ID invalido.\n");
                }
                break;

            case 4:
                printf("\nIntroduza o ID do Mercado: ");
                scanf("%d", &v_origem);
                getchar();
                raiz_mercado = obter_trie_do_mercado(*catalogos, v_origem);
                if (raiz_mercado != NULL) {
                    printf("Nome do produto a remover: ");
                    fgets(produto, sizeof(produto), stdin); produto[strcspn(produto, "\n")] = '\0';
                    if (pesquisar(raiz_mercado, produto)) {
                        remover(raiz_mercado, produto, 0);
                        printf("-> Produto removido.\n");
                    } else {
                        printf("[!] Produto nao existe neste mercado.\n");
                    }
                } else {
                    printf("[!] Mercado nao encontrado.\n");
                }
                break;

            case 5:
                grafo_mostrar(g);
                break;

            case 6:
                printf("\nNovo Username: ");
                fgets(user, sizeof(user), stdin); user[strcspn(user, "\n")] = '\0';
                printf("Nova Password: ");
                fgets(pass, sizeof(pass), stdin); pass[strcspn(pass, "\n")] = '\0';
                cadastrar_usuario(usuarios, user, pass);
                printf("-> Novo administrador cadastrado com sucesso!\n");
                break;

				case 7:
            printf("\nNome da localizacao a adicionar: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = '\0';
            {
                FILE *fl = fopen("localizacoes.txt", "a");
                if (fl != NULL) {
                    fprintf(fl, "%s\n", nome);
                    fclose(fl);
                    printf("-> Localizacao '%s' adicionada!\n", nome);
                } else {
                    printf("[!] Erro ao abrir localizacoes.txt.\n");
                }
            }
            break;

        case 8:
        {
            printf("\nIntroduza o ID do Mercado: ");
            scanf("%d", &v_origem);
            getchar();
            raiz_mercado = obter_trie_do_mercado(*catalogos, v_origem);
            if (raiz_mercado == NULL) {
                printf("[!] Mercado nao encontrado.\n");
                break;
            }
            char caminho[512];
            printf("\nA abrir explorador de ficheiros...\n");
            escolher_ficheiro_windows(caminho, sizeof(caminho));

            if (strlen(caminho) == 0) {
                printf("[!] Nenhum ficheiro seleccionado.\n");
                break;
            }
            printf("Ficheiro seleccionado: %s\n", caminho);

            FILE *fp = fopen(caminho, "r");
            if (fp == NULL) {
                printf("[!] Nao foi possivel abrir o ficheiro.\n");
                break;
            }
            char linha[100];
            int contador = 0;
            while (fgets(linha, sizeof(linha), fp) != NULL) {
                linha[strcspn(linha, "\n")] = '\0';
                if (strlen(linha) > 0) {
                    inserir(raiz_mercado, linha);
                    contador++;
                }
            }
            fclose(fp);
            printf("-> %d produtos carregados com sucesso!\n", contador);
            break;
        }

        case 9:
        {
            printf("\nIntroduza o ID do Mercado: ");
            scanf("%d", &v_origem);
            getchar();
            raiz_mercado = obter_trie_do_mercado(*catalogos, v_origem);
            if (raiz_mercado == NULL) {
                printf("[!] Mercado nao encontrado.\n");
                break;
            }
            char palavra[100] = "";
            printf("\n--- Produtos do Mercado %d (%s) ---\n",
                v_origem, g->vertices[v_origem].nome);
            listar(raiz_mercado, palavra, 0);
            printf("-----------------------------------\n");
            break;
        }

        case 10:
        {
            printf("\nIntroduza o ID do Mercado: ");
            scanf("%d", &v_origem);
            getchar();
            raiz_mercado = obter_trie_do_mercado(*catalogos, v_origem);
            if (raiz_mercado == NULL) {
                printf("[!] Mercado nao encontrado.\n");
                break;
            }
            char caminho[512];
            printf("\nA abrir explorador para guardar ficheiro...\n");
            escolher_ficheiro_windows(caminho, sizeof(caminho));

            if (strlen(caminho) == 0) {
                /* Se nao escolheu, usa nome padrao */
                sprintf(caminho, "produtos_mercado_%d.txt", v_origem);
            }
            FILE *fout = fopen(caminho, "w");
            if (fout == NULL) {
                printf("[!] Erro ao criar ficheiro.\n");
                break;
            }
            char buf[100] = "";
            escrever_trie(fout, raiz_mercado, buf, 0);
            fclose(fout);
            printf("-> Lista exportada para '%s'!\n", caminho);
            break;
        }

            case 0:
                printf("\nA terminar sessao...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);
}

// Interface do Cliente
void menuCliente(Grafo *g, CatalogoMercado *catalogos) {
    int opcao, i, localOp, idx_cliente = -1;
    char prefixo[100], nome_local[50];
    
    // Lista de localizacoes guardadas pre-definidas para selecao rapida
   char bairros[50][50];
int num_bairros = 0;

FILE *fl = fopen("localizacoes.txt", "r");
if (fl != NULL) {
    while (num_bairros < 50 && fgets(bairros[num_bairros], 50, fl) != NULL) {
        bairros[num_bairros][strcspn(bairros[num_bairros], "\n")] = '\0';
        if (strlen(bairros[num_bairros]) > 0)
            num_bairros++;
    }
    fclose(fl);
} else {
    printf("[!] Ficheiro localizacoes.txt nao encontrado.\n");
    return;
}
    
    printf("\n=================================");
    printf("\n        PORTAL DO CLIENTE        ");
    printf("\n=================================");
    printf("\nSelecione a sua localizacao atual:\n");
    
    for (i = 0; i < num_bairros; i++) {
        printf("%d. %s\n", i + 1, bairros[i]);
    }
    printf("Escolha uma opcao: ");
    scanf("%d", &localOp);
    getchar();

    if (localOp >= 1 && localOp <= 4) {
        strcpy(nome_local, bairros[localOp - 1]);
    } else {
        strcpy(nome_local, "Cliente Desconhecido");
    }

    // Cria o vertice temporario para o cliente (eh_utilizador = 1)
    idx_cliente = grafo_adicionar_vertice(g, nome_local, 1);
    
    if (idx_cliente == -1) {
        printf("[ERRO] Nao foi possivel definir a localizacao temporaria.\n");
        return;
    }

    // Procura mercados vizinhos no mesmo bairro/nome para criar conexao de proximidade automatica inicial
    for (i = 0; i < g->num_vertices; i++) {
        if (i != idx_cliente && strcmp(g->vertices[i].nome, nome_local) == 0 && !g->vertices[i].eh_utilizador) {
            grafo_adicionar_aresta(g, idx_cliente, i, 0.5); // Conecta com 0.5km de distancia padrao
        }
    }

    do {
        printf("\n=================================");
        printf("\n    MENU DE BUSCA DO CLIENTE     ");
        printf("\n=================================");
        printf("\nLocalizacao Atual: %s (ID Vertice: %d)", g->vertices[idx_cliente].nome, idx_cliente);
        printf("\n1. Autocomplete / Pesquisar Produto num Mercado");
        printf("\n2. Buscar Mercado Mais Proximo com um Produto (BFS)");
        printf("\n3. Visualizar Caminhos e Conexoes (DFS)");
        printf("\n4. Ver todos os Mercados e IDs");
    printf("\n5. Ver Produtos de um Mercado");
    printf("\n0. Sair e Remover Localizacao");

        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);
        getchar();

        switch(opcao) {
            case 1: {
                int id_m, i;
                printf("\nIntroduza o ID do mercado onde quer buscar: ");
                scanf("%d", &id_m);
                getchar();
                trie *raiz_m = obter_trie_do_mercado(catalogos, id_m);
                if (raiz_m != NULL) {
                    printf("Comece a digitar o nome do produto: ");
                    fgets(prefixo, sizeof(prefixo), stdin); prefixo[strcspn(prefixo, "\n")] = '\0';
                    printf("\n--- Sugestoes de Autocomplete ---\n");
                    autocomplete(raiz_m, prefixo);
                } else {
                    printf("[!] Mercado invalido.\n");
                }
                break;
            }

            case 2: {
                // Prepara o vetor dinamico para o BFS indicar quais mercados tem o produto
                int tem_produto[MAX_VERTICES] = {0};
                printf("\nQue produto procura? ");
                fgets(prefixo, sizeof(prefixo), stdin); prefixo[strcspn(prefixo, "\n")] = '\0';
                
                // Varre os catalogos para ver quais mercados contem o produto exato ou prefixo
                for (i = 0; i < g->num_vertices; i++) {
                    trie *raiz_m = obter_trie_do_mercado(catalogos, i);
                    if (raiz_m != NULL && pesquisar(raiz_m, prefixo)) {
                        tem_produto[i] = 1;
                    }
                }
                
                // Executa a busca em largura nativa criada pelo teu grupo
                grafo_bfs_mercado_proximo(g, idx_cliente, tem_produto);
                break;
            }

            case 3:
                grafo_dfs(g, idx_cliente);
                break;

			case 4:
                /* Mostra mercados com ID — versao simplificada so com nome */
                printf("\n--- Lista de Mercados ---\n");
                {
                    int i;
                    for (i = 0; i < g->num_vertices; i++) {
                        if (!g->vertices[i].eh_utilizador)
                            printf("  ID %d — %s\n", i, g->vertices[i].nome);
                    }
                    printf("-------------------------\n");
                }
                break;

            case 5:
            {
                int id_m;
                printf("\nIntroduza o ID do mercado: ");
                scanf("%d", &id_m);
                getchar();
                trie *raiz_m = obter_trie_do_mercado(catalogos, id_m);
                if (raiz_m != NULL) {
                    char palavra[100] = "";
                    printf("\n--- Produtos do Mercado %d (%s) ---\n",
                        id_m, g->vertices[id_m].nome);
                    listar(raiz_m, palavra, 0);
                    printf("-----------------------------------\n");
                } else {
                    printf("[!] Mercado invalido.\n");
                }
                break;
            }



            case 0:
                printf("\nA remover dados temporarios do cliente...\n");
                break;
            default:
                printf("\nOpcao invalida!\n");
        }
    } while (opcao != 0);

    // Remove o cliente do grafo limpando as arestas dele antes de sair
    Aresta* atual = g->vertices[idx_cliente].lista_adj;
    while (atual) {
        Aresta* prox = atual->prox;
        free(atual);
        atual = prox;
    }
    g->vertices[idx_cliente].lista_adj = NULL;
    g->num_vertices--; // Remove o vertice temporario de forma limpa do contador
}

// Funcao Principal
int main() {
    Grafo mapa;
    grafo_init(&mapa);

    Usuario *listaUsuarios = NULL;
    CatalogoMercado *listaCatalogos = NULL;

    // Criar conta de administrador padrao
    cadastrar_usuario(&listaUsuarios, "admin", "1234");

    // Inserir alguns dados de teste geograficos iniciais
    int m1 = grafo_adicionar_vertice(&mapa, "Maianga", 0);
    int m2 = grafo_adicionar_vertice(&mapa, "Estalagem", 0);
    grafo_adicionar_aresta(&mapa, m1, m2, 4.5); // Conecta os dois bairros

    associar_trie_ao_mercado(&listaCatalogos, m1);
    associar_trie_ao_mercado(&listaCatalogos, m2);

    // Carregar alguns produtos simulados nas tries desses mercados
    trie *t1 = obter_trie_do_mercado(listaCatalogos, m1);
    if(t1) { carregarProdutos(t1);}

    int perfil;
    char user[50], pass[50];

    do {
        printf("\n=================================");
        printf("\n   MOTOR DE BUSCA GEOGRAFICO     ");
        printf("\n=================================");
        printf("\n1. Entrar como Administrador");
        printf("\n2. Entrar como Cliente");
        printf("\n0. Sair do Programa");
        printf("\nEscolha o perfil: ");
        scanf("%d", &perfil);
        getchar();

        switch(perfil) {
            case 1:
                printf("\n--- Autenticacao ---");
                printf("\nUsername: ");
                fgets(user, sizeof(user), stdin); user[strcspn(user, "\n")] = '\0';
                printf("Password: ");
                fgets(pass, sizeof(pass), stdin); pass[strcspn(pass, "\n")] = '\0';

                if (fazer_login(listaUsuarios, user, pass)) {
                    menuAdmin(&mapa, &listaCatalogos, &listaUsuarios);
                } else {
                    printf("\n[!] Credenciais incorretas.\n");
                }
                break;

            case 2:
                menuCliente(&mapa, listaCatalogos);
                break;

            case 0:
                printf("\nA fechar o programa...\n");
                break;
            default:
                printf("\nOpcao inválida.\n");
        }
    } while (perfil != 0);

    // Libertacao de memoria total
    libertar_catalogos(listaCatalogos);
    grafo_destruir(&mapa);
    
    Usuario *u_atual = listaUsuarios;
    while(u_atual) {
        Usuario *u_aux = u_atual->proximo;
        free(u_atual);
        u_atual = u_aux;
    }

    return 0;
}
