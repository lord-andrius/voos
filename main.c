#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "main.h"
#include <time.h>

#define COUNT 10

#define HORA_MINUTOS 60LL
#define DIA_MINUTOS 1440LL
#define MES_MINUTOS 43200LL
#define ANO_MINUTOS 518400LL

#define PESO_MINUTO 122LL
#define DATA_MINIMA_MINUTOS 11834LL // abaixo disso pode dar conflito


long long cria_numero_voo(struct data data, const char* origem, const char* destino) {
	long long soma_origem, soma_destino;
	soma_destino = soma_origem = 0LL;
	for (int i = 0; i < strlen(origem); i++) {
		soma_origem += origem[i] * (i == 0 ? 1 : i);
	}
	for (int i = 0; i < strlen(destino); i++) {
		soma_destino += destino[i];
	}
	long long data_minutos = (data.ano * ANO_MINUTOS) + (data.mes * MES_MINUTOS) + (data.dia * DIA_MINUTOS) + (data.horas * HORA_MINUTOS) + (data.minutos * PESO_MINUTO); // O peso do minuto é necessario para quando a data difere de poucos minutos
	if(data_minutos < DATA_MINIMA_MINUTOS) {
		puts("Essa data é inválida");
		exit(0);
	}
	return data_minutos + soma_destino + soma_origem;
}

// cria ou adiciona elementos na árvore
// retorna 0 no sucesso
int adiciona_ou_cria_arvore(struct arvore** arvore, struct data data, int num_assentos, const char* origem, const char* destino) {
	if(num_assentos < 0) return 1;
	long long numero_voo = cria_numero_voo(data, origem, destino);
	if(pegar_elemento(*arvore, numero_voo) != NULL) return 1; // já tem um elemento igual

	size_t tamanho_str_origem = strlen(origem) > 9 ? 10 : strlen(origem);
	size_t tamanho_str_destino = strlen(destino) > 9 ? 10 : strlen(destino);


	if (*arvore == NULL) { // se o ponteiro é nulo temos que criar
		*arvore = (struct arvore*)malloc(sizeof(struct arvore)); // usando calloc para evitar ter que zerar os ponteiros!
		if (*arvore == NULL) {
			return 1;
		}
		(*arvore)->pai = NULL;
		(*arvore)->filho_esquerda = NULL;
		(*arvore)->filho_direita = NULL;
		(*arvore)->voo.numero_voo = numero_voo;
		(*arvore)->voo.num_assentos = num_assentos;
		memcpy(&((*arvore)->voo.data), &data, sizeof(struct data));
		memset(&((*arvore)->voo.origem), 0, 11);
		memset(&((*arvore)->voo.destino), 0, 11);

		strncpy((*arvore)->voo.origem, origem, tamanho_str_origem);
		strncpy((*arvore)->voo.destino, destino, tamanho_str_destino);
		return 0;
	}
	else {
		struct arvore* pai = *arvore;
		struct arvore* a = *arvore;
		while (a) {
			pai = a;
			if (numero_voo > a->voo.numero_voo) {
				a = a->filho_direita;
			}
			else {
				a = a->filho_esquerda;
			}
		}

		a = (struct arvore*)malloc(sizeof(struct arvore)); // usando calloc para evitar ter que zerar os ponteiros!
		if (a == NULL) {
			return 1;
		}
		a->pai = pai;
		a->filho_esquerda = NULL;
		a->filho_direita = NULL;
		a->voo.numero_voo = numero_voo;
		a->voo.num_assentos = num_assentos;
		memcpy(&(a->voo.data), &data, sizeof(struct data));
		memset(&(a->voo.origem), 0, 11);
		memset(&(a->voo.destino), 0, 11);
		strncpy(a->voo.origem, origem, tamanho_str_origem);
		strncpy(a->voo.destino, destino, tamanho_str_destino);

		if (numero_voo > pai->voo.numero_voo) {
			pai->filho_direita = a;
		}
		else {
			pai->filho_esquerda = a;
		}
	}

	
	return 0;
}

