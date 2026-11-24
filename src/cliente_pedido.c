#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int d=0;


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

void remover_pedido(struct pedido *p, int *A)
    {

        for(int i=0; i<d; i++)
        {
            if(*A==p[i].c.codigo)
            {
                for(int j=i; j<d; j++)
                {
                    p[j]=p[j+1];
                }
                
            d=d-1;
            
            p=(struct pedido *) realloc(p, d*sizeof(struct pedido));
            
            break;    
            }
                
        }
            
            
    }

    void adicionar_cliente(struct pedido *p, receita_t *R, int *e)
    {
        struct pedido *aux;

        aux=(struct pedido *) realloc(p, (d+1)*sizeof(struct pedido));
        if(aux!=NULL)
        {
            p=aux;
        }

        if("aux!=NULL")
        {
            printf("ERRO: ESTOURO DE MEMÓRIA");
            system("pause");
        }

        int n=0, m=1, l=0, quant_r;
        float retorno;
        p[d].valor=0;

        printf("digite o código do pedido: ");
        scanf("%f", &retorno);
        p[d].codigo=retorno;
        

        for(int i=0; i<d; i++)
        {
            l=0;

            if(p[d].codigo==p[i].codigo)
            {
                l=1;

                break;
            }
        }

        while(p[d].codigo-retorno!=0 || p[d].codigo<0 || l==1)
        {
            printf("o valor digitado não é inteiro ou é negativo ou o código está repetido, tente novamente\n");

            printf("digite o código do pedido: ");
            scanf("%f", &retorno);
            p[d].codigo=retorno;

            for(int i=0; i<d; i++)
            {
                l=0;
                
                if(p[d].codigo==p[i].codigo)
                {
                    l=1;

                    break;
                }
             }

        }

        getchar();

        printf("digite a data do pedido: ");
        fgets(p[d].data, sizeof(p[d].data), stdin);

        printf("digite o nome do cliente: ");
        fgets(p[d].c.nome, sizeof(p[d].c.nome), stdin);

        for(int i=0; i<d; i++)
        {
            if(strcmp(p[d].c.nome, p[i].c.nome)==0)
            {
                n=1;

                p[d].c.codigo=p[d].c.codigo;

                p[d].c.telefone=p[d].c.telefone;

                strcpy(p[d].c.endereço, p[i].c.endereço);


            }
        }

        if(n==0)
        {

            printf("digite o código do cliente: ");
            scanf("%f", &retorno);
            p[d].c.codigo=retorno;

            for(int i=0; i<d; i++)
            {
                l=0;
                
                if(p[d].c.codigo==p[i].c.codigo)
                {
                    l=1;

                    break;
                }
            }

            while(p[d].c.codigo-retorno!=0 || p[d].c.codigo<0 || l==1)
            {
                printf("o valor digitado não é inteiro ou é negativo ou o código está repetido, tente novamente\n");
                
                printf("digite o código do pedido: ");
                scanf("%f", &retorno);
                p[d].c.codigo=retorno;

                for(int i=0; i<d; i++)
                {
                    l=0;
                    
                    if(p[d].c.codigo==p[i].c.codigo)
                    {
                        l=1;

                        break;
                    }
                }
            }

            getchar();
            
            printf("digite o endereço do cliente: ");
            fgets(p[d].c.endereço, sizeof(p[d].c.endereço), stdin);
            
            printf("digite o telefone do cliente: ");
            scanf("%d", &p[d].c.telefone);
            getchar();
        }

        printf("digite a quantidade de receitas solicitadas: ");
        scanf("%d", &quant_r);
        

        p[d].receita=(int *) malloc(quant_r*sizeof(int));
        p[d].quantidade=(int *) malloc(quant_r*sizeof(int));
        
        if(p[d].receita==NULL || p[d].quantidade==NULL)
        {
            printf("ERRO: ESTOURO DE MEMÓRIA");
            system("pause");
        }

        l=0;

        for(int i=0; i<quant_r; i++)
        {
            printf("digite o código da receita solicitada: ");
            scanf("%f", &retorno);
            p[d].receita[i]=retorno;

            for(int j=0; j<*e; j++)
             {
                if(p[d].receita[i]==R[j].PRECO)
                {
                    l=1;
                }
            }

            while(p[d].receita[i]-retorno!=0 || p[d].receita[i]<0 || l!=1)
        {
            printf("o valor digitado não é inteiro ou é negativo ou a receita solicitada não existe, tente novamente\n");

            printf("digite o código da receita solicitada: ");
            scanf("%f", &retorno);
            p[d].receita[i]=retorno;

            for(int j=0; j<*e; j++)
             {
                if(p[d].receita==R[j].PRECO)
                {
                    l=1;
                }
            }

        }

            getchar();

            printf("digite a quantidade: ");
            scanf("%f", &retorno);
            p[d].quantidade[i]=retorno;

            while(p[d].quantidade[i]-retorno!=0 || p[d].quantidade[i]<0)
        {
            printf("o valor digitado não é inteiro ou é negativo, tente novamente\n");

            printf("digite o código do pedido: ");
            scanf("%d", &p[d].quantidade[i]);

        }

            getchar();

            for(int j=0; j<*e; j++)
            {
                if(p[d].receita==R[j].RECEITA_PRECO)
                {
                    p[d].valor=p[d].valor+(p[d].quantidade*R[j].RECEITA.PRECO);

                    break;
                }
            }

            i=i+1;
        
        }

        printf("digite o status do pedido");
        fgets(p[d].status, sizeof(p[d].status), stdin);

        d=d+1;        
    }

    void alterar_cliente(struct pedido *p, int *A)
    {

        for(int i=0; i<d; i++)
        {
            if(*A==p[i].c.codigo)
            {
                printf("digite o nome do cliente: ");
                fgets(p[i].c.nome, sizeof(p[i].c.nome), stdin);
                
                printf("digite o código do cliente: ");
                scanf("%d", &p[i].c.codigo);
                getchar();
                
                printf("digite o endereço do cliente: ");
                fgets(p[i].c.endereço, sizeof(p[i].c.endereço), stdin);

                printf("digite o telefone do cliente: ");
                scanf("%d", &p[i].c.telefone);
                getchar();
            
            break;    
            }
        }
            
    }

    void alterar_status_do_pedido(struct pedido *p, int *A)
    {
        for(int i=0; i<d; i++)
        {
            if(*A==p[i].codigo)
            {
                printf("digite o status do pedido");
                fgets(p[i].status, sizeof(p[i].status), stdin);

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
                printf("código do pedido: %d\n", p[i].codigo);
                printf("código do cliente: %d\n", p[i].c.codigo);
                printf("data do pedido: %s\n", p[i].data);
                printf("nome do cliente: %s\n", p[i].c.nome);
                printf("endereço do cliente: %s\n", p[i].c.endereço);
                printf("telefone do cliente: %d\n", p[i].c.telefone);

                for(int j=0; j<(sizeof(p[d].receita)/sizeof(p[d].receita[0]))+1; j++)
                {
                    printf("código das receitas solicitadas e suas quantidades:\n");

                    printf("%d: %d", p[d].receita[j], p[d].quantidade[j]);
                }

                printf("valor do pedido: %f\n", p[i].valor);
                printf("status do pedido: %s\n", p[i].status);

                break;
            }
        }
    }

    void buscar_por_nome_do_cliente(struct pedido *p, char *busca)
    {
        for(int i=0; i<d; i++)
        {
            if(strstr(p[i].c.nome, busca)!=NULL)
            {
                printf("código do pedido: %d\n", p[i].codigo);
                printf("código do cliente: %d\n", p[i].c.codigo);
                printf("data do pedido: %s\n", p[i].data);
                printf("nome do cliente: %s\n", p[i].c.nome);
                printf("endereço do cliente: %s\n", p[i].c.endereço);
                printf("telefone do cliente: %d\n", p[i].c.telefone);
                
                for(int j=0; j<(sizeof(p[d].receita)/sizeof(p[d].receita[0]))+1; j++)
                {
                    printf("código das receitas solicitadas e suas quantidades:\n");

                    printf("%d: %d", p[d].receita[j], p[d].quantidade[j]);
                }

                printf("valor do pedido: %f\n", p[i].valor);
                printf("status do pedido: %s\n", p[i].status);
            }
        }
    }

    void ordenar_cliente_por_codigo(struct pedido *p, struct pedido *P, int *A)
    {
        for(int i=0; i<d-1; i++)
        {
            for(int j=0; j<d-i-1; j++)
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
        for(int i=0; i<d-1; i++)
        {
            for(int j=0; j<d-i-1; j++)
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
        for(int i=0; i<d-1; i++)
        {
            for(int j=0; j<d-i-1; j++)
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
        for(int i=0; i<d-1; i++)
        {
            for(int j=0; j<d-i-1; j++)
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
