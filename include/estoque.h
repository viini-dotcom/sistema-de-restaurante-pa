#ifndef ESTOQUE_H
#define ESTOQUE_H
typedef enum {
	KILOGRAMAS,
	LITROS,
	MILILITROS,
	UNIDADE,
} unidade_t;

typedef enum {
	SUCESSO = 0,
	INGREDIENTE_EXISTENTE,
	SEM_MEMORIA,
} estoque_status_t;

typedef struct {
	unsigned codigo;
	char nome[64];
	unidade_t unidade_de_medida;
	float quantidade;
	float quatidade_minima;
	float custo_unitario;
} ingrediente_t;

int procurar_ingredientes_por_nome(ingrediente_t * restrict destino, const char * restrict nome);
int procurar_ingredientes_por_codigo(ingrediente_t * restrict destino, const unsigned codigo);

int lista_ingrediente_por_nome(char * restrict destino);
int lista_ingrediente_por_codigo(char * restrict destino);

ingrediente_t checar_ingrediante(const unsigned codigo);

estoque_status_t adicionar_ingrediente(const char * restrict nome,
		const unsigned codigo, 
		const unidade_t unidade_de_medida, 
		const float quantidade_minima, 
		const float custo_unitario);

int estocar_ingrediente(const unsigned codigo, const unsigned quantidade);
int usar_ingrediente(const unsigned codigo, const unsigned quantidade);


#endif











