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

---

## Como Usar no Code::Blocks

### Passos para Configuração

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

---

## Como Usar no Visual Studio Code (VSCode)

### Passos para Configuração

1. **Instalar o Visual Studio Code**:
   - Se ainda não tem o Visual Studio Code, baixe e instale [aqui](https://code.visualstudio.com/).

2. **Instalar o compilador C (GCC)**:
   - **No Windows**: Instale o **MinGW** para obter o GCC. Siga o tutorial [aqui](https://code.visualstudio.com/docs/cpp/config-mingw).
   - **No Linux (Ubuntu/Debian)**: Execute o seguinte comando no terminal:
     ```bash
     sudo apt update
     sudo apt install build-essential
     ```
   - **No macOS**: Instale o **Xcode Command Line Tools** com o comando:
     ```bash
     xcode-select --install
     ```

3. **Instalar as Extensões Necessárias no VSCode**:
   - Abra o VSCode e vá para a aba de extensões (`Ctrl + Shift + X`).
   - Instale as seguintes extensões:
     - **C/C++**: Para suporte a compilação e depuração de programas C/C++.
     - **Code Runner** (opcional): Para rodar programas diretamente do VSCode sem precisar de configurações adicionais.

4. **Configurar o VSCode para Compilação e Execução**:
   - No VSCode, abra a pasta do seu projeto.
   - Selecione **Terminal** no menu superior e depois **New Terminal** para abrir o terminal integrado.
   - Se você não tiver um arquivo `tasks.json`, crie um para automatizar a compilação e execução:

     Vá para **Terminal > Configure Tasks > Create tasks.json from template > Others** e depois adicione a configuração abaixo no arquivo `tasks.json`:
     ```json
     {
       "version": "2.0.0",
       "tasks": [
         {
           "label": "build and run",
           "type": "shell",
           "command": "gcc",
           "args": [
             "-o",
             "sistema_agendamento",
             "sistema_agendamento.c",
             "&&",
             "./sistema_agendamento"
           ],
           "group": {
             "kind": "build",
             "isDefault": true
           }
         }
       ]
     }
     ```

5. **Compilar e Executar no VSCode**:
   - Com o arquivo `tasks.json` configurado, pressione `Ctrl + Shift + B` para compilar e rodar o programa diretamente no VSCode. O terminal integrado exibirá o menu do sistema, e você poderá interagir com ele.

### Compilar e Rodar Manualmente (sem `tasks.json`):
- Caso prefira, você pode compilar e rodar o código manualmente no terminal integrado do VSCode com os seguintes comandos:
  ```bash
  gcc -o sistema_agendamento sistema_agendamento.c
  ./sistema_agendamento
  ```

---

## Requisitos

- **Code::Blocks** ou **Visual Studio Code**
- **Compilador C** (GCC, Clang, MinGW)
- Sistema operacional: **Windows**, **Linux** ou **macOS**
