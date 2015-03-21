#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int lin, col, i, j, def, def2, opt;
    double val;
    FILE *f;
    srand ( time(NULL) );
    f = fopen("matriz1.txt","w");
    
    if (f == NULL) {
        exit(0);
    }

    printf("# Gerador Randomico de Matrizes # \n\n");
    printf("Digite o numero de linhas da matriz: ");
    scanf("%d", &lin);
    printf("\n\nDigite o numero de colunas da matriz: ");
    scanf("%d", &col);    
    fprintf(f, "%d %d\n", lin, col);   
    printf("(1) Matriz Cheia\n");
    printf("(2) Matriz Esparsa\n\n");         
    printf("Selecione uma opt: ");
    scanf("%d",&opt);
    
    switch (opt) {
        case 1:
            for(i = 0; i < lin; i++) {
                for(j = 0; j < col; j++) {
                    val = rand()%50;
                    fprintf(f, "%d %d %f\n", i, j, val);             
                }   
            }                        
        break;
        
        case 2:
            for(i = 0; i < lin; i++) {
                for(j = 0; j < col; j++) {
                    def = rand()%1000;
                    def2 = rand()%1000 + rand()%2;
                    // condicionais para gerar posições zeradas na matriz
                    if(def > def2) {
                        if((i + j / 2) == 0) {
                            val = rand()%50;
                            fprintf(f, "%d %d %f\n", i, j, val);             
                        }                
                    } else {
                        if((i + j / 2) != 0) {
                            val = rand()%50;
                            fprintf(f, "%d %d %f\n", i, j, val);             
                        }                                
                    }
                }   
            }            
        break;
        
        default:
            for(i = 0; i < lin; i++) {
                for(j = 0; j < col; j++) {
                    val = rand()%50;
                    fprintf(f, "%d %d %f\n", i, j, val);             
                }   
            }                
        break;   
    }
    
    printf("\n\n@ matriz1.txt criada com sucesso\n\n");        
}