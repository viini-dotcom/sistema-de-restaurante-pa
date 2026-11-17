int *d;


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
        int quantidade[32];
        int receita[32];
        char data[10];
        float valor;
        char status[13];
    };

void remover_cliente(struct pedido *p, int *A)
    {

        for(int i=0; i<*d; i++)
        {
            if(*A==p[i].c.codigo)
            {
                for(int j=i; j<(d+1-i); j++)
                {
                    p[j]=p[j+1];
                }
                
            *d=(*d)-1;
            
            break;    
            }
                
        }
            
            
    }

    void adicionar_cliente(struct pedido *p, receita_t *R, int *e)
    {
        int n=0, m=1, l;
        p[*d].valor=0;

        getchar();
        printf("digite o código do pedido: ");
        scanf("%d", &p[*d].codigo);

        for(int i=0; i<*d; i++)
        {
            l=0;

            if(p[*d].codigo==p[i].codigo)
            {
                l=1;

                break;
            }
        }

        while(scanf("%d", &p[*d].codigo)!=1 || l==1)
        {
            printf("o valor digitado não é inteiro ou o código está repetido, tente novamente\n");

            printf("digite o código do pedido: ");
            scanf("%d", &p[*d].codigo);

            for(int i=0; i<*d; i++)
            {
                l=0;
                
                if(p[*d].codigo==p[i].codigo)
                {
                    l=1;

                    break;
                }
             }

        }

        getchar();

        printf("digite a data do pedido: ");
        fgets(p[*d].data, sizeof(p[*d].data), stdin);
        getchar();

        printf("digite o nome do cliente: ");
        fgets(p[*d].c.nome, sizeof(p[*d].c.nome), stdin);
        getchar();

        for(int i=0; i<(*d); i++)
        {
            if(strcmp(p[*d].c.nome, p[i].c.nome)==0)
            {
                n=1;

                p[*d].c.codigo=p[*d].c.codigo;

                p[*d].c.telefone=p[*d].c.telefone;

                strcpy(p[*d].c.endereço, p[i].c.endereço);


            }
        }

        if(n==0)
        {

            printf("digite o código do cliente: ");
            scanf("%d", &p[*d].c.codigo);

            for(int i=0; i<*d; i++)
            {
                l=0;
                
                if(p[*d].c.codigo==p[i].c.codigo)
                {
                    l=1;

                    break;
                }
            }

            while(scanf("%d", &p[*d].c.codigo)!=1 || l=1)
            {
                printf("o valor digitado não é inteiro ou o código está repetido, tente novamente\n");
                
                printf("digite o código do pedido: ");
                scanf("%d", &p[*d].c.codigo);

                for(int i=0; i<*d; i++)
                {
                    l=0;
                    
                    if(p[*d].c.codigo==p[i].c.codigo)
                    {
                        l=1;

                        break;
                    }
                }
            }

            getchar();
            
            printf("digite o endereço do cliente: ");
            fgets(p[*d].c.endereço, sizeof(p[*d].c.endereço), stdin);
            getchar();
            
            printf("digite o telefone do cliente: ");
            scanf("%d", &p[*d].c.telefone);
            getchar();
        }

        int i=0;

        while(m==1 && i<=32)
        {
            printf("digite o código da receita solicitada: ");
            scanf("%d", &p[*d].receita[i]);

            while(scanf("%d", &p[*d].receita[i])!=1)
        {
            printf("o valor digitado não é inteiro, tente novamente\n");

            printf("digite o código da receita solicitada: ");
            scanf("%d", &p[*d].receita[i]);

        }

        l=0;

        while(l!=1)
        {
            printf("a receita solicitada não existe, tente novamente\n");

            for(int j=0; j<*e; j++)
             {
                if(p[*d].receita==R[j].PRECO)
                {
                    l=1;
                }
            }
        }

            getchar();

            printf("digite a quantidade: ");
            scanf("%d", p[*d].quantidade[i]);

            while(scanf("%d", &p[*d].quantidade[i])!=1)
        {
            printf("o valor digitado não é inteiro, tente novamente\n");

            printf("digite o código do pedido: ");
            scanf("%d", &p[*d].quantidade[i]);

        }

            getchar();

            for(int j=0; j<*e; j++)
            {
                if(p[*d].receita==R[j].RECEITA_PRECO)
                {
                    p[*d].valor=p[*d].valor+(p[*d].quantidade*R[j].RECEITA.PRECO);

                    break;
                }
            }

            i=i+1;

            printf("digite 1 para acrescentar alguma receita ou 0 para seguir para o próximo passo: ");
            scanf("%d", &m);
            getchar(); 
        }

        printf("digite o status do pedido");
        fgets(p[*d].status, sizeof(p[*d].status), stdin);
        getchar();

        *d=(*d)+1;        
    }

    void alterar_cliente(struct pedido *p, int *A)
    {

        for(int i=0; i<*d; i++)
        {
            if(*A==p[i].c.codigo)
            {
                getchar();
                printf("digite o nome do cliente: ");
                fgets(p[i].c.nome, sizeof(p[i].c.nome), stdin);
                
                printf("digite o código do cliente: ");
                scanf("%d", &p[i].c.codigo);
                
                printf("digite o endereço do cliente: ");
                fgets(p[i].c.endereço, sizeof(p[i].c.endereço), stdin);
                getchar();

                printf("digite o telefone do cliente: ");
                scanf("%d", &p[i].c.telefone);
                getchar();
            
            break;    
            }
        }
            
    }

    void alterar_status_do_pedido(struct pedido *p, int *A)
    {
        for(int i=0; i<*d; i++)
        {
            if(*A==p[i].codigo)
            {
                printf("digite o status do pedido");
                fgets(p[*d].status, sizeof(p[*d].status), stdin);
                getchar();

            break;    
            }
        }
    }

    void buscar_por_codigo(struct pedido *p, int *A)
    {
        for(int i=0; i<d; i++)
        {
            if(*A==p[i].c.codigo || *A==p[i].codigo)
            {
                getchar();
                printf("código do pedido: %d\n", p[i].codigo);
                printf("código do cliente: %d\n", p[i].c.codigo);
                printf("data do pedido: %s\n", p[i].data);
                printf("nome do cliente: %s\n", p[i].c.nome);
                printf("endereço do cliente: %s\n", p[i].c.endereço);
                printf("telefone do cliente: %d\n", p[i].c.telefone);
                printf("quantidade do pedido: %d\n", p[i].quantidade);
                printf("valor do pedido: %f\n", p[i].valor);
                printf("status do pedido: %s\n", p[i].status);

                break;
            }
        }
    }

    void buscar_por_nome_do_cliente(struct pedido *p, char *busca)
    {
        for(int i=0; i<*d; i++)
        {
            if(strstr(p[i].c.nome, busca)!=NULL)
            {
                getchar();
                printf("código do pedido: %d\n", p[i].codigo);
                printf("código do cliente: %d\n", p[i].c.codigo);
                printf("data do pedido: %s\n", p[i].data);
                printf("nome do cliente: %s\n", p[i].c.nome);
                printf("endereço do cliente: %s\n", p[i].c.endereço);
                printf("telefone do cliente: %d\n", p[i].c.telefone);
                printf("quantidade do pedido: %d\n", p[i].quantidade);
                printf("valor do pedido: %f\n", p[i].valor);
                printf("status do pedido: %s\n", p[i].status);
            }
        }
    }

    void ordenar_cliente_por_codigo(struct pedido *p, struct pedido *P, int *A)
    {
        for(int i=0; i<(*d)-1; i++)
        {
            for(int j=0; j<(*d)-i-1; j++)
            {
                if(p[j].c.codigo>p[j+1].c.codigo)
                {
                    *P=p[j];
                    p[j]=p[j+1];
                    p[j+1]=*P;
                }
            }
        }

    }

    void ordenar_pedido_por_codigo(struct pedido *p, struct pedido *P, int *A)
    {
        for(int i=0; i<(*d)-1; i++)
        {
            for(int j=0; j<(*d)-i-1; j++)
            {
                if(p[j].codigo>p[j+1].codigo)
                {
                    *P=p[j];
                    p[j]=p[j+1];
                    p[j+1]=*P;
                }
            }
        }

    }

    void ordenar_cliente_por_nome(struct pedido *p, struct pedido *P, int *A)
    {
        for(int i=0; i<(*d)-1; i++)
        {
            for(int j=0; j<(*d)-i-1; j++)
            {
                if(strcmp(p[j].c.nome, p[j+1].c.nome)>0)
                {
                    *P=p[j];
                    p[j]=p[j+1];
                    p[j+1]=*P;
                }
            }
        }

    }

    void ordenar_pedido_por_preço(struct pedido *p, struct pedido *P, int *A)
    {
        for(int i=0; i<(*d)-1; i++)
        {
            for(int j=0; j<(*d)-i-1; j++)
            {
                if(p[j].valor>p[j+1].valor)
                {
                    *P=p[j];
                    p[j]=p[j+1];
                    p[j+1]=*P;
                }
            }
        }

    }
