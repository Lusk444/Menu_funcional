#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <shlobj.h>
#include <direct.h>
#include <locale.h>

using namespace std;

struct Paciente {
    char nome[50];
    char telefone[20];
    char email[50];
    char status;
};
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

string criarPastaRelatorios() {
    string pastaBase = getUserFolderPath() + "\\RelatoriosPaciente\\";
    if (_mkdir(pastaBase.c_str()) == 0 || errno == EEXIST) {
        return pastaBase;
    }
    else {
        return "";
    }
}
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
    }
    else {
        cout << "Código inválido!" << endl;
    }
}

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

    arquivo << "Nome                Telefone       E-mail                Status\n";
    arquivo << "==============================================================\n";

    for (int i = 0; i < numPacientes; i++) {
        arquivo << pacientes[i].nome << "   "
            << pacientes[i].telefone << "   "
            << pacientes[i].email << "   "
            << pacientes[i].status << endl;
    }

    arquivo.close();
    cout << "Relatório gerado com sucesso em: " << caminhoArquivo << endl;
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    Paciente pacientes[1000];  
    int numPacientes = 0;      
    int opcao;

    do {
        system("CLS");
        cout << "\n======= SISTEMA DE PACIENTES ======= \n";
        cout << "1. Cadastrar Paciente\n";
        cout << "2. Consultar Paciente pelo Código\n";
        cout << "3. Gerar Relatório\n";
        cout << "4. Sair\n";
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
            gerarRelatorio(pacientes, numPacientes);
            break;
        case 4:
            cout << "Saindo do programa." << endl;
            break;
        default:
            cout << "Opção inválida! Tente novamente." << endl;
        }
        system("pause");

    } while (opcao != 4);

    return 0;
}
