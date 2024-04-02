#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define COUNT 10

#define HORA_MINUTOS 60LL
#define DIA_MINUTOS 1440LL
#define MES_MINUTOS 43200LL
#define ANO_MINUTOS 518400LL

#define PESO_MINUTO 122LL
#define DATA_MINIMA_MINUTOS 11834LL // abaixo disso pode dar conflito

struct arvore {
	struct voo {
		long long numero_voo;
		int num_assentos;
		struct data {
			int horas;
			int minutos;
			int dia;
			int mes;
			int ano;
		} data;
		char origem[11];
		char destino[11];
	} voo;
	struct arvore* pai;
	struct arvore* filho_esquerda;
	struct arvore* filho_direita;
};

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
	if (num_assentos < 0) return 1;

	long long numero_voo = cria_numero_voo(data, origem, destino);
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

#if 0

void andar_em_ordem_crescente(struct arvore* arvore) {

	if (arvore != NULL) {
		andar_em_ordem_crescente(arvore->filho_esquerda);
		printf("%d\n", arvore->dado);
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

	printf("%d\n", arvore->dado);

	mostrar(arvore->filho_esquerda, space);
}

struct arvore* pegar_sucessor(struct arvore* arvore) {
	struct arvore* sucessor = arvore->filho_direita;
	while (sucessor->filho_esquerda != NULL) sucessor = sucessor->filho_esquerda;
	return sucessor;
}

void deletar_elemento(struct arvore* arvore, int dado) {
	struct arvore* no = pegar_elemento(arvore, dado);
	if (no == NULL) return;

	if (no->filho_esquerda == NULL && no->filho_direita == NULL) {
		if (no->pai->dado > dado) { // Quer dizer que o que eu estou tentando exclúir está na esquerda do pai
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
		no->dado = sucessor->dado;
		//sucessor->pai->filho_esquerda = NULL;
		deletar_elemento(sucessor, sucessor->dado);
	}
	else if (no->filho_esquerda) {
		no->dado = no->filho_esquerda->dado;
		deletar_elemento(no->filho_esquerda, no->filho_esquerda->dado);
		no->filho_esquerda = NULL;
	}
	else if (no->filho_direita) {
		no->dado = no->filho_direita->dado;
		deletar_elemento(no->filho_direita, no->filho_direita->dado);
		no->filho_direita = NULL;
	}
}
#endif
int main(void) {
	struct arvore* arvore = NULL;
	struct voo voos[] = {
		(struct voo) {
 .num_assentos = 5, .origem = "SBSP" ,.destino = "SBAM", .data = {10,45,1,4,2024}
},
(struct voo) {
.num_assentos = 10, .origem = "SBAM" ,.destino = "SBSP", .data = {10,45,2,4,2024}
},
	};
	adiciona_elementos(&arvore, voos, 2);


	//adiciona_ou_cria_arvore(&arvore, (struct data) { 10, 45, 1, 4, 2024 },5, "rio de janeiro", "teresina");
	puts("comeco arvore 1");
	printf("%lld\n", arvore->voo.numero_voo);
	puts(arvore->voo.origem);
	puts(arvore->voo.destino);
	puts("comeco pegada");
	struct arvore *pegada = pegar_elemento(arvore, arvore->voo.numero_voo);	
	printf("%lld\n", pegada->voo.numero_voo);
	puts(pegada->voo.origem);
	puts(pegada->voo.destino);

	return 0;
}

