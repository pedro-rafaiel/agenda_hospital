//automatizado
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PACIENTES 100
#define MAX_SALAS 4
#define MAX_HORARIOS 8
#define MAX_SEMANAS 20
#define MAX_NOME 50
#define MAX_SINTOMA 20
#define MAX_TELEFONE 15
#define MAX_CPF 12

typedef struct {
    char sintoma[MAX_SINTOMA];
    char nome[MAX_NOME];
    char sobrenome[MAX_NOME];
    int idade;
    char cpf[MAX_CPF];
    char telefone[MAX_TELEFONE];
    float peso;
    float altura;
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

void alocarConsultas(Paciente pacientes[], int num_pacientes, Medico medicos[], int num_medicos, Sala salas[][MAX_SALAS], int *num_semanas) {
    int pacientes_atendidos = 0;
    *num_semanas = 0;
    int medico_atual = 0;

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

void gerarRelatorio(Medico medicos[], int num_medicos, Sala salas[][MAX_SALAS], int num_semanas) {
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
                    fprintf(arquivo, "  Horário %d: Paciente: %s, Médico: %s\n",
                            hora + 1,
                            salas[semana][sala].horarios[hora].paciente,
                            salas[semana][sala].horarios[hora].medico);
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

    carregarPacientes(pacientes, &num_pacientes);
    inicializarMedicos(medicos, &num_medicos);
    alocarConsultas(pacientes, num_pacientes, medicos, num_medicos, salas, &num_semanas);
    gerarRelatorio(medicos, num_medicos, salas, num_semanas);

    return 0;
}
