#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // Para gerar numeros aleatorios

#define MAX_NOME 100
#define MAX_SINTOMAS 1
#define MAX_PACIENTES 100
#define MAX_SALAS 4
#define MAX_PACIENTES_POR_DIA 8  // Cada medico pode atender 7 pacientes por dia
#define HORARIOS_FUNCIONAMENTO 8 // De 08:00h as 16:00h, temos 9 horarios
#define MAX_DIAS 30  // Máximo de dias para agendamento

typedef enum {
    DOR_NO_PEITO,
    DOR_DE_CABECA,
    DOR_ABDOMINAL,
    FEBRE,
    CANSACO,
    ROTINA
} Sintoma;

typedef enum {
    PEDIATRA,
    CARDIOLOGISTA,
    NEUROLOGISTA,
    CLINICO_GERAL
} Especialidade;

typedef enum {
    BAIXA,
    MEDIA,
    ALTA,
    MUITO_ALTA,
    CRITICA
} Prioridade;

typedef struct {
    char nome[MAX_NOME];
    char sobrenome[MAX_NOME];
    int idade;
    char cpf[12];
    char telefone[15];
    float peso;
    float altura;
    Sintoma sintomas[MAX_SINTOMAS];
    int num_sintomas;
    Especialidade especialidade_atendimento;
    Prioridade prioridade;
    int agendado;
    int horario_agendado;
    int dia_agendado;  // Indica o dia em que o paciente foi agendado
} Paciente;

typedef struct {
    int numero;
    Especialidade especialidade;
    char nome_medico[MAX_NOME];
    Paciente pacientes_agendados[MAX_DIAS][MAX_PACIENTES_POR_DIA];  // Pacientes agendados por dia
    int pacientes_count[MAX_DIAS];  // Quantos pacientes por dia
    int horarios_ocupados[MAX_DIAS][HORARIOS_FUNCIONAMENTO];  // Marca horários ocupados por dia
} Sala;

Sintoma converter_sintoma_para_enum(const char *sintoma_str) {
    if (strcmp(sintoma_str, "DorNoPeito") == 0) return DOR_NO_PEITO;
    if (strcmp(sintoma_str, "DorDeCabeca") == 0) return DOR_DE_CABECA;
    if (strcmp(sintoma_str, "DorAbdominal") == 0) return DOR_ABDOMINAL;
    if (strcmp(sintoma_str, "Febre") == 0) return FEBRE;
    if (strcmp(sintoma_str, "Cansaco") == 0) return CANSACO;
    return ROTINA;
}

Especialidade determinar_especialidade(int idade, Sintoma sintoma) {
    if (idade >= 1 && idade <= 15) return PEDIATRA;

    switch (sintoma) {
        case DOR_NO_PEITO:
            return CARDIOLOGISTA;
        case DOR_DE_CABECA:
            return NEUROLOGISTA;
        default:
            return CLINICO_GERAL;
    }
}

Prioridade gerar_prioridade_aleatoria() {
    Prioridade prioridades[] = { BAIXA, MEDIA, ALTA, MUITO_ALTA, CRITICA };
    return prioridades[rand() % 5];
}

int verificar_duplicata(Paciente pacientes[], int num_pacientes, const char *cpf) {
    for (int i = 0; i < num_pacientes; i++) {
        if (strcmp(pacientes[i].cpf, cpf) == 0) {
            return 1; // CPF já existe
        }
    }
    return 0; // CPF não encontrado
}

void cadastrar_pacientes_do_arquivo(const char *nome_arquivo, Paciente pacientes[], int *num_pacientes) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), arquivo)) {
        Paciente paciente;
        char sintoma1[MAX_NOME], sintoma2[MAX_NOME];

        if (sscanf(linha, "%s %s %s %d %s %s %f %f",
                   sintoma1, paciente.nome, paciente.sobrenome,
                   &paciente.idade, paciente.cpf, paciente.telefone,
                   &paciente.peso, &paciente.altura) != 8) {
            continue;
        }

        if (verificar_duplicata(pacientes, *num_pacientes, paciente.cpf)) {
            continue;
        }

        paciente.sintomas[0] = converter_sintoma_para_enum(sintoma1);
        paciente.num_sintomas = 1;
        paciente.especialidade_atendimento = determinar_especialidade(paciente.idade, paciente.sintomas[0]);
        paciente.prioridade = gerar_prioridade_aleatoria();
        paciente.agendado = 0;

        pacientes[*num_pacientes] = paciente;
        (*num_pacientes)++;
    }

    fclose(arquivo);
    printf("Pacientes cadastrados com sucesso.\n");
}

