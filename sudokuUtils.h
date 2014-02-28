/*Convierte a gchar un numero entero.*/
gchar toString(int numero){
    gchar temp[2];
	sprintf(temp,"%d", numero);

	return temp;
}

/*Devuelve 1 si es que el array contiene el valor dado. Sino 0.*/
int containsInt(int array[], int length, int value){
   int i;

   for (i = 0; i < length; i++){
	 if (array[i] == value){
	    return 1;
	 }
   }

   return 0;
}

/*Agrega un numero al array*/
void addInt(int array[],int length, int value){
	int i;//, length=sizeof(array)/sizeof(int);

	for (i = 0; i < length; i++){

	 if (!array[i]){
	    array[i] = value;
		break;
	 }
   }
}

/*Pinta por consola el sudoku pasado como parametro.*/
void printSudoku(int tabla[9][9]){
	printf("*** Pintando sudoku ***\n");
	int i,j;

    for (i=0; i < 9; i++) {
		for (j=0; j < 9; j++) {
			printf("%i", tabla[i][j]);
		}

		printf("\n");
	}
}

/*Pinta por consola, como una sola linea, el sudoku pasado como parametro.*/
void printSudokuSolaLinea(int tabla[9][9]){
	printf("/n *** Pintando sudoku Sola Linea***\n");
	int i,j;

    for (i=0; i < 9; i++) {
		for (j=0; j < 9; j++) {
			printf("%i", tabla[i][j]);
		}
	}
	printf("\n");
}

/*Pinta por consola el vector pasado como parametro.*/
void printVector(int array[], int length){
	int i;

	printf("*** Pintando vector ***\n");

	for (i=0; i < length; i++) {
		printf("%i,", array[i]);
	}
	printf("\n");
}

/*Reinicia a 0. las posiciones del arreglo pasado como parametro.*/
void reiniciarVector(int array[], int length){
	int i;

	for (i=0; i < length; i++) {
		array[i] = 0;
	}
}

/*Copia los valores de la primera matriz a la segunda.*/
void copyMatriz(int m1[9][9], int m2[9][9]){
	int i,j;
	
    for (i = 0; i < 9; i++){
	  for (j = 0; j < 9; j++){
		  m2[i][j] = m1[i][j];
	  }
    }
}

/*Copia los valores del primer arreglo al segundo.*/
void copyVector(int v1[], int v2[], int length){
	int i;
	
    for (i = 0; i < length; i++){
	    v2[i] = v1[i]; 
    }
}

/*Pone en 0 las posiciones del sudoku.*/
void reiniciarSudoku(int sudoku[9][9]){
	int i,j;

	for (i = 0; i < 9; i++){
	  for (j = 0; j < 9; j++){
		  sudoku[i][j] = 0;
	  }
    }
}

/*Desordena un array.*/
void shuffle(int *array, size_t n)
{
    if (n > 1) {
        size_t i;
	for (i = 0; i < n - 1; i++) {
	  size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
	  int t = array[j];
	  array[j] = array[i];
	  array[i] = t;
	}
    }
}

/* Es lo que lleva ocupado del array. No el length, ya que el length seria la longitud total del arreglo
  * Se considera 0 como espacio vacio*/
int getSize(int *array, int length){
	int i, count;

	for (i=0; i < length; i++){
		if(array[i] != 0){
			count++;
		}
	}

	return count;
}