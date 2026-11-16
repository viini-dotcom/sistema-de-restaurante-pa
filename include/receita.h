#include <estoque.h>

#ifndef RECEITA_H
#define RECEITA_H
enum {
	RECEITA_INVALIDA = 0,
	RECEITA_INGREDIENTE_INVALIDO,
	INGREDIENTE_INVALIDO,
	RECEITA_JA_EXISTE,	
	RECEITA_INGREDIENTE_JA_EXISTE,	
};




enum {
	MASSA = 0,
	SALADA,
	SOBREMESA,
	BEBIDAS,
	SALGADO,
	CARNE,
	FRANGO,
};

enum {
	RECEITA_NOME,
	RECEITA_PRECO,
	RECEITA_CATEGORIA,
	RECEITA_CODIGO,
};

typedef struct {
	unsigned codigo;
	float quantidade;
	unsigned ativo;
} __ing_info_t;


typedef struct {
	unsigned codigo;
	char nome[128];
	int categoria;
	float preco;

	__ing_info_t ing[64];
	unsigned ing_num;
} receita_t;

int receita_iniciar();
int receita_encerrar();

int receita_adicionar(const char * restrict nome, const unsigned codigo, int categoria,const float preco);
int receita_editar(const unsigned codigo, int secao, unsigned valor);
int receita_remover(const unsigned codigo);

int receita_adicionar_ingrediente(const unsigned codigo, const ingrediente_t *i);
int receita_remover_ingrediente(const unsigned receita_codigo, const unsigned ing_codigo);
ingrediente_t * receita_checar_ingrediente(const unsigned receita_codigo, const unsigned ing_codigo);

int listar_receitas(const receita_t * restrict dest, const unsigned tamanho, int tipo, int ordem);


#endif
