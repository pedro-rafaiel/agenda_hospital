## Simulador de Gerenciamento de Consultas Médicas

Este programa em C simula o agendamento e gerenciamento de consultas médicas. Ele aloca pacientes para consultas com base em prioridades, gerencia faltas com um sistema de substituição de banco de reservas, e gera relatórios detalhados sobre o processo.

### **Funcionalidades**
- Carregamento de dados de pacientes a partir de um arquivo `pacientes.txt`.
- Alocação de consultas com base na prioridade dos pacientes.
- Gerenciamento de faltas com substituição dinâmica de pacientes pelo banco de reservas.
- Geração de relatórios com o histórico de consultas realizadas e faltas registradas.

---

## **Estrutura do Projeto**

### **Arquivos**
1. **`main.c`**  
   Contém o código principal do programa.
   
2. **`pacientes.txt`**  
   Arquivo contendo os dados dos pacientes. O formato é o seguinte:
   ```
   sintoma nome sobrenome idade cpf telefone peso altura
   ```
   Exemplo:
   ```
   Febre Joao Silva 30 12345678901 11987654321 70.5 1.75
   Dor Maria Oliveira 25 98765432109 11912345678 65.0 1.68
   ```

3. **`relatorio.txt`**  
   Gerado automaticamente após a execução do programa. Contém o relatório das consultas realizadas.

4. **`banco_de_reservas.txt`**  
   Registra as faltas dos pacientes e os substitutos utilizados.

---

## **Como Executar**

### **Pré-requisitos**
- **Code::Blocks** ou **Visual Studio Code** (com extensão de C/C++).
- Compilador GCC ou MinGW instalado.
- Biblioteca padrão de C (inclusa em compilações comuns de GCC ou MinGW).

---

### **Execução no Code::Blocks**
1. Abra o **Code::Blocks**.
2. Crie um novo projeto:
   - **File** > **New** > **Project** > **Console Application**.
3. Escolha `C` como linguagem.
4. Copie o conteúdo de `main.c` para o arquivo principal do projeto.
5. Certifique-se de que o arquivo `pacientes.txt` está na mesma pasta do executável gerado.
6. Compile e execute:
   - **Build** > **Build and Run**.

---

### **Execução no Visual Studio Code**
1. Instale o [Visual Studio Code](https://code.visualstudio.com/) e a extensão **C/C++**:
   - Acesse **Extensions** (ícone de cubo) e procure por **C/C++**.
2. Configure o ambiente:
   - Certifique-se de que o GCC ou MinGW está instalado e configurado no PATH do sistema.
3. Crie um arquivo `main.c` e cole o código.
4. Certifique-se de que o arquivo `pacientes.txt` está no mesmo diretório.
5. Compile e execute:
   - Abra o terminal integrado (**Ctrl + `**).
   - Compile o código com:
     ```
     gcc main.c -o simulador
     ```
   - Execute com:
     ```
     ./simulador
     ```

---

## **Personalizações**
- **Configuração do banco de reservas**: O programa garante que os 5 últimos pacientes do arquivo `pacientes.txt` são reservados exclusivamente para substituições.
- **Probabilidade de faltas**: Definida como 5% no código (linha `if (rand() % 100 < 5)`).

---

## **Relatórios**
1. **`banco_de_reservas.txt`**  
   Detalha:
   - Pacientes que faltaram.
   - Substitutos utilizados ou ausência de substitutos disponíveis.

2. **`relatorio.txt`**  
   Contém:
   - Horas trabalhadas por cada médico.
   - Consultas realizadas, incluindo prioridades dos pacientes atendidos.

---

## **Autor**
Este projeto é uma simulação educacional para gerenciamento de consultas em clínicas médicas.
Feito como trabalho prático para a cadeira de Algoritmos e Estruturas de Dados II, Professor:Carlos Vinícius, Semestre 2024.1
Feito pelos alunos Pedro Rafael Pereira de Oliveira e Vinícius Inácio dos Santos
