#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#include <locale.h>
#include <ctime>

using namespace std;

// Estrutura para armazenar os dados do paciente
struct Paciente {
    char nome[50];
    char telefone[20];
    char email[50];
    char status;
    string medico;       
    string especializacao;
    string dataConsulta;
};

// Estrutura para armazenar os médicos disponíveis
struct Medico {
    string nome;
    string especializacao;
};

// Função para obter o caminho da pasta "Documentos"
string getUserFolderPath() {
    wchar_t path[MAX_PATH];
    if (SHGetFolderPathW(NULL, CSIDL_MYDOCUMENTS, NULL, 0, path) == S_OK) {
        char caminho[MAX_PATH];
        size_t convertedChars = 0;
        wcstombs_s(&convertedChars, caminho, MAX_PATH, path, _TRUNCATE);
        return string(caminho);
    }
    return "";
}

// Função para criar a pasta de relatórios, se não existir
string criarPastaRelatorios() {
    string pastaBase = getUserFolderPath() + "\\RelatoriosPaciente\\";
    if (_mkdir(pastaBase.c_str()) == 0 || errno == EEXIST) {
        return pastaBase;
    }
    else {
        return "";
    }
}

// Função para cadastrar um novo paciente
void cadastrar(Paciente pacientes[], int& numPacientes) {
    Paciente novoPaciente;
    char confirma;

    cout << "Cadastrando novo paciente:" << endl;
    cout << "Nome: ";
    cin.ignore();
    cin.getline(novoPaciente.nome, sizeof(novoPaciente.nome));
    cout << "Telefone: ";
    cin.getline(novoPaciente.telefone, sizeof(novoPaciente.telefone));
    cout << "E-mail: ";
    cin.getline(novoPaciente.email, sizeof(novoPaciente.email));

    cout << "Confirma cadastro? <s/n>: ";
    cin >> confirma;

    if (toupper(confirma) == 'S') {
        novoPaciente.status = 'A';
        pacientes[numPacientes] = novoPaciente;
        numPacientes++;
        cout << "Paciente cadastrado com sucesso!" << endl;
    }
    else {
        cout << "Cadastro cancelado." << endl;
    }
}

// Função para consultar um paciente pelo código
void consultarPaciente(Paciente pacientes[], int numPacientes) {
    int codigo;
    cout << "Digite o código do paciente (1 a " << numPacientes << "): ";
    cin >> codigo;
    if (codigo > 0 && codigo <= numPacientes) {
        Paciente p = pacientes[codigo - 1];
        cout << "Paciente encontrado:" << endl;
        cout << "Nome: " << p.nome << endl;
        cout << "Telefone: " << p.telefone << endl;
        cout << "E-mail: " << p.email << endl;
        cout << "Status: " << (p.status == 'A' ? "Ativo" : "Inativo") << endl;
        cout << "Médico: " << p.medico << endl;
        cout << "Especialização: " << p.especializacao << endl;
        cout << "Data da consulta: " << p.dataConsulta << endl;
    }
    else {
        cout << "Código inválido!" << endl;
    }
}

// Função para buscar um paciente pelo nome
int buscarPacientePorNome(Paciente pacientes[], int numPacientes, string nomePaciente) {
    for (int i = 0; i < numPacientes; i++) {
        if (nomePaciente == pacientes[i].nome) {
            return i;
        }
    }
    return -1;
}

