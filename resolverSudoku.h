//#define _CAJAS 3

/*Verifica las tres condiciones basicas.*/
int verificarDatosSudokuInicial(int s[9][9]){

int f, c, k;
// Verificar todas las filas.
for(f = 0 ; f < 9 ; f++){
   for(c = 0 ; c < 9 ; c++){
       if(s[f][c]){
           for(k = c + 1 ; k < 9 ; k++){
               if(s[f][c] == s[f][k]){
                   printf("ERROR fila %hd\n", f);
                   return 0;
               }
           }
       }
   }
}
// Verificar todas las columnas...
for(c = 0 ; c < 9 ; c++){
   for(f = 0 ; f < 9 ; f++){
       if(s[f][c]){
           for(k = f + 1 ; k < 9 ; k++){
               if(s[f][c] == s[k][c]){
                   printf("ERROR columna %hd\n", c);
                   return 0;
               }
           }
       }
   }
}
// Verificar todas las cajas..
for(f = 0 ; f < 9 ; f += 3){
   for(c = 0 ; c < 9 ; c += 3){
       int ff, cc;
       for(ff = 0 ; ff < 9 / 3 ; ff++){
           for(cc = 0 ; cc < 9 / 3 ; cc++){
               if(s[f + ff][c + cc]){
                   for(k = ff * 3 + cc + 1 ; k < 9; k++){
                       if(s[f + ff][c + cc] == s[f + k / 3][c + k % 3]){
                           printf("\nERROR caja %hd - %hd\n", f, c);
                           for(k = 0 ; k < 9 ; k++){
                               if(k % 3 == 0){
                                   printf("\n");
                               }
                               printf("%4hd",
                               s[f + k / 3][c + k % 3]);
                           }
                           break;
                       }
                   }
               }
           }
       }
   }
}

return 1;
}

/* Completa celdas del sudoku si encuentra un valor (1 al 9) con una posiciÓn Única vÁlida en una fila, o en una columna, o en una caja.*/
int rellenarSiUnicaPosicion(int s[9][9]){
   
   // Devuelve 1 si se han realizado cambios en el sudoku.
   // Devuelve 0 si no se han realizado cambios.
   int candidato;
   int f, c;
   int noSeHanRealizadoCambios;
   
   for(noSeHanRealizadoCambios = 1 , candidato = 1 ;candidato <= 9; candidato++){
       for(f = 0 ; f < 9 ; f++){
           for(c = 0 ; c < 9 ; c++){
               // Si la posición ya tiene un valor, entonces no se le puede asignar otro.
               // Debemos verificar que el candidato cabe en esa celda:
               // 1. Cabe en esa fila.
               // 2. Cabe en esa columna.
               // 3. Cabe en esa misma caja.
               if(s[f][c] == 0 && candidatoCabeEnFila(s, f, candidato) && 
               candidatoCabeEnColumna(s, c, candidato) &&candidatoCabeEnCaja(s, f, c, candidato)){
                   // Llegados aquí, el candidato cabe en la posición f, c.
                   // Siguiente cuestión:
                   // ¿Es la posición f, c la única donde cabe candidato?
                   // 1. Miramos si es la única ubicación posible de la fila.
                   // Si lo es, inserta el valor y devuelve 1.
                   // Si no es la ubicación única devuelve un 0.
                   noSeHanRealizadoCambios = !ubicacionUnicaEnFila(s, f, candidato);
                   // 2. Miramos si es la única ubicación posible de la columna.
                   // Si lo es, inserta el valor y devuelve 1.
                   // Si no es la ubicación única devuelve un 0.
                       if(noSeHanRealizadoCambios){
                           noSeHanRealizadoCambios = !ubicacionUnicaEnColumna(s, c, candidato);
                       }
                   // 3. Miramos si es la única ubicación posible de la caja.
                   // Si lo es, inserta el valor y devuelve 1.
                   // Si no es la ubicación única devuelve un 0.
                       if(noSeHanRealizadoCambios){
                           noSeHanRealizadoCambios = !ubicacionUnicaEnCaja(s, f, c, candidato);
                       }
                       if(!noSeHanRealizadoCambios){
                           // Tenemos que el candidato es...
                           // el valor único en la fila, en la columna y en la celda.
                           // Podemos, pues, insertar el valor en esa posición (f, c)
                           s[f][c] = candidato;
                           return 1; // Valor True: inserción realizada.
                       }
               }
           }
       }
   }
return 0; // Valor False; inserción NO realizada.
}

