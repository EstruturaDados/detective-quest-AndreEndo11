#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================
// Estrutura da sala
// =====================
struct Sala {
    char nome[50];
    char pista[100]; // nova informação: pista da sala
    struct Sala* esquerda;
    struct Sala* direita;
};

// =====================
// Estrutura da BST de pistas
// =====================
struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
};

// =====================
// Cria uma nova sala
// =====================
struct Sala* criarSala(char* nome, char* pista) {
    struct Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// =====================
// Insere pista na BST
// =====================
struct PistaNode* inserirPista(struct PistaNode* raiz, char* pista) {
    if (raiz == NULL) {
        struct PistaNode* novo = (struct PistaNode*) malloc(sizeof(struct PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    // Ordenação alfabética
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }

    return raiz;
}

// =====================
// Exibe pistas em ordem (in-order)
// =====================
void exibirPistas(struct PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

// =====================
// Exploração com coleta de pistas
// =====================
void explorarSalasComPistas(struct Sala* raiz, struct PistaNode** pistasColetadas) {
    struct Sala* atual = raiz;
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coletar pista automaticamente (se existir)
        if (strlen(atual->pista) > 0) {
            printf(" Você encontrou uma pista: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        }

        printf("\nEscolha um caminho:\n");
        if (atual->esquerda != NULL)
            printf(" (e) Ir para %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)
            printf(" (d) Ir para %s\n", atual->direita->nome);
        printf(" (s) Sair\n");

        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } 
        else if (opcao == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } 
        else if (opcao == 's') {
            printf("Exploração encerrada.\n");
            break;
        } 
        else {
            printf("Opção inválida!\n");
        }
    }
}

// =====================
// Libera árvore de salas
// =====================
void liberarSalas(struct Sala* raiz) {
    if (raiz != NULL) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

// =====================
// Libera árvore de pistas
// =====================
void liberarPistas(struct PistaNode* raiz) {
    if (raiz != NULL) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// =====================
// MAIN
// =====================
int main() {
    // Montagem da mansão com pistas
    struct Sala* raiz = criarSala("Hall de Entrada", "Pegadas estranhas no chão");

    raiz->esquerda = criarSala("Sala de Estar", "Janela quebrada");
    raiz->direita = criarSala("Biblioteca", "Livro fora do lugar");

    raiz->esquerda->esquerda = criarSala("Quarto", "Lençóis bagunçados");
    raiz->esquerda->direita = criarSala("Cozinha", "Faca desaparecida");

    raiz->direita->esquerda = criarSala("Escritório", "Documento rasgado");
    raiz->direita->direita = criarSala("Jardim", "Pegadas na terra");

    // Árvore de pistas (BST)
    struct PistaNode* pistas = NULL;

    printf("=== Detective Quest: Caça às Pistas ===\n");

    explorarSalasComPistas(raiz, &pistas);

    // Exibir pistas coletadas
    printf("\n Pistas coletadas (ordem alfabética):\n");
    exibirPistas(pistas);

    // Liberar memória
    liberarSalas(raiz);
    liberarPistas(pistas);

    return 0;
}
