#define NO_POSICION 10

//int blackList[9] = {0,0,0,0,0,0,0,0,0};
//int blackListForzarColumna[9] = {0,0,0,0,0,0,0,0,0};
//int blackListCandidatosMenorCantidadConflictos[9] = {0,0,0,0,0,0,0,0,0};

/*Genera el sudoku, pero cuando no hay solucion lo reinicia.*/
void generarSudoku(int sudokuSolucion[9][9]){
	int  ok = 0;

	/*test area
	int tabla[9][9] = {
		{4,5,3,1,8,2,6,9,0},
		{8,6,9,5,7,3,4,2,0},
		{7,1,2,9,6,4,3,5,0},
		{9,7,5,6,2,8,1,0,0},
		{3,4,8,7,9,1,5,6,0},
		{1,2,6,3,4,5,9,7,0},
		{2,3,1,8,5,6,7,4,0},
		{6,9,4,2,3,7,8,1,0},
		{5,8,7,4,1,9,2,3,0},
	                  };
	copyMatriz(tabla, sudokuSolucion);
    int	blackListTemp[9] = {4,1,3,0,0,0,0,0,0};
	copyVector(blackListTemp, blackList, 9);
	printVector(blackList, 9);
	*jAnterior = 7;
	end test area*/

	while (!ok) {
		reiniciarSudoku(sudokuSolucion);
		ok = kernel(sudokuSolucion);
	}
}

/*Codigo mas importante, genera el sudoku.*/
int kernel(int sudoku[9][9]){
	srand(time(NULL));
	
	int i ,j, ok, candidato, jAnterior = 0,*pjAnterior = &jAnterior,  intentos[9] = {0,0,0,0,0,0,0,0,0};
	int blackList[9] = {0,0,0,0,0,0,0,0,0}, *pBlackList = &blackList;
	int blackListForzarColumna[9] = {0,0,0,0,0,0,0,0,0}, *pBlackListForzarColumna = &blackListForzarColumna;
    int blackListCandidatosMenorCantidadConflictos[9] = {0,0,0,0,0,0,0,0,0}, *pBlackListCandidatosMenorCantidadConflictos = &blackListCandidatosMenorCantidadConflictos;

	for (j=0; j < 9; j++) {
      for (i=0; i < 9; i++) {

		  /*Si esa posicion ya tiene un valor saltamos a la siguiente posicion*/
		  if(sudoku[i][j]){
			  continue;
		  }

		  ok = 0;
		  candidato = 0;
		  reiniciarVector(intentos, 9);

	      while(!ok){
			  candidato = (rand() % 9) + 1;
			  ok = verificarReglas(sudoku, i, j, candidato, 0);

			  if(ok){
				  //printf("ok:%i\n",candidato);
				  printf("%i",candidato);
				  sudoku[i][j] = candidato;
			  }else{
				  if(!containsInt(intentos, 9, candidato)){
					  addInt(intentos, 9, candidato);

			          if(intentoConTodosLosNumeros(intentos, 9)){
						  int *pj = &j;
				          ok = forzar (sudoku, i, j, pj, pjAnterior, pBlackList, pBlackListForzarColumna, pBlackListCandidatosMenorCantidadConflictos);//no se debe enviar candidato

						  if(!ok){
							  //volvemos a generar sudoku
							  return 0;
						  }
			          }
				  }
			  }

		  }
		}
	  }
	  return 1;
}

/*Devuelve 1 si es que el arreglo contiene los numeros del 1-9. Sino 0.*/
int intentoConTodosLosNumeros(int array[], int length){
	int i;

	for (i = 0; i < length; i++){

	 if (!array[i]){
	    return 0;
	 }
   }

   return 1;
}

/*Devuelve la posicion inicial en cada cuadrado en funcion del parametro. Como el sudoku es una estructura simetrica, 
 * se puede usar tanto para las filas como para las columnas.*/
