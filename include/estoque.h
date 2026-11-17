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
	NOME = 0,
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

int __ingredientes_procurar_NOME(ingrediente_t *restrict destino, const char *restrict nome); //
int __ingredientes_procurar_CODIGO(ingrediente_t *restrict destino, const unsigned long codigo); //
																			
int __ingrediente_editar_NOME(const unsigned codigo, const char * restrict nome);
int __ingrediente_editar_UNIDADE_DE_MEDIDA(const unsigned codigo, const int unidade_de_medida);
int __ingrediente_editar_QUANTIDADE_MINIMA(const unsigned codigo, const float quantidade_minima);
int __ingrediente_editar_CUSTO_UNITARIO(const unsigned codigo, const float custo_unitario);

int estoque_iniciar();
void estoque_encerrar();

#define ingredientes_procurar(dest, value, tipo) __ingredientes_procurar_##tipo(dest, value) //
int ingrediente_listar(ingrediente_t * restrict destino, const unsigned tamanho ,int valor, int ordem); //
int ingredientes_listar_faltando(ingrediente_t * restrict destino);

ingrediente_t *ingrediente_checar(const unsigned codigo); //
int 	ingrediente_adicionar(	const char * restrict nome,
						const unsigned codigo, 
						int unidade_de_medida, 
						const float quantidade_minima, 
						const float custo_unitario); //

int 	ingrediente_remover(const unsigned codigo); //
#define ingrediente_editar(secao, codigo, valor) __ingrediente_editar_##secao(codigo, valor)
int  ingrediente_existe(const unsigned codigo);

int 	estoque_adicionar(const unsigned codigo, const unsigned quantidade); //
int 	estoque_remover(const unsigned codigo, const unsigned quantidade); //

void gerar_registro(estoque_registro_t *dest);


#endif











