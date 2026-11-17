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

static inline void 
panic(const char * restrict message, const unsigned exit_code) 
{
	printf("%s\n", message);
	exit(exit_code);
}


//Controle de memoria
static block_t *freelist = NULL;
static block_t *ingredientes = NULL;
static unsigned buffer_size = 0;
static block_t * buffer_top = NULL;
static unsigned diff = 0;

static inline int 
block_init() 
{
	ingredientes = malloc(buffer_size);
	if(ingredientes == NULL) return SEM_MEMORIA;
	buffer_top = ingredientes;

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

	if(buffer_size < buffer_top - ingredientes) {
		void *tmp = realloc(ingredientes, (buffer_size * 2) * sizeof(ingrediente_t));
		if(tmp == NULL) return NULL;

		ingredientes = tmp;
		buffer_size *= 2;
	}

	buffer_top->ativo = 1;
	buffer_top->next = NULL;
	diff += 1;

	return buffer_top++;
}

//Registros de saida e entrada
static estoque_registro_t *registros = NULL;
static unsigned log_idx = 0;
static unsigned log_max_idx = 4096;


//Operações com registros
static void 
registro_criar(ingrediente_t i, int quantity) 
{
	if(registros == NULL) registros = malloc(4096 * sizeof(estoque_registro_t));
	if(log_idx == log_max_idx) {
		void *tmp = realloc(registros, (log_max_idx *= 2) * sizeof(estoque_registro_t));
		if(tmp != NULL) registros = tmp;
		else panic("Falhou ao reallocar", SEM_MEMORIA);
	}

	registros[log_idx].name = i.nome;
	registros[log_idx].code = i.codigo;
	registros[log_idx].quantity = quantity;

	log_idx++;
}

void 
gerar_registro(estoque_registro_t *dest) 
{
	for(int i = 0; i < log_idx; i++)
		dest[i] = registros[i];
}

//Ferramentas
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

static inline void 
trocar_posicao_ingredientes(ingrediente_t * restrict i1, ingrediente_t * restrict i2) 
{
	ingrediente_t tmp = *i1;
	*i1 = *i2;
	*i2 = tmp;
}


static ingrediente_t * 
ordernar_ingredientes_nome(ingrediente_t * restrict dest, unsigned length, int ordem) 
{
	for(int i = 0; i < length; i++) {
		for(int j = i + 1; j < length; j++) {
			int n = strcmp_lower(dest[i].nome, dest[j].nome);
			if(ordem == CRESCENTE) {
				if(n < 0) continue; //aqui se caso nao, então não é pra troca
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
				continue;
			}
			if(ordem == DECRESCENTE) {
				if(n > 0) continue; //aqui se caso sim, não é pra trocar
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
				continue;
			}
		}
	}

	return dest;
}