int calcularValorInicialEnReglaCuadrado(int x){
	return x <= 2 ? 0 : x >= 3 && x <= 5 ? 3 : x >= 6 && x <= 8 ? 6 : 8;
}

/*Devuelve la cantidad de conflictos en el cuadrado que contiene a la posicion pasada por parametro.*/
int getCantidadConflictosEnReglaCuadrado(int tabla[9][9], int ii, int jj, int candidato, int enGral){

	int i, j, count = 0;

	/*Calculamos el valor inicial de cada indice para empezar a buscar en la primera
      casilla de la caja correspondiente*/
	int iInicial = calcularValorInicialEnReglaCuadrado(ii);
	int jInicial = calcularValorInicialEnReglaCuadrado(jj);

	for (j = jInicial; j <= jInicial+2; j++) {
      for (i = iInicial; i <= iInicial+2; i++) {

		    if(enGral && i == ii && j == jj){
			    continue;
		    }
		  
			if(tabla[i][j] == candidato){
			    count++;
		    }
	  }
	}

	return count;
}

/*Devuelve la cantidad de conflictos en la regla vertical.*/
int getCantidadConflictosEnReglaVertical(int tabla[9][9], int ii, int jj, int candidato, int enGral){
	int i, count = 0;

    for (i=0; i < 9; i++) {

		if(enGral && i == ii){
			continue;
		}

		if(tabla[i][jj] == candidato){
		    count++;
	    }
	}

	return count;
}

/*Devuelve la cantidad de conflictos en la regla horizontal.*/
int getCantidadConflictosEnReglaHorizontal(int tabla[9][9], int ii, int jj, int candidato, int enGral){
	int j, count = 0;

    for (j=0; j < 9; j++) {

		if(enGral && j == jj){
			continue;
		}

		if(tabla[ii][j] == candidato){
		    count++;
	    }
	}

	return count;
}

/*Verifica si el sudoku cumple las tres reglas. Si es asi devuelve un 1, sino un 0.*/
int verificarReglas(int tabla[9][9], int ii, int jj, int candidato, int enGeneral){
	//Poner los 3 if como un solo ||, ahora lo dejo asi para debug mas sencillo
	if(getCantidadConflictosEnReglaHorizontal(tabla, ii, jj, candidato, enGeneral)){
		return 0;
	}

	if(getCantidadConflictosEnReglaVertical(tabla, ii, jj, candidato, enGeneral)){
		return 0;
	}

	if(getCantidadConflictosEnReglaCuadrado(tabla, ii, jj, candidato, enGeneral)){
		return 0;
	}

	return 1;
}

/*Devuelve un 1  si el candidato en esa posicion, tiene algun conflicto en la columna correspondiente.*/
int hayConflictoConCandidatoEnColumna(int sudoku[9][9], int jj, int candidato){
	int i;

    for (i = 0; i < 9; i++){

	   if(sudoku[i][jj] == candidato){
	          return 1;
          }
      }

	return 0;
}

/*Recorremos la columna para ver si hay algun numero que de conflicto en dicha columna*/
int hayConflictoEnColumna(int sudoku[9][9], int jj){
	int i, nuevoCandidato;

	for (nuevoCandidato = 1; nuevoCandidato <= 9; nuevoCandidato++){
	    for (i = 0; i < 9; i++){

		    if(sudoku[i][jj] == nuevoCandidato){
	            return 1;
            }
        }
	}

	return 0;
}

/*Devuelve la fila con la cual candidato tiene conflicto en esa columna, sino hay conflicto devuelve -1.*/
int getFilaConflictoEnColumna(int sudoku[9][9], int jj, int candidato){
	int i;

	for (i = 0; i < 9; i++){

        if(sudoku[i][jj] == candidato){
			//Si hay conflicto en columna, devolvemos la fila con la cual se tiene conflicto
			return i;
        }
    }

	return -1;
}