void bubble_sort(struct voo voos[], size_t length) {
	int troca;
	do {
		troca = 0;
		for (size_t i = 1; i < length; i++) {
			if (cria_numero_voo(voos[i - 1].data, voos[i - 1].origem, voos[i - 1].destino) > cria_numero_voo(voos[i].data, voos[i].origem, voos[i].destino)) {
				struct voo tmp = voos[i - 1];
				voos[i - 1] = voos[i];
				voos[i] = tmp;
				troca = 1;
			}
		}
	} while (troca);
}

// NOTA: Essa função pegas voos apenas pela nessecidade de associar as strings de origem e destino as datas
int adiciona_elementos(struct arvore** arvore, struct voo voos[], size_t length) {
	bubble_sort(voos, length);
	if (length == 1) {
		return adiciona_ou_cria_arvore(arvore, voos[0].data, voos[0].num_assentos, voos[0].origem, voos[0].destino);
	}
	else if (length == 2) {

		if (adiciona_ou_cria_arvore(arvore, voos[0].data, voos[0].num_assentos, voos[0].origem, voos[0].destino) != 0) return 1;
		return adiciona_ou_cria_arvore(arvore, voos[1].data, voos[1].num_assentos, voos[1].origem, voos[1].destino);
	}
	else {
		size_t tamanho_metade = length / 2; // tambem é o indice do elemento da metade!
		if (adiciona_ou_cria_arvore(arvore, voos[tamanho_metade].data, voos[tamanho_metade].num_assentos, voos[tamanho_metade].origem, voos[tamanho_metade].destino) != 0) return 1;
		if (adiciona_elementos(arvore, voos, tamanho_metade) != 0) return 1;
		return adiciona_elementos(arvore, voos + (tamanho_metade + 1), length - (tamanho_metade + 1));
	}
}

struct arvore* pegar_elemento(struct arvore* arvore, long long numero_voo) {
	if (arvore == NULL) {
		return NULL;
	}

	else if (arvore->voo.numero_voo == numero_voo) {
		return arvore;
	}
	else if (arvore->voo.numero_voo < numero_voo) {
		return pegar_elemento(arvore->filho_direita, numero_voo);
	}
	else {
		return pegar_elemento(arvore->filho_esquerda, numero_voo);
	}
}


void andar_em_ordem_crescente(struct arvore* arvore) {
	if (arvore != NULL) {
		andar_em_ordem_crescente(arvore->filho_esquerda);
		printf("numer:%lld - assentos disponiveis: %d - data: %02d:%02d %02d/%02d/%02d - origem: %s - destino: %s\n", 
			arvore->voo.numero_voo, 
			arvore->voo.num_assentos,
			arvore->voo.data.horas, 
			arvore->voo.data.minutos,
			arvore->voo.data.dia,
			arvore->voo.data.mes,
			arvore->voo.data.ano,
			arvore->voo.origem,
			arvore->voo.destino
		);
		andar_em_ordem_crescente(arvore->filho_direita);
	}
}

void andar_em_ordem_crescente_disponiveis(struct arvore* arvore) {
	if (arvore != NULL) {
		andar_em_ordem_crescente(arvore->filho_esquerda);
		if(arvore->voo.num_assentos > 0) {
			printf("numer:%lld - assentos disponiveis: %d - data: %02d:%02d %02d/%02d/%02d - origem: %s - destino: %s\n", 
				arvore->voo.numero_voo, 
				arvore->voo.num_assentos,
				arvore->voo.data.horas, 
				arvore->voo.data.minutos,
				arvore->voo.data.dia,
				arvore->voo.data.mes,
				arvore->voo.data.ano,
				arvore->voo.origem,
				arvore->voo.destino
			);
		}
		andar_em_ordem_crescente(arvore->filho_direita);
	}
}

