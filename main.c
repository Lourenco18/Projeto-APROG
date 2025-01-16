
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// Procedimento para limpar a tela
#ifdef _WIN32
    #define LIMPAR_TELA "cls"
#else
    #define LIMPAR_TELA "clear"
#endif

// Valores máximos de senhas de urgência e senhas normais
#define MAX_SENHAS_URGENTES 10
#define MAX_SENHAS 100

// Predefined accounts
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin"
#define NORMAL_USER "user"
#define NORMAL_PASS "user"

typedef struct {
    int id;
    char tipo[20]; // "Consulta" ou "Urgente"
    time_t horario_gerado;
    time_t horario_atendido;
    char informacoes_adicionais[100];
    float pagamento; // Valor pago pela consulta
    int balcao_atendido; // Balcão onde foi atendido
} Senha;

Senha senhas[MAX_SENHAS]; // Vetor de senhas
int contador_senhas = 0;
int contador_senhas_urgentes = 0;

// Função para limpar a tela
void limpar_tela() {
    system(LIMPAR_TELA);
}

// Função para esperar o utilizador pressionar qualquer tecla
void esperar_tecla() {
    printf("\nPressione qualquer tecla para continuar...\n");
    getchar(); // Para capturar o Enter pendente
    getchar(); // Aguarda o utilizador pressionar uma tecla
}

// Função para validar o formato da data escrita pelo utilizador
time_t validar_data() {
    struct tm data;
    char entrada[20];

    while (1) {
        printf("Digite a data e hora no formato 'DD-MM-AAAA HH:MM:SS': ");
        scanf(" %[^\n]s", entrada);

        // Limpar a estrutura e preencher com a entrada
        memset(&data, 0, sizeof(data));
        if (strptime(entrada, "%d-%m-%Y %H:%M:%S", &data)) {
            return mktime(&data); // Retorna a data
        } else {
            printf("Formato inválido. Tente novamente.\n");
        }
    }
}

// Função para formatar o horário a "DD-MM-AAAA HH:MM:SS"
void formatar_data(char *buffer, time_t tempo) {
    struct tm *tm_info = localtime(&tempo);
    strftime(buffer, 20, "%d-%m-%Y %H:%M:%S", tm_info);
}

// Função para gerar uma nova senha
void gerar_senha(char *tipo) {
    limpar_tela();

    // Verificar se o limite de senhas de urgência foi atingido
    if (strcmp(tipo, "Urgente") == 0 && contador_senhas_urgentes >= MAX_SENHAS_URGENTES) {
        printf("=========================================\n");
        printf("       Limite de Senhas de Urgência      \n");
        printf("=========================================\n");
        printf("O limite de senhas de urgência foi atingido.\n");
        esperar_tecla();
        return;
    }

    Senha nova_senha;
    nova_senha.id = contador_senhas + 1; // ID sequencial
    strcpy(nova_senha.tipo, tipo);
    nova_senha.horario_gerado = time(NULL);
    nova_senha.horario_atendido = 0; // Ainda não atendido
    nova_senha.balcao_atendido = 0; // Ainda não atendido
    strcpy(nova_senha.informacoes_adicionais, ""); // Informações adicionais vazias
    nova_senha.pagamento = 0.0; // Pagamento 0

    // Para senhas de consulta, pedir o valor
    if (strcmp(tipo, "Consulta") == 0) {
        printf("=========================================\n");
        printf("      Gerando Senha de Consulta          \n");
        printf("=========================================\n");
        printf("Digite o valor a pagar pela consulta (0€ ou superior): ");
        scanf("%f", &nova_senha.pagamento);
        if (nova_senha.pagamento < 0) {
            printf("\n=========================================\n");
            printf("        Valor inválido!                  \n");
            printf("=========================================\n");
            printf("Senha não gerada.\n");
            esperar_tecla();
            return;
        }
    }

    senhas[contador_senhas++] = nova_senha;

    if (strcmp(tipo, "Urgente") == 0) {
        contador_senhas_urgentes++;
    }

    printf("=========================================\n");
    printf("          Senha Gerada com Sucesso       \n");
    printf("=========================================\n");
    printf("ID: %d\n", nova_senha.id);
    printf("Tipo: %s\n", nova_senha.tipo);
    printf("Valor: %.2f€\n", nova_senha.pagamento);
    printf("=========================================\n");

    esperar_tecla();
}