/*Devuelve la columna con la cual candidato tiene conflicto en esa fila, sino hay conflicto devuelve -1.*/
int getColumnaConflictoEnFila(int sudoku[9][9], int ii, int candidato){
	int j;

	for (j = 0; j < 9; j++){

        if(sudoku[ii][j] == candidato){
			//Si hay conflicto en fila, devolvemos la fila con la cual se tiene conflicto
			return j;
        }
    }

	return -1;
}

/*Devuelve el primer conflicto en el cuadrado, como no se puede tener
 * conflictos a la derecha de la posicion en cuestion (por la logica utilizada)
 * basta con buscar en las posiciones anteriores y posteriores pero no en posiciones de una columna mas a la derecha a la posicion en cuestion.
 */
void setPosicionConflictoEnCuadrado(int sudoku[9][9], int candidato, int ii, int jj, int *pos){
	int i, j;

	/*Calculamos el valor inicial de cada indice para empezar a buscar en la primera
      casilla de la caja correspondiente*/
	int iInicial = calcularValorInicialEnReglaCuadrado(ii);
	int jInicial = calcularValorInicialEnReglaCuadrado(jj);
	
	for (j = jInicial; j <= jInicial+2; j++) {
      for (i = iInicial; i <= iInicial+2; i++) {

			if(sudoku[i][j] == candidato){
			    pos[0] = i;
				pos[1] = j;
		    }
	  }
	}
}

/*Devuelve 1 si el sudoku tiene conflicto en esta fila para este candidato.*/
int tieneConflictoEnEstaFila(int sudoku[9][9], int ii, int candidato){
	if(getColumnaConflictoEnFila(sudoku, ii, candidato) != -1){
		return 1;
	}

	return 0;
}

/*Devuelve 1 si el sudoku tiene conflicto en esta columna para este candidato.*/
int tieneConflictoEnEstaColumna(int sudoku[9][9], int jj, int candidato){
	if(getFilaConflictoEnColumna(sudoku, jj, candidato) != -1){
		return 1;
	}

	return 0;
}

/*Devuelve 1 si habiendo conflicto en el cuadrado, es en la misma columna. Esto es para que sea cierta la condicion de conflicto solo en columna.*/
int siTieneConflictoEnCuadradoEsEnLaMismaColumna(int posicionConflictoEnCuadrado[], int jj){
	//Si existe conflicto en cuadrado...
	if(posicionConflictoEnCuadrado[0] != NO_POSICION || posicionConflictoEnCuadrado[1] != NO_POSICION){
	    //Debe ser la misma columna, para que sea conflicto en col unica.. junto con las otras condiciones.
		if(posicionConflictoEnCuadrado[1] == jj){
			return 1;
		}
	}else{
		return 1;
	}

	return 0;
}

/*Devuelve 1 si habiendo conflicto en el cuadrado, es en la misma columna. Esto es para que sea cierta la condicion de conflicto solo en columna.*/
int hayConflictoSoloEnColumna(int sudoku[9][9], int ii, int jj, int candidato){
    int posicionConflicto[2] = {NO_POSICION, NO_POSICION};//Se le pone 10, porque puede ser que haya conflicto con el (0,0)
	int *pPosicionConflicto = posicionConflicto;
	setPosicionConflictoEnCuadrado(sudoku, candidato, ii, jj, pPosicionConflicto);

	if(!tieneConflictoEnEstaFila(sudoku, ii, candidato) && tieneConflictoEnEstaColumna(sudoku, jj, candidato) && siTieneConflictoEnCuadradoEsEnLaMismaColumna(posicionConflicto, jj)){
		return 1;
	}

	return 0;
}

