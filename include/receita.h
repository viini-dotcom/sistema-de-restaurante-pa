#include <estoque.h>

#ifndef RECEITA_H
#define RECEITA_H
enum {
	MASSA = 0,
	SALADA,
	SOBREMESA,
	BEBIDAS,
	SALGADO,
	CARNE,
	FRANGO,
};



typedef struct {
	unsigned codigo;
	char nome[128];
	int categoria;
	float preco;
	ingrediente_t ings[64];
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
