#ifndef MAIN_H
#define MAIN_H
long long cria_numero_voo(struct data data, const char* origem, const char* destino);

int adiciona_ou_cria_arvore(struct arvore** arvore, struct data data, int num_assentos, const char* origem, const char* destino);

void bubble_sort(struct voo voos[], size_t length);

int adiciona_elementos(struct arvore** arvore, struct voo voos[], size_t length);

struct arvore* pegar_elemento(struct arvore* arvore, long long numero_voo);

void andar_em_ordem_crescente(struct arvore* arvore); 

void mostrar(struct arvore* arvore, int space); 

struct arvore* pegar_sucessor(struct arvore* arvore); 

void deletar_elemento(struct arvore* arvore, long long numero_voo);

int ultimo_nivel(struct arvore *arvore, int nivel); 

int qtd_voos(struct arvore *arvore); 

int precisa_balancear(struct arvore* arvore);

void arvore_para_vetor(struct arvore* arvore, struct voo** voos, int* index);

#endif