// Função para listar todas as senhas
void listar_senhas() {
    limpar_tela();
    printf("Lista de Senhas:\n\n");

    if (contador_senhas == 0) {
        printf("Nenhuma senha foi gerada ainda.\n");
        esperar_tecla();
        return;
    }

    char buffer[20];
    for (int i = 0; i < contador_senhas; i++) {
        formatar_data(buffer, senhas[i].horario_gerado);
        printf("ID: %d\n", senhas[i].id);
        printf("Tipo: %s\n", senhas[i].tipo);
        printf("Valor: %.2f€\n", senhas[i].pagamento);
        printf("Gerado a: %s\n", buffer);

        if (senhas[i].horario_atendido != 0) {
            formatar_data(buffer, senhas[i].horario_atendido);
            printf("Atendido a: %s\n", buffer);
            printf("Balcão: %d\n", senhas[i].balcao_atendido);
            printf("Informações adicionais: %s\n", senhas[i].informacoes_adicionais);
        } else {
            printf("Status: Ainda não atendido.\n");
        }
        printf("-----------------------------\n");
    }

    esperar_tecla();
}

// Função para atender uma senha
void atender_senha(int id, int balcao) {
    limpar_tela();
    printf("=========================================\n");
    printf("             Atendimento de Senha         \n");
    printf("=========================================\n");

    // Verificar se o balcão está entre 1 e 3
    if (balcao < 1 || balcao > 3) {
        printf("=========================================\n");
        printf("            Balcão Inválido               \n");
        printf("=========================================\n");
        printf("Escolha um balcão entre 1 e 3.\n");
        esperar_tecla();
        return;
    }

    // Procurar a senha correspondente ao ID
    for (int i = 0; i < contador_senhas; i++) {
        if (senhas[i].id == id) {
            // Verificar se a senha já foi atendida
            if (senhas[i].horario_atendido != 0) {
                printf("=========================================\n");
                printf("         Senha Já Atendida               \n");
                printf("=========================================\n");
                printf("A senha ID %d já foi atendida. Não é possível realizar o atendimento novamente.\n", id);
                esperar_tecla();
                return;
            }

            // Verificar se o balcão 3 só pode atender senhas urgentes
            if (balcao == 3 && strcmp(senhas[i].tipo, "Consulta") == 0) {
                printf("=========================================\n");
                printf("      Restrição no Balcão 3              \n");
                printf("=========================================\n");
                printf("O balcão 3 só pode atender consultas de urgência.\n");
                esperar_tecla();
                return;
            }

            // Registrar atendimento
            senhas[i].horario_atendido = time(NULL);
            senhas[i].balcao_atendido = balcao;

            printf("=========================================\n");
            printf("        Atendimento Registrado           \n");
            printf("=========================================\n");
            printf("Senha ID %d atendida pelo balcão %d.\n", id, balcao);

            // Solicitar informações adicionais para o atendimento
            printf("Digite informações adicionais sobre o atendimento:\n");
            getchar(); // Consumir o caractere de nova linha
            fgets(senhas[i].informacoes_adicionais, sizeof(senhas[i].informacoes_adicionais), stdin);
            senhas[i].informacoes_adicionais[strcspn(senhas[i].informacoes_adicionais, "\n")] = 0; // Remover nova linha

            printf("Informações adicionais registradas com sucesso.\n");
            esperar_tecla();
            return;
        }
    }

    // Caso o ID da senha não seja encontrado
    printf("=========================================\n");
    printf("         Senha Não Encontrada            \n");
    printf("=========================================\n");
    printf("Senha ID %d não encontrada.\n", id);
    esperar_tecla();
}

