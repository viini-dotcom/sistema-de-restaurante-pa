#include <estoque.h>
#include <stdio.h>



int main() {
	estoque_iniciar();
	adicionar_ingrediente("Arroz", 165, KILOGRAMAS, 1, 1.66);
	adicionar_ingrediente("Feijão", 167, KILOGRAMAS, 1, 1.66);
	adicionar_ingrediente("Macarrão", 168, KILOGRAMAS, 1, 1.66);
	adicionar_ingrediente("Batata", 166, KILOGRAMAS, 1, 1.66);

	ingrediente_t list[64];
	ingrediente_t list2[64];

	listar_ingrediente_por(list, 4, NOME, CRESCENTE);
	listar_ingrediente_por(list2, 4, NOME, DECRESCENTE);
	for(int i = 0; i < 4; i++) {
		printf("[%d] %s\n",i ,list[i].nome);
	}

	for(int i = 0; i < 4; i++) {
		printf("[%d] %s\n",i ,list2[i].nome);
	}
}