static ingrediente_t * 
ordernar_ingredientes_codigo(ingrediente_t * restrict dest, unsigned length, int ordem) 
{
	for(int i = 0; i < diff; i++) {
		for(int j = i + 1; j < diff; j++) {
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

static ingrediente_t * 
ordernar_ingredientes_preco(ingrediente_t * restrict dest, unsigned length, int ordem) 
{
	for(int i = 0; i < length; i++) {
		for(int j = i + 1; j < length; j++) {
			if(ordem == CRESCENTE && dest[j].custo_unitario > dest[i].custo_unitario)
				trocar_posicao_ingredientes(&dest[i], &dest[j]);

			if(ordem == DECRESCENTE && dest[j].custo_unitario < dest[i].custo_unitario)
				trocar_posicao_ingredientes(&dest[i], &dest[j]);
		}
	}

	return dest;
}


//função para manipular os ingredientes com segurança
static int 
existe(const unsigned codigo) 
{
	for(int i = 0; i < diff; i++)
		if(ingredientes[i].ativo == 1 && ingredientes[i].ingrediente.codigo == codigo) return true;

	return false;
}

static block_t *
encontrar(const unsigned codigo) 
{
	for(int i = 0; i < diff; i++)
		if(ingredientes[i].ativo == 1 && ingredientes[i].ingrediente.codigo == codigo) return &ingredientes[i];

	return NULL;
}


static inline void 
estoque_iniciado_checar() 
{
	if(buffer_size == 0) panic("Estoque não iniciado\nUse estoque_iniciar()", ESTOQUE_NAO_INICIADO);
	if(ingredientes == NULL) panic("Referencia de estoque perdida", ESTOQUE_NULO);
}

static int 
adicionar(ingrediente_t ingrediente) 
{
	if(existe(ingrediente.codigo)) return CODIGO_EXISTENTE;

	block_t *b = new_block();
	if(b == NULL) return SEM_MEMORIA;

	b->ingrediente = ingrediente;

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

//Função de inicio
int 
estoque_iniciar(ingrediente_t *lista) 
{
	buffer_size = ((buffer_size + 4096) / 4096) * 4096 * sizeof(ingrediente_t);
	block_init();
	while(lista++) adicionar(*lista);

	return SUCESSO;
}

void 
estoque_encerrar() 
{
	free(ingredientes);
}

//Procura no estoque
ingrediente_t *
ingrediente_checar(const unsigned codigo) 
{
	estoque_iniciado_checar();


	block_t *bloco = encontrar(codigo);
	if(bloco != NULL) return &bloco->ingrediente;


	return NULL;
}

int 
__ingredientes_procurar_NOME(ingrediente_t *restrict destino, const char *restrict nome) 
{
	estoque_iniciado_checar();

	unsigned dest_idx = 0;
	for(int i = 0; i < diff; i++) {
		if(ingredientes[i].ativo == 0) continue;
		if(strncmp_lower(nome , ingredientes[i].ingrediente.nome, strlen(nome)) == 0) {
			destino[dest_idx] = ingredientes[i].ingrediente;
			dest_idx++;
		}
	}

	ordernar_ingredientes_nome(destino, dest_idx, CRESCENTE);

	return dest_idx;
}

int 
__ingredientes_procurar_CODIGO(ingrediente_t *restrict destino, const unsigned long codigo) 
{
	estoque_iniciado_checar();

	unsigned dest_idx = 0;
	for(int i = 0; i < diff; i++) {
		if(ingredientes[i].ativo == 0) continue;

		if(numero_comeca_com(ingredientes[i].ingrediente.codigo, codigo))
			destino[dest_idx++] = ingredientes[i].ingrediente;
	}

	ordernar_ingredientes_codigo(destino, dest_idx, CRESCENTE);

	return dest_idx;
}


//Editar ingrediente
int 
__ingrediente_editar_NOME(const unsigned int codigo, const char *restrict nome) 
{
	block_t *ing = encontrar(codigo);
	if(ing == NULL) return INGREDIENTE_INEXISTENTE;
	strcpy(ing->ingrediente.nome, nome);

	return SUCESSO;
}

int 
__ingrediente_editar_UNIDADE_DE_MEDIDA(const unsigned int codigo, const int unidade_de_medida) 
{
	block_t *ing = encontrar(codigo);
	if(ing == NULL) return INGREDIENTE_INEXISTENTE;
	ing->ingrediente.unidade_de_medida = unidade_de_medida;

	return SUCESSO;
}


int 
__ingrediente_editar_QUANTIDADE_MINIMA(const unsigned int codigo, const float quantidade_minima) 
{
	block_t *ing = encontrar(codigo);
	if(ing == NULL) return INGREDIENTE_INEXISTENTE;
	ing->ingrediente.quatidade_minima = quantidade_minima;

	return SUCESSO;
}

int 
__ingrediente_editar_CUSTO_UNITARIO(const unsigned int codigo, const float custo_unitario) 
{
	block_t *ing = encontrar(codigo);
	if(ing == NULL) return INGREDIENTE_INEXISTENTE;
	ing->ingrediente.custo_unitario = custo_unitario;

	return SUCESSO;
}

//Controle de estoque
int 
ingrediente_adicionar(const char * restrict nome,
		const unsigned codigo, 
		const int unidade_de_medida, 
		const float quantidade_minima, 
		const float custo_unitario) 
{
	estoque_iniciado_checar();

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

int 
ingrediente_remover(const unsigned codigo) 
{
	estoque_iniciado_checar();

	ingrediente_t *ing = ingrediente_checar(codigo);
	if(ing == NULL) return INGREDIENTE_INEXISTENTE;
	if(ing->quantidade != 0) return ESTOQUE_AINDA_DISPONIVEL;

	remover(codigo);


	return SUCESSO;
}

int 
estoque_adicionar(const unsigned int codigo, const unsigned int quantidade) 
{
	estoque_iniciado_checar();

	ingrediente_t *ingrediente = NULL;
	if(!(ingrediente = ingrediente_checar(codigo))) return INGREDIENTE_INEXISTENTE;
	
	ingrediente->quantidade += quantidade;
	registro_criar(*ingrediente, quantidade);

	return SUCESSO;
}

int 
estoque_remover(const unsigned int codigo, const unsigned int quantidade) 
{
	estoque_iniciado_checar();

	ingrediente_t *ingrediente = NULL;
	if(!(ingrediente = ingrediente_checar(codigo))) return INGREDIENTE_INEXISTENTE;
	if(ingrediente->quantidade < quantidade) return ESTOQUE_POUCO;
	
	ingrediente->quantidade -= quantidade;
	registro_criar(*ingrediente, -quantidade);

	return SUCESSO;
}


//Listar em ordem os ingredientes
int 
ingrediente_listar(ingrediente_t *restrict destino,const unsigned tamanho, int tipo, int ordem) 
{
	estoque_iniciado_checar();

	for(int i = 0; i < diff; i++) destino[i] = ingredientes[i].ingrediente;

	switch(tipo) {
		case NOME:
			ordernar_ingredientes_nome(destino,  tamanho, ordem);
			break;
		case CODIGO:
			ordernar_ingredientes_codigo(destino, tamanho, ordem);
			break;
		case PRECO:
			ordernar_ingredientes_preco(destino, tamanho, ordem);
			break;

		default: return TIPO_INVALIDO;
	}

	return SUCESSO;
}

int 
ingredientes_listar_faltando(ingrediente_t *restrict destino) 
{
	estoque_iniciado_checar();

	int dest_idx = 0;

	for(int i = 0; i < diff; i++) {
		if(ingredientes[i].ativo == false) continue;
		if(ingredientes[i].ingrediente.quantidade > ingredientes[i].ingrediente.quatidade_minima) continue;

		destino[dest_idx++] = ingredientes[i].ingrediente;
	}

	return dest_idx;
}