void andar_em_ordem_crescente_assentos_10(struct arvore* arvore) {
	if (arvore != NULL) {
		andar_em_ordem_crescente(arvore->filho_esquerda);
		if(arvore->voo.num_assentos < 10) {
			printf("numer:%lld - assentos disponiveis: %d - data: %02d:%02d %02d/%02d/%02d - origem: %s - destino: %s\n", 
				arvore->voo.numero_voo, 
				arvore->voo.num_assentos,
				arvore->voo.data.horas, 
				arvore->voo.data.minutos,
				arvore->voo.data.dia,
				arvore->voo.data.mes,
				arvore->voo.data.ano,
				arvore->voo.origem,
				arvore->voo.destino
			);
		}
		andar_em_ordem_crescente(arvore->filho_direita);
	}

}
void mostrar(struct arvore* arvore, int space) {

	if (arvore == NULL) {
		return;
	}

	space += COUNT;

	mostrar(arvore->filho_direita, space);

	puts("");

	for (int i = COUNT; i < space; i++) printf(" ");

	printf("%lld\n", arvore->voo.numero_voo);

	mostrar(arvore->filho_esquerda, space);
}

struct arvore* pegar_sucessor(struct arvore* arvore) {
	struct arvore* sucessor = arvore->filho_direita;
	while (sucessor->filho_esquerda != NULL) sucessor = sucessor->filho_esquerda;
	return sucessor;
}

void deletar_elemento(struct arvore* arvore, long long numero_voo) {
	struct arvore* no = pegar_elemento(arvore, numero_voo);
	if (no == NULL) return;

	if (no->filho_esquerda == NULL && no->filho_direita == NULL) {
		if(no->pai == NULL) {
			free(no);
			return;
		}
		if (no->pai->voo.numero_voo > numero_voo) { // Quer dizer que o que eu estou tentando exclúir está na esquerda do pai
			no->pai->filho_esquerda = NULL;
			free(no);
		}
		else {
			no->pai->filho_direita = NULL;
			free(no);
		}
	}
	else if (no->filho_esquerda && no->filho_direita) {
		struct arvore* sucessor = pegar_sucessor(no);
		no->voo = sucessor->voo;
		//sucessor->pai->filho_esquerda = NULL;
		deletar_elemento(sucessor, sucessor->voo.numero_voo);
	}
	else if (no->filho_esquerda) {
		no->voo = no->filho_esquerda->voo;
		deletar_elemento(no->filho_esquerda, no->filho_esquerda->voo.numero_voo);
		no->filho_esquerda = NULL;
	}
	else if (no->filho_direita) {
		no->voo = no->filho_direita->voo;
		deletar_elemento(no->filho_direita, no->filho_direita->voo.numero_voo);
		no->filho_direita = NULL;
	}
}

// Se o nivel for 0 vai retornar a potencia de dois correspondente ao último nível
// se o nivel for 1 vai retornar a quantidade de niveis
int ultimo_nivel(struct arvore *arvore, int nivel) {
	if(arvore == NULL) {
		return 0;
	} else {
		int nivel_esquerda = ultimo_nivel(arvore->filho_esquerda, nivel + 1);
		int nivel_direita = ultimo_nivel(arvore->filho_direita, nivel + 1);
		if(nivel > nivel_esquerda && nivel > nivel_direita) {
			return nivel;
		} else {
			return nivel_esquerda > nivel_direita ? nivel_esquerda : nivel_direita;
		}
	}
}

int qtd_voos(struct arvore *arvore) {
	if(arvore == NULL) return 0;
	return 1 + qtd_voos(arvore->filho_esquerda) + qtd_voos(arvore->filho_direita);
}
	

void arvore_para_vetor(struct arvore* arvore, struct vetor_voos *voos) {
	if(arvore == NULL) {
		return;
	}
	arvore_para_vetor(arvore->filho_esquerda, voos);
	arvore_para_vetor(arvore->filho_direita, voos);
	voos->tamanho += 1;
	voos->voos = realloc(voos->voos, sizeof(struct voo) * (voos->tamanho == 0 ? 1 : voos->tamanho));
	voos->voos[voos->tamanho - 1] = arvore->voo;

}

int precisa_balancear(struct arvore *arvore) {
	if(arvore == NULL) return 0;

	return abs(ultimo_nivel(arvore->filho_esquerda,1) - ultimo_nivel(arvore->filho_direita, 1)) > 1;
}

void deletar_arvore(struct arvore *arvore) {
	if(arvore != NULL) {
		deletar_arvore(arvore->filho_esquerda);
		deletar_arvore(arvore->filho_direita);
		free(arvore);
	}
}

