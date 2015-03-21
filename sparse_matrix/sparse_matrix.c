/*
* Aluno: Nícolas Baptista
* Disciplina: Estrutura de Dados
* Instituição: SPEI
* Professor: Razer Montaño
* 
* Dezembro de 2008
*/
#include <stdio.h>
#include <stdlib.h>

typedef struct node *p_node;
typedef struct node 
{
	p_node Right;   
	p_node Bellow;   
	int Row;   
	int Column;   
	double Value;
	int dRows,dColumns;        
} s_node;

p_node createMatrix(int rows, int columns);
void alocateNode(p_node matrix,p_node node, int lin, int col);
void insert(p_node matrix, int lin, int col, double val);
void printMatrix(p_node matrix);
void printTranspose(p_node matrix);
p_node sumMatrix(p_node matrix_A, p_node matrix_B, p_node *matrix_C);
void readMatrix(s_node **matrix, char *nomeArquivo);
void eraseMatrix(p_node matrix);
p_node multiplyMatrix(p_node matrix_A, p_node matrix_B, p_node *matrix_C);
p_node searchNode(p_node matrix, int lin, int col);

int main(void) 
{
	p_node sparseMatrix_A;
	p_node sparseMatrix_B;
	p_node sparseMatrix_C;	
		
	readMatrix(&sparseMatrix_A, "matrix1.txt");
	printf("Imprime matrix A\n");      
	printMatrix(sparseMatrix_A);
	readMatrix(&sparseMatrix_B, "matrix2.txt"); 

	printf("Imprime matrix B\n");      
	printMatrix(sparseMatrix_B);
	
	sumMatrix(sparseMatrix_A, sparseMatrix_B, &sparseMatrix_C); 
	printf("Imprime C = A + B\n");      
	printMatrix(sparseMatrix_C); 
	printf("Apaga C\n");      
	eraseMatrix(sparseMatrix_C);

	printf("Imprime C = A * B\n");      
	multiplyMatrix(sparseMatrix_A, sparseMatrix_B, &sparseMatrix_C); 
	printMatrix(sparseMatrix_C);

	printf("Apaga B\n");          
	eraseMatrix(sparseMatrix_B); 
	printf("Apaga C\n");      
	eraseMatrix(sparseMatrix_C);
	
	printf("Le nova matrix em B\n");      
	readMatrix(&sparseMatrix_B, "matrix3.txt");
	printf("Imprime matrix A\n");      
	printMatrix(sparseMatrix_A); 
	printf("Imprime matrix B\n");          
	printMatrix(sparseMatrix_B);
	
	printf("Imprime C = A + B\n");      
	sumMatrix(sparseMatrix_A, sparseMatrix_B, &sparseMatrix_C); 
	printMatrix(sparseMatrix_C);
	
	printf("Imprime C = A * B\n");          
	multiplyMatrix(sparseMatrix_A, sparseMatrix_B, &sparseMatrix_C); 
	printMatrix(sparseMatrix_C);
	
	printf("Imprime C =  B * B\n");          
	multiplyMatrix(sparseMatrix_B, sparseMatrix_B, &sparseMatrix_C);
	printMatrix(sparseMatrix_C);

	
	printf("Imprime matrix B\n");      
	printMatrix(sparseMatrix_B); 
	printf("Imprime matrix B\n");  
	printMatrix(sparseMatrix_B); 
	printf("Imprime matrix C\n");  
	printMatrix(sparseMatrix_C);
	
	eraseMatrix(sparseMatrix_A); 
	eraseMatrix(sparseMatrix_B); 
	eraseMatrix(sparseMatrix_C);
			
	system("pause");        
	return 0;
}

p_node createMatrix(int rows, int columns) 
{
	// variável local que receberá a matrix
	p_node matrix;
	// variáveis auxiliares para caminhar pelos nodes
	p_node ptr, p_aux;
	// contadores para poder criar os nodes
	int i, j;
	// Cria node principal, a matrix que será retornada
	matrix = (p_node)malloc(sizeof(s_node));
	matrix->Row = -1;
	matrix->Column = -1;
	matrix->dRows = rows;
	matrix->dColumns = columns;      
	matrix->Value = 0;
	matrix->Right = matrix;
	matrix->Bellow = matrix;
	// Cria os nodes cabeça das rows
	ptr = matrix;
	for (i=0; i<rows; i++) {
		// Cria novo node
		p_aux = (p_node)malloc(sizeof(s_node));
		p_aux->Row = i;
		p_aux->Column = -1;
		p_aux->Value = 0;
		//A Right dos nodes cabeça de row apontam para si mesmos
		p_aux->Right = p_aux;
		p_aux->Bellow = matrix;
		// Coloca node na matrix esparsa, abaixo
		ptr->Bellow = p_aux;
		// anda para poder inserir o próximo
		ptr = ptr->Bellow;
	}

	// Cria os nodes cabeça das columns
	ptr = matrix;
	for (j=0; j<columns; j++) {
		// Cria novo node
		p_aux = (p_node)malloc(sizeof(s_node));
		p_aux->Row = -1;
		p_aux->Column = j;
		p_aux->Value = 0;
		p_aux->Right = matrix;
		//Bellow dos nodes cabeça de row apontam para si mesmos
		p_aux->Bellow = p_aux;
		ptr->Right = p_aux;
		// anda para poder inserir o próximo
		ptr = ptr->Right;
	}
	// retorna o ponteiro principal, a matrix esparsa
	return matrix;
}

