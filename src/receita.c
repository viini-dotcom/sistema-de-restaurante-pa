#include "estoque.h"
#include <assert.h>
#include <receita.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define true 1 
#define false 0

typedef struct __block_t {
	receita_t receita;
	int ativo;
	struct __block_t * next;
} block_t;

static inline void 
panic(const char * restrict message, const unsigned exit_code) 
{
	printf("%s\n", message);
	exit(exit_code);
}


//Controle de memoria
static block_t *freelist = NULL;
static block_t *receitas = NULL;
static unsigned buffer_size = 0;
static block_t * buffer_top = NULL;
static unsigned diff = 0;

static inline int 
block_init(unsigned size) 
{
	buffer_size = size * sizeof(block_t);
	receitas = malloc(buffer_size);

	if(receitas == NULL) return SEM_MEMORIA;
	buffer_top = receitas;

	return 0;
}

static inline block_t *
take_free_block() 
{
	if(freelist == NULL) return NULL;
	block_t *bloco = freelist;
	freelist = freelist->next;
	bloco->ativo = 1;
	return bloco;
}

static inline void 
free_block(block_t *b) 
{
	if(freelist != NULL) b->next = freelist;
	b->ativo = 0;
	freelist = b;
}

static inline block_t *
new_block() 
{
	block_t * b = NULL;
	if((b = take_free_block())) return b;
	
	if(buffer_size <= (buffer_top - receitas) * sizeof(block_t)) {
		unsigned long diff = buffer_top - receitas;
		void *tmp = realloc(receitas, (buffer_size * 2));
		if(tmp == NULL) return NULL;
			
		receitas = tmp;
		buffer_top = receitas + diff;

		buffer_size *= 2;
	}

	buffer_top->ativo = 1;
	buffer_top->next = NULL;
	diff += 1;

	return buffer_top++;
}

//ferramentas
static int 
numero_comeca_com(unsigned numero, const unsigned comeco) 
{

	while(numero != 0 && numero !=  comeco)
		numero /= 10;

	if(numero == 0) return false;

	return true;
}

static char * 
strlower(char * restrict dest,const char * restrict s) 
{
	for(int i = 0; i < strlen(s); i++)
		dest[i] = tolower(s[i]);

	return dest;
}

static int 
strcmp_lower(const char * restrict s1, const char * restrict s2) 
{
	char buf1[256];
	char buf2[256];

	return strcmp(strlower(buf1, s1), strlower(buf2, s2));
}

static int 
strncmp_lower(const char * restrict s1, const char * restrict s2, unsigned len) 
{
	char buf1[256];
	char buf2[256];

	return strncmp(strlower(buf1, s1), strlower(buf2, s2), len);
}


static void 
trocar_posicao_receita(receita_t * restrict i1, receita_t * restrict i2) 
{
	receita_t tmp = *i1;
	*i1 = *i2;
	*i2 = tmp;
}

//ordenação
static receita_t * 
ordernar_receitas_nome(receita_t * restrict dest, unsigned length, int ordem) 
{
	for(int i = 0; i < length; i++) {
		for(int j = i + 1; j < length; j++) {
			int n = strcmp_lower(dest[i].nome, dest[j].nome);
			if(ordem == CRESCENTE) {
				if(n < 0) continue; //aqui se caso nao, então não é pra troca
				trocar_posicao_receita(&dest[i], &dest[j]);
				continue;
			}
			if(ordem == DECRESCENTE) {
				if(n > 0) continue; //aqui se caso sim, não é pra trocar
				trocar_posicao_receita(&dest[i], &dest[j]);
				continue;
			}
		}
	}

	return dest;
}

