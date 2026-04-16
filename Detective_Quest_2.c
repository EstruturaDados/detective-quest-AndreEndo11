#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =====================
// Estrutura da Sala (Árvore Binária)
// =====================
struct Sala {
    char nome[50];
    char pista[100];
    struct Sala* esquerda;
    struct Sala* direita;
};

// =====================
// BST de Pistas
// =====================
struct PistaNode {
    char pista[100];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
};

// =====================
// Tabela Hash (pista -> suspeito)
// =====================
#define TAM 13

struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode* prox;
};

struct HashNode* tabela[TAM];

// =====================
// Hash simples
// =====================
int hashFunc(char* str) {
    int soma = 0;
    for (int i = 0; str[i]; i++)
        soma += str[i];
    return soma % TAM;
}

/*
 * inserirNaHash()
 * Insere a associação pista -> suspeito na tabela hash.
 */
void inserirNaHash(char* pista, char* suspeito) {
    int index = hashFunc(pista);

    struct HashNode* novo = (struct HashNode*) malloc(sizeof(struct HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[index];
    tabela[index] = novo;
}

/*
 * encontrarSuspeito()
 * Retorna o suspeito associado a uma pista.
 */
char* encontrarSuspeito(char* pista) {
    int index = hashFunc(pista);
    struct HashNode* aux = tabela[index];

    while (aux != NULL) {
        if (strcmp(aux->pista, pista) == 0)
            return aux->suspeito;
        aux = aux->prox;
    }

    return "Desconhecido";
}

/*
 * criarSala()
 * Cria dinamicamente um cômodo da mansão.
 */
struct Sala* criarSala(char* nome, char* pista) {
    struct Sala* nova = (struct Sala*) malloc(sizeof(struct Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/*
 * inserirPista() / adicionarPista()
 * Insere pista na BST de forma ordenada.
 */
struct PistaNode* inserirPista(struct PistaNode* raiz, char* pista) {
    if (raiz == NULL) {
        struct PistaNode* novo = (struct PistaNode*) malloc(sizeof(struct PistaNode));
        strcpy(novo->pista, pista);
        novo->esquerda = novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);

    return raiz;
}

/*
 * exibirPistas()
 * Exibe pistas em ordem alfabética (BST in-order).
 */
void exibirPistas(struct PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/*
 * explorarSalas()
 * Navegação pela mansão e coleta de pistas.
 */
void explorarSalas(struct Sala* raiz, struct PistaNode** pistasColetadas) {
    struct Sala* atual = raiz;
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (strlen(atual->pista) > 0) {
            printf("Pista encontrada: %s\n", atual->pista);
            *pistasColetadas = inserirPista(*pistasColetadas, atual->pista);
        }

        printf("\n(e) esquerda | (d) direita | (s) sair\n");
        scanf(" %c", &opcao);

        if (opcao == 'e' && atual->esquerda)
            atual = atual->esquerda;
        else if (opcao == 'd' && atual->direita)
            atual = atual->direita;
        else if (opcao == 's') {
            printf("Exploração encerrada.\n");
            break;
        } else {
            printf("Movimento inválido.\n");
        }
    }
}

/*
 * verificarSuspeitoFinal()
 * Verifica se há pistas suficientes contra o suspeito acusado.
 */
void verificarSuspeitoFinal(struct PistaNode* raiz, char* acusado) {
    int cont = 0;

    if (raiz == NULL) return;

    // percorre BST
    struct PistaNode* stack[100];
    int top = 0;
    stack[top++] = raiz;

    while (top > 0) {
        struct PistaNode* atual = stack[--top];

        char* suspeito = encontrarSuspeito(atual->pista);

        if (strcmp(suspeito, acusado) == 0)
            cont++;

        if (atual->esquerda) stack[top++] = atual->esquerda;
        if (atual->direita) stack[top++] = atual->direita;
    }

    printf("\n=== JULGAMENTO FINAL ===\n");

    if (cont >= 2) {
        printf("CONCLUSÃO: %s é o CULPADO! (%d pistas encontradas)\n", acusado, cont);
    } else {
        printf("CONCLUSÃO: %s NÃO pode ser condenado (%d pistas insuficientes)\n", acusado, cont);
    }
}

/*
 * liberar memória
 */
void liberarSalas(struct Sala* raiz) {
    if (raiz) {
        liberarSalas(raiz->esquerda);
        liberarSalas(raiz->direita);
        free(raiz);
    }
}

void liberarPistas(struct PistaNode* raiz) {
    if (raiz) {
        liberarPistas(raiz->esquerda);
        liberarPistas(raiz->direita);
        free(raiz);
    }
}

// =====================
// MAIN
// =====================
int main() {

    // inicializa hash
    for (int i = 0; i < TAM; i++)
        tabela[i] = NULL;

    // associa pistas a suspeitos
    inserirNaHash("Pegadas estranhas no chão", "Joaquim");
    inserirNaHash("Janela quebrada", "Maria");
    inserirNaHash("Livro fora do lugar", "Carlos");
    inserirNaHash("Lençóis bagunçados", "Joaquim");
    inserirNaHash("Faca desaparecida", "Ana");
    inserirNaHash("Documento rasgado", "Carlos");
    inserirNaHash("Pegadas na terra", "Joaquim");

    // árvore da mansão
    struct Sala* raiz = criarSala("Hall de Entrada", "Pegadas estranhas no chão");

    raiz->esquerda = criarSala("Sala de Estar", "Janela quebrada");
    raiz->direita = criarSala("Biblioteca", "Livro fora do lugar");

    raiz->esquerda->esquerda = criarSala("Quarto", "Lençóis bagunçados");
    raiz->esquerda->direita = criarSala("Cozinha", "Faca desaparecida");

    raiz->direita->esquerda = criarSala("Escritório", "Documento rasgado");
    raiz->direita->direita = criarSala("Jardim", "Pegadas na terra");

    struct PistaNode* pistas = NULL;

    printf("=== DETECTIVE QUEST ===\n");

    explorarSalas(raiz, &pistas);

    printf("\nPistas coletadas:\n");
    exibirPistas(pistas);

    char acusado[50];
    printf("\nQuem é o suspeito? ");
    scanf(" %[^\n]", acusado);

    verificarSuspeitoFinal(pistas, acusado);

    liberarSalas(raiz);
    liberarPistas(pistas);

    return 0;
}