void balancear_arvore(struct arvore **arvore) {
	if(qtd_voos(*arvore) == 0) {
		puts("entrou aqui");
		return;
	}
	struct vetor_voos voos = {NULL, 0};
	arvore_para_vetor(*arvore, &voos);
	struct arvore *nova_arvore = NULL;
	adiciona_elementos(&nova_arvore, voos.voos, voos.tamanho);
	deletar_arvore(*arvore);
	*arvore = nova_arvore;
}
struct voo pegar_voo_console(void) {
	char origem[11] = {0};
	char destino[11] = {0};
	int hora,minutos,dia, mes, ano, num_assentos;
	printf("Digite a origem(10 caracteres apenas): ");
	scanf("%s",origem);	
	printf("Digite o destino(10 caracteres apenas): ");
	scanf("%s", destino);
	printf("Digite o numero de assentos disponiveis: ");
	scanf("%d", &num_assentos);
	limpar_stdin();
	printf("Digite a hora: ");
	scanf("%d", &hora);
	limpar_stdin();
	printf("Digite os minutos: ");
	scanf("%d", &minutos);
	limpar_stdin();
	printf("Digite o dia: ");
	scanf("%d", &dia);
	limpar_stdin();
	printf("Digite o mes(01 - 12): ");
	scanf("%d", &mes);
	limpar_stdin();
	printf("Digite o ano: ");
	scanf("%d", &ano);
	limpar_stdin();
	struct voo voo = (struct voo){
		.data = (struct data){hora,minutos,dia, mes, ano},
		.num_assentos = num_assentos,
		.origem = {0},
		.destino = {0},
	};
	memcpy(voo.origem, origem, strlen(origem));
	memcpy(voo.destino, destino, strlen(destino));
	return voo;
}

void esperar_espaco(void) {
	puts("Aperte <ENTER> para continuar..!");
	while(getchar() != '\n');
}

void limpar_stdin(void) {
	while(getchar() != '\n');
}

int numero_aleatorio(int minimo, int maximo) {
	return (rand() % (maximo - minimo + 1)) + minimo;
}

void palavra_aleatoria(char str[], int tamanho) {
	int i = 0;
	for(; i < tamanho; i++) {
		char letra = (char)numero_aleatorio(65,90);
		str[i] = letra; 
	}
	str[i] = '\0';
} 

struct voo voo_aleatorio(struct arvore *arvore) {
	struct voo voo;
	while(1) {
		voo =  (struct voo){
			.data = (struct data){
						.horas = numero_aleatorio(1, 23),
						.minutos = numero_aleatorio(1, 59),
						.dia = numero_aleatorio(1,31),
						.mes = numero_aleatorio(1,12),
						.ano = numero_aleatorio(2000, 3000)
			},
			.num_assentos = numero_aleatorio(0,255),
			.origem = {0},
			.destino = {0}
		};
		palavra_aleatoria(voo.origem, 10);
		palavra_aleatoria(voo.destino, 10);
		if(pegar_elemento(arvore, cria_numero_voo(voo.data, voo.origem, voo.destino)) == NULL) {
			break;
		}
	}
		
	return voo;
}

void adiciona_7_elementos(struct arvore **arvore) {
	struct voo voos[] = {
		voo_aleatorio(*arvore),
		voo_aleatorio(*arvore),
		voo_aleatorio(*arvore),
		voo_aleatorio(*arvore),
		voo_aleatorio(*arvore),
		voo_aleatorio(*arvore),
		voo_aleatorio(*arvore)
	};
	adiciona_elementos(arvore, voos, 7);
}

void pesquisar(struct arvore *arvore, struct voo voo) {
	if (arvore != NULL) {
		pesquisar(arvore->filho_esquerda, voo);
		if(arvore->voo.num_assentos >= voo.num_assentos && 
		   	arvore->voo.data.ano >= voo.data.ano &&
			arvore->voo.data.mes >= voo.data.mes &&
			arvore->voo.data.dia >= voo.data.dia
		   ) {
			printf("numer:%lld - assentos disponiveis: %d - data: %02d:%02d %02d/%02d/%02d - origem: %s - destino: %s\n", 
				arvore->voo.numero_voo, 
				arvore->voo.num_assentos,
				arvore->voo.data.horas, 
				arvore->voo.data.minutos,
				arvore->voo.data.dia,
				arvore->voo.data.mes,
				arvore->voo.data.ano,
				arvore->voo.origem,
				arvore->voo.destino
			);
		}
		pesquisar(arvore->filho_direita, voo);
	}
}


