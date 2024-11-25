#include <iostream>
#include <string.h>
#include <Windows.h>
#include <locale>

struct Paciente {
    char nome[50];
    char cpf[20];
    char dataNascimento[15];
    char diagnostico[100];
    char status; // ' ' para ativo, '*' para excluído
};

int tamanho(FILE *arq);
void cadastrar(FILE *arq);
void consultar(FILE *arq);
void gerarRelatorio(FILE *arq);
void excluir(FILE *arq);

int main()
{
    int op;
    FILE *arq;

    // Abre o arquivo binário, se não existir cria um novo
    if ((arq = fopen("pacientes.dat", "rb+")) == NULL)
    {
        if ((arq = fopen("pacientes.dat", "wb+")) == NULL)
        {
            printf("Falha ao abrir o arquivo!\n");
            system("pause");
            return 0;
        }
    }

    do
    {
        system("CLS");
        printf("\n======= HOSPITAL ======= \n");
        printf("1.Cadastrar Paciente\n");
        printf("2.Consultar Paciente\n");
        printf("3.Gerar Relatório\n");
        printf("4.Excluir Paciente\n");
        printf("5.Sair\n");
        printf("===========Pacientes Cadastrados:%d===========\n", tamanho(arq));
        printf("Opcao: ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            cadastrar(arq);
            break;
        case 2:
            consultar(arq);
            break;
        case 3:
            gerarRelatorio(arq);
            break;
        case 4:
            excluir(arq);
            break;
        case 5:
            fclose(arq);
            break;
        default:
            printf("Opcao invalida!\n");
            break;
        }
    } while (op != 5);

    return 0;
}

void cadastrar(FILE *arq)
{
    Paciente paciente;
    char confirma;
    paciente.status = ' ';
    fflush(stdin);

    printf("Cadastrando novo paciente:\n");
    printf("Nome.............: ");
    gets(paciente.nome);
    printf("CPF..............: ");
    gets(paciente.cpf);
    printf("Data Nascimento..: ");
    gets(paciente.dataNascimento);
    printf("Diagnóstico......: ");
    gets(paciente.diagnostico);
    printf("\nConfirma <s/n>: ");
    fflush(stdin);
    scanf("%c", &confirma);

    if (toupper(confirma) == 'S')
    {
        printf("\nGravando...\n\n");
        fseek(arq, 0, SEEK_END);
        fwrite(&paciente, sizeof(Paciente), 1, arq);
    }
    system("pause");
}

void consultar(FILE *arq)
{
    Paciente paciente;
    int nr;
    printf("\nConsulta pelo codigo\n");
    printf("\nInforme o Codigo: ");
    scanf("%d", &nr);
    if ((nr <= tamanho(arq)) && (nr > 0))
    {
        fseek(arq, (nr - 1) * sizeof(Paciente), SEEK_SET);
        fread(&paciente, sizeof(Paciente), 1, arq);
        if (paciente.status == ' ')
        {
            printf("\nNome........: %s", paciente.nome);
            printf("\nCPF.........: %s", paciente.cpf);
            printf("\nData Nascimento: %s", paciente.dataNascimento);
            printf("\nDiagnóstico..: %s\n", paciente.diagnostico);
        }
        else
        {
            printf("\nPaciente excluído!\n");
        }
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

void gerarRelatorio(FILE *arq)
{
    char nomearq[20];
    printf("Nome do arquivo texto: ");
    scanf("%s", nomearq);
    strcat(nomearq, ".txt");
    FILE *arqtxt = fopen(nomearq, "w");
    if (!arqtxt)
    {
        printf("Não foi possível criar esse arquivo!\n");
        system("pause");
        return;
    }

    fprintf(arqtxt, "Nome                 CPF              Data Nascimento    Diagnóstico        Status\n");
    fprintf(arqtxt, "==========================================================================\n");

    int nr;
    Paciente paciente;
    for (nr = 0; nr < tamanho(arq); nr++)
    {
        fseek(arq, nr * sizeof(Paciente), SEEK_SET);
        fread(&paciente, sizeof(Paciente), 1, arq);
        fprintf(arqtxt, "%-20s%-18s%-20s%-20s- %c\n", paciente.nome, paciente.cpf, paciente.dataNascimento, paciente.diagnostico, paciente.status);
    }
    fprintf(arqtxt, "==========================================================================\n");
    fclose(arqtxt);
}

void excluir(FILE *arq)
{
    Paciente paciente;
    char confirma;
    int nr;

    printf("\nInforme o código do paciente para excluir\n");
    scanf("%d", &nr);
    if ((nr <= tamanho(arq)) && (nr > 0))
    {
        fseek(arq, (nr - 1) * sizeof(Paciente), SEEK_SET);
        fread(&paciente, sizeof(Paciente), 1, arq);
        if (paciente.status == ' ')
        {
            printf("\nNome........: %s", paciente.nome);
            printf("\nCPF.........: %s", paciente.cpf);
            printf("\nData Nascimento: %s", paciente.dataNascimento);
            printf("\nDiagnóstico..: %s\n", paciente.diagnostico);
            printf("\nConfirma a exclusao: <s/n>\n");
            getchar();
            scanf("%c", &confirma);

            if (toupper(confirma) == 'S')
            {
                printf("\nExcluindo...\n\n");
                fseek(arq, (nr - 1) * sizeof(Paciente), SEEK_SET);
                paciente.status = '*';
                fwrite(&paciente, sizeof(Paciente), 1, arq);
            }
        }
        else
        {
            printf("Paciente inexistente!\n");
        }
    }
    else
    {
        printf("\nNúmero de registro inválido!\n");
    }
    system("pause");
}

int tamanho(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    return ftell(arq) / sizeof(Paciente);
}