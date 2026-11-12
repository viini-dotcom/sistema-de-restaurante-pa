#include <estoque.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define true 1 
#define false 0

extern void carregar(void * restrict buffer, const char * path);

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


static inline int estoque_iniciado_checar() {
	if(buffer_size == 0) {
		printf("Estoque não iniciado\nUse estoque_iniciar()\n");
		return  false;
	}

	return true;
}

static inline estoque_status_t tentar_adicionar_novo(ingrediente_t ingrediente) {
	if(existe_ingrediente(ingrediente.codigo)) return INGREDIENTE_EXISTENTE;

	if(numero_ingredientes * sizeof(ingrediente) > buffer_size)
		if(buffer_aumentar()) {
			ingredientes[numero_ingredientes++] = ingrediente;
			return SUCESSO;
		}

	return SEM_MEMORIA;
}

//funções
estoque_status_t estoque_iniciar() {
	buffer_size = ((numero_ingredientes + 4095) / 4096) * sizeof(ingrediente_t);
	ingredientes = malloc(buffer_size);
	carregar(ingredientes, "stock.sav");
	if(ingredientes == NULL) return SEM_MEMORIA;

	return SUCESSO;
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

	int status = tentar_adicionar_novo(new);
	if(status) return status;

	return SUCESSO;
}