/*Busca el candidato con el que haya solo un conflicto y sea en la misma columna. Borra el numero con el que hay conflicto y coloca el nuevo en la posicion correspondiente;  asi se deja el sudoku sin conflictos, la casilla vacia se tratara de nuevo mas tarde.*/
int forzarColumnaUnica(int sudoku[9][9], int iPosicionAtratar, int jPosicionAtratar, int *j, int *blackList){
	if(!hayConflictoEnColumna (sudoku, jPosicionAtratar)){
		return 0;
	}

	int nuevoCandidato;

	for(nuevoCandidato = 1; nuevoCandidato <= 9; nuevoCandidato++){
		if(hayConflictoSoloEnColumna(sudoku, iPosicionAtratar, jPosicionAtratar, nuevoCandidato) && !containsInt(blackList, 9, nuevoCandidato)){

			addInt(blackList, 9, nuevoCandidato);
			int iPosicionConflicto = getFilaConflictoEnColumna(sudoku, jPosicionAtratar, nuevoCandidato);
			int jPosicionConflicto = jPosicionAtratar;
			sudoku[iPosicionConflicto][jPosicionConflicto] = 0;
			sudoku[iPosicionAtratar][jPosicionAtratar] = nuevoCandidato;
			*j-=1;
			return 1;
		}
	}

	return 0;
}

/*Borra el numero con el que hay conflicto y coloca el nuevo en la posicion correspondiente;  asi se deja el sudoku sin conflictos,
 *la casilla vacia se tratara de nuevo mas tarde. Pueden haber mas conflictos para este candidato, pero eso a esta funcion no le interesa.*/
int forzarColumna(int sudoku[9][9], int candidato, int iPosicionAtratar, int jPosicionAtratar, int *j, int *jAnterior, int *blackListForzarColumna){
	if(!hayConflictoConCandidatoEnColumna (sudoku, jPosicionAtratar, candidato)){
		return 0;
	}

    addInt(blackListForzarColumna, 9, candidato);
	int iPosicionConflicto = getFilaConflictoEnColumna(sudoku, jPosicionAtratar, candidato);
	int jPosicionConflicto = jPosicionAtratar;
	sudoku[iPosicionConflicto][jPosicionConflicto] = 0;
	sudoku[iPosicionAtratar][jPosicionAtratar] = candidato;

	//Se vuelve a recorrer la misma columna, si es que no se modifico ya.
	if(*jAnterior == *j){
    	*j-=1;
	}

	return 1;
}

/*Devuelve 1 si es que puede salvar la situacion de callejon sin salida, 0 de lo contrario.*/
int forzar(int sudoku[9][9], int iPosicionAtratar, int jPosicionAtratar, int *j, int *jAnterior, int *blackList, int *blackListForzarColumna, int *blackListCandidatosMenorCantidadConflictos){
	int loop = -1;

	do{
		//Se reinicia la blackList por cada columna.
	    if(*jAnterior != jPosicionAtratar || loop != -1){
		    reiniciarVector(blackList, 9);
		    reiniciarVector(blackListForzarColumna, 9);
		    reiniciarVector(blackListCandidatosMenorCantidadConflictos, 9);

		    *jAnterior = jPosicionAtratar;
	    }

	    //Primero corregimos los conflictos en la misma columna si es que los hay
	    //printVector(blackList, 9);
	    if(forzarColumnaUnica(sudoku, iPosicionAtratar, jPosicionAtratar, j, blackList)){
		    return 1;
	    }

		loop = loop == -1 ? 20 : loop;
		loop--;

	}while(loop);

	//Se usa para evitar seguir restando a "j" cuando ya se resto una vez. Es que en el while tambien se llama a forzarColumna().
	*jAnterior = *j;

	int sudokuSimulacion[9][9];
	copyMatriz(sudoku, sudokuSimulacion);

	int candidatoConMenorCantidadDeConflictos = getCandidatoMenorCantidadConflictos(sudoku, iPosicionAtratar, jPosicionAtratar, blackList, blackListCandidatosMenorCantidadConflictos);
	
	forzarColumna(sudoku, candidatoConMenorCantidadDeConflictos, iPosicionAtratar, jPosicionAtratar, j,  jAnterior, blackListForzarColumna);

     //Si no hay conflicto en la columna o no se puede resolver, forzamos los conflictos en filas y cuadros
	do{

	   if(!forzarFilasCuadros(sudoku, candidatoConMenorCantidadDeConflictos, iPosicionAtratar, jPosicionAtratar)){
		   candidatoConMenorCantidadDeConflictos = getCandidatoMenorCantidadConflictos(sudoku, iPosicionAtratar, jPosicionAtratar, blackList, blackListCandidatosMenorCantidadConflictos);

		   if(!candidatoConMenorCantidadDeConflictos){
			   //Ya se probo con todos los numeros y con todos hay conflicto insolucionable
			   //Este sudoku no tiene solucion
			   return 0;
		   }
		   forzarColumna(sudoku, candidatoConMenorCantidadDeConflictos, iPosicionAtratar, jPosicionAtratar, j, jAnterior, blackListForzarColumna);
	   }
	}while(!verificarReglas(sudoku, iPosicionAtratar, jPosicionAtratar, candidatoConMenorCantidadDeConflictos, 0));

	//Se encontro una solucion
	return 1;
}

