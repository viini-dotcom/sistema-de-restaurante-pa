#include <estoque.h>
#include <receita.h>
#include <stdio.h>






int main() {
	estoque_iniciar();
	receita_iniciar();
	ingrediente_adicionar("Batata", 15, KILOGRAMAS, 1.5, 1.5);
	ingrediente_adicionar("Arroz", 16, KILOGRAMAS, 1.5, 1.5);
	ingrediente_adicionar("Feijão", 17, KILOGRAMAS, 1.5, 1.5);
	ingrediente_adicionar("Trigo", 18, KILOGRAMAS, 1.5, 1.5);
	ingrediente_adicionar("Carne de gado", 19, KILOGRAMAS, 1.5, 1.5);
	ingrediente_adicionar("Carne de carneiro", 20, KILOGRAMAS, 1.5, 1.5);


	receita_adicionar("Batata Assada", 165, FRANGO, 15);
	receita_adicionar("Frango Assado", 155, FRANGO, 13);
	receita_adicionar("Frango Cozido", 177, FRANGO, 13);

	receita_t test[64];
	receita_procurar(NOME, test, "frango");

	printf("%s %s\n", test[0].nome, test[1].nome);
		



}
