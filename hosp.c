//automatizado
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

typedef enum { CRITICO, MUITO_ALTA, ALTA, MEDIA, BAIXA, MUITO_BAIXA } Prioridade;

typedef struct {
    char sintoma[MAX_SINTOMA];
    char nome[MAX_NOME];
    char sobrenome[MAX_NOME];
    int idade;
    char cpf[MAX_CPF];
    char telefone[MAX_TELEFONE];
    float peso;
    float altura;
    Prioridade prioridade;
} Paciente;

typedef struct {
    char nome[MAX_NOME];
    int horas_trabalhadas;
} Medico;

typedef struct {
    char paciente[MAX_NOME];
    char medico[MAX_NOME];
} Horario;

typedef struct {
    Horario horarios[MAX_HORARIOS];
} Sala;

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
        // Atribuindo uma prioridade aleatória a cada paciente
        pacientes[*num_pacientes].prioridade = rand() % 6;
        (*num_pacientes)++;
    }

    fclose(arquivo);
}

void inicializarMedicos(Medico medicos[], int *num_medicos) {
    const char *nomes[] = {"Dr. Vinicius", "Dr. Carlos", "Dr. Pedro", "Dr. Vinera"};
    *num_medicos = 4;
    for (int i = 0; i < *num_medicos; i++) {
        strcpy(medicos[i].nome, nomes[i]);
        medicos[i].horas_trabalhadas = 0;
    }
}

int compararPrioridade(const void *a, const void *b) {
    Paciente *pacienteA = (Paciente *)a;
    Paciente *pacienteB = (Paciente *)b;
    return pacienteA->prioridade - pacienteB->prioridade;
}

void alocarConsultas(Paciente pacientes[], int num_pacientes, Medico medicos[], int num_medicos, Sala salas[][MAX_SALAS], int *num_semanas) {
    int pacientes_atendidos = 0;
    *num_semanas = 0;
    int medico_atual = 0;

    // Ordena os pacientes com base na sua prioridade
    qsort(pacientes, num_pacientes, sizeof(Paciente), compararPrioridade);

    while (pacientes_atendidos < num_pacientes) {
        for (int sala = 0; sala < MAX_SALAS; sala++) {
            for (int hora = 0; hora < MAX_HORARIOS; hora++) {
                if (pacientes_atendidos >= num_pacientes) break;

                char nome_completo[MAX_NOME * 2];
                snprintf(nome_completo, sizeof(nome_completo), "%s %s",
                         pacientes[pacientes_atendidos].nome,
                         pacientes[pacientes_atendidos].sobrenome);

                strcpy(salas[*num_semanas][sala].horarios[hora].paciente, nome_completo);
                strcpy(salas[*num_semanas][sala].horarios[hora].medico, medicos[medico_atual].nome);

                medicos[medico_atual].horas_trabalhadas++;
                pacientes_atendidos++;
                medico_atual = (medico_atual + 1) % num_medicos;
            }
        }
        (*num_semanas)++;
    }
}
void gerenciarFaltas(Paciente pacientes[], int *num_pacientes, Sala salas[][MAX_SALAS], int semana, int sala, int hora, char *faltou, char *substituto) {
    strcpy(faltou, "");
    strcpy(substituto, "");

    // Simular a falta com 5% de probabilidade
    if (rand() % 100 < 5) {
        strcpy(faltou, salas[semana][sala].horarios[hora].paciente);

        // Substituir pelo primeiro paciente do dia seguinte, se disponível
        for (int prox_sala = 0; prox_sala < MAX_SALAS; prox_sala++) {
            for (int prox_hora = 0; prox_hora < MAX_HORARIOS; prox_hora++) {
                if (semana + 1 < MAX_SEMANAS && salas[semana + 1][prox_sala].horarios[prox_hora].paciente[0] != '\0') {
                    strcpy(substituto, salas[semana + 1][prox_sala].horarios[prox_hora].paciente);
                    strcpy(salas[semana][sala].horarios[hora].paciente, substituto);
                    salas[semana + 1][prox_sala].horarios[prox_hora].paciente[0] = '\0';
                    return;
                }
            }
        }
    }
}

void gerarRelatorio(Paciente pacientes[], Medico medicos[], int num_medicos, Sala salas[][MAX_SALAS], int num_semanas, int num_pacientes) {
    FILE *arquivo = fopen("resultado_simulacao.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo de resultado.\n");
        exit(1);
    }

    fprintf(arquivo, "Número de semanas necessárias: %d\n\n", num_semanas);
    fprintf(arquivo, "Ocupação das salas:\n");

    for (int semana = 0; semana < num_semanas; semana++) {
        fprintf(arquivo, "Semana %d:\n", semana + 1);
        for (int sala = 0; sala < MAX_SALAS; sala++) {
            fprintf(arquivo, " Sala %d:\n", sala + 1);
            for (int hora = 0; hora < MAX_HORARIOS; hora++) {
                if (salas[semana][sala].horarios[hora].paciente[0] != '\0') {
                    char faltou[MAX_NOME * 2], substituto[MAX_NOME * 2];
                    char paciente_atual[MAX_NOME * 2];
                    strcpy(paciente_atual, salas[semana][sala].horarios[hora].paciente);

                    // Verificar faltas
                    gerenciarFaltas(pacientes, &num_pacientes, salas, semana, sala, hora, faltou, substituto);

                    // Localizar prioridade do paciente
                    Prioridade prioridade_paciente = MEDIA;
                    for (int i = 0; i < num_pacientes; i++) {
                        char nome_completo[MAX_NOME * 2];
                        snprintf(nome_completo, sizeof(nome_completo), "%s %s",
                                 pacientes[i].nome, pacientes[i].sobrenome);
                        if (strcmp(nome_completo, paciente_atual) == 0) {
                            prioridade_paciente = pacientes[i].prioridade;
                            break;
                        }
                    }

                    fprintf(arquivo, "  Horário %d: Paciente: %s (Prioridade: %s), Médico: %s\n",
                            hora + 1,
                            paciente_atual,
                            prioridadeParaString(prioridade_paciente),
                            salas[semana][sala].horarios[hora].medico);

                    if (strlen(faltou) > 0) {
                        fprintf(arquivo, "   -> Paciente faltou: %s\n", faltou);
                        if (strlen(substituto) > 0) {
                            fprintf(arquivo, "   -> Substituído por: %s\n", substituto);
                        } else {
                            fprintf(arquivo, "   -> Sem substituição disponível.\n");
                        }
                    }
                }
            }
        }
        fprintf(arquivo, "\n");
    }

    fprintf(arquivo, "Relatório de horas trabalhadas por médico:\n");
    for (int i = 0; i < num_medicos; i++) {
        fprintf(arquivo, " Médico: %s, Horas Trabalhadas: %d\n",
                medicos[i].nome,
                medicos[i].horas_trabalhadas);
    }

    fclose(arquivo);
    printf("Relatório gerado em 'resultado_simulacao.txt'\n");
}

int main() {
    Paciente pacientes[MAX_PACIENTES];
    Medico medicos[MAX_SALAS];
    Sala salas[MAX_SEMANAS][MAX_SALAS];
    int num_pacientes, num_medicos, num_semanas;

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    carregarPacientes(pacientes, &num_pacientes);
    inicializarMedicos(medicos, &num_medicos);
    alocarConsultas(pacientes, num_pacientes, medicos, num_medicos, salas, &num_semanas);
    gerarRelatorio(pacientes, medicos, num_medicos, salas, num_semanas, num_pacientes);

    return 0;
}
