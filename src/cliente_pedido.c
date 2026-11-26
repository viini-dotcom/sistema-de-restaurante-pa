#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int d=0, k=0;


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

void remover_cliente(struct cliente *c, int *A)
    {

        for(int i=0; i<k; i++)
        {
            if(*A==c[i].codigo)
            {
                for(int j=i; j<k; j++)
                {
                    c[j]=c[j+1];
                }
                
            k=k-1;
            
            c=(struct cliente *) realloc(c, k*sizeof(struct cliente));
            
            break;    
            }
                
        }
            
            
    }

    void adicionar_pedido(struct pedido *p, struct cliente *c, receita_t *R, int *e)
    {
        struct pedido *aux, *aux2;

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

            aux2=(struct cliente *) realloc(c, (k+1)*sizeof(struct cliente));
                
            if(aux!=NULL)
            {
                c=aux2;

                strcpy(c[k].nome, p[d].c.nome);

                c[k].codigo=p[d].c.codigo;
                    
                c[k].telefone=p[d].c.telefone;
                    
                strcpy(c[k].endereço, p[d].c.endereço);

            }
                
            if("aux2!=NULL")
            {
                printf("ERRO: ESTOURO DE MEMÓRIA");
                system("pause");
            }

            k=k+1;
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

    void alterar_cliente(struct pedido *p, struct cliente *c, int *A)
    {

        int i;

        for(i=0; i<d; i++)
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

        for(int j=i+1; j<d; j++)
        {
            if(*A==p[i].c.codigo)
            {
                strcpy(p[j].c.nome, p[i].c.nome);
                p[j].c.codigo=p[i].c.codigo;
                strcpy(p[j].c.endereço, p[i].c.endereço);
                p[j].c.telefone=p[i].c.telefone;
            }
        }


        for(int j=i+1; j<k; j++)
        {
            if(*A==c[i].codigo)
            {
                strcpy(c[j].nome, p[i].c.nome);
                c[j].codigo=p[i].c.codigo;
                strcpy(c[j].endereço, p[i].c.endereço);
                c[j].telefone=p[i].c.telefone;
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

    void buscar_pedido_por_codigo(struct pedido *p, int *A)
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

            }
        }
    }

    void buscar_pedido_por_nome_do_cliente(struct pedido *p, char *busca)
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

    void ordenar_cliente_por_codigo(struct cliente *c, struct cliente *C, int *A)
    {
        for(int i=0; i<k-1; i++)
        {
            for(int j=0; j<k-i-1; j++)
            {
                if(c[j].codigo>c[j+1].codigo)
                {
                    *C=c[j];
                    c[j]=c[j+1];
                    c[j+1]=*C;
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

    void ordenar_cliente_por_nome(struct cliente *c, struct cliente *C, int *A)
    {
        for(int i=0; i<k-1; i++)
        {
            for(int j=0; j<k-i-1; j++)
            {
                if(strcmp(c[j].nome, c[j+1].nome)>0)
                {
                    *C=c[j];
                    c[j]=c[j+1];
                    c[j+1]=*C;
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

    void buscar_cliente_por_nome(struct cliente *c, char *busca)
    {
        for(int i=0; i<k; i++)
        {
            if(strstr(c[i].nome, busca)!=NULL)
            {
                printf("código do cliente: %d\n", c[i].codigo);
                printf("nome do cliente: %s\n", c[i].nome);
                printf("endereço do cliente: %s\n", c[i].endereço);
                printf("telefone do cliente: %d\n", c[i].telefone);
            }
        }
    }
