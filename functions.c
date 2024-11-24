void gerarTicketConsultaMarcada() {
    if (contagemMarcados >= MAX_TICKETS) {
        printf("Limite de tickets de consulta marcada atingido.\n");
        return;
    }
    TicketConsultaMarcada novoTicket;
    novoTicket.numero = contagemMarcados + 1;
    novoTicket.data_hora_geracao = obterDataHoraAtual();
    novoTicket.data_hora_atendimento = 0; // Será preenchido no atendimento
    novoTicket.balcao = 0; // Será preenchido no atendimento

    // Solicita informações adicionais
    printf("Informe o médico: ");
    scanf("%s", novoTicket.medico);
    printf("Informe a especialidade: ");
    scanf("%s", novoTicket.especialidade);
    printf("Informe o gabinete: ");
    scanf("%d", &novoTicket.gabinete);
    printf("Informe o valor da consulta: ");
    scanf("%f", &novoTicket.valor);
    if (novoTicket.valor < 0) {
        printf("Valor da consulta deve ser 0 ou superior.\n");
        return;
    }
    ticketsMarcados[contagemMarcados++] = novoTicket;
    printf("Ticket de consulta marcada gerado com sucesso.\n");
}