// Função para gerar relatórios
void gerar_relatorios() {
    limpar_tela();
    time_t inicio, fim;
    
    printf("=========================================\n");
    printf("        Geração de Relatórios            \n");
    printf("=========================================\n");
    printf("Digite o horário de início:\n");
    inicio = validar_data();
    printf("\nDigite o horário de fim:\n");
    fim = validar_data();

    if (inicio >= fim) {
        printf("=========================================\n");
        printf("        Intervalo de datas inválido       \n");
        printf("=========================================\n");
        printf("O início deve ser anterior ao fim.\n");
        esperar_tecla();
        return;
    }

    while (1) {
        limpar_tela();
        printf("=========================================\n");
        printf("               Relatórios                 \n");
        printf("=========================================\n");
        printf("1. Volume de senhas atendidas\n");
        printf("2. Médias de espera entre atendimentos\n");
        printf("3. Atendimentos por balcão\n");
        printf("4. Receitas de consultas marcadas\n");
        printf("5. Alterar intervalo de datas\n");
        printf("6. Voltar ao menu principal\n");
        printf("=========================================\n");
        printf("Escolha uma opção: ");

        int opcao;
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int atendidas = 0;
                for (int i = 0; i < contador_senhas; i++) {
                    if (senhas[i].horario_atendido >= inicio && senhas[i].horario_atendido <= fim) {
                        atendidas++;
                    }
                }
                printf("=========================================\n");
                printf("   Volume de Senhas Atendidas no Intervalo\n");
                printf("=========================================\n");
                printf("Total: %d\n", atendidas);
                esperar_tecla();
                break;
            }
            case 2: {
                double tempo_total_espera = 0;
                int atendidas = 0;
                for (int i = 0; i < contador_senhas; i++) {
                    if (senhas[i].horario_atendido >= inicio && senhas[i].horario_atendido <= fim) {
                        tempo_total_espera += difftime(senhas[i].horario_atendido, senhas[i].horario_gerado);
                        atendidas++;
                    }
                }
                printf("=========================================\n");
                printf("   Média de Espera entre Atendimentos    \n");
                printf("=========================================\n");
                if (atendidas == 0) {
                    printf("Nenhuma senha atendida no intervalo.\n");
                } else {
                    printf("Tempo médio de espera: %.2f segundos.\n", tempo_total_espera / atendidas);
                }
                esperar_tecla();
                break;
            }
            case 3: {
                int atendimentos_balcao[4] = {0};
                for (int i = 0; i < contador_senhas; i++) {
                    if (senhas[i].horario_atendido >= inicio && senhas[i].horario_atendido <= fim) {
                        atendimentos_balcao[senhas[i].balcao_atendido]++;
                    }
                }
                printf("=========================================\n");
                printf("           Atendimentos por Balcão        \n");
                printf("=========================================\n");
                for (int i = 1; i <= 3; i++) {
                    printf("Balcão %d: %d atendimentos\n", i, atendimentos_balcao[i]);
                }
                esperar_tecla();
                break;
            }
            case 4: {
                double total_receita = 0.0;
                for (int i = 0; i < contador_senhas; i++) {
                    if (senhas[i].horario_atendido >= inicio && senhas[i].horario_atendido <= fim) {
                        total_receita += senhas[i].pagamento;
                    }
                }
                printf("=========================================\n");
                printf("         Receitas das Consultas            \n");
                printf("=========================================\n");
                printf("Receitas totais: %.2f€\n", total_receita);
                esperar_tecla();
                break;
            }
            case 5:
                limpar_tela();
                printf("=========================================\n");
                printf("         Alterar Intervalo de Datas       \n");
                printf("=========================================\n");
                printf("Digite o horário de início:\n");
                inicio = validar_data();
                printf("\nDigite o horário de fim:\n");
                fim = validar_data();
                if (inicio >= fim) {
                    printf("=========================================\n");
                    printf("        Intervalo de datas inválido       \n");
                    printf("=========================================\n");
                    printf("O início deve ser anterior ao fim.\n");
                    esperar_tecla();
                }
                break;
            case 6:
                return;
            default:
                printf("=========================================\n");
                printf("           Opção Inválida                 \n");
                printf("=========================================\n");
                esperar_tecla();
        }
    }
}

