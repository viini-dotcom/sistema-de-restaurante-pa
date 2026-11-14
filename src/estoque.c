#include <assert.h>
#include <ctype.h>
#include <estoque.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define true 1 
#define false 0

typedef struct __block_t {
	ingrediente_t ingrediente;
	int ativo;
	struct __block_t * next;
} block_t;

static block_t *freelist = NULL;
static block_t *ingredientes = NULL;
static unsigned numero_ingredientes = 0;
static unsigned buffer_size = 0;

static int numero_comeca_com(unsigned numero, const unsigned comeco) {
	while(numero != 0 && numero !=  comeco)
		numero /= 10;

	if(numero == 0) return false;

	return true;
}


static char * lower_string(char * restrict dest, const char * restrict string) {
	for(int i = 0; i < strlen(string); i++) dest[i] = tolower(string[i]);

	return dest;
}


//Retorna false se i2 vem primeiro que i1
static int compare_ingredients_nome(ingrediente_t * restrict i1, ingrediente_t * restrict i2) {
	unsigned s1len = strlen(i1->nome);
	unsigned s2len = strlen(i2->nome);
	unsigned min = (s1len > s2len) ? s2len : s1len;
	char s1[32];
	char s2[32];
	unsigned cmp = strncmp(lower_string(s1, i1->nome), lower_string(s2, i2->nome), min);

	if(cmp == 0) {
		if(s1len > s2len)
			return false;
		return true;
	}

	for(int i = 0; i < min; i++) {
		if(i1->nome[i] - i2->nome[i] == 0) continue;
		if(-(i1->nome[i] - i2->nome[i]) > 0) return true;
		return false;
	}



	return true;
}

static inline void trocar_posicao_ingredientes(ingrediente_t * restrict i1, ingrediente_t * restrict i2) {
	ingrediente_t tmp = *i1;
	*i1 = *i2;
	*i2 = tmp;
}


static ingrediente_t * ordernar_ingredientes_nome(ingrediente_t * restrict dest, unsigned length, ordem_t ordem) {
	for(int i = 0; i < numero_ingredientes; i++) dest[i] = ingredientes[i].ingrediente;

	for(int i = 0; i < numero_ingredientes; i++) {
		for(int j = i + 1; j < numero_ingredientes; j++) {
			int vem_primeiro = compare_ingredients_nome(&dest[j], &dest[i]); //dest[j].nome vem primeiro que dest[i].nome ?
			if(ordem == CRESCENTE) {
				if(vem_primeiro == false) continue; //aqui se caso nao, então não é pra troca
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
				continue;
			}
			if(ordem == DECRESCENTE) {
				if(vem_primeiro == true) continue; //aqui se caso sim, não é pra trocar
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
				continue;
			}
		}
	}

	return dest;
}

