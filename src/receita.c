#include "estoque.h"
#include <assert.h>
#include <receita.h>
#include <stdlib.h>
#include <stdio.h>

#define true 1 
#define false 0

typedef struct __block_t {
	receita_t receita;
	int ativo;
	struct __block_t * next;
} block_t;

static inline void panic(const char * restrict message, const unsigned exit_code) {
	printf("%s\n", message);
	exit(exit_code);
}


//Controle de memoria
static block_t *freelist = NULL;
static block_t *receitas = NULL;
static unsigned buffer_size = 0;
static block_t * buffer_top = NULL;
static unsigned diff = 0;

static inline int block_init() {
	receitas = malloc(buffer_size);
	if(receitas == NULL) return SEM_MEMORIA;
	buffer_top = receitas;

	return 0;
}

static inline block_t *take_free_block() {
	if(freelist == NULL) return NULL;
	block_t *bloco = freelist;
	freelist = freelist->next;
	bloco->ativo = 1;
	return bloco;
}

static inline void free_block(block_t *b) {
	if(freelist != NULL) b->next = freelist;
	b->ativo = 0;
	freelist = b;
}

static inline block_t *new_block() {
	block_t * b = NULL;
	if((b = take_free_block())) return b;

	if(buffer_size < buffer_top - receitas) {
		void *tmp = realloc(receitas, (buffer_size * 2) * sizeof(ingrediente_t));
		if(tmp == NULL) return NULL;

		receitas = tmp;
		buffer_size *= 2;
	}

	buffer_top->ativo = 1;
	buffer_top->next = NULL;
	diff += 1;

	return buffer_top++;
}


//Manipular com segurança
static inline int existe(const unsigned codigo) {
	for(int i = 0; i < diff; i++)
		if(receitas[i].ativo == 1 && receitas[i].receita.codigo == codigo) return true;

	return false;
}

static inline block_t *encontrar(const unsigned codigo) {
	for(int i = 0; i < diff; i++)
		if(receitas[i].ativo == 1 && receitas[i].receita.codigo == codigo) return &receitas[i];

	return NULL;
}


static inline void receita_iniciado_checar() {
	if(buffer_size == 0) panic("Estoque não iniciado\nUse estoque_iniciar()", ESTOQUE_NAO_INICIADO);
	if(receitas == NULL) panic("Referencia de estoque perdida", ESTOQUE_NULO);
}

static inline int adicionar(receita_t receita) {
	if(existe(receita.codigo)) return CODIGO_EXISTENTE;

	block_t *b = new_block();
	if(b == NULL) return SEM_MEMORIA;

	b->receita = receita;

	return SUCESSO;
}

static inline int remover(const unsigned codigo) {
	block_t *bloco = encontrar(codigo);
	if(bloco == NULL) return INGREDIENTE_INEXISTENTE;
	free_block(bloco);

	return SUCESSO;
}

int receita_iniciar() {
	buffer_size = ((buffer_size + 4096)/4096) * 4096 * sizeof(receita_t);
	block_init();

	return SUCESSO;
}

int receita_encerrar() {
	free(receitas);

	return SUCESSO;
}
