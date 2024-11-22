#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PACIENTES 100
#define MAX_SALAS 4
#define MAX_HORARIOS 8
#define MAX_SEMANAS 20
#define MAX_NOME 50
#define MAX_SINTOMA 20
#define MAX_TELEFONE 15
#define MAX_CPF 12
#define MAX_MEDICACAO 50
#define TAM_BANCO 5

typedef enum { CRITICO, MUITO_ALTA, ALTA, MEDIA, BAIXA, MUITO_BAIXA } Prioridade;

typedef struct {
    char sintoma[MAX_SINTOMA];
    char nome[MAX_NOME];
    char sobrenome[MAX_NOME];
    int idade;
    char cpf[MAX_CPF];
    char telefone[MAX_TELEFONE];
    char medicacao[MAX_MEDICACAO];
    float peso;
    float altura;
    Prioridade prioridade;
    int faltas;
    int id; // Identificação única
} Paciente;

typedef struct {
    char nome[MAX_NOME];
    char especialidade[MAX_NOME];
    int horas_trabalhadas;
} Medico;

typedef struct {
    char paciente[MAX_NOME];
    char medico[MAX_NOME];
} Horario;

typedef struct {
    Horario horarios[MAX_HORARIOS];
} Sala;

int bancoReservas[TAM_BANCO];
int tam_banco = 0;

const char *prioridadeParaString(Prioridade prioridade) {
    switch (prioridade) {
        case CRITICO: return "Crítico";
        case MUITO_ALTA: return "Muito Alta";
        case ALTA: return "Alta";
        case MEDIA: return "Média";
        case BAIXA: return "Baixa";
        case MUITO_BAIXA: return "Muito Baixa";
        default: return "Desconhecida";
    }
}

void carregarPacientes(Paciente pacientes[], int *num_pacientes) {
    FILE *arquivo = fopen("pacientes.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de pacientes.\n");
        exit(1);
    }

    *num_pacientes = 0;
    while (fscanf(arquivo, "%s %s %s %d %s %s %f %f\n",
                  pacientes[*num_pacientes].sintoma,
                  pacientes[*num_pacientes].nome,
                  pacientes[*num_pacientes].sobrenome,
                  &pacientes[*num_pacientes].idade,
                  pacientes[*num_pacientes].cpf,
                  pacientes[*num_pacientes].telefone,
                  &pacientes[*num_pacientes].peso,
                  &pacientes[*num_pacientes].altura) == 8) {
        pacientes[*num_pacientes].id = *num_pacientes + 1; // IDs únicos baseados na ordem de leitura
        pacientes[*num_pacientes].prioridade = rand() % 6;
        pacientes[*num_pacientes].faltas = 0;
        (*num_pacientes)++;
    }

    fclose(arquivo);
}


void inicializarBancoReservas(Paciente pacientes[], int num_pacientes) {
    if (num_pacientes < TAM_BANCO) {
        printf("Não há pacientes suficientes para o banco de reservas.\n");
        exit(1);
    }

    for (int i = 0; i < TAM_BANCO; i++) {
        bancoReservas[i] = pacientes[num_pacientes - TAM_BANCO + i].id;
    }
}
void exibirBancoReservas(Paciente pacientes[], int num_pacientes) {
    printf("\nBanco de Reservas:\n");
    for (int i = 0; i < TAM_BANCO; i++) {
        int id = bancoReservas[i];
        for (int j = 0; j < num_pacientes; j++) {
            if (pacientes[j].id == id) {
                printf("Paciente %d: %s %s, ID: %d\n", i + 1, pacientes[j].nome, pacientes[j].sobrenome, id);
                break;
            }
        }
    }
}
void inicializarMedicos(Medico medicos[], int *num_medicos) {
    const char *nomes[] = {"Dr. Vinicius", "Dr. Carlos", "Dr. Pedro", "Dr. Vinera"};
    const char *especialidades[] = {"Clínico Geral", "Cardiologia", "Neurologia", "Pediatria"};
    *num_medicos = 4;
    for (int i = 0; i < *num_medicos; i++) {
        strcpy(medicos[i].nome, nomes[i]);
        strcpy(medicos[i].especialidade, especialidades[i]);
        medicos[i].horas_trabalhadas = 0;
    }
}

