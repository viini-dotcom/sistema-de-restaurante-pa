#ifndef cliente_pedido.h
#define cliente_pedido.h
struct cliente
    {
        int codigo;
        char nome[20];
        char endereço[50];
        int telefone;
    };
    
    struct pedido
    {
        int codigo;
        struct cliente c;
        int *quantidade;
        int *receita;
        char data[10];
        float valor;
        char status[13];
    };

    void remover_cliente(struct pedido *c, int *A);

    void adicionar_pedido(struct pedido *p, struct cliente *c, receita_t *R, int *e);

    void alterar_cliente(struct pedido *p, struct cliente *c, int *A);

    void alterar_status_do_pedido(struct pedido *p, int *A);

    void buscar_pedido_por_codigo(struct pedido *p, int *A);

    void buscar_pedido_por_nome_do_cliente(struct pedido *p, char *busca);

    void ordenar_cliente_por_codigo(struct pedido *c, struct pedido *C, int *A);

    void ordenar_pedido_por_codigo(struct pedido *p, struct pedido *P, int *A);

    void ordenar_cliente_por_nome(struct pedido *c, struct pedido *C, int *A);

    void ordenar_pedido_por_preço(struct pedido *p, struct pedido *P, int *A);

    void buscar_cliente_por_nome(struct cliete *c, char *busca);

#endif