int main(void) {
	srand(time(0));
	struct arvore* arvore = NULL;
	int opcao;
	int continuar = 1;
	struct voo voo;
	//arvore_para_vetor(arvore, &voos, &indice);
	while(continuar) {
		puts("==========MENU==========");
		puts("[1] - para adicionar um voo");
		puts("[2] - para adicionar 7 voos aleatorios");
		puts("[3] - para ver todos os voos");
		puts("[4] - para ver todos os voos disponiveis");
		puts("[5] - para ver todos os voos com menos de 10 assentos disponiveis");
		puts("[6] - para ver a arvore(numero voo apenas)");
		puts("[7] - para excluir um voo");
		puts("[8] - para pesquisar: ");
		puts("[9] - para sair");
		puts("========================");
		printf("Digite a sua opcao:");
		scanf("%d", &opcao);
		limpar_stdin();
		switch(opcao) {
			case 1:
				 voo = pegar_voo_console();
				if(adiciona_ou_cria_arvore(&arvore, voo.data, voo.num_assentos, voo.origem, voo.destino)){
						puts("não foi possivel adicionar esse voo!");
				} else {

						puts("voo adicionado com sucesso");
						if(precisa_balancear(arvore)) {
							balancear_arvore(&arvore);	
						}
				}
				esperar_espaco();
				break;
			case 2:
				adiciona_7_elementos(&arvore);
				puts("Elementos adicionados com sucesso!");
				if(precisa_balancear(arvore)) {
					balancear_arvore(&arvore);	
				}
				esperar_espaco();
				break;
			case 3:
				andar_em_ordem_crescente(arvore);
				esperar_espaco();
				break;
			case 4:
				andar_em_ordem_crescente_disponiveis(arvore);
				esperar_espaco();
				break;
			case 5:
				andar_em_ordem_crescente_assentos_10(arvore); 
				esperar_espaco();
				break;
			case 6:
				mostrar(arvore,1);
				esperar_espaco();
				break;

			case 7:
				voo = pegar_voo_console();
				voo.numero_voo = cria_numero_voo(voo.data, voo.origem, voo.destino);
				int anular = qtd_voos(arvore) == 1 && pegar_elemento(arvore, voo.numero_voo) != NULL;
				deletar_elemento(arvore,voo.numero_voo)	;
				if(anular) {
					arvore = NULL;
				}
				esperar_espaco();
				break;
			case 8:
				puts("==========REGRAS DA PESQUISA==========");
				puts("1 - Seram mostrados os voos com o número de assentos maiores o iguais ao valor digitado!");
				puts("2 - Para ignorar pegar voos com qualquer número de assentos use um valor negativo no numero de assentos");
				puts("3 - Seram mostrados os voos apartir da data informada");
				puts("======================================");
				esperar_espaco();
				int dia, mes, ano, num_assentos;
				printf("Digite o numero de assentos: ");
				scanf("%d",&num_assentos);
				limpar_stdin();
				printf("Digite o dia: ");
				scanf("%d", &dia);
				limpar_stdin();
				printf("Digite o mes(01 - 12): ");
				scanf("%d", &mes);
				limpar_stdin();
				printf("Digite o ano: ");
				scanf("%d", &ano);
				limpar_stdin();
				voo = (struct voo){
					.data = (struct data){0,0,dia, mes, ano},
					.num_assentos = num_assentos,
					.origem = {0},
					.destino = {0},
				};
				pesquisar(arvore, voo);

				break;
			case 9:
				continuar = 0;
				break;
			default:
				puts("Opcao invalida");	
				esperar_espaco();	
				break;
		}
	}
	if(arvore) deletar_arvore(arvore);
	return 0;
}

