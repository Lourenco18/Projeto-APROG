#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functions.c"
// Limites e constantes do sistema
#define MAX_TICKETS_URGENCIA 50
#define MAX_TICKETS 100
#define NUM_BALCOES 3

// Estrutura para armazenar informações de um ticket de consulta marcada
typedef struct {
    int numero;
    time_t data_hora_geracao;
    time_t data_hora_atendimento;
    int balcao;
    char medico[50];
    char especialidade[50];
    int gabinete;
    float valor;
} TicketConsultaMarcada;

// Estrutura para armazenar informações de um ticket de consulta de urgência
typedef struct {
    int numero;
    time_t data_hora_geracao;
    time_t data_hora_atendimento;
    int balcao;
    int sala_enfermagem;
} TicketConsultaUrgencia;

// Vetores de tickets de cada tipo
TicketConsultaMarcada ticketsMarcados[MAX_TICKETS];
TicketConsultaUrgencia ticketsUrgencia[MAX_TICKETS_URGENCIA];
int contagemMarcados = 0, contagemUrgencia = 0;

// Função para obter data e hora atual
time_t obterDataHoraAtual() {
    return time(NULL);
}

// Função para gerar um ticket de consulta marcada


// Função para gerar um ticket de consulta de urgência
void gerarTicketConsultaUrgencia() {
    if (contagemUrgencia >= MAX_TICKETS_URGENCIA) {
        printf("Limite de tickets de urgência atingido.\n");
        return;
    }
    TicketConsultaUrgencia novoTicket;
    novoTicket.numero = contagemUrgencia + 1;
    novoTicket.data_hora_geracao = obterDataHoraAtual();
    novoTicket.data_hora_atendimento = 0; // Será preenchido no atendimento
    novoTicket.balcao = 0; // Será preenchido no atendimento
    novoTicket.sala_enfermagem = 0; // Será preenchido no atendimento

    ticketsUrgencia[contagemUrgencia++] = novoTicket;
    printf("Ticket de consulta de urgência gerado com sucesso.\n");
}

// Função para listar todos os tickets (marcados e urgência)

// Função auxiliar para imprimir data e hora formatada
void imprimirDataHora(time_t timestamp) {
    if (timestamp == 0) {
        printf("Não atendido");
    } else {
        struct tm *tm_info = localtime(&timestamp);
        char buffer[26];
        strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
        printf("%s", buffer);
    }
}

// Função para listar todos os tickets (consultas marcadas e urgências)
void listarTickets() {
    printf("\n----- LISTAGEM DE TICKETS -----\n");

    printf("\nTickets de Consulta Marcada:\n");
    for (int i = 0; i < contagemMarcados; i++) {
        printf("Número do Ticket: %d\n", ticketsMarcados[i].numero);
        printf("Data e Hora de Geração: ");
        imprimirDataHora(ticketsMarcados[i].data_hora_geracao);
        printf("\nData e Hora de Atendimento: ");
        imprimirDataHora(ticketsMarcados[i].data_hora_atendimento);
        printf("\nBalcão: %d\n", ticketsMarcados[i].balcao);
        printf("Médico: %s\n", ticketsMarcados[i].medico);
        printf("Especialidade: %s\n", ticketsMarcados[i].especialidade);
        printf("Gabinete: %d\n", ticketsMarcados[i].gabinete);
        printf("Valor a Pagar: %.2f\n", ticketsMarcados[i].valor);
        printf("--------------------------------\n");
    }

    printf("\nTickets de Consulta de Urgência:\n");
    for (int i = 0; i < contagemUrgencia; i++) {
        printf("Número do Ticket: %d\n", ticketsUrgencia[i].numero);
        printf("Data e Hora de Geração: ");
        imprimirDataHora(ticketsUrgencia[i].data_hora_geracao);
        printf("\nData e Hora de Atendimento: ");
        imprimirDataHora(ticketsUrgencia[i].data_hora_atendimento);
        printf("\nBalcão: %d\n", ticketsUrgencia[i].balcao);
        printf("Sala de Enfermagem: %d\n", ticketsUrgencia[i].sala_enfermagem);
        printf("--------------------------------\n");
    }
}


// Função para atualizar o atendimento de um ticket de consulta marcada
void atualizarAtendimentoConsultaMarcada(int numero) {
    for (int i = 0; i < contagemMarcados; i++) {
        if (ticketsMarcados[i].numero == numero) {
            ticketsMarcados[i].data_hora_atendimento = obterDataHoraAtual();
            printf("Informe o balcão (1 ou 2): "); 
            scanf("%d", &ticketsMarcados[i].balcao);
            if (ticketsMarcados[i].balcao == 3) {
                printf("Erro: Balcão 3 não atende consultas marcadas.\n");
                return;
            }
            printf("Atendimento atualizado com sucesso.\n");
            return;
        }
    }
    printf("Ticket de consulta marcada não encontrado.\n");
}

// Função para atualizar o atendimento de um ticket de consulta de urgência
void atualizarAtendimentoConsultaUrgencia(int numero) {
    for (int i = 0; i < contagemUrgencia; i++) {
        if (ticketsUrgencia[i].numero == numero) {
            ticketsUrgencia[i].data_hora_atendimento = obterDataHoraAtual();
            printf("Informe o balcão (1, 2 ou 3): ");
            scanf("%d", &ticketsUrgencia[i].balcao);
            printf("Informe a sala de enfermagem: ");
            scanf("%d", &ticketsUrgencia[i].sala_enfermagem);
            printf("Atendimento atualizado com sucesso.\n");
            return;
        }
    }
    printf("Ticket de urgência não encontrado.\n");
}

// Função principal para o menu
int main() {
    int opcao, numero;
    do {
        printf("\nSistema de Atendimento Hospitalar\n");
        printf("1. Gerar Ticket de Consulta Marcada\n");
        printf("2. Gerar Ticket de Consulta de Urgência\n");
        printf("3. Listar Tickets\n");
        printf("4. Atualizar Atendimento de Consulta Marcada\n");
        printf("5. Atualizar Atendimento de Consulta de Urgência\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                gerarTicketConsultaMarcada();
                break;
            case 2:
                gerarTicketConsultaUrgencia();
                break;
            case 3:
                listarTickets();
                break;
            case 4:
                printf("Informe o número do ticket de consulta marcada: ");
                scanf("%d", &numero);
                atualizarAtendimentoConsultaMarcada(numero);
                break;
            case 5:
                printf("Informe o número do ticket de urgência: ");
                scanf("%d", &numero);
                atualizarAtendimentoConsultaUrgencia(numero);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
    
    return 0;
}