static receita_t * 
ordernar_receitas_codigo(receita_t * restrict dest, unsigned length, int ordem) 
{
	for(int i = 0; i < diff; i++) {
		for(int j = i + 1; j < diff; j++) {
			if(ordem == CRESCENTE && dest[j].codigo > dest[i].codigo) {
				trocar_posicao_receita(&dest[i], &dest[j]);
			}
			if(ordem == DECRESCENTE && dest[j].codigo < dest[i].codigo) {
				trocar_posicao_receita(&dest[i], &dest[j]);
				continue;
			}
		}
	}

	return dest;
}

static receita_t * 
ordernar_receitas_preco(receita_t * restrict dest, unsigned length, int ordem) 
{
	for(int i = 0; i < length; i++) {
		for(int j = i + 1; j < length; j++) {
			if(ordem == CRESCENTE && dest[j].preco > dest[i].preco)
				trocar_posicao_receita(&dest[i], &dest[j]);

			if(ordem == DECRESCENTE && dest[j].preco < dest[i].preco)
				trocar_posicao_receita(&dest[i], &dest[j]);
		}
	}

	return dest;
}

//Manipular com segurança
static inline int 
existe(const unsigned codigo) 
{
	for(int i = 0; i < diff; i++)
		if(receitas[i].ativo == 1 && receitas[i].receita.codigo == codigo) return true;

	return false;
}

static block_t *
encontrar(const unsigned codigo) 
{
	for(int i = 0; i < diff; i++)
		if(receitas[i].ativo == 1 && receitas[i].receita.codigo == codigo) return &receitas[i];

	return NULL;
}


static inline
void receita_iniciado_checar() 
{
	if(buffer_size == 0) panic("Estoque não iniciado\nUse receita_iniciar()", ESTOQUE_NAO_INICIADO);
	if(receitas == NULL) panic("Referencia de estoque perdida", ESTOQUE_NULO);
}

static int 
adicionar(receita_t receita) 
{
	if(existe(receita.codigo)) return CODIGO_EXISTENTE;

	block_t *b = new_block();
	if(b == NULL) return SEM_MEMORIA;

	b->receita = receita;

	return SUCESSO;
}

static int 
remover(const unsigned codigo) 
{
	block_t *bloco = encontrar(codigo);
	if(bloco == NULL) return INGREDIENTE_INEXISTENTE;
	free_block(bloco);

	return SUCESSO;
}

static int 
existe_ing(const unsigned cod_rec, const unsigned cod_ing) 
{
	block_t * rec = encontrar(cod_rec);
	if(rec == NULL) return false;
	for(int i = 0; i < rec->receita.ing_num; i++)
		if(rec->receita.ing[i].ativo == true && rec->receita.ing[i].codigo == cod_ing) return true;


	return false;
}

static __ing_info_t * 
encontrar_ing(const unsigned cod_rec, const unsigned cod_ing) 
{
	block_t * rec = encontrar(cod_rec);
	if(rec == NULL) return NULL;

	for(int i = 0; i < rec->receita.ing_num; i++)
		if(rec->receita.ing[i].ativo == true && rec->receita.ing[i].codigo == cod_ing) return &rec->receita.ing[i];

	return NULL;
}

static int 
adicionar_ing(const unsigned cod_rec, const unsigned cod_ing, const unsigned qnt) 
{
	block_t *r = encontrar(cod_rec);
	if(r == NULL) return RECEITA_INVALIDA;
	if(existe_ing(cod_rec, cod_ing)) return RECEITA_INGREDIENTE_JA_EXISTE;
	if(ingrediente_checar(cod_ing) == NULL) return INGREDIENTE_INVALIDO;

	if(r->receita.ing_num < 64) {
		r->receita.ing[r->receita.ing_num].codigo = cod_ing;
		r->receita.ing[r->receita.ing_num].quantidade = qnt;
		r->receita.ing[r->receita.ing_num].ativo = true;

		r->receita.ing_num++;

		return SUCESSO;
	}

	for(int i = 0; i < 64; i++)
		if(r->receita.ing[r->receita.ing_num].ativo == false) {
			r->receita.ing[r->receita.ing_num].codigo = cod_ing;
               r->receita.ing[r->receita.ing_num].quantidade = qnt;
               r->receita.ing[r->receita.ing_num].ativo = true;

			return SUCESSO;
		}

	return -1;
}