/*Completa celdas del sudoku si encuentra una posicion donde sÓlo sea posible insertar un valor (1 al 9).*/
int rellenarSiUnicoValor(int s[9][9]){
   // Devuelve 1 si se han realizado cambios en el sudoku.
   // Devuelve 0 si no se han realizado cambios.
   int candidato;
   int f, c;
   
   for(candidato = 1 ;candidato <= 9; candidato++){
       for(f = 0 ; f < 9 ; f++){
           for(c = 0 ; c < 9 ; c++){
               // Si la posición ya tiene un valor, entonces no se le puede asignar otro.
               // Debemos verificar que el candidato cabe en esa celda:
               // 1. Cabe en esa fila.
               // 2. Cabe en esa columna.
               // 3. Cabe en esa misma caja.
               
               if(s[f][c] == 0 && candidatoCabeEnFila(s, f, candidato) && 
               candidatoCabeEnColumna(s, c, candidato) &&candidatoCabeEnCaja(s, f, c, candidato)){
                   // Llegados aquí, el candidato cabe en la posición f, c.
               
                   if(s[f][c] == 0 && candidatoUnicoEnPosicion(s, f, c, candidato)){
                       // Tenemos que el candidato es...
                       // el valor único en la fila, en la columna y en la celda.
                       // Podemos, pues, insertar el valor en esa posición (f, c)
                       s[f][c] = candidato;
                       return 1; // Valor True: inserción realizada.
                   }
               }
           }
       }
   }
   return 0; // Valor False; inserción NO realizada.
}


//devolver un valor verdadero (distinto de 0) si efectivamente en la fila recibida como segundo parámetro (int f)
//sí se puede insertar el valor recibido como tercer parámetro (int candidato).
int candidatoCabeEnFila(int s[9][9] , int f , int candidato){
   int c; // Variable que recorrerá todas las columnas
   
   for(c = 0 ; c < 9 ; c++){
       if(s[f][c] == candidato){ 
           // Si encuentra el valor candidato en la fila...
           return 0; 
           // No cabe: ya está el valor en la fila
       }
   }

   return 1; // Sí cabe el valor en esa fila: aún no está ubicado.
}

//devolver un valor verdadero (distinto de 0) si efectivamente en la columna recibida como segundo parámetro (int c)
//sí se puede insertar el valor recibido como tercer parámetro (int candidato)
int candidatoCabeEnColumna(int s[9][9] , int c , int candidato){
   int f; // Variable que recorrerá todas las filas
   
   for(f = 0 ; f < 9 ; f++){
       if(s[f][c] == candidato){ 
           // Si encuentra el valor candidato en la fila...
           return 0; 
           // No cabe: ya está el valor en la fila
       }
   }

   return 1; // Sí cabe el valor en esa fila: aún no está ubicado.
}


