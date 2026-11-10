#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#define MAX_ACOES 100
#define TAM 120

typedef enum { PLANEJADA, REALIZADA, CANCELADA } Situacao;
typedef struct {
    int codigo;
    char escola[TAM], tema[TAM], data[11], publico[TAM], responsavel[TAM];
    int prevista, efetiva;
    Situacao situacao;
} Acao;

static Acao acoes[MAX_ACOES];
static int total = 0;

static void linha(void) { puts("------------------------------------------------------------"); }
static void lerLinha(char *dest, size_t tam) {
    int c;
    if (!fgets(dest, (int)tam, stdin)) { puts("Entrada encerrada."); exit(0); }
    size_t n = strlen(dest);
    if (n && dest[n-1] == '\n') dest[n-1] = '\0';
    else while ((c = getchar()) != '\n' && c != EOF) {}
}
static int vazio(const char *s) {
    while (*s) { if (!isspace((unsigned char)*s)) return 0; s++; }
    return 1;
}
static void textoObrigatorio(const char *rotulo, char *dest, size_t tam) {
    do {
        printf("%s: ", rotulo); lerLinha(dest, tam);
        if (vazio(dest)) puts("Erro: campo obrigatorio nao pode ficar vazio.");
    } while (vazio(dest));
}
static int inteiro(const char *rotulo, int minimo, int maximo) {
    char buf[128], *fim; long n;
    for (;;) {
        printf("%s: ", rotulo); lerLinha(buf, sizeof buf);
        char *inicio = buf;
        while (isspace((unsigned char)*inicio)) inicio++;
        if (!*inicio) { puts("Erro: informe um numero inteiro."); continue; }
        n = strtol(inicio, &fim, 10);
        while (isspace((unsigned char)*fim)) fim++;
        if (fim == inicio || *fim || n < minimo || n > maximo) {
            printf("Erro: informe um inteiro entre %d e %d.\n", minimo, maximo);
        } else return (int)n;
    }
}
static int indice(int codigo) {
    for (int i = 0; i < total; i++) if (acoes[i].codigo == codigo) return i;
    return -1;
}
static const char *nomeSituacao(Situacao s) {
    return s == PLANEJADA ? "Planejada" : s == REALIZADA ? "Realizada" : "Cancelada";
}
static int dataValida(const char *s) {
    if (strlen(s) != 10 || s[2] != '/' || s[5] != '/') return 0;
    for (int i = 0; i < 10; i++) if (i != 2 && i != 5 && !isdigit((unsigned char)s[i])) return 0;
    int d = (s[0]-'0')*10+s[1]-'0', m=(s[3]-'0')*10+s[4]-'0';
    int y=(s[6]-'0')*1000+(s[7]-'0')*100+(s[8]-'0')*10+s[9]-'0';
    if (y < 1900 || m < 1 || m > 12) return 0;
    int dias[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (m == 2 && (y%400 == 0 || (y%4 == 0 && y%100 != 0))) dias[2] = 29;
    return d >= 1 && d <= dias[m];
}
static void exibir(const Acao *a) {
    linha();
    printf("Codigo: %d\nEscola: %s\nTema: %s\nData prevista: %s\n", a->codigo, a->escola, a->tema, a->data);
    printf("Publico-alvo: %s\nResponsavel: %s\n", a->publico, a->responsavel);
    printf("Participantes previstos: %d\nSituacao: %s\n", a->prevista, nomeSituacao(a->situacao));
    if (a->situacao == REALIZADA) printf("Participantes efetivos: %d\n", a->efetiva);
}
static void cadastrar(void) {
    if (total >= MAX_ACOES) { puts("Erro: limite de armazenamento atingido."); return; }
    int codigo = inteiro("Codigo positivo", 1, INT_MAX);
    if (indice(codigo) >= 0) { puts("Erro: codigo ja cadastrado."); return; }
    Acao a = {0}; a.codigo = codigo; a.situacao = PLANEJADA;
    textoObrigatorio("Escola", a.escola, sizeof a.escola);
    textoObrigatorio("Tema", a.tema, sizeof a.tema);
    do {
        textoObrigatorio("Data prevista (DD/MM/AAAA)", a.data, sizeof a.data);
        if (!dataValida(a.data)) puts("Erro: data invalida.");
    } while (!dataValida(a.data));
    textoObrigatorio("Publico-alvo", a.publico, sizeof a.publico);
    textoObrigatorio("Responsavel", a.responsavel, sizeof a.responsavel);
    a.prevista = inteiro("Quantidade prevista de participantes", 0, INT_MAX);
    acoes[total++] = a;
    puts("Acao cadastrada com sucesso!");
}
static void listar(void) {
    if (!total) { puts("Nenhuma acao cadastrada."); return; }
    for (int i = 0; i < total; i++) exibir(&acoes[i]);
    linha();
}
static int contemIgnoreCase(const char *texto, const char *busca) {
    size_t n = strlen(busca);
    for (; *texto; texto++) {
        size_t j = 0;
        while (j < n && texto[j] && tolower((unsigned char)texto[j]) == tolower((unsigned char)busca[j])) j++;
        if (j == n) return 1;
    }
    return n == 0;
}
static void pesquisar(void) {
    puts("1 - Por codigo\n2 - Por escola\n3 - Por tema");
    int op = inteiro("Tipo de pesquisa", 1, 3), encontrados = 0;
    if (op == 1) {
        int id = indice(inteiro("Codigo", 1, INT_MAX));
        if (id >= 0) { exibir(&acoes[id]); encontrados++; }
    } else {
        char termo[TAM]; textoObrigatorio("Texto da pesquisa", termo, sizeof termo);
        for (int i = 0; i < total; i++) {
            const char *campo = op == 2 ? acoes[i].escola : acoes[i].tema;
            if (contemIgnoreCase(campo, termo)) { exibir(&acoes[i]); encontrados++; }
        }
    }
    if (!encontrados) puts("Nenhuma acao encontrada.");
    else printf("Total de resultados: %d\n", encontrados);
}
static void atualizar(void) {
    int id = indice(inteiro("Codigo da acao", 1, INT_MAX));
    if (id < 0) { puts("Erro: registro inexistente."); return; }
    printf("Situacao atual: %s\n", nomeSituacao(acoes[id].situacao));
    puts("1 - Planejada\n2 - Realizada\n3 - Cancelada");
    int op = inteiro("Nova situacao", 1, 3);
    if (op == 2) acoes[id].efetiva = inteiro("Quantidade efetiva de participantes", 0, INT_MAX);
    else acoes[id].efetiva = 0;
    acoes[id].situacao = (Situacao)(op-1);
    puts("Situacao atualizada com sucesso!");
}
static void resumo(void) {
    int p=0, r=0, c=0;
    long long previsto=0, efetivo=0;
    for (int i=0; i<total; i++) {
        if (acoes[i].situacao == PLANEJADA) p++;
        else if (acoes[i].situacao == REALIZADA) {
            r++; previsto += acoes[i].prevista; efetivo += acoes[i].efetiva;
        } else c++;
    }
    linha(); puts("RESUMO GERAL");
    printf("Acoes cadastradas: %d\nPlanejadas: %d\nRealizadas: %d\nCanceladas: %d\n", total,p,r,c);
    printf("Participantes efetivos (acoes realizadas): %lld\n", efetivo);
    printf("Participantes previstos (acoes realizadas): %lld\n", previsto);
    if (previsto > 0) printf("Percentual de participacao: %.2f%%\n", 100.0*efetivo/previsto);
    else puts("Percentual de participacao: N/A (previsao igual a zero).");
    linha();
}
int main(void) {
    int op;
    puts("PSE EM ACAO - Planejamento de acoes coletivas de saude mental");
    do {
        puts("\n===== MENU PRINCIPAL =====");
        puts("1 - Cadastrar acao\n2 - Listar acoes\n3 - Pesquisar acoes");
        puts("4 - Atualizar situacao\n5 - Resumo geral\n6 - Validar entradas (orientacoes)\n0 - Sair");
        op = inteiro("Escolha uma opcao", 0, 6);
        switch (op) {
            case 1: cadastrar(); break;
            case 2: listar(); break;
            case 3: pesquisar(); break;
            case 4: atualizar(); break;
            case 5: resumo(); break;
            case 6: puts("Validacoes automaticas: codigo unico e positivo; campos obrigatorios; data valida; quantidades nao negativas; menu valido; limite de 100 acoes."); break;
            case 0: puts("Encerrando o sistema. Dados em memoria serao perdidos."); break;
        }
    } while (op != 0);
    return 0;
}