static ingrediente_t * ordernar_ingredientes_codigo(ingrediente_t * restrict dest, unsigned length, ordem_t ordem) {
	for(int i = 0; i < numero_ingredientes; i++) dest[i] = ingredientes[i].ingrediente;

	for(int i = 0; i < numero_ingredientes; i++) {
		for(int j = i + 1; j < numero_ingredientes; j++) {
			if(ordem == CRESCENTE && dest[j].codigo > dest[i].codigo) {
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
			}
			if(ordem == DECRESCENTE && dest[j].codigo < dest[i].codigo) {
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
				continue;
			}
		}
	}

	return dest;
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

static inline block_t *take_free_block() {
	if(freelist == NULL) return NULL;
	block_t *bloco = freelist;
	freelist = freelist->next;
	return bloco;
}

static inline void free_block(block_t *b) {
	if(freelist != NULL) b->next = freelist;
	b->ativo = 0;
	freelist = b;
}

//função pra checar por errors
static inline void panic(const char * restrict message, const unsigned exit_code) {
	printf("%s\n", message);
	exit(exit_code);
}
static inline int existe(const unsigned codigo) {
	for(int i = 0; i < numero_ingredientes; i++)
		if(ingredientes[i].ativo == 1 && ingredientes[i].ingrediente.codigo == codigo) return true;

	return false;
}

static inline block_t *encontrar(const unsigned codigo) {
	for(int i = 0; i < numero_ingredientes; i++)
		if(ingredientes[i].ativo == 1 && ingredientes[i].ingrediente.codigo == codigo) return &ingredientes[i];

	return NULL;
}


static inline void estoque_iniciado_checar() {
	if(buffer_size == 0) panic("Estoque não iniciado\nUse estoque_iniciar()", ESTOQUE_NAO_INICIADO);
	if(ingredientes == NULL) panic("Referencia de estoque perdida", ESTOQUE_NULO);
}

static inline estoque_status_t adicionar(ingrediente_t ingrediente) {
	if(existe(ingrediente.codigo)) return CODIGO_EXISTENTE;

	block_t *b = take_free_block();
	if(b != NULL) {
		b->ingrediente = ingrediente;
		b->ativo = 1;
		b->next = NULL;

		return SUCESSO;
	}

	if(numero_ingredientes * sizeof(ingrediente) > buffer_size)
		if(!buffer_aumentar()) return SEM_MEMORIA;
	ingredientes[numero_ingredientes].ativo = 1;
	ingredientes[numero_ingredientes++].ingrediente = ingrediente;


	return SUCESSO;
}

static inline estoque_status_t remover(const unsigned codigo) {
	block_t *bloco = encontrar(codigo);
	if(bloco == NULL) return INGREDIENTE_INEXISTENTE;
	free_block(bloco);

	return SUCESSO;
}

//Função de inicio
estoque_status_t estoque_iniciar() {
	buffer_size = ((numero_ingredientes + 4096) / 4096) * 4096 * sizeof(ingrediente_t);

	ingredientes = malloc(buffer_size);
	if(ingredientes == NULL) return SEM_MEMORIA;

	return SUCESSO;
}

void estoque_encerrar() {
	free(ingredientes);
}

//Procura no estoque
ingrediente_t *checar_ingrediente(const unsigned codigo) {
	estoque_iniciado_checar();


	block_t *bloco = encontrar(codigo);
	if(bloco != NULL) return &bloco->ingrediente;


	return NULL;
}

int __procurar_ingredientes_NOME(ingrediente_t *restrict destino, const char *restrict nome) {
	unsigned dest_idx = 0;
	for(int i = 0; i < numero_ingredientes; i++) {
		if(ingredientes[i].ativo == 0) continue;

		if(strncmp(nome, ingredientes[i].ingrediente.nome, strlen(nome)) == 0) {
			destino[dest_idx] = ingredientes[i].ingrediente;
			dest_idx++;
		}
	}

	return dest_idx;
}

int __procurar_ingredientes_CODIGO(ingrediente_t *restrict destino, const unsigned long codigo) {
	unsigned dest_idx = 0;
	for(int i = 0; i < numero_ingredientes; i++) {
		if(ingredientes[i].ativo == 0) continue;

		if(numero_comeca_com(ingredientes[i].ingrediente.codigo, codigo))
			destino[dest_idx++] = ingredientes[i].ingrediente;
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

estoque_status_t editar_ingrediente(const unsigned codigo, const unsigned secao, secao_t valor) {
	ingrediente_t *ingrediente = checar_ingrediente(codigo);
	if(ingrediente == NULL) return INGREDIENTE_INEXISTENTE;

	switch(secao) {
		case UNIDADE_DE_MEDIDA:;
			unsigned valor_em_int = (unidade_t)valor;
			if(valor_em_int > 3 || valor_em_int < 0) return UNIDADE_INVALIDA;

			ingrediente->unidade_de_medida = valor_em_int;
			break;
		case QUANTIDADE_MINIMA:
			ingrediente->quatidade_minima = valor;
			break;
		case CUSTO_UNITARIO:
			ingrediente->custo_unitario = valor;
			break;
		default: return SECAO_INVALIDA;
	}

	return SUCESSO;
}

estoque_status_t estocar_ingrediente(const unsigned int codigo, const unsigned int quantidade) {
	ingrediente_t *ingrediente = NULL;
	if(!(ingrediente = checar_ingrediente(codigo))) return INGREDIENTE_INEXISTENTE;
	
	ingrediente->quantidade += quantidade;
	printf("%f\n", ingrediente->quantidade);

	return SUCESSO;
}

estoque_status_t usar_ingrediente(const unsigned int codigo, const unsigned int quantidade) {
	ingrediente_t *ingrediente = NULL;
	if(!(ingrediente = checar_ingrediente(codigo))) return INGREDIENTE_INEXISTENTE;
	if(ingrediente->quantidade < quantidade) return ESTOQUE_POUCO;
	
	ingrediente->quantidade -= quantidade;

	return SUCESSO;
}

int listar_ingrediente_por(ingrediente_t *restrict destino,const unsigned tamanho, tipo_t tipo, ordem_t ordem) {
	switch(tipo) {
		case NOME:
			ordernar_ingredientes_nome(destino,  tamanho, ordem);
			break;
		case CODIGO:
			ordernar_ingredientes_codigo(destino, tamanho, ordem);
			break;
		default: return -1;
	}

	return 0;
}