//devolver un valor verdadero (distinto de 0) si efectivamente en la caja donde está la celda marcada por la fila recibida
//como segundo parámetro (int f) y columna recibida como tercer parámetro (int c) sí se puede insertar el valor
//recibido como cuarto parámetro (int candidato)
int candidatoCabeEnCaja(int s[9][9] , int f , int c , int candidato){
   int f2, c2;//contadores que recorrerán las cajas
   int cajas1, cajas2; //para recorrer las cajas, ya que si ponemos f2+3, como f2 va creciendo se haria un bucle infinito
   if(f<3)f2=0;
   if(f>=3 && f<6)f2=3;
   if(f>=6 && f<9)f2=6;
   if(c<3)c2=0;
   if(c>=3 && c<6)c2=3;
   if(c>=6 && c<9)c2=6;
   
   cajas1 = f2+3;
   cajas2 = c2+3;
   int i,j;
   for(i = 0 ; i < 3 ; i++)
       for(j = 0 ; j < 3 ; j++){
           if(s[f2 + i][c2 + j] == candidato) return 0;
       }
   return 1;
} 


/*Devuelve 1 si efectivamente se cumple que en la celda indicada por la fila y columna
*  recibidas como segundo y tercer parámetro (int f, int c) solamente el valor recibido como cuarto parámetro (int candidato)
*  tiene cabida en esa celda.*/
int candidatoUnicoEnPosicion(int s[9][9], int f, int c, int candidato){
   
   int otroCandidato;

   for( otroCandidato = 1 ; otroCandidato <= 9 ; otroCandidato++){
       if( otroCandidato != candidato && candidatoCabeEnFila(s, f, otroCandidato) &&
           candidatoCabeEnColumna(s, c, otroCandidato) && candidatoCabeEnCaja(s, f, c, otroCandidato)){
           // Es decir: otro candidato (distinto del recibido
           // como cuarto parámetro) cabe en la fila y en la
           // columna y en la caja indicada por los parámetros
           // segundo y tercero.
           return 0;
       }
   }

   return 1;
}


/*Rastrea si en la fila indicada como segundo parámetro (int f) hay una única columna donde se puede insertar
*el valor recibido como tercer parámetro (int candidato): si es así, entonces la función inserta ese valor en
*esa posición única y devuelve un valor True (distinto de 0)*/
int ubicacionUnicaEnFila(int s[9][9] , int f , int candidato){
   int c; // Variable para las distintas columnas
   int pu; // Cuenta el número de posiciones posibles.
   int pc; // Almacena el valor de la última columna donde
               // puede ubicarse el valor candidato.
   
   for(pu = 0 , c = 0 ; c < 9 ; c++){
       if( s[f][c] == 0 && candidatoCabeEnColumna(s, c, candidato) && candidatoCabeEnCaja(s, f, c, candidato)){
           // Es decir: si la celda s[f][c] está vacía...
           // y el valor candidato cabe en esa Columna...
           // y el valor candidato cabe en esa Caja...
           pu++; // Una nueva posición donde cabe candidato.
           pc = c; // Columna donde cabe candidato.
       }
   }

   if(pu == 1){ // Es decir, si solo hay una posición válida...
       s[f][pc] = candidato; // Asignamos candidato a posición.
       return 1; // Devolvemos True
   }
   
   return 0; // No se ha podido insertar valor: devolvemos False.
}

/*Rastrea si en la columna indicada como segundo parámetro (int c) hay una única fila donde se puede insertar el valor
*  recibido como tercer parámetro (int candidato): si es así, entonces la función inserta ese valor en esa posición única
*  y devuelve un valor True (distinto de 0)*/
int ubicacionUnicaEnColumna(int s[9][9] , int c , int candidato){
   int f; // Variable para las distintas filas
   int pu; // Cuenta el número de posiciones posibles.
   int pf; // Almacena el valor de la última fila donde
               // puede ubicarse el valor candidato.
   
   for(pu = 0 , f = 0 ; f < 9 ; f++){
       if( s[f][c] == 0 && candidatoCabeEnFila(s, f, candidato) && candidatoCabeEnCaja(s, f, c, candidato)){
           // Es decir: si la celda s[f][c] está vacía...
           // y el valor candidato cabe en esa fila...
           // y el valor candidato cabe en esa Caja...
           pu++; // Una nueva posición donde cabe candidato.
           pf= f; // Fila donde cabe candidato.
       }
   }

   if(pu == 1){ // Es decir, si solo hay una posición válida...
       s[pf][c] = candidato; // Asignamos candidato a posición.
       return 1; // Devolvemos True
   }
   
   return 0; // No se ha podido insertar valor: devolvemos False.
}

