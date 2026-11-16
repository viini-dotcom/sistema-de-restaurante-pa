#include "estoque.h"
#include <assert.h>
#include <receita.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
static inline int 
existe(const unsigned codigo) {
	for(int i = 0; i < diff; i++)
		if(receitas[i].ativo == 1 && receitas[i].receita.codigo == codigo) return true;

	return false;
}

static block_t *
encontrar(const unsigned codigo) {
	for(int i = 0; i < diff; i++)
		if(receitas[i].ativo == 1 && receitas[i].receita.codigo == codigo) return &receitas[i];

	return NULL;
}


static inline
void receita_iniciado_checar() {
	if(buffer_size == 0) panic("Estoque não iniciado\nUse estoque_iniciar()", ESTOQUE_NAO_INICIADO);
	if(receitas == NULL) panic("Referencia de estoque perdida", ESTOQUE_NULO);
}

static int 
adicionar(receita_t receita) {
	if(existe(receita.codigo)) return CODIGO_EXISTENTE;

	block_t *b = new_block();
	if(b == NULL) return SEM_MEMORIA;

	b->receita = receita;

	return SUCESSO;
}

static int 
remover(const unsigned codigo) {
	block_t *bloco = encontrar(codigo);
	if(bloco == NULL) return INGREDIENTE_INEXISTENTE;
	free_block(bloco);

	return SUCESSO;
}

static int
editar(const unsigned codigo, const int secao, const long long valor) {
	block_t *blk = encontrar(codigo);
	if(blk == NULL) return INGREDIENTE_INEXISTENTE;

	switch(secao) {
		case RECEITA_NOME:;
			char *str = (char *)valor;
			strcpy(blk->receita.nome, str);

			break;
		case RECEITA_CATEGORIA:
			blk->receita.categoria = valor;
			break;
		case RECEITA_PRECO:;
			float preco = (float)valor;
			blk->receita.preco = preco;

			break;
		case RECEITA_CODIGO:;


		default: return -1;
	}

	return SUCESSO;
}

static int 
existe_ing(const unsigned cod_rec, const unsigned cod_ing) {
	block_t * rec = encontrar(cod_rec);
	if(rec == NULL) return false;
	for(int i = 0; i < rec->receita.ing_num; i++)
		if(rec->receita.ing[i].ativo == true && rec->receita.ing[i].codigo == cod_ing) return true;


	return false;
}

static __ing_info_t * 
encontrar_ing(const unsigned cod_rec, const unsigned cod_ing) {
	block_t * rec = encontrar(cod_rec);
	if(rec == NULL) return NULL;
	for(int i = 0; i < rec->receita.ing_num; i++)
		if(rec->receita.ing[i].ativo == true && rec->receita.ing[i].codigo == cod_ing) return &rec->receita.ing[i];

	return NULL;
}

static int 
adicionar_ing(const unsigned cod_rec, const unsigned cod_ing, const unsigned qnt) {
	block_t *r = encontrar(cod_rec);
	if(r == NULL) return RECEITA_INVALIDA;
	if(existe_ing(cod_rec, cod_ing)) return RECEITA_INGREDIENTE_JA_EXISTE;
	if(ingrediente_checar(cod_ing) == NULL) return INGREDIENTE_INVALIDO;

	if(r->receita.ing_num < 64) {
		r->receita.ing[r->receita.ing_num].codigo = cod_ing;
		r->receita.ing[r->receita.ing_num].quantidade = qnt;
		r->receita.ing[r->receita.ing_num].ativo = true;

		r->receita.ing_num++;

		return SUCESSO;
	}

	for(int i = 0; i < 64; i++)
		if(r->receita.ing[r->receita.ing_num].ativo == false) {
			r->receita.ing[r->receita.ing_num].codigo = cod_ing;
               r->receita.ing[r->receita.ing_num].quantidade = qnt;
               r->receita.ing[r->receita.ing_num].ativo = true;

			return SUCESSO;
		}

	return -1;
}

static int 
remover_ing(const unsigned cod_rec, const unsigned cod_ing) {
	if(existe(cod_rec) == false) return RECEITA_INVALIDA;
	if(ingrediente_checar(cod_ing) == NULL) return INGREDIENTE_INVALIDO;

	__ing_info_t *ing = encontrar_ing(cod_rec, cod_ing);
	if(ing == NULL) return RECEITA_INGREDIENTE_INVALIDO;

	
	ing->ativo = false;
	
	return  SUCESSO;
}

static int 
editar_ing(const unsigned cod_rec, const unsigned cod_ing, const unsigned qnt) {
	if(existe(cod_rec) == false) return RECEITA_INVALIDA;
	if(ingrediente_checar(cod_ing) == NULL) return INGREDIENTE_INVALIDO;

	__ing_info_t *ing = encontrar_ing(cod_rec, cod_ing);
	if(ing == NULL) return RECEITA_INGREDIENTE_INVALIDO;



	ing->quantidade = qnt;

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


int receita_adicionar(const char *restrict nome, const unsigned int codigo, int categoria, const float preco) {
	receita_t new = {
		.categoria = categoria,
		.codigo = codigo,
		.ing_num = 0,
		.preco = preco,
	};
	strcpy(new.nome, nome);
	memset(new.ing, 0, sizeof(new.ing));
	
	int status = adicionar(new);
	if(status) return status;
	
	return SUCESSO;
}

int receita_remover(const unsigned int codigo) {
	int status = remover(codigo);
	if(status) return status;

	return SUCESSO;
}

int receita_editar(const unsigned int codigo, int secao, unsigned int valor) {
	
}


