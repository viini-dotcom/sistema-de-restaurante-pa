#include <assert.h>
#include <estoque.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define true 1 
#define false 0

static ingrediente_t *ingredientes;
static unsigned numero_ingredientes;
static unsigned buffer_size = 0;

static int numero_comeca_com(unsigned numero, const unsigned comeco) {
	while(numero != 0 && numero !=  comeco)
		numero /= 10;

	if(numero == 0) return false;

	return true;
}

//buffer
static inline int buffer_aumentar() {
	buffer_size = buffer_size * 2;

	void * tmp = 0;
	tmp = realloc(ingredientes, buffer_size);
	if(tmp == NULL) {
		buffer_size /= 2;
		return false;
	}

	ingredientes = tmp;
	return true;
}

static inline int buffer_diminuir() {
	buffer_size /= 2;

	void * tmp = 0;
	tmp = realloc(ingredientes, buffer_size);
	if(tmp == NULL) { 
		buffer_size *= 2;
		return false;
	}

	ingredientes = tmp;
	return true;
}

//função pra checar por errors
static inline int existe_ingrediente(const unsigned codigo) {
	for(int i = 0; i < numero_ingredientes; i++)
		if(ingredientes[i].codigo == codigo) return true;

	return false;
}

static inline ingrediente_t *encontrar_ingrediente(const unsigned codigo) {
	for(int i = 0; i < numero_ingredientes; i++)
		if(ingredientes[i].codigo == codigo) return &ingredientes[i];

	return NULL;
}


static inline void estoque_iniciado_checar() {
	if(buffer_size == 0) {
		printf("Estoque não iniciado\nUse estoque_iniciar()\n");
		exit(ESTOQUE_NAO_INICIADO);
	}
	if(ingredientes == NULL) {
		printf("Referencia de estoque é nulo\n");
		exit(ESTOQUE_NULO);
	}
}

static inline estoque_status_t adicionar(ingrediente_t ingrediente) {
	if(existe_ingrediente(ingrediente.codigo)) return INGREDIENTE_EXISTENTE;

	if(numero_ingredientes * sizeof(ingrediente) > buffer_size)
		if(!buffer_aumentar()) return SEM_MEMORIA;

	ingredientes[numero_ingredientes++] = ingrediente;

	return SUCESSO;
}

static inline estoque_status_t remover(const unsigned codigo) {
	if(!existe_ingrediente(codigo)) return INGREDIENTE_INEXISTENTE;
	if(numero_ingredientes * sizeof(ingrediente_t) < buffer_size / 2)
		if(!buffer_diminuir()) return SEM_MEMORIA;

	numero_ingredientes--;

	return SUCESSO;
}

//Função de inicio
estoque_status_t estoque_iniciar() {
	buffer_size = ((numero_ingredientes + 4095) / 4096) * sizeof(ingrediente_t);
	ingredientes = malloc(buffer_size);
	if(ingredientes == NULL) return SEM_MEMORIA;
	static_assert(1, "Implementar carregamento de dados\n");
	//Essa parte é pra carregar arquivos

	return SUCESSO;
}

//Procura no estoque
ingrediente_t *checar_ingrediente(const unsigned codigo) {
	for(int i = 0; i < numero_ingredientes; i++)
		if(ingredientes[i].codigo == codigo) return &ingredientes[i];

	return NULL;
}

int procurar_ingredientes_por_nome(ingrediente_t *restrict destino, const char *restrict nome) {
	estoque_iniciado_checar();

	unsigned dest_idx = 0;
	for(int i = 0; i < numero_ingredientes; i++) {
		if(strncmp(nome, ingredientes[i].nome, strlen(nome)) == 0) {
			destino[dest_idx] = ingredientes[i];
			dest_idx++;
		}
	}

	return dest_idx;
}

int procurar_ingredientes_por_codigo(ingrediente_t *restrict destino, const unsigned int codigo) {
	estoque_iniciado_checar();
	
	unsigned dest_idx = 0;
	for(int i = 0; i < numero_ingredientes; i++) {
		if(numero_comeca_com(ingredientes[i].codigo, codigo))
			destino[dest_idx++] = ingredientes[i];
	}

	return dest_idx;
}

//Controle de estoque
estoque_status_t adicionar_ingrediente(const char * restrict nome,
		const unsigned codigo, 
		const unidade_t unidade_de_medida, 
		const float quantidade_minima, 
		const float custo_unitario) {

	ingrediente_t new = {
		.codigo = codigo,
		.unidade_de_medida = unidade_de_medida,
		.quantidade = 0,
		.quatidade_minima = quantidade_minima,
		.custo_unitario = custo_unitario,
	};
	strcpy(new.nome, nome);

	int status = adicionar(new);
	if(status) return status;

	return SUCESSO;
}

estoque_status_t estocar_ingrediente(const unsigned int codigo, const unsigned int quantidade) {
	ingrediente_t *ingrediente = NULL;
	if(!(ingrediente = checar_ingrediente(codigo))) return INGREDIENTE_INEXISTENTE;
	
	ingrediente->quantidade += quantidade;

	return SUCESSO;
}

estoque_status_t usar_ingrediente(const unsigned int codigo, const unsigned int quantidade) {
	ingrediente_t *ingrediente = NULL;
	if(!(ingrediente = checar_ingrediente(codigo))) return INGREDIENTE_INEXISTENTE;
	if(ingrediente->quantidade < quantidade) return ESTOQUE_POUCO;
	
	ingrediente->quantidade -= quantidade;

	return SUCESSO;
}