static int 
remover_ing(const unsigned cod_rec, const unsigned cod_ing) 
{
	if(existe(cod_rec) == false) return RECEITA_INVALIDA;
	if(ingrediente_checar(cod_ing) == NULL) return INGREDIENTE_INVALIDO;

	__ing_info_t *ing = encontrar_ing(cod_rec, cod_ing);
	if(ing == NULL) return RECEITA_INGREDIENTE_INVALIDO;

	
	ing->ativo = false;
	
	return  SUCESSO;
}

static int 
editar_ing(const unsigned cod_rec, const unsigned cod_ing, const unsigned qnt) 
{
	if(existe(cod_rec) == false) return RECEITA_INVALIDA;
	if(ingrediente_checar(cod_ing) == NULL) return INGREDIENTE_INVALIDO;

	__ing_info_t *ing = encontrar_ing(cod_rec, cod_ing);
	if(ing == NULL) return RECEITA_INGREDIENTE_INVALIDO;



	ing->quantidade = qnt;

	return SUCESSO;
}







//Principal
int 
receita_iniciar(receita_t *lista) 
{
	block_init(512);
	while(lista)
		adicionar(*lista++);

	return SUCESSO;
}

int 
receita_encerrar() 
{
	free(receitas);

	return SUCESSO;
}


int 
receita_adicionar(const char *restrict nome, const unsigned int codigo, int categoria, const float preco) 
{
	receita_iniciado_checar();

	receita_t new = {
		.categoria = categoria,
		.codigo = codigo,
		.ing_num = 0,
		.preco = preco,
	};
	strcpy(new.nome, nome);
	memset(new.ing, 0, sizeof(new.ing));
	
	int status = adicionar(new);
	if(status) return status;
	
	return SUCESSO;
}

int 
receita_remover(const unsigned int codigo) 
{
	receita_iniciado_checar();

	int status = remover(codigo);
	if(status) return status;

	return SUCESSO;
}

int 
receita_adicionar_ingrediente(const unsigned int codigo, const unsigned ing_codigo, const unsigned quantidade) 
{
	receita_iniciado_checar();

	int status = adicionar_ing(codigo, ing_codigo, quantidade);
	if(status) return status;

	return SUCESSO;
}

int 
receita_remover_ingrediente(const unsigned int receita_codigo, const unsigned int ing_codigo) 
{
	receita_iniciado_checar();

	int status = remover_ing(receita_codigo, ing_codigo);
	if(status) return status;

	return SUCESSO;
}

ingrediente_t * 
receita_checar_ingrediente(const unsigned receita_codigo, const unsigned ing_codigo) 
{
	receita_iniciado_checar();

	__ing_info_t *i = encontrar_ing(receita_codigo, ing_codigo);
	if(i == NULL) return NULL;

	return ingrediente_checar(i->codigo);
}

//editar 
int 
__receita_editar_CATEGORIA(const unsigned int codigo, const int categoria) 
{
	receita_iniciado_checar();

	block_t *r = encontrar(codigo);
	if(r == NULL) return RECEITA_INVALIDA;
	r->receita.categoria = categoria;

	return SUCESSO;
}

int 
__receita_editar_NOME(const unsigned int codigo, const char * restrict nome) 
{
	receita_iniciado_checar();

	block_t *r = encontrar(codigo);
	if(r == NULL) return RECEITA_INVALIDA;
	strcpy(r->receita.nome, nome);

	return SUCESSO;
}

int 
__receita_editar_PRECO(const unsigned int codigo, const float preco) 
{
	receita_iniciado_checar();

	block_t *r = encontrar(codigo);
	if(r == NULL) return RECEITA_INVALIDA;
	r->receita.preco = preco;

	return SUCESSO;
}