int compararPrioridade(const void *a, const void *b) {
    Paciente *pacienteA = (Paciente *)a;
    Paciente *pacienteB = (Paciente *)b;
    return pacienteA->prioridade - pacienteB->prioridade;
}

int encontrarPacientePorId(Paciente pacientes[], int num_pacientes, int id) {
    for (int i = 0; i < num_pacientes; i++) {
        if (pacientes[i].id == id) {
            return i;
        }
    }
    return -1;
}

void alocarConsultas(Paciente pacientes[], int *num_pacientes, Medico medicos[], int num_medicos, Sala salas[][MAX_SALAS], int *num_semanas) {
    int pacientes_atendidos = 0;
    *num_semanas = 0;

    qsort(pacientes, *num_pacientes, sizeof(Paciente), compararPrioridade);

    while (pacientes_atendidos < *num_pacientes) {
        for (int sala = 0; sala < MAX_SALAS; sala++) {
            for (int hora = 0; hora < MAX_HORARIOS; hora++) {
                if (pacientes_atendidos >= *num_pacientes) break;

                // Selecionar um médico disponível
                int medico_encontrado = -1;
                for (int m = 0; m < num_medicos; m++) {
                    int medico_em_uso = 0;
                    for (int s = 0; s < MAX_SALAS; s++) {
                        if (strcmp(salas[*num_semanas][s].horarios[hora].medico, medicos[m].nome) == 0) {
                            medico_em_uso = 1;
                            break;
                        }
                    }
                    if (!medico_em_uso) {
                        medico_encontrado = m;
                        break;
                    }
                }

                if (medico_encontrado == -1) continue;

                char nome_completo[MAX_NOME * 2];
                snprintf(nome_completo, sizeof(nome_completo), "%s %s",
                         pacientes[pacientes_atendidos].nome,
                         pacientes[pacientes_atendidos].sobrenome);

                strcpy(salas[*num_semanas][sala].horarios[hora].paciente, nome_completo);
                strcpy(salas[*num_semanas][sala].horarios[hora].medico, medicos[medico_encontrado].nome);

                medicos[medico_encontrado].horas_trabalhadas++;
                pacientes_atendidos++;
            }
        }
        (*num_semanas)++;
    }
}

void gerenciarFaltasComReservas(Paciente pacientes[], int *num_pacientes, Sala salas[][MAX_SALAS], int semana, int sala, int hora, FILE *arquivo) {
    if (rand() % 100 < 5) { // 5% de chance de falta
        char nome_paciente[MAX_NOME * 2];
        strcpy(nome_paciente, salas[semana][sala].horarios[hora].paciente);

        for (int i = 0; i < *num_pacientes; i++) {
            char nome_completo[MAX_NOME * 2];
            snprintf(nome_completo, sizeof(nome_completo), "%s %s", pacientes[i].nome, pacientes[i].sobrenome);

            if (strcmp(nome_completo, nome_paciente) == 0) {
                // Registrando a falta do paciente
                fprintf(arquivo, "Semana %d, Sala %d, Horário %d: Paciente '%s' faltou.\n", 
                        semana + 1, sala + 1, hora + 1, nome_completo);

                int substituido = 0; // Variável para indicar se houve substituição
                // Percorre o banco de reservas para encontrar um substituto
                for (int j = 0; j < TAM_BANCO; j++) {
                    int substituto_id = bancoReservas[j];
                    for (int k = 0; k < *num_pacientes; k++) {
                        if (pacientes[k].id == substituto_id) {
                            // Substitui o paciente faltoso pelo do banco de reservas
                            snprintf(nome_completo, sizeof(nome_completo), "%s %s", pacientes[k].nome, pacientes[k].sobrenome);
                            fprintf(arquivo, "Substituído por: %s do banco de reservas.\n", nome_completo);
                            
                            // Atualiza o horário da consulta
                            strcpy(salas[semana][sala].horarios[hora].paciente, nome_completo);

                            // Atualiza o banco de reservas: retira o substituto e coloca o paciente faltoso no final
                            for (int m = j; m < TAM_BANCO - 1; m++) {
                                bancoReservas[m] = bancoReservas[m + 1];
                            }
                            bancoReservas[TAM_BANCO - 1] = pacientes[i].id; // Coloca o paciente faltoso no final

                            substituido = 1; // Indica que a substituição foi feita
                            break;
                        }
                    }
                    if (substituido) break; // Se já substituiu, sai do loop
                }

                if (!substituido) { // Se não houve substituto no banco de reservas
                    fprintf(arquivo, "Não há substituto disponível para o paciente '%s'.\n", nome_completo);
                    strcpy(salas[semana][sala].horarios[hora].paciente, "Faltou");
                }
                break;
            }
        }
    }
}