// Função para agendar consulta
void agendarConsulta(Paciente pacientes[], int numPacientes, Medico medicos[], int numMedicos) {
    string nomePaciente;
    cout << "Digite o nome do paciente para agendar consulta: ";
    cin.ignore();
    getline(cin, nomePaciente);

    int indicePaciente = buscarPacientePorNome(pacientes, numPacientes, nomePaciente);

    if (indicePaciente != -1) {
        int escolhaMedico;
        cout << "Escolha o médico para a consulta (1 a " << numMedicos << "):" << endl;
        for (int i = 0; i < numMedicos; i++) {
            cout << i + 1 << ". " << medicos[i].nome << " - " << medicos[i].especializacao << endl;
        }
        cin >> escolhaMedico;

        if (escolhaMedico > 0 && escolhaMedico <= numMedicos) {
            Paciente& p = pacientes[indicePaciente]; // Referência para o paciente
            p.medico = medicos[escolhaMedico - 1].nome;
            p.especializacao = medicos[escolhaMedico - 1].especializacao;

            // Obter data da consulta
            time_t now = time(0);
            tm ltm;
            localtime_s(&ltm, &now);  // Usando localtime_s para evitar o aviso de depreciação

            // Formatar a data como DD/MM/AAAA
            char data[11]; // Formato: DD/MM/AAAA
            strftime(data, sizeof(data), "%d/%m/%Y", &ltm);
            p.dataConsulta = data;

            cout << "Consulta agendada com sucesso!" << endl;
        }
        else {
            cout << "Médico inválido!" << endl;
        }
    }
    else {
        cout << "Paciente não encontrado!" << endl;
    }
}

// Função para gerar o relatório de pacientes
void gerarRelatorio(Paciente pacientes[], int numPacientes) {
    string nomeArquivo;
    cout << "Digite o nome do arquivo (sem a extensão): ";
    cin >> nomeArquivo;
    string pastaRelatorios = criarPastaRelatorios();
    if (pastaRelatorios.empty()) {
        cout << "Erro ao criar a pasta para os relatórios!" << endl;
        return;
    }
    string caminhoArquivo = pastaRelatorios + nomeArquivo + ".txt";
    ofstream arquivo(caminhoArquivo);

    if (!arquivo) {
        cout << "Erro ao criar o arquivo! Verifique as permissões de escrita." << endl;
        return;
    }

    arquivo << "Nome                Telefone       E-mail                Status        Médico          Especialização        Data Consulta\n";
    arquivo << "==============================================================================================================\n";

    for (int i = 0; i < numPacientes; i++) {
        arquivo << pacientes[i].nome << "   "
            << pacientes[i].telefone << "   "
            << pacientes[i].email << "   "
            << (pacientes[i].status == 'A' ? "Ativo" : "Inativo") << "   "
            << pacientes[i].medico << "   "
            << pacientes[i].especializacao << "   "
            << pacientes[i].dataConsulta << endl;
    }

    arquivo.close();
    cout << "Relatório gerado com sucesso em: " << caminhoArquivo << endl;
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    // Lista de médicos com especializações
    Medico medicos[5] = {
        {"Dr. João Silva", "Cardiologista"},
        {"Dra. Ana Souza", "Pediatra"},
        {"Dr. Carlos Pereira", "Ortopedista"},
        {"Dra. Beatriz Oliveira", "Ginecologista"},
        {"Dr. Eduardo Costa", "Neurologista"}
    };

    Paciente pacientes[1000];
    int numPacientes = 0;
    int opcao;

    do {
        system("CLS");
        cout << "\n======= SISTEMA DE PACIENTES ======= \n";
        cout << "1. Cadastrar Paciente\n";
        cout << "2. Consultar Paciente pelo Código\n";
        cout << "3. Agendar Consulta\n";
        cout << "4. Gerar Relatório\n";
        cout << "5. Sair\n";
        cout << "Escolha uma opção: ";
        cin >> opcao;

        switch (opcao) {
        case 1:
            cadastrar(pacientes, numPacientes);
            break;
        case 2:
            consultarPaciente(pacientes, numPacientes);
            break;
        case 3:
            agendarConsulta(pacientes, numPacientes, medicos, 5);
            break;
        case 4:
            gerarRelatorio(pacientes, numPacientes);
            break;
        case 5:
            cout << "Saindo do programa." << endl;
            break;
        default:
            cout << "Opção inválida! Tente novamente." << endl;
        }
        system("pause");

    } while (opcao != 5);

    return 0;
}
