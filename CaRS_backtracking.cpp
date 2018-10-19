#include <bits/stdc++.h>
#include <stdlib.h>
#include <string> 
#include <chrono>

using namespace std;

#define a_sz 20
int ORDEM;
int CARS;
int ORIGEM = 0;

#define RANDSEED 40
//#define CUSTO_TRECHO_MAX 50
//#define CUSTO_DEVOLUCAO_MAX 10

struct custoXcarro {
	int custo;
	vector <int> carro;
};

/* FUNÇÃO BACKTRACKING RECURSIVA
*  Vai testar o cenário de continuar com o mesmo carro e os cenários de trocar para qualquer um 
*  dos outros carros disponíveis.
*/

custoXcarro calcular_custo_sub_caminho(int matriz[a_sz][a_sz][a_sz], int s, vector <int> sub_caminho, int carro_atual, bool carros_disponiveis[]) {	
	//SE CHEGOU NO ULTIMO VERTICE
	if (s == sub_caminho.size()-1) {
		custoXcarro aux;
		aux.custo = matriz[carro_atual][sub_caminho[s]][ORIGEM];
		aux.carro.push_back(carro_atual);
		return aux;
	}
	
	custoXcarro melhor_custo;
	
	/* TESTAR CENARIO DE CONTINUAR COM CARRO ATUAL */
	custoXcarro aux;
	custoXcarro aux_recursive = calcular_custo_sub_caminho(matriz, s+1, sub_caminho, carro_atual, carros_disponiveis);
	aux.custo = matriz[carro_atual][sub_caminho[s]][sub_caminho[s+1]] + aux_recursive.custo;
	
	aux.carro.push_back(carro_atual);
	aux.carro.insert(aux.carro.end(), aux_recursive.carro.begin(),aux_recursive.carro.end()); 
	
		
	melhor_custo = aux;
		
	/* TESTAR CENARIO DE TROCAR PARA CADA CARRO DISPONIVEL */ 
	for (int carro_i = 0; carro_i < CARS; carro_i++) {
		if (carros_disponiveis[carro_i]) {
			custoXcarro aux2;
			/* TROCA PARA CARRO_I, FAZ O PASSO ATUAL COM O CARRO_I E VAI NA RECURSAO COM CARRO_I */ 
			
			carros_disponiveis[carro_i] = false;
			custoXcarro aux2_recursive = calcular_custo_sub_caminho(matriz, s+1, sub_caminho, carro_i, carros_disponiveis);
		
			/* CUSTO = Passo Atual + Restante do caminho */
			aux2.custo = matriz[carro_i][sub_caminho[s]][sub_caminho[s+1]] + aux2_recursive.custo;
			aux2.carro.push_back(carro_i);
			aux2.carro.insert(aux2.carro.end(), aux2_recursive.carro.begin(),aux2_recursive.carro.end()); 
			carros_disponiveis[carro_i] = true;
			
			if (aux2.custo <= melhor_custo.custo)
				melhor_custo = aux2;
		}
	}
	
	return melhor_custo;
}

/*
* FUNÇÃO INICIAL PARA CALCULAR UMA O MENOR CUSTO PARA UMA PERMUTACAO DO CAMINHO.
* Simula os cenários começando com cada um dos carros e chamando a função recursiva
* calcular_custo_sub_caminho para calcular os proximos passos.
*/


custoXcarro calcular_custo_caminho (int matriz[a_sz][a_sz][a_sz], vector <int> caminho,int matriz_custo_devolucao[a_sz][a_sz][a_sz]) {
		// Outra permutação, outra possível solução, é necessário resetar o custo.
		int custo_atual = 0;
		//começa no primeiro carro
		int carro_atual = 0;
		//carros disponiveis
		bool carros_disponiveis[CARS];
	
		custoXcarro melhor_custo;
		melhor_custo.custo = INT_MAX;
		
		//seta todos os carros como disponiveis
		for (int i = 0; i < CARS; i++) {
			carros_disponiveis[i] = true;
		}
		
		//Ponto de partida
		int s = ORIGEM;
		/* PARA TODO CARRO DISPONIVEL, COMEÇAR CENARIO COM ELE */ 
		for (int carro_i = 0; carro_i < CARS; carro_i++) {
			/* COMEÇA O CAMINHO COM CARRO_I */
			custoXcarro aux;
			carros_disponiveis[carro_i] = false; //Aluga o carro;
			custoXcarro aux_recursive = calcular_custo_sub_caminho(matriz, s+1, caminho, carro_i, carros_disponiveis); //Calcula o resto da rota com ele
			aux.carro.push_back(carro_i);
			aux.carro.insert(aux.carro.end(), aux_recursive.carro.begin(),aux_recursive.carro.end()); 
			aux.custo = matriz[carro_i][caminho[s]][caminho[s+1]] + aux_recursive.custo;
			/* VOLTA DISPONIBILIDADE DO CARRO PARA O BackTrack */ 
			carros_disponiveis[carro_i] = true;
			
			
			/* ACRESCENTAR OS CUSTOS DAS DEVOLUCOES NO CUSTO FINAL */ 
			int custo_final_devolucoes = 0;
			int pivot = 0;
			for (int j = 0; j < aux.carro.size(); j++) {
				if (aux.carro[j] != aux.carro[pivot]) {
					custo_final_devolucoes += matriz_custo_devolucao[aux.carro[pivot]][caminho[pivot]][caminho[j]];
					pivot = j;
				}
			}

			/* ultima devolucao de carro */
				custo_final_devolucoes += matriz_custo_devolucao[aux.carro[pivot]][caminho[pivot]][ORIGEM];

			
			aux.custo += custo_final_devolucoes;
			
			/*SE CUSTO FINAL FOR MELHOR ATÈ AGORA, SALVA */
			if (aux.custo < melhor_custo.custo)
				melhor_custo = aux;
		}
		
		return melhor_custo;
}