void alocateNode(p_node matrix,p_node node, int lin, int col)
{
	p_node headerRow, headerColumn, ptrL, ptrC, aux;		
	p_node nodePrevious = (p_node)malloc(sizeof(s_node));
	p_node nodeNext = (p_node)malloc(sizeof(s_node));
	p_node nodeAbove = (p_node)malloc(sizeof(s_node));
	p_node nodeBellow = (p_node)malloc(sizeof(s_node));
	nodePrevious = NULL;
	nodeNext = NULL;
	nodeAbove = NULL;
	nodeBellow = NULL;
	int i, j;

	if (matrix == NULL) {
		printf("Matriz nula\n");
		return;
	}  

	//procura em qual row está o node cabeça correpondente
	for (i = 0, ptrL = matrix->Bellow; i < matrix->dRows; i++, ptrL = ptrL->Bellow) {        
		if (ptrL->Row == lin)
			break;
	}
	
	//anda pelas columns na row desejada para descobrir se há mais algum elemento
	for (i = 0, headerColumn = ptrL; i < matrix->dColumns || headerColumn != ptrL; i++, headerColumn = headerColumn->Right)
	{
		// não há nenhum elemento nessa row pois o node cabeça aponta para si mesmo
		if (headerColumn == headerColumn->Right)
		{
			ptrL->Right = node;
			node->Right = ptrL;
		}
		//Aqui insert entre dois elementos                
		else if (headerColumn->Column < col && headerColumn->Right->Column > col)
		{
			aux = headerColumn->Right;
			headerColumn->Right = node;
			node->Right = aux; 
			break;            
		}             
		// Aqui insert elemento após a ultima celula
		else if (headerColumn->Right == ptrL && headerColumn->Column < col)
		{
			headerColumn->Right = node;
			node->Right = ptrL;
		}
		// Aqui insert elemento antes da ultima celula
		else if (headerColumn->Right == ptrL && headerColumn->Column > col)
		{
			aux = headerColumn;
			headerColumn = node;
			node->Right = aux;
		}		
	}   
 
	 //procura em qual column está o node cabeça correpondente   
	for (i = 0, ptrC = matrix->Right; i < matrix->dColumns; i++, ptrC = ptrC->Right) {
		if (ptrC->Column == col)
			break;
	}       		

	for (i = -1, headerRow = ptrC; i < matrix->dRows || headerRow != ptrC; headerRow = headerRow->Bellow, i++) {
		if (headerRow == headerRow->Bellow) {
			ptrC->Bellow = node;
			node->Bellow = ptrC;
			break;
		} else if (headerRow->Row < lin && headerRow->Bellow->Row > lin) {
			aux = headerRow->Bellow;
			headerRow->Bellow = node;
			node->Bellow = aux;
			break;   
		} else if (headerRow->Bellow == ptrC && headerRow->Row < lin) {
			headerRow->Bellow = node;
			node->Bellow = ptrC;
			break;
		} else if (headerRow->Bellow == ptrC && headerRow->Row > lin) {
			aux = headerRow;
			headerRow = node;
			node->Bellow = aux;
			break;
		}
	}
}

void readMatrix (s_node **matrix, char *nomeArquivo) 
{
	FILE *fp;
	int rows, columns;
	int lin, col;
	double val;
	p_node sparseMatrix; 
	fp = fopen(nomeArquivo, "r");
	if (fp == NULL) {
		printf("Erro ao abrir arquivo %s.", nomeArquivo);
		exit(1);
		return;
	}

	fscanf(fp, "%d %d\n", &rows, &columns);
	sparseMatrix = createMatrix(rows, columns);
	while (!feof(fp)) {
		fscanf(fp, "%d %d %lf\n", &lin, &col, &val);             
		insert(sparseMatrix, lin, col, val);
	}
	fclose(fp);
	*matrix = sparseMatrix;
}

void insert (p_node matrix, int lin, int col, double val)
{  
	p_node p_aux = (p_node)malloc(sizeof(s_node));
	p_aux->Row = lin;
	p_aux->Column = col;
	p_aux->Value = val; 
	alocateNode(matrix, p_aux, lin, col);    
}

