#ifndef ESTOQUE_H
#define ESTOQUE_H
enum {
	KILOGRAMAS = 0,
	LITROS,
	MILILITROS,
	UNIDADE,
};

enum {
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
};

enum {
	UNIDADE_DE_MEDIDA,
	QUANTIDADE_MINIMA,
	CUSTO_UNITARIO,
};

enum {
	CRESCENTE = 0,
	DECRESCENTE,
};

enum {
	NOME,
	PRECO,
	CODIGO,
};

typedef struct {
	char * name;
	unsigned code;
	int quantity;
} estoque_registro_t;

typedef struct {
	unsigned codigo;
	char nome[64];
	int unidade_de_medida;
	float quantidade;
	float quatidade_minima;
	float custo_unitario;
} ingrediente_t;

int __procurar_ingredientes_NOME(ingrediente_t *restrict destino, const char *restrict nome); //
int __procurar_ingredientes_CODIGO(ingrediente_t *restrict destino, const unsigned long codigo); //

int estoque_iniciar();
void estoque_encerrar();

#define 			ingredientes_procurar(dest, value, tipo) __procurar_ingredientes_##tipo(dest, value) //
int ingrediente_listar(ingrediente_t * restrict destino, const unsigned tamanho ,int tipo, int ordem); //
int ingredientes_listar_faltando(ingrediente_t * restrict destino);

ingrediente_t *ingrediente_checar(const unsigned codigo); //
int 	ingrediente_adicionar(	const char * restrict nome,
									const unsigned codigo, 
									int unidade_de_medida, 
									const float quantidade_minima, 
									const float custo_unitario); //

int 	ingrediente_remover(const unsigned codigo); //
int 	ingrediente_editar(const unsigned codigo, const int secao,const float valor);

int 	ingrediente_estocar(const unsigned codigo, const unsigned quantidade); //
int 	ingrediente_usar(const unsigned codigo, const unsigned quantidade); //

void gerar_registro(estoque_registro_t *dest);


#endif