void gerarRelatorio(Paciente pacientes[], Medico medicos[], int num_medicos, Sala salas[][MAX_SALAS], int num_semanas) {
    FILE *arquivo = fopen("relatorio.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo de relatório.\n");
        return;
    }

    fprintf(arquivo, "Relatório Geral\n\n");

    // Relatório dos médicos
    for (int i = 0; i < num_medicos; i++) {
        fprintf(arquivo, "Médico: %s, Horas Trabalhadas: %d\n", medicos[i].nome, medicos[i].horas_trabalhadas);
    }

    // Relatório das consultas
    for (int i = 0; i < num_semanas; i++) {
        fprintf(arquivo, "\nDia %d:\n", i + 1);
        for (int j = 0; j < MAX_SALAS; j++) {
            fprintf(arquivo, "Sala %d:\n", j + 1);
            for (int k = 0; k < MAX_HORARIOS; k++) {
                char *paciente = salas[i][j].horarios[k].paciente;
                char *medico = salas[i][j].horarios[k].medico;

                // Busca pela prioridade do paciente
                const char *prioridade = NULL;
                for (int p = 0; p < MAX_PACIENTES; p++) {
                    char nome_completo[MAX_NOME * 2];
                    snprintf(nome_completo, sizeof(nome_completo), "%s %s", pacientes[p].nome, pacientes[p].sobrenome);
                    if (strcmp(paciente, nome_completo) == 0) {
                        prioridade = prioridadeParaString(pacientes[p].prioridade);
                        break;
                    }
                }

                // Se o paciente for "Faltou" ou substituto, a prioridade será omitida
                if (strcmp(paciente, "Faltou") == 0 || prioridade == NULL) {
                    fprintf(arquivo, "  Horário %d: Paciente: %s, Médico: %s\n", k + 1, paciente, medico);
                } else {
                    fprintf(arquivo, "  Horário %d: Paciente: %s (Prioridade: %s), Médico: %s\n", k + 1, paciente, prioridade, medico);
                }
            }
        }
    }

    fclose(arquivo);
}

int main() {
    srand(time(NULL));

    Paciente pacientes[MAX_PACIENTES];
    int num_pacientes;

    Medico medicos[MAX_SALAS];
    int num_medicos;

    Sala salas[MAX_SEMANAS][MAX_SALAS];
    memset(salas, 0, sizeof(salas));

    int num_semanas;

    carregarPacientes(pacientes, &num_pacientes);
    inicializarBancoReservas(pacientes, num_pacientes);
    inicializarMedicos(medicos, &num_medicos);
    alocarConsultas(pacientes, &num_pacientes, medicos, num_medicos, salas, &num_semanas);

    FILE *arquivo = fopen("resultado_simulacao.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo de resultado.\n");
        exit(1);
    }

    for (int semana = 0; semana < num_semanas; semana++) {
        for (int sala = 0; sala < MAX_SALAS; sala++) {
            for (int hora = 0; hora < MAX_HORARIOS; hora++) {
                gerenciarFaltasComReservas(pacientes, &num_pacientes, salas, semana, sala, hora, arquivo);
            }
        }
    }

    gerarRelatorio(pacientes, medicos, num_medicos, salas, num_semanas);
    fclose(arquivo);

    return 0;
}
