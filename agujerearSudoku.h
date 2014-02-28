/*Quita numeros del sudoku resuelto, coloca ceros para indicar que debe ir un entry/input.*/
void agujerearSudoku(int dificultad, int sudokuSolucion[9][9], int sudokuAresolver[9][9]){
	copyMatriz(sudokuSolucion, sudokuAresolver);

	int listaBorrar[81];

	int i;

	for (i=0; i < 81; i++) {
		listaBorrar[i] = i;
	}

	int *pListaBorrar = listaBorrar;

	shuffle(pListaBorrar, 81);

	int posicionAborrar, coordenadasPosicionAborrar[2] = {0,0}, *pCoordenadasPosicionAborrar = coordenadasPosicionAborrar;
	int pistas = 81 - calcularCntPistas(dificultad);

	for (i=0; i < 81; i++) {
		if(pistas == 0){
			break;
		}

		posicionAborrar = listaBorrar[i];

		decodePosicionAborrar(pCoordenadasPosicionAborrar, 81, posicionAborrar);

		if(solucionUnica(coordenadasPosicionAborrar[0], coordenadasPosicionAborrar[1], sudokuAresolver)){
			sudokuAresolver[coordenadasPosicionAborrar[0]][coordenadasPosicionAborrar[1]] = 0;
			pistas--;
		}
	}
	printSudokuSolaLinea(sudokuAresolver);
}

/*Devuelve un 1 si es que al borrar tal posicion, el sudoku mantendra la unicidad de su solucion.*/
int solucionUnica(int iBorrar, int jBorrar, int sudokuAresolver[9][9]){

	int sudokuSimulacion[9][9];
	reiniciarSudoku(sudokuSimulacion);
	copyMatriz(sudokuAresolver, sudokuSimulacion);
	sudokuSimulacion[iBorrar][jBorrar] = 0;

	int posibleCandidato;
	int sePuedeResolver;

	//Probamos primero su el sudoku queda en un estado consistente sacando el nro.
	sePuedeResolver = resolverSudoku(sudokuSimulacion);
	copyMatriz(sudokuAresolver, sudokuSimulacion);
	sudokuSimulacion[iBorrar][jBorrar] = 0;

	if(!sePuedeResolver){
		return 0;
	}

	int contadorSoluciones = 0;
    //Ahora si, vemos si es que la casilla tiene solucino unica, entonces si la podemos borrar.
	for (posibleCandidato = 1; posibleCandidato < 10; posibleCandidato++){

		if(verificarReglas(sudokuSimulacion, iBorrar, jBorrar, posibleCandidato, 0)){
			sudokuSimulacion[iBorrar][jBorrar] = posibleCandidato;

			sePuedeResolver = resolverSudoku(sudokuSimulacion);
			copyMatriz(sudokuAresolver, sudokuSimulacion);
			//"verificaRreglas()" necesita que no este el nro
			sudokuSimulacion[iBorrar][jBorrar] = 0;

			if(!sePuedeResolver){
				continue;
			}

			contadorSoluciones++;

			if(contadorSoluciones > 1){
				return 0;
			}
		}
	}

	if(contadorSoluciones == 0){
		//Si no se pudo resolver para ningun candidato, error.
	    return 0;
	}

	return 1;
}

/*Decodifica la posicion recibida, del 0-80, en representacion de las 81 casillas Carga el array con la posicion i y j decodificada.*/
void decodePosicionAborrar(int *pCoordenadasPosicionAborrar, int length, int posicionAborrar){
	if(posicionAborrar > 80){
		return;
	}

	int i, j, count = 0;

	for (j=0; j < 9; j++) {
      for (i=0; i < 9; i++) {

		  if(count == posicionAborrar){
			  pCoordenadasPosicionAborrar[0] = i;
			  pCoordenadasPosicionAborrar[1] = j;
			  return;
		  }

		  count++;
	  }
	}
}