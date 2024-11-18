```markdown
# Sistema de Agendamento de Consultas Médicas

Este é um sistema simples para agendamento de consultas médicas em um hospital. O sistema permite que o usuário visualize pacientes cadastrados, consulte salas de atendimento, agende horários e exiba horários agendados.

## Funcionalidades

- **Cadastrar pacientes**: O sistema permite cadastrar pacientes a partir de um arquivo de texto.
- **Ver salas de atendimento**: Exibe as salas disponíveis, com a especialidade médica correspondente e o médico responsável.
- **Ver horários de funcionamento**: Informa os horários de funcionamento da clínica (das 08:00h às 16:00h).
- **Agendar consultas**: Permite agendar consultas para os pacientes nas salas disponíveis.
- **Exibir horários agendados**: Exibe todos os horários já agendados para as salas de atendimento.

## Estrutura do Código

O sistema é composto por funções que realizam as seguintes ações:

- `menu()`: Exibe o menu principal com as opções de navegação.
- `exibir_dados_pacientes()`: Exibe a lista de pacientes cadastrados.
- `cadastrar_pacientes_do_arquivo()`: Cadastra os pacientes a partir de um arquivo de texto.
- `agendar_horarios()`: Permite o agendamento dos pacientes nas salas de atendimento.
- `exibir_horarios_agendados()`: Exibe os horários de consulta já agendados nas salas.

## Estrutura do Arquivo

O sistema utiliza um arquivo de texto chamado `pacientes.txt` para carregar informações dos pacientes cadastrados. Este arquivo deve conter informações como nome, idade e especialidade dos pacientes.

## Como Usar

### Compilação e Execução no Terminal

1. Compile o código usando um compilador de C (por exemplo, GCC):
   
   ```bash
   gcc -o sistema_agendamento sistema_agendamento.c
   ```

2. Execute o programa:

   ```bash
   ./sistema_agendamento
   ```

3. O menu será exibido, e você poderá escolher a opção desejada (ver pacientes, ver salas, agendar consultas, etc.).

### Como Usar no Code::Blocks

1. **Baixe o Code::Blocks**: Se você ainda não tem o Code::Blocks instalado, pode baixá-lo [aqui](https://www.codeblocks.org/downloads/26).

2. **Criar um novo projeto**:

   - Abra o Code::Blocks e crie um novo projeto.
   - Selecione "Console application" e a linguagem "C".
   - Escolha um nome para o projeto e onde salvar os arquivos.

3. **Adicionar o código**:

   - No Code::Blocks, após criar o projeto, substitua o conteúdo do arquivo `main.c` com o código fonte do Sistema de Agendamento de Consultas Médicas.

4. **Criar o arquivo de pacientes**:

   - Crie um arquivo de texto chamado `pacientes.txt` no mesmo diretório do seu projeto.
   - Preencha o arquivo com dados de exemplo conforme mostrado no exemplo do sistema.

5. **Compilar e rodar**:

   - Para compilar o projeto, clique em "Build and Run" ou pressione `F9`.
   - O programa será compilado e executado no terminal do Code::Blocks, exibindo o menu interativo.

6. **Interagir com o programa**:

   - O menu aparecerá no console, e você poderá escolher as opções interativas para cadastrar pacientes, agendar consultas e visualizar as informações.

## Requisitos

- Code::Blocks (ou outro ambiente de desenvolvimento C)
- Compilador C (GCC, Clang, etc.)
- Arquivo `pacientes.txt` (opcional, mas necessário para carregar os pacientes)

## Mudanças para o Code::Blocks

A principal alteração aqui é que, no Code::Blocks, você não precisa compilar manualmente como faria no terminal. Basta pressionar `F9` (ou clicar em "Build and Run") para compilar e rodar o código diretamente.

O console dentro do Code::Blocks vai ser utilizado para exibir o menu e interagir com o programa. É onde o usuário poderá fazer as seleções interativas.

```
