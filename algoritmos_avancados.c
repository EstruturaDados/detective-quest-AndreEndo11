#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da sala (nó da árvore)
struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
};

// Cria uma nova sala
struct Sala* criarSala(char* nome) {
    struct Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// Função de exploração interativa
void explorarSalas(struct Sala* raiz) {
    struct Sala* atual = raiz;
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Se for folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Você chegou a um cômodo final. Fim da exploração!\n");
            break;
        }

        printf("Escolha um caminho:\n");
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
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } 
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

// Libera memória da árvore
void liberar(struct Sala* raiz) {
    if (raiz != NULL) {
        liberar(raiz->esquerda);
        liberar(raiz->direita);
        free(raiz);
    }
}

int main() {
    // Montando a mansão (árvore binária)
    struct Sala* raiz = criarSala("Hall de Entrada");

    raiz->esquerda = criarSala("Sala de Estar");
    raiz->direita = criarSala("Biblioteca");

    raiz->esquerda->esquerda = criarSala("Quarto");
    raiz->esquerda->direita = criarSala("Cozinha");

    raiz->direita->esquerda = criarSala("Escritório");
    raiz->direita->direita = criarSala("Jardim");

    // Inicia exploração
    printf("=== Detective Quest: Exploração da Mansão ===\n");
    explorarSalas(raiz);

    // Libera memória
    liberar(raiz);

    return 0;
}
