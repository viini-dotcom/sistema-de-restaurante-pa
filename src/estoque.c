#include <estoque.h>
#include <string.h>
#include <stdlib.h>

extern void carregar(void * restrict buffer, const char * path);

static ingrediente_t *ingredientes;
static unsigned numero_ingredientes;
static unsigned buffer_size;

static int numero_comeca_com(unsigned numero, const unsigned comeco) {
	while(numero != 0 && numero !=  comeco)
		numero /= 10;

	if(numero == 0) return 0;

	return 1;
}


//buffer
static inline void buffer_aumentar() {
	buffer_size = buffer_size * 2;
	ingredientes = realloc(ingredientes, buffer_size);
}

static inline void buffer_diminuir() {
	buffer_size /= 2;
	ingredientes = realloc(ingredientes, buffer_size);
}

int estoque_iniciar() {
	buffer_size = ((numero_ingredientes + 4095) / 4096) * sizeof(ingrediente_t);
	ingredientes = malloc(buffer_size);
	carregar(ingredientes, "stock.sav");
	if(ingredientes == NULL) return 0;

	return 1;
}


int __procura_ingrediente_por_nome(ingrediente_t *restrict destino, const char *restrict nome) {
	unsigned dest_idx = 0;
	for(int i = 0; i < numero_ingredientes; i++) {
		if(strncmp(nome, ingredientes[i].nome, strlen(nome)) == 0) {
			destino[dest_idx] = ingredientes[i];
			dest_idx++;
		}
	}

	return dest_idx;
}

int __procura_ingrediente_por_codigo(ingrediente_t *restrict destino, const unsigned int codigo) {
	unsigned dest_idx = 0;
	for(int i = 0; i < numero_ingredientes; i++) {
		if(numero_comeca_com(ingredientes[i].codigo, codigo))
			destino[dest_idx++] = ingredientes[i];
	}

	return dest_idx;
}

int adicionar_ingrediente(const char *restrict nome, 
		const unidade_t unidade_de_media, 
		const float quantidade_minima, 
		const float custo_unitario) {
	ingrediente_t novo_ingrediente = {
		.custo_unitario = custo_unitario,
		.codigo = numero_ingredientes + 1,
		.quantidade = 0,
		.unidade_de_medida = unidade_de_media,
		.quatidade_minima = quantidade_minima,
	};
	strcpy(novo_ingrediente.nome, nome);
}
