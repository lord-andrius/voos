#ifndef MAIN_H
#define MAIN_H

struct data {
	int horas;
	int minutos;
	int dia;
	int mes;
	int ano;
};
struct voo {
	long long numero_voo;
	int num_assentos;
	struct data data;
	char origem[11];
	char destino[11];
};
struct arvore {
	struct voo voo;
	struct arvore* pai;
	struct arvore* filho_esquerda;
	struct arvore* filho_direita;
};

struct vetor_voos {
	struct voo *voos;
	int tamanho;
};


void adiciona_vetor_voos(struct vetor_voos *voos);

long long cria_numero_voo(struct data data, const char* origem, const char* destino);

int adiciona_ou_cria_arvore(struct arvore** arvore, struct data data, int num_assentos, const char* origem, const char* destino);

void bubble_sort(struct voo voos[], size_t length);

int adiciona_elementos(struct arvore** arvore, struct voo voos[], size_t length);

struct arvore* pegar_elemento(struct arvore* arvore, long long numero_voo);

void andar_em_ordem_crescente(struct arvore* arvore); 
void andar_em_ordem_crescente_disponiveis(struct arvore* arvore); 
void andar_em_ordem_crescente_assentos_10(struct arvore* arvore); 

void mostrar(struct arvore* arvore, int space); 

struct arvore* pegar_sucessor(struct arvore* arvore); 

void deletar_elemento(struct arvore* arvore, long long numero_voo);

int ultimo_nivel(struct arvore *arvore, int nivel); 

int qtd_voos(struct arvore *arvore); 

int precisa_balancear(struct arvore* arvore);

void arvore_para_vetor(struct arvore* arvore, struct vetor_voos *voos);

void balancear_arvore(struct arvore** arvore);

void deletar_arvore(struct arvore *arvore);

struct voo pegar_voo_console(void);

void esperar_espaco(void);

void limpar_stdin(void);

int numero_aleatorio(int minimo, int maximo);

void palavra_aleatoria(char str[], int tamanho);

struct voo voo_aleatorio(struct arvore *arvore); 

void adiciona_7_elementos(struct arvore **arvore); 

void pesquisar(struct arvore *arvore, struct voo voo);

#endif
