#include <estoque.h>

#ifndef RECEITA_H
#define RECEITA_H
enum {
	RECEITA_INVALIDA = 0,
	RECEITA_INGREDIENTE_INVALIDO,
	INGREDIENTE_INVALIDO,
	RECEITA_JA_EXISTE,	
	RECEITA_INGREDIENTE_JA_EXISTE,	
	INGREDIENTE_INSUFICIENTE,
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

int receita_iniciar(receita_t *);
int receita_encerrar();

int __receita_editar_NOME(const unsigned codigo, const char * restrict nome);
int __receita_editar_CATEGORIA(const unsigned codigo, const int categoria);
int __receita_editar_PRECO(const unsigned codigo, const float preco);

int __receita_procurar_CODIGO(receita_t * restrict dest, const unsigned codigo);
int __receita_procurar_NOME(receita_t * restrict dest, const char * restrict nome);
#define receita_procurar(secao, dest, valor) \
	__receita_procurar_##secao(dest, valor)

int receita_listar(receita_t * restrict dest, const unsigned tamanho, int valor, int ordem);

int receita_adicionar(const char * restrict nome, const unsigned codigo, int categoria,const float preco);
#define receita_editar(secao, codigo, valor) __receita_editar_##secao(codigo, valor)
int receita_remover(const unsigned codigo);

int receita_adicionar_ingrediente(const unsigned codigo, const unsigned ing_codigo, const unsigned quantidade);
int receita_remover_ingrediente(const unsigned receita_codigo, const unsigned ing_codigo);
ingrediente_t * receita_checar_ingrediente(const unsigned receita_codigo, const unsigned ing_codigo);

int receita_ingrediente_listar(ingrediente_t * dest, const unsigned receita_codigo);
int receita_ingrediente_disponivel(const unsigned int receita_codigo, const unsigned quantidade);
int receita_fazer(const unsigned int receita_codigo, const unsigned quantidade);
int receita_disponiveis(receita_t * restrict dest);


#endif
