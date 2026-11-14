#ifndef ESTOQUE_H
#define ESTOQUE_H
typedef enum {
	KILOGRAMAS = 0,
	LITROS,
	MILILITROS,
	UNIDADE,
} unidade_t;

typedef const enum {
	SUCESSO = 0,

	CODIGO_EXISTENTE,
	CODIGO_INEXISTENTE,
	
	INGREDIENTE_EXISTENTE,
	INGREDIENTE_INEXISTENTE,
	
	SEM_MEMORIA,

	ESTOQUE_NAO_INICIADO,
	ESTOQUE_NULO,
	ESTOQUE_POUCO,
	ESTOQUE_AINDA_DISPONIVEL,

	SECAO_INVALIDA,
	TIPO_INVALIDO,
	UNIDADE_INVALIDA,
} estoque_status_t;

typedef const enum {
	UNIDADE_DE_MEDIDA,
	QUANTIDADE_MINIMA,
	CUSTO_UNITARIO,
} secao_t;

typedef const enum {
	CRESCENTE = 0,
	DECRESCENTE,
} ordem_t;

typedef const enum {
	NOME,
	PRECO,
	CODIGO,
} tipo_t;

typedef struct {
	char * name;
	unsigned code;
	int quantity;
} estoque_registro_t;

typedef struct {
	unsigned codigo;
	char nome[64];
	unidade_t unidade_de_medida;
	float quantidade;
	float quatidade_minima;
	float custo_unitario;
} ingrediente_t;

estoque_status_t estoque_iniciar();
void estoque_encerrar();

int __procurar_ingredientes_NOME(ingrediente_t *restrict destino, const char *restrict nome); //
int __procurar_ingredientes_CODIGO(ingrediente_t *restrict destino, const unsigned long codigo); //
#define procurar_ingredientes(dest, value, tipo) __procurar_ingredientes_##tipo(dest, value) //
int listar_ingrediente(ingrediente_t * restrict destino, const unsigned tamanho ,tipo_t tipo, ordem_t ordem); //
int listar_ingredientes_faltando(ingrediente_t * restrict destino);

ingrediente_t *checar_ingrediente(const unsigned codigo); //
estoque_status_t adicionar_ingrediente(const char * restrict nome,
		const unsigned codigo, 
		unidade_t unidade_de_medida, 
		const float quantidade_minima, 
		const float custo_unitario); //
estoque_status_t remover_ingrediente(const unsigned codigo); //
estoque_status_t editar_ingrediente(const unsigned codigo, const secao_t secao,const float valor);

estoque_status_t estocar_ingrediente(const unsigned codigo, const unsigned quantidade); //
estoque_status_t usar_ingrediente(const unsigned codigo, const unsigned quantidade); //

void gerar_registro(estoque_registro_t *dest);


#endif











