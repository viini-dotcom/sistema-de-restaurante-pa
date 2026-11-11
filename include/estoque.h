#ifndef ESTOQUE_H
#define ESTOQUE_H
typedef enum {
	KILOGRAMAS,
	LITROS,
	MILILITROS,
	UNIDADE,
} unidade_t;



typedef struct {
	unsigned codigo;
	char nome[64];
	unidade_t unidade_de_medida;
	float quantidade;
	float quatidade_minima;
	float custo_unitario;
} ingrediente_t;







int __procura_ingrediente_por_nome(ingrediente_t * restrict destino, const char * restrict nome);
int __procura_ingrediente_por_codigo(ingrediente_t * restrict destino, const unsigned codigo);

int lista_ingrediente_por_nome(char * restrict destino);
int lista_ingrediente_por_codigo(char * restrict destino);

ingrediente_t checar_ingrediante(const unsigned codigo);

int adicionar_ingrediente(const char * restrict nome, 
		const unidade_t unidade_de_media, 
		const float quantidade_minima, 
		const float custo_unitario);


int remover_estoque_ingrediente(const unsigned codigo, const float quantidade);
int adicionar_estoque_ingrediente(const unsigned codigo, const float quantidade);

#define procurar_ingrediente(dest, ingrediente) 		\
	_Generic((ingrediente), 			  			\
			char *: __procura_ingrediente_por_nome, \
			int: __procura_ingrediente_por_codigo)(dest, ingrediente)







#endif