// (n-1)! ?
int alg(int matriz[a_sz][a_sz][a_sz], int s, vector<int> &caminho, vector<int> &carros_usados, int matriz_custo_devolucao[a_sz][a_sz][a_sz]) {
	
	// A origem e o destino são sempre os mesmos, então fixei.
	vector<int> vertice;
	for (int i = 0; i < ORDEM; i++)
			vertice.push_back(i);
	
	
	// Não temos caminho ainda e usaremos min: N x N -> N.
	int menor_custo = INT_MAX;
	
	// É um do while pois podemos só ter uma permutação.
	do {
		/* PRINTAR PERMUTACAO ATUAL
		for (int i = 0; i < vertice.size(); i++){
			std::cout << vertice[i]+1;
			if(i+1 != vertice.size())
				std::cout << " -> ";
		}
		cout << endl;
		*/
		
		/* PRINTAR MELHOR CUSTO PARA PERMUTACAO ATUAL*/
		custoXcarro melhor_custo = calcular_custo_caminho(matriz, vertice, matriz_custo_devolucao);
		
		int custo_atual = melhor_custo.custo;

		/*
		cout << "Melhor custo para a permutação : " << custo_atual << endl;
		*/
		
		/* ATUALIZAR MELHOR CAMINHO */ 
		if (custo_atual < menor_custo) {
			menor_custo = custo_atual;
			caminho = vertice;
			carros_usados = melhor_custo.carro;
		}

	} while (next_permutation(vertice.begin()+1, vertice.end()));
	caminho.push_back(ORIGEM);
	return menor_custo;
}
 

int main() {

	vector <string> instancias = {"AfricaSul11n.car"};


	ofstream result_file;
	result_file.open("result",std::ios_base::app);

    /* Instancias */ 
	for (int k = 0; k < 1; ++k) {
		// Grafo
		
		int matriz[a_sz][a_sz][a_sz];
		
		/*MATRIZ DE CUSTO DE DEVOLUCAO*/			
		int matriz_custo_devolucao[a_sz][a_sz][a_sz];



		
		/* LER ARQUIVO */
		fstream file;
		fstream result;
		string key_word, aux;
		file.open(instancias[k]);


		result_file << instancias[k] << endl;
		while (file >> key_word) {
			if (key_word == "DIMENSION") {
				file >> aux;
				file >> ORDEM;
				
				result_file << key_word << "  " << ORDEM << endl;
			}

			if (key_word == "CARS_NUMBER") {
				file >> aux;
				file >> CARS;

				result_file << key_word << "  " << CARS << endl;

			}

			if (key_word == "EDGE_WEIGHT_SECTION") {
				for (int i = 0; i < CARS; i++) {
					file >> aux;
					//cout << aux << endl;
					for (int j = 0; j < ORDEM; j++) {
						for (int k = 0; k < ORDEM; k++) {
							file >> matriz[i][j][k];
						}
					}
				}
			}

			if (key_word == "RETURN_RATE_SECTION") {
				for (int i = 0; i < CARS; i++) {
					file >> aux;
					//cout << aux << endl;
					for (int j = 0; j < ORDEM; j++) {
						for (int k = 0; k < ORDEM; k++) {
							file >> matriz_custo_devolucao[i][j][k];
						}	
					}
				}
			}
		}	


		file.close();


		//preencher_matriz(matriz,matriz_custo_devolucao);
		
		// Vértice inicial.	
		
		int s = ORIGEM;

		std::vector<int> caminho;
		std::vector<int> carros_usados;

		auto start = std::chrono::system_clock::now();
		alg(matriz, s, caminho, carros_usados, matriz_custo_devolucao);
		auto end = std::chrono::system_clock::now();

		std::chrono::duration<double> diff = end-start;

		diff.count();

		result_file << diff.count() << endl;

		result_file << endl;
		result_file << endl;

		/*
		std::cout << "Custo do menor caminho: " << alg(matriz, s, caminho, carros_usados, matriz_custo_devolucao) << endl;
		std::cout << "Caminho de menor custo: ";
		for (unsigned i = 0; i < caminho.size(); i++){
			std::cout << caminho[i]+1;
			if(i+1 != caminho.size())
				std::cout << " -> ";
		}
		std::cout << endl;
		std::cout << "Carros usados: ";
		for (unsigned i = 0; i < carros_usados.size(); i++){
			std::cout << carros_usados[i];
			if(i+1 != carros_usados.size())
				std::cout << " -> ";
		}
		*/
	}
	//cout << endl;
	return 0;
}
