#include <iostream>
#include <ctime>
#include "FilaPrioridadesLE.h"

using namespace std;

// Função para preencher as filas de solicitação de pouso/decolagem
void PreencheSolicitacoes(no **SPeD) {
    srand(time(0));

    for (int i = 0; i < 50; i++) {
        int aleatPrior = 1 + (rand() % 2);
        *SPeD = insereFilaChegada(*SPeD, i + 10, aleatPrior);
    }
}

// Função para exibir fila com tempo de espera estimado
void exibeComEspera(no* L, int intervalo) {
    no* P = L;
    int posicao = 0;
    cout << " ";
    while (P != NULL) {
        int tempoEspera = posicao * intervalo;
        cout << "AV:" << P->info << " P:" << P->prior 
             << " [Espera: " << tempoEspera << " UTs]" << endl;
        P = P->link;
        posicao++;
    }
}

int main() {
    no* FP1, * FP2;
    no* SolicitacoesPeD;
    int aviaoP, aviaoD, num, prioridade, continua = 1;

    srand(time(0));

    FP1 = inicializaFP(FP1);
    FP2 = inicializaFP(FP2);
    SolicitacoesPeD = inicializaFP(SolicitacoesPeD);

    PreencheSolicitacoes(&SolicitacoesPeD);

    cout << "Solicitacoes de avioes: ( ";
    exibe(SolicitacoesPeD);
    cout << " )\n" << endl;

    for (int ut = 1; continua; ut++) {
        // Quantidade de solicitações de pouso/decolagem por UT
        for (int solicitacoes = 0; solicitacoes < 5; solicitacoes++) {
            int aleat = 1 + (rand() % 2);
            if (aleat == 1) {
                if (!verificaSeVazia(SolicitacoesPeD)) {
                    SolicitacoesPeD = removeFP(SolicitacoesPeD, &num, &prioridade);
                    FP1 = insereFP(FP1, num, prioridade);
                }
            }
            else {
                if (!verificaSeVazia(SolicitacoesPeD)) {
                    SolicitacoesPeD = removeFP(SolicitacoesPeD, &num, &prioridade);
                    FP2 = insereFP(FP2, num, prioridade);
                }
            }
        }

        cout << "Solicitacoes de pouso: ";
        exibeComEspera(FP1, 2); // Aqui com tempo de espera de pouso
        cout << endl;

        cout << "Solicitacoes de decolagem: ";
        exibeComEspera(FP2, 3); // Aqui com tempo de espera de decolagem
        cout << "\n" << endl;

        if ((ut % 2) & (continua)) {
            if (FP1 != NULL) {
                cout << "======================== Unidade de tempo: " << ut << "================================================\n" << endl;
                FP1 = removeFP(FP1, &aviaoP, &prioridade);
                if (prioridade == 1) {
                    cout << "Pista 1 sendo utilizada por (AVIAO: " << aviaoP << ") para pouso com prioridade alta." << endl
                        << "\nEspera para proxima (POUSO): aproximadamente 1 UTs\n__________________________________________________" << endl;
                }
                else {
                    cout << "Pista 1 sendo utilizada por (AVIAO: " << aviaoP << ") para pouso." << endl
                        << "\nEspera para proxima (POUSO): aproximadamente 1 UTs\n__________________________________________________" << endl;
                }
            }
            else {
                cout << "======================== Unidade de tempo: " << ut << "================================================\n" << endl;
                cout << "Pista 1 esta vazia." << endl;
                if ((FP2 == NULL) & (ut % 2)) {
                    continua = 0;
                }
            }
        }
        else {
            cout << "======================== Unidade de tempo: " << ut << "================================================" << endl;
            if ((FP1 != NULL) || (ut % 2 != 0)) {
                cout << "(AVIAO: " << aviaoP << ") taxiando na pista 1." << endl
                    << "\nEspera para proxima (POUSO): PROXIMA UT.\n__________________________________________________" << endl;
            }
            else {
                cout << "Pista 1 esta vazia." << endl;
            }
        }

        if ((ut % 3) & (continua)) {
            if (FP2 != NULL) {
                FP2 = removeFP(FP2, &aviaoD, &prioridade);
                if (prioridade == 1) {
                    cout << "Pista 2 sendo utilizada por (AVIAO: " << aviaoD << ") para decolagem com prioridade alta." << endl
                        << "\nEspera para proxima (DECOLAGEM): aproximadamente 2 UTs.\n__________________________________________________" << endl;
                }
                else {
                    cout << "Pista 2 sendo utilizada por (AVIAO: " << aviaoD << ") para decolagem.\n" << endl
                        << "Espera para proxima (DECOLAGEM): aproximadamente 2 UTs\n" << endl;
                }
            }
            else {
                cout << "Pista 2 esta vazia.\n" << endl;
                if ((FP1 == NULL) & (ut % 3)) {
                    continua = 0;
                }
            }
        }
        else {
            if ((FP2 != NULL) || (ut % 3 != 0)) {
                cout << "(AVIAO: " << aviaoD << ") taxiando na pista 2." << endl
                    << "\nEspera para proximo (DECOLAGEM): aproximadamente 1 UTs\n__________________________________________________" << endl;
            }
            else {
                cout << "Pista 2 esta vazia.\n" << endl;
            }
        }
    }
}