void agendar_horarios(Sala salas[], int num_salas, Paciente pacientes[], int num_pacientes) {
    // Ordenar os pacientes pela prioridade (da mais alta para a mais baixa)
    for (int i = 0; i < num_pacientes - 1; i++) {
        for (int j = i + 1; j < num_pacientes; j++) {
            if (pacientes[i].prioridade < pacientes[j].prioridade) {
                Paciente temp = pacientes[i];
                pacientes[i] = pacientes[j];
                pacientes[j] = temp;
            }
        }
    }

    // Agendar os pacientes de acordo com a prioridade
    for (int i = 0; i < num_pacientes; i++) {
        if (pacientes[i].agendado) continue;  // Ignorar pacientes já agendados

        for (int j = 0; j < num_salas; j++) {
            if (salas[j].especialidade == pacientes[i].especialidade_atendimento) {
                for (int d = 0; d < MAX_DIAS; d++) {  // Iterar pelos dias
                    if (salas[j].pacientes_count[d] < MAX_PACIENTES_POR_DIA) {
                        for (int h = 0; h < HORARIOS_FUNCIONAMENTO; h++) {
                            if (salas[j].horarios_ocupados[d][h] == 0) {
                                // Agendar o paciente
                                salas[j].pacientes_agendados[d][salas[j].pacientes_count[d]] = pacientes[i];
                                salas[j].pacientes_agendados[d][salas[j].pacientes_count[d]].horario_agendado = h;
                                salas[j].pacientes_agendados[d][salas[j].pacientes_count[d]].dia_agendado = d + 1;  // Dia começa em 1
                                salas[j].pacientes_count[d]++;
                                salas[j].horarios_ocupados[d][h] = 1;

                                pacientes[i].agendado = 1;
                                pacientes[i].horario_agendado = h;
                                pacientes[i].dia_agendado = d + 1;

                                goto PROXIMO_PACIENTE;  // Prosseguir para o próximo paciente
                            }
                        }
                    }
                }
            }
        }
    PROXIMO_PACIENTE:;
    }
}

