#include <receita.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct __block_t {
	ingrediente_t ingrediente;
	int ativo;
	struct __block_t * next;
} block_t;

static inline void panic(const char * restrict message, const unsigned exit_code) {
	printf("%s\n", message);
	exit(exit_code);
}

static block_t *freelist = NULL;
static block_t *receitas = NULL;
static unsigned num_receitas = 0;
static unsigned buffer_size = 0;

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