/*Devuelve el candidato con la menor cantidad de conflictos.*/
int getCandidatoMenorCantidadConflictos(int sudoku[9][9], int iPosicionAtratar, int jPosicionAtratar, int *blackList, int *blackListCandidatosMenorCantidadConflictos){
	int count, min = 81, candidato = 0, nuevoCandidato;

	for(nuevoCandidato = 1; nuevoCandidato <= 9; nuevoCandidato++){
		count = 0;

		if(containsInt(blackList, 9, nuevoCandidato) || containsInt(blackListCandidatosMenorCantidadConflictos, 9, nuevoCandidato)){
			continue;
		}

        count += getCantidadConflictosEnReglaCuadrado(sudoku, iPosicionAtratar, jPosicionAtratar, nuevoCandidato, 0);
	    count += getCantidadConflictosEnReglaHorizontal(sudoku, iPosicionAtratar, 0, nuevoCandidato, 0);
	    count += getCantidadConflictosEnReglaVertical(sudoku, 0, jPosicionAtratar, nuevoCandidato, 0);

    	if(count < min){
		    min = count;
			candidato = nuevoCandidato;
	    }
	}

	addInt(blackListCandidatosMenorCantidadConflictos, 9, candidato);
	return candidato;
}

/*Devuelve un 0 o un numero distinto de 0, para indicar si se llego al limite superior del sudoku, es decir, si es que nos estamos saliendo del cuadro*/
int sePuedeSwapArriba(int iPosicionConflicto){
	return iPosicionConflicto-1;
}

/*Devuelve un 0 o un numero distinto de 0, para indicar si se llego al limite inferior del sudoku, es decir, si es que nos estamos saliendo del cuadro*/
int sePuedeSwapAbajo(int iPosicionConflicto){
	if(iPosicionConflicto < 7){
		return 1;
	}

	return 0;
}

/*Realiza el intercambio de casillas*/
void swap(int sudokuSimulacion[9][9], int iPosicionConflicto, int iPosicionConflictoSwap, int jPosicionConflicto){
	int temp = sudokuSimulacion[iPosicionConflictoSwap][jPosicionConflicto];
	sudokuSimulacion[iPosicionConflictoSwap][jPosicionConflicto] = sudokuSimulacion[iPosicionConflicto][jPosicionConflicto];
	sudokuSimulacion[iPosicionConflicto][jPosicionConflicto] = temp;
}

/*Devuevle 1 si es que el sudoku es correcto, 0 en caso contrario.*/
int verificarReglasEnGeneral(int sudoku[9][9]){
	int i,j;

	for (j = 0; j < 9; j++) {
      for (i = 0; i < 9; i++) {
		  if(!verificarReglas(sudoku, i, j, sudoku[i][j], 1)){
			  return 0;
		  }
	  }
	}

	return 1;
}