void printMatrix (p_node matrix) 
{
	if (matrix == NULL) {
		printf("Matriz nula\n");
		return;
	}
	int i,j;
		
	p_node node;
	for (i = 0; i < matrix->dRows; i++) {
		for (j = 0; j < matrix->dColumns; j++) {
			node = searchNode(matrix, i, j);
			if (node != NULL) {
				printf("%5.2f ", node->Value);                             
			} else {
				printf(" 0.00 ");
			}
		}
		printf("\n");
	}
	printf("\n\n\n");
}

p_node searchNode (p_node matrix, int lin, int col) {
	p_node ptrRow;
	p_node ptrColumn;    	
	for (ptrRow = matrix->Bellow; ptrRow != matrix; ptrRow = ptrRow->Bellow) {
		for (ptrColumn = ptrRow->Right; ptrColumn != ptrRow; ptrColumn = ptrColumn->Right) {
			if (ptrColumn->Row == lin && ptrColumn->Column == col) {
				return ptrColumn;
			}						
		}
	}
	return NULL;
}

void eraseMatrix (p_node matrix) {
	if (matrix == NULL) {
		printf("Matriz nula\n");
		return;
	}

	p_node headerRow, headerColumn, ptrL, ptrC, aux, node;
	int i, j,lin,col;

	for (lin = 0; lin < matrix->dRows; lin++) {
		for (i = 0, ptrL = matrix->Bellow; i < matrix->dRows; i++, ptrL = ptrL->Bellow) {        
			if (ptrL->Row == lin) {
				break;
			}
		}
		for (i = 0, headerColumn = ptrL; i < matrix->dColumns; i++, headerColumn = headerColumn->Right) {
			if (headerColumn->Right->Column == col) {
				aux = headerColumn->Right;
				headerColumn = aux->Right;
				free(aux);
				break;            
			}             
		}   			
		for (col = 0; col < matrix->dColumns; col++) {
			for (i = 0, ptrC = matrix->Right; i < matrix->dColumns; i++, ptrC = ptrC->Right) {
				if (ptrC->Column == col) {
					break;  
				}
			}       
			for (i = 0, headerRow = ptrC; i < matrix->dRows; headerRow = headerRow->Bellow, i++) {
				if (headerRow->Bellow->Row == lin) {
					aux = headerRow->Bellow;
					headerRow = aux->Bellow;
					free(aux);
					break;   
				}           
			}
		}
	}
	free(matrix);
	matrix = NULL;
}

p_node sumMatrix (p_node matrix_A, p_node matrix_B, p_node *matrix_C) {
	if (matrix_A == NULL || matrix_B == NULL) {
		printf("Matriz nula\n"); 
		return NULL;
	}

	int i,j;
	double resultSoma = 0.0;
	p_node resultMatrix,node,node2;
	resultMatrix = createMatrix(matrix_A->dRows, matrix_A->dColumns);
	 
	if (matrix_B->dRows != matrix_A->dRows) {
		exit(0);
		printf("Erro! So e possivel efetuar das matrixes se ambas forem quadradas");
	} 

	for (i = 0; i < matrix_A->dRows; i++) {
		resultSoma = 0.0;
		for (j = 0; j < matrix_A->dColumns; j++) {
			node = searchNode(matrix_A, i, j);
			node2 = searchNode(matrix_B, i, j);            
			if (node != NULL && node2 != NULL) {
				resultSoma = node->Value + node2->Value;  
			} else {            
				if (node2 == NULL && node == NULL) {
					resultSoma = 0;
				} else if (node == NULL) {
					resultSoma = node2->Value;
				} else if (node2 == NULL) {
					resultSoma = node->Value;
				}  
			}
			insert(resultMatrix, i, j, resultSoma);            
		}
	}
	*matrix_C = resultMatrix;
	return NULL;
}

p_node multiplyMatrix (p_node matrix_A, p_node matrix_B, p_node *matrix_C) {
	if (matrix_A == NULL || matrix_B == NULL) {
		printf("Matriz nula\n");
		return NULL;
	}
	
	int i,j,k = 0;
	double resultMultiplication = 0;
	p_node resultMatrix,node,node2;
	resultMatrix = createMatrix(matrix_A->dRows, matrix_A->dColumns);
	 
	for (i = 0; i < matrix_A->dRows; i++) {
		for (j = 0; j < matrix_A->dColumns; j++) {
			resultMultiplication = 0;
			for (k = 0; k < matrix_A->dColumns; k++) {
				node = searchNode(matrix_A, i, k);
				node2 = searchNode(matrix_B, k, j);				
				if (node != NULL && node2 != NULL) {
					resultMultiplication += node->Value * node2->Value;
				} else {
					if (node2 == NULL && node == NULL) {
						resultMultiplication += 0;  
					} else if (node == NULL) {
						resultMultiplication += 0 * node2->Value;  
					} else if (node2 == NULL) {
						resultMultiplication += node->Value * 0;
					}
				}                     
			}
			insert(resultMatrix, i, j, resultMultiplication);                   
		}
	}     
	*matrix_C = resultMatrix;     
	return NULL;
}