// Function prototypes
void login();
int is_admin = 0;

void login() {
    char username[20], password[20];
    limpar_tela();

    printf("=========================================\n");
    printf("                 Login                   \n");
    printf("=========================================\n");

    printf("Utilizador: ");
    scanf("%s", username);
    printf("Palavra-Passe: ");
    scanf("%s", password);

    if (strcmp(username, ADMIN_USER) == 0 && strcmp(password, ADMIN_PASS) == 0) {
        printf("=========================================\n");
        printf("           Login Bem-Sucedido            \n");
        printf("=========================================\n");
        printf("Bem-vindo, Administrador!\n");
        is_admin = 1;
        esperar_tecla();
    } else if (strcmp(username, NORMAL_USER) == 0 && strcmp(password, NORMAL_PASS) == 0) {
        printf("=========================================\n");
        printf("           Login Bem-Sucedido            \n");
        printf("=========================================\n");
        printf("Bem-vindo, Usuário!\n");
        is_admin = 0;
        esperar_tecla();
    } else {
        printf("=========================================\n");
        printf("          Credenciais Inválidas          \n");
        printf("=========================================\n");
        printf("Usuário ou senha incorretos. Encerrando...\n");
        esperar_tecla();
        exit(1);
    }
}

// Função principal
int main() {
    while (1) {
        limpar_tela();
        printf("=========================================\n");
        printf("          Sistema de Atendimento         \n");
        printf("=========================================\n");
        printf("1. Login\n");
        printf("2. Sair\n");
        printf("=========================================\n");
        printf("Escolha uma opção: ");
        
        int sair = 1;
        int op;
        scanf("%d", &op);

        if (op == 1) {
            login();

            while (sair) {
                limpar_tela();
                printf("=========================================\n");
                printf("              Menu Principal             \n");
                printf("=========================================\n");
                printf("1. Gerar senha de consulta\n");
                printf("2. Gerar senha de urgência\n");
                if (is_admin) {
                    printf("3. Listar senhas\n");
                    printf("4. Atender senha\n");
                    printf("5. Gerar relatórios\n");
                }
                printf("6. Sair\n");
                printf("=========================================\n");
                printf("Escolha uma opção: ");
                
                int opcao;
                scanf("%d", &opcao);

                switch (opcao) {
                    case 1:
                        gerar_senha("Consulta");
                        break;
                    case 2:
                        gerar_senha("Urgente");
                        break;
                    case 3:
                        if (is_admin) {
                            listar_senhas();
                        } else {
                            printf("=========================================\n");
                            printf("        Acesso negado. Permissão insuficiente.\n");
                            printf("=========================================\n");
                            esperar_tecla();
                        }
                        break;
                    case 4: {
                        if (is_admin) {
                            int id, balcao;
                            printf("Digite o ID da senha a ser atendida: ");
                            scanf("%d", &id);
                            printf("Digite o balcão de atendimento (1, 2 ou 3): ");
                            scanf("%d", &balcao);
                            atender_senha(id, balcao);
                        } else {
                            printf("=========================================\n");
                            printf("        Acesso negado. Permissão insuficiente.\n");
                            printf("=========================================\n");
                            esperar_tecla();
                        }
                        break;
                    }
                    case 5:
                        if (is_admin) {
                            gerar_relatorios();
                        } else {
                            printf("=========================================\n");
                            printf("        Acesso negado. Permissão insuficiente.\n");
                            printf("=========================================\n");
                            esperar_tecla();
                        }
                        break;
                    case 6:
                        sair = 0;
                        break;
                    default:
                        printf("=========================================\n");
                        printf("            Opção inválida.              \n");
                        printf("=========================================\n");
                        esperar_tecla();
                }
            }
        } else if (op == 2) {
            printf("=========================================\n");
            printf("        Obrigado por usar o sistema!     \n");
            printf("=========================================\n");
            return 0;
        } else {
            printf("=========================================\n");
            printf("            Opção inválida.              \n");
            printf("=========================================\n");
            esperar_tecla();
        }
    }
}