/*Realiza el intercambio recursivo de casillas hacia arriba, para ver si se puede salvar el conficto dejando el sudoku con los mismos numeros pero en otro orden.*/
int swapArriba(int sudokuSolucion[9][9], int sudokuSimulacion[9][9], int iPosicionConflicto, int jPosicionConflicto, int fatalError){
	swap(sudokuSimulacion, iPosicionConflicto, iPosicionConflicto-1, jPosicionConflicto);

	if(verificarReglasEnGeneral(sudokuSimulacion)){
		return 1;
	}else{
		if(fatalError){
			return 0;
		}

		if(!sePuedeSwapArriba(iPosicionConflicto)){
			return 0;
		}

        //Volvemos a un estado seguro.
		copyMatriz(sudokuSolucion, sudokuSimulacion);

		if(swapArriba(sudokuSolucion, sudokuSimulacion, iPosicionConflicto-1, jPosicionConflicto, 0)){
			return swapArriba(sudokuSolucion, sudokuSimulacion, iPosicionConflicto, jPosicionConflicto, 1);
		}

		return 0;
	}
}

/*Realiza el intercambio recursivo de casillas hacia abajo, para ver si se puede salvar el conficto dejando el sudoku con los mismos numeros pero en otro orden.*/
int swapAbajo(int sudokuSolucion[9][9], int sudokuSimulacion[9][9], int iPosicionConflicto, int jPosicionConflicto, int fatalError){
	swap(sudokuSimulacion, iPosicionConflicto, iPosicionConflicto+1, jPosicionConflicto);

	if(verificarReglasEnGeneral(sudokuSimulacion)){
		return 1;
	}else{
		if(fatalError){
			return 0;
		}

		if(!sePuedeSwapAbajo(iPosicionConflicto)){
			return 0;
		}

		//Volvemos a un estado seguro.
		copyMatriz(sudokuSolucion, sudokuSimulacion);

		if(swapAbajo(sudokuSolucion, sudokuSimulacion, iPosicionConflicto+1, jPosicionConflicto, 0)){
			return swapAbajo(sudokuSolucion, sudokuSimulacion, iPosicionConflicto, jPosicionConflicto, 1);
		}

		return 0;
	}
}

/*Devuelve 1 si es que se puede salvar el conflicto con casillas en la misma fila y/o cuadrado.
 * Realiza intercambios recursivos (hacia arriba o hacia abajo) para salvar los conflictos, dejando el sudoku con los mismos numeros pero en un orden diferente.*/
int forzarFilasCuadros(int sudoku[9][9], int candidato, int iPosicionAtratar, int jPosicionAtratar){
	int iPosicionConflicto = getFilaConflictoEnColumna(sudoku, jPosicionAtratar, candidato);
	iPosicionConflicto = iPosicionConflicto == -1 ? iPosicionAtratar : iPosicionConflicto;
	int jPosicionConflicto = getColumnaConflictoEnFila(sudoku, iPosicionAtratar, candidato);
	jPosicionConflicto = jPosicionConflicto == -1 ? jPosicionAtratar : jPosicionConflicto;

	int sudokuSimulacion[9][9];
	copyMatriz(sudoku, sudokuSimulacion);

	if(sePuedeSwapArriba(iPosicionConflicto)){
		if(swapArriba(sudoku, sudokuSimulacion, iPosicionConflicto, jPosicionConflicto, 0)){
			copyMatriz(sudokuSimulacion, sudoku);
			return 1;
		}
	}

	copyMatriz(sudoku, sudokuSimulacion);

	if(sePuedeSwapAbajo(iPosicionConflicto)){
		if(swapAbajo(sudoku, sudokuSimulacion, iPosicionConflicto, jPosicionConflicto, 0)){
			copyMatriz(sudokuSimulacion, sudoku);
		    return 1;
		}	
	}
	
	
	return 0;
}