void exibir_horarios_agendados(Sala salas[], int num_salas, const char *nome_arquivo, const char *titulo) {
    FILE *arquivo = fopen(nome_arquivo, "a"); // Abrir em modo de append para não sobrescrever o arquivo
    if (arquivo == NULL) {
        perror("Erro ao criar o arquivo de horários");
        return;
    }

    fprintf(arquivo, "%s\n\n", titulo);

    for (int i = 0; i < num_salas; i++) {
        fprintf(arquivo, "Horários agendados para a sala %d (Médico: %s):\n", salas[i].numero, salas[i].nome_medico);

        for (int d = 0; d < MAX_DIAS; d++) {
            if (salas[i].pacientes_count[d] == 0) continue;  // Pular dias sem pacientes

            fprintf(arquivo, "  Dia %d:\n", d + 1);
            for (int j = 0; j < salas[i].pacientes_count[d]; j++) {
                Paciente p = salas[i].pacientes_agendados[d][j];
                fprintf(arquivo, "    %d:00h - %s %s (Prioridade: ", 8 + p.horario_agendado, p.nome, p.sobrenome);

                // Exibir a prioridade
                switch (p.prioridade) {
                    case BAIXA: fprintf(arquivo, "Baixa"); break;
                    case MEDIA: fprintf(arquivo, "Média"); break;
                    case ALTA: fprintf(arquivo, "Alta"); break;
                    case MUITO_ALTA: fprintf(arquivo, "Muito Alta"); break;
                    case CRITICA: fprintf(arquivo, "Crítica"); break;
                }
                fprintf(arquivo, ")\n");
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

void exibir_dados_pacientes(Paciente pacientes[], int num_pacientes) {
    printf("Lista de Pacientes Cadastrados:\n");
    for (int i = 0; i < num_pacientes; i++) {
        printf("\nNome: %s %s\n", pacientes[i].nome, pacientes[i].sobrenome);
        printf("Idade: %d\n", pacientes[i].idade);
        printf("CPF: %s\n", pacientes[i].cpf);
        printf("Telefone: %s\n", pacientes[i].telefone);
        printf("Peso: %.2f\n", pacientes[i].peso);
        printf("Altura: %.2f\n", pacientes[i].altura);
        printf("Prioridade: ");
        switch (pacientes[i].prioridade) {
            case BAIXA: printf("Baixa\n"); break;
            case MEDIA: printf("Media\n"); break;
            case ALTA: printf("Alta\n"); break;
            case MUITO_ALTA: printf("Muito Alta\n"); break;
            case CRITICA: printf("Critica\n"); break;
        }
        printf("Especialidade: ");
        switch (pacientes[i].especialidade_atendimento) {
            case PEDIATRA: printf("Pediatra\n"); break;
            case CARDIOLOGISTA: printf("Cardiologista\n"); break;
            case NEUROLOGISTA: printf("Neurologista\n"); break;
            case CLINICO_GERAL: printf("Clinico Geral\n"); break;
        }
    }
}

void simular(Sala salas[], int num_salas, Paciente pacientes[], int num_pacientes) {
    int pacientes_cadastrados = 0;
    int horarios_agendados = 0;

    // Verificar se há pacientes cadastrados
    for (int i = 0; i < num_pacientes; i++) {
        if (strlen(pacientes[i].cpf) > 0) {
            pacientes_cadastrados++;
        }
    }

    if (pacientes_cadastrados == 0) {
        printf("Nenhum paciente cadastrado. Por favor, cadastre os pacientes antes de simular.\n");
        return;
    }

    // Verificar se há horários agendados
    for (int i = 0; i < num_pacientes; i++) {
        if (pacientes[i].agendado == 1) {
            horarios_agendados++;
        }
    }

    if (horarios_agendados == 0) {
        printf("Nenhum paciente agendado. Por favor, agende os horários antes de simular.\n");
        return;
    }

    printf("Simulação iniciada com sucesso!\n");

    srand(time(NULL));  // Inicializar gerador de números aleatórios

    for (int i = 0; i < num_pacientes; i++) {
        if (pacientes[i].agendado == 1) {
            // Simular faltas e retornos
            if (rand() % 100 < 5) {  // 5% de chance de faltar
                // Reduzir prioridade, etc.
            }
            if (rand() % 100 < 10) {  // 10% de chance de retorno
                // Atualizar horário e prioridade, etc.
            }
        }
    }

    // Gerar tabela atualizada de horários
    exibir_horarios_agendados(salas, num_salas, "horarios_simulacao.txt", "Horários após a simulação");
}

void menu() {
    Paciente pacientes[MAX_PACIENTES];
    Sala salas[MAX_SALAS] = {
        {1, PEDIATRA, "Dr. Maria", {}, 0, {0}},
        {2, CARDIOLOGISTA, "Dr. Carlos", {}, 0, {0}},
        {3, NEUROLOGISTA, "Dr. Ana", {}, 0, {0}},
        {4, CLINICO_GERAL, "Dr. Lucas", {}, 0, {0}},
    };
    int num_pacientes = 0;

    int opcao;
    while (1) {
        printf("Escolha uma opcao:\n");
        printf("1. Ver pacientes\n");
        printf("2. Ver salas de atendimento\n");
        printf("3. Ver horarios de funcionamento\n");
        printf("4. Cadastrar pacientes do arquivo\n");
        printf("5. Agendar horarios\n");
        printf("6. Exibir horarios agendados\n");
        printf("7. Simular atendimento\n");
        printf("8. Sair\n");
        printf(": ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                exibir_dados_pacientes(pacientes, num_pacientes);
                break;
            case 2:
                printf("Salas de atendimento:\n");
                for (int i = 0; i < MAX_SALAS; i++) {
                    printf("Sala %d - Medico: %s, Especialidade: ", salas[i].numero, salas[i].nome_medico);
                    switch (salas[i].especialidade) {
                        case PEDIATRA: printf("Pediatra\n"); break;
                        case CARDIOLOGISTA: printf("Cardiologista\n"); break;
                        case NEUROLOGISTA: printf("Neurologista\n"); break;
                        case CLINICO_GERAL: printf("Clinico Geral\n"); break;
                    }
                }
                break;
            case 3:
                printf("Horarios de funcionamento: 08:00h - 16:00h\n");
                break;
            case 4:
                cadastrar_pacientes_do_arquivo("pacientes.txt", pacientes, &num_pacientes);
                break;
            case 5:
                agendar_horarios(salas, MAX_SALAS, pacientes, num_pacientes);
                break;
            case 6:
                exibir_horarios_agendados(salas, MAX_SALAS, "horarios_simulacao.txt", "Horários antes da simulação");
                break;
            case 7:
                simular(salas, MAX_SALAS, pacientes, num_pacientes);
                break;
            case 8:
                printf("Saindo...\n");
                return;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}
