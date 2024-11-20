#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  // Para gerar numeros aleatorios

#define MAX_NOME 100
#define MAX_SINTOMAS 2
#define MAX_PACIENTES 100
#define MAX_SALAS 5
#define MAX_PACIENTES_POR_DIA 7  // Cada medico pode atender 7 pacientes por dia
#define HORARIOS_FUNCIONAMENTO 9 // De 08:00h as 16:00h, temos 9 horarios

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
    Paciente pacientes_agendados[MAX_PACIENTES_POR_DIA];  // Pacientes agendados por medico
    int pacientes_count;  // Quantos pacientes o medico ja agendou
    int horarios_ocupados[HORARIOS_FUNCIONAMENTO];  // Marca os horarios ocupados
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

        if (sscanf(linha, "%s %s %s %s %d %s %s %f %f",
                   sintoma1, sintoma2, paciente.nome, paciente.sobrenome,
                   &paciente.idade, paciente.cpf, paciente.telefone,
                   &paciente.peso, &paciente.altura) != 9) {
            continue;
        }

        if (verificar_duplicata(pacientes, *num_pacientes, paciente.cpf)) {
            continue;
        }

        paciente.sintomas[0] = converter_sintoma_para_enum(sintoma1);
        paciente.sintomas[1] = converter_sintoma_para_enum(sintoma2);
        paciente.num_sintomas = 2;
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
        if (pacientes[i].agendado) continue; // Ignorar pacientes já agendados

        for (int j = 0; j < num_salas; j++) {
            if (salas[j].especialidade == pacientes[i].especialidade_atendimento) {
                if (salas[j].pacientes_count < MAX_PACIENTES_POR_DIA) {
                    for (int h = 0; h < HORARIOS_FUNCIONAMENTO; h++) {
                        if (salas[j].horarios_ocupados[h] == 0) {
                            // Agendar o paciente
                            salas[j].pacientes_agendados[salas[j].pacientes_count] = pacientes[i];
                            salas[j].pacientes_agendados[salas[j].pacientes_count].horario_agendado = h;
                            salas[j].pacientes_count++;
                            salas[j].horarios_ocupados[h] = 1;

                            pacientes[i].agendado = 1;
                            pacientes[i].horario_agendado = h;
                            pacientes[i].dia_agendado = 0; // Primeiro dia

                            break;
                        }
                    }
                }
            }
        }
    }
}

void exibir_horarios_agendados(Sala salas[], int num_salas) {
    for (int i = 0; i < num_salas; i++) {
        printf("Horarios agendados para a sala %d (Medico: %s):\n", salas[i].numero, salas[i].nome_medico);
        for (int j = 0; j < salas[i].pacientes_count; j++) {
            Paciente p = salas[i].pacientes_agendados[j];
            printf("    %d:00h - %s %s (Prioridade: ", 8 + p.horario_agendado, p.nome, p.sobrenome);

            // Exibir a prioridade
            switch (p.prioridade) {
                case BAIXA: printf("Baixa"); break;
                case MEDIA: printf("Media"); break;
                case ALTA: printf("Alta"); break;
                case MUITO_ALTA: printf("Muito Alta"); break;
                case CRITICA: printf("Critica"); break;
            }
            printf(")\n");
        }
        printf("\n");
    }
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
        printf("7. Sair\n");
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
                exibir_horarios_agendados(salas, MAX_SALAS);
                break;
            case 7:
                printf("Saindo...\n");
                return;
            default:
                printf("Opcao invalida!\n");
        }
    }
}

int main() {
    srand(time(NULL));
    menu();
    return 0;
}