int __receita_procurar_NOME(receita_t *restrict destino, const char *restrict nome) {
	receita_iniciado_checar();

	unsigned dest_idx = 0;
	for(int i = 0; i < diff; i++) {
		if(receitas[i].ativo == 0) continue;
		if(strncmp_lower(nome , receitas[i].receita.nome, strlen(nome)) == 0) {
			destino[dest_idx] = receitas[i].receita;
			dest_idx++;
		}
	}

	ordernar_receitas_nome(destino, dest_idx, CRESCENTE);

	return dest_idx;
}

int __receita_procurar_CODIGO(receita_t *restrict destino, const unsigned codigo) {
	receita_iniciado_checar();

	unsigned dest_idx = 0;
	for(int i = 0; i < diff; i++) {
		if(receitas[i].ativo == 0) continue;

		if(numero_comeca_com(receitas[i].receita.codigo, codigo))
			destino[dest_idx++] = receitas[i].receita;
	}

	ordernar_receitas_codigo(destino, dest_idx, CRESCENTE);

	return dest_idx;
}

int 
receita_listar(receita_t *restrict destino,const unsigned tamanho, int tipo, int ordem) 
{
	receita_iniciado_checar();

	for(int i = 0; i < diff; i++) destino[i] = receitas[i].receita;

	switch(tipo) {
		case NOME:
			ordernar_receitas_nome(destino,  tamanho, ordem);
			break;
		case CODIGO:
			ordernar_receitas_codigo(destino, tamanho, ordem);
			break;
		case PRECO:
			ordernar_receitas_preco(destino, tamanho, ordem);
			break;

		default: return TIPO_INVALIDO;
	}

	return SUCESSO;
}

int 
receita_ingrediente_listar(ingrediente_t *dest, const unsigned int receita_codigo)
{
	receita_iniciado_checar();

	int dest_idx = 0;
	block_t *r = encontrar(receita_codigo);
	if(r == NULL) return RECEITA_INVALIDA;
	for(int i = 0; i < r->receita.ing_num; i++)
		if(r->receita.ing[i].ativo == 1) {
			ingrediente_t *ing = ingrediente_checar(r->receita.ing[i].codigo);
			if(ing == NULL) continue;

			dest[dest_idx++] = *ing;
		}

	return dest_idx;
}

int
receita_ingrediente_disponivel(const unsigned int receita_codigo, const unsigned quantidade) 
{
	receita_iniciado_checar();

	ingrediente_t dest[64];
	int idx = receita_ingrediente_listar(dest, receita_codigo);
	if(idx <= 0) return -1;

	for(int i = 0; i < idx; i++) {
		__ing_info_t *ing = encontrar_ing(receita_codigo, dest[i].codigo);
		if(ing == NULL) return -1;
		if(dest[i].quantidade < ing->quantidade * quantidade) return INGREDIENTE_INSUFICIENTE;
	}
	
	return SUCESSO;
}

int 
receita_fazer(const unsigned int receita_codigo, const unsigned int quantidade) {
	int status = receita_ingrediente_disponivel(receita_codigo, quantidade);
	if(status) return status;

	ingrediente_t dest[64];
	int idx = receita_ingrediente_listar(dest, receita_codigo);
	if(idx <= 0) return -1;

	for(int i = 0; i < idx; i++) 
	{
		__ing_info_t *ing = encontrar_ing(receita_codigo, dest[i].codigo);
		status  = estoque_remover(ing->codigo, ing->quantidade * quantidade);
		if(status) return status;
	}

	return SUCESSO;
}

int
receita_disponiveis(receita_t * restrict dest) 
{
	int idx = 0;
	
	for(int i = 0; i < diff; i++) { 
		if(receitas[i].ativo == false) continue;
		if(receita_ingrediente_disponivel(receitas[i].receita.codigo, 1) != 0) continue;
		dest[idx++] = receitas[i].receita;
	}

	return idx;
}