/* Rastrea si en la caja donde se encuentra la posición marcada por la fila y columna recibidas como segundo y tercer parámetro
*  (int f, int c) hay una única celda donde se puede insertar el valor recibido como cuarto parámetro (int candidato): si es así,
*  entonces la función inserta ese valor en esa posición única y devuelve un valor True (distinto de 0).*/
int ubicacionUnicaEnCaja(int s[9][9] , int f , int c , int candidato){
   
   int f2, c2;//contadores que recorreran las cajas
   int cajas1, cajas2; //para recorrer las cajas, ya que si ponemos f2+3 (cajas), como f2 va creciendo se haria un bucle infinito
   int pu=0; // Cuenta el numero de posiciones posibles.
   int pf; // Almacena el valor de la ultima fila donde
               // puede ubicarse el valor candidato.
   int pc; // Almacena el valor de la ultima columna donde
               // puede ubicarse el valor candidato.
   
   if(f<3)f2=0;
   if(f>=3 && f<6)f2=3;
   if(f>=6 && f<9)f2=6;
   if(c<3)c2=0;
   if(c>=3 && c<6)c2=3;
   if(c>=6 && c<9)c2=6;
   
   cajas1 = f2+3;
   cajas2 = c2+3;
   
   int i,j;
   //if (candidatoCabeEnCaja(s, f, c, candidato))
   
   for(pu = 0, i = 0 ; i < 3 ; i++)
       for( j = 0; j < 3 ; j++){
           if( s[f2 + i][c2 + j] == 0 && candidatoCabeEnFila(s, f2 + i, candidato) && candidatoCabeEnColumna(s, c2 + j, candidato)){
               // Es decir: si la celda s[f][c] esta vacia...
               // y el valor candidato cabe en esa fila...
               // y el valor candidato cabe en esa Caja...
               pu++; // Una nueva posicion donde cabe candidato.
               pf= f2 + i; // Fila donde cabe candidato.
               pc= c2 + j; // Columna donde cabe candidato.
           }
       }

       if(pu == 1){ // Es decir, si solo hay una posici√≥n v√°lida...
           s[pf][pc] = candidato; // Asignamos candidato a posici√≥n.
           return 1; // Devolvemos True
       }

   return 0; // No se ha podido insertar valor: devolvemos False.
}


/*Soluciona el sudoku, se usa para saber si un sudoku tiene solución y asi verificar su unicidad.*/
int resolverSudoku(int s[9][9]){
	
   int cambio;
   /** Primero rellenamos lo que se pueda.
   * Existen dos técnicas:
   * 1. Dado un número, averiguar si en un fila, o en una columna,
   * o en un caja sólo cabe en una posición.
   * 2. Dada una posición, averiguar si en ella sólo es posible
   * ubicar un dígito.
   **/
   int count = 0;
   
   do{
       cambio = 0;
       // Vayamos a la primera opción...
       while(rellenarSiUnicaPosicion(s)){
           cambio = 1;
       }
       // Vayamos a la segunda opción...
       while(rellenarSiUnicoValor(s)){
           cambio = 1;
       }
	   
	   count++;

	   if(count == 50){
		   cambio = 0;
		}
   }
   while(cambio);

   if(!verificar81Datos(s)){
       //error
	   return 0;
   }

   return 1;
}



/*Verifica que todas las celdas del sudoku tienen un valor asignado.*/
int verificar81Datos(int s[9][9]){
   int f, c;

   for(f = 0 ; f < 9 ; f++)
       for(c = 0 ; c < 9 ; c++)
           if(s[f][c] == 0){
               return 0;
           }


   return 1;
}