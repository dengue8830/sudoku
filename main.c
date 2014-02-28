#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <ctype.h>
#include <time.h>
#include <string.h>

#include "agujerearSudoku.h"
#include "generarSudoku.h"
#include "sudokuUtils.h"
#include "resolverSudoku.h"

#define SEPARADOR_HORIZONTAL "--"
#define SEPARADOR_VERTICAL " | "
#define NO_POSICION 10
#define DIFICULTAD_FACIL 1
#define DIFICULTAD_MEDIO 2
#define DIFICULTAD_DIFICIL 3
#define PISTAS_DIFICULTAD_FACIL 51
#define PISTAS_DIFICULTAD_MEDIO 34
#define PISTAS_DIFICULTAD_DIFICIL 17
#define CANTIDAD_MINIMA_PARA_UNICIDAD 17

/*Variables globales*/
int sudokuSolucion[9][9];
GtkWidget *window;

/*Funcion que destruye la ventana y termina los procesos del programa. Se usa para capturar el evento de salir.*/
static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}

/*inicializa las propiedades de la ventana principal.*/
static void initialize_window()
{
  gtk_window_set_title(GTK_WINDOW(window),"Sudoku - Programacion II - dengue8830"); //Set window title
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 200); //Set default size for the window
  g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL); //End application when close button clicked

}

/*Muestra un mensaje, devuelve el numero entero correspondiente a la opcion seleccionada.*/
int mostrarMensaje(int GTK_MESSAGE_X, int GTK_BUTTONS_X, const gchar *mensaje)
{
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_X,
            GTK_BUTTONS_X,
            mensaje);
  gtk_window_set_title(GTK_WINDOW(dialog), GTK_MESSAGE_X == GTK_MESSAGE_ERROR ? "Error" : "Información");
  int response = gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
  return response;
}

/*Verifica la cerrectitud del sudoku completeado por el usuario.*/
void verificarSudoku (GtkWidget *btn, GtkWidget *tablero){

int i, j, ipos = -1, jpos = -1, ok = 1;

    for (j=0; j <= 10; j++) {
	  jpos++;
	  ipos = -1;

      for (i=0; i <= 10; i++) {
		  ipos++;
		   
		  if(i  == 3 || i == 7){
			  ipos--;
			  continue;
		   }
		   
		   if(j  == 3 || j == 7){

			  if(i == 10){
				  jpos--;
			  }
			  continue;
		   }

		  GtkWidget *child =  gtk_grid_get_child_at(GTK_GRID (tablero), j, i);

		  if(GTK_IS_ENTRY(child)){
			  const gchar *inputText = gtk_entry_get_text(GTK_ENTRY(child));
			  gchar sudokuElemento[2];
	          sprintf(sudokuElemento,"%d", sudokuSolucion[ipos][jpos]);

			  if(strcmp(sudokuElemento, inputText)){
				  //Son distintos.
				  //Marcamos como que ya hay algo mal, pero seguimos para tratar esas casillas que estan mal.
				  ok = 0;

				  //Si son distintos... ponemos en rojo
				  GdkColor color;
				  gdk_color_parse ("red", &color);
				  gtk_widget_modify_fg(GTK_WIDGET(child), GTK_STATE_NORMAL, &color);
			  }
		  }
		  
	  }
  }

  if(!ok){
	  mostrarMensaje(GTK_MESSAGE_INFO, GTK_BUTTONS_OK,  "Ups! :S ... verifica las casillas que están en rojo o vacías.");
	  return;
  }
  
  int selectedOption = mostrarMensaje(GTK_MESSAGE_QUESTION, GTK_BUTTONS_YES_NO,"Es correcto. Felicidades!!!  ¿Quieres seguir jugando? :B");

  if(selectedOption == GTK_RESPONSE_YES){
	  toMenuPrincipal(NULL, tablero);
  }else{
	  gtk_main_quit ();
  }

  return;
}

/*Manejador para controlar la entrada de solo numeros del 1-9. Codigo que se ejecuta como respuesta a un evento.*/
void insert_text_handler (GtkEntry    *entry,
                          const gchar *text,
                          gint         length,
                          gint        *position,
                          gpointer     data)
{				  
  GtkEditable *editable = GTK_EDITABLE(entry);
  int i, count=0;
  gchar *result = g_new (gchar, length);

  for (i=0; i < length; i++) {
    if (!isdigit(text[i]) || !strcmp(&text[i], "0")) 
      continue;
    result[count++] = text[i];
  }
  
  if (count > 0) {
    g_signal_handlers_block_by_func (G_OBJECT (editable),
                                     G_CALLBACK (insert_text_handler),
                                     data);
    gtk_editable_insert_text (editable, result, count, position);
    g_signal_handlers_unblock_by_func (G_OBJECT (editable),
                                       G_CALLBACK (insert_text_handler),
                                       data);
  }
  g_signal_stop_emission_by_name (G_OBJECT (editable), "insert_text");

  g_free (result);
}

/*Devuelve la cantidad de pistas para el sudoku, en funcion de la dificultad elegida.*/
int calcularCntPistas(int dificultad){
	switch(dificultad){
		case DIFICULTAD_DIFICIL:
					return PISTAS_DIFICULTAD_DIFICIL;
		case DIFICULTAD_MEDIO:
					return PISTAS_DIFICULTAD_MEDIO;
		case DIFICULTAD_FACIL:
					return PISTAS_DIFICULTAD_FACIL;
		default:
					return 0;
	}
}

/*Destruye el Widget actual y crea el menu principal*/
void toMenuPrincipal(GtkWidget *btn, GtkWidget *tablero){
	gtk_widget_destroy(tablero);
	menuPrincipal();
}

/*Cambia el color del input al recibir el focus. Lo pinta de negro.*/
void onFocusSetBlack(GtkWidget *entry){
  GdkColor color;
  gdk_color_parse ("black", &color);
  gtk_widget_modify_fg(GTK_WIDGET(entry), GTK_STATE_NORMAL, &color);
}

/*Crea los Widget necesarios para la interfaz.*/
void pintarSudoku(int sudoku[9][9]){//int argc, char *argv[], 
	GtkWidget *tablero, *input, *pista;
	 
    tablero = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (window), tablero);
  
	int i, j, ipos = -1, jpos = -1;

    for (j=0; j <= 10; j++) {
	  jpos++;
	  ipos = -1;

      for (i=0; i <= 10; i++) {
		  ipos++;
		   
		  if(i  == 3 || i == 7){
			  GtkWidget *separador = gtk_label_new (SEPARADOR_HORIZONTAL);
			  gtk_grid_attach (GTK_GRID (tablero), separador, j, i, 1, 1);
			  ipos--;
			  continue;
		   }
		   
		   if(j  == 3 || j == 7){
			  GtkWidget *separador = gtk_label_new (SEPARADOR_VERTICAL);
			  gtk_grid_attach (GTK_GRID (tablero), separador, j, i, 1, 1);

			  if(i == 10){
				  jpos--;
			  }
			  continue;
		   }

		  if(sudoku[ipos][jpos]){
			  //Es una pista
			  gchar temp[2];
	          sprintf(temp,"%d", sudoku[ipos][jpos]);
			  GtkWidget *pista = gtk_label_new (temp);
	 
			  gtk_grid_attach (GTK_GRID (tablero), pista, j, i, 1, 1);
		  }else{
			  //Es un input
			  input = gtk_entry_new ();
			  gtk_entry_set_alignment (GTK_ENTRY (input), 1.5);
			  gtk_entry_set_width_chars (GTK_ENTRY (input),1);
			  gtk_entry_set_max_length (GTK_ENTRY (input),1);
			  g_signal_connect (G_OBJECT(input), "insert_text", G_CALLBACK (insert_text_handler), NULL);
			  g_signal_connect (G_OBJECT(input), "focus-in-event", G_CALLBACK (onFocusSetBlack), NULL);
	
			  gtk_grid_attach (GTK_GRID (tablero), input, j, i, 1, 1);
		  }
	  }
	}
    
	GtkWidget *separador = gtk_label_new ("       ");
	gtk_grid_attach (GTK_GRID (tablero), separador, 11, 5, 1, 1);

	GtkWidget *btnListo = gtk_button_new_with_label ("Listo! B|");
	g_signal_connect (btnListo, "clicked", G_CALLBACK (verificarSudoku), tablero);
    gtk_grid_attach (GTK_GRID (tablero), btnListo, 12, 5, 2, 1);
	
	GtkWidget *btnAtras = gtk_button_new_with_label ("Atrás :[");
	g_signal_connect (btnAtras, "clicked", G_CALLBACK (toMenuPrincipal), tablero);
    gtk_grid_attach (GTK_GRID (tablero), btnAtras, 12, 7, 2, 1);
	
	GtkWidget *btnSalir = gtk_button_new_with_label ("Salir :(");
	g_signal_connect (btnSalir, "clicked", G_CALLBACK (destroy), NULL);
    gtk_grid_attach (GTK_GRID (tablero), btnSalir, 12, 9, 2, 1);

    gtk_widget_show_all(window);
}

/*Genera el sudoku y crea la interfaz.*/
  void jugar(int dificultad){
    int sudokuAresolver[9][9];

	reiniciarSudoku(sudokuSolucion);
	reiniciarSudoku(sudokuAresolver);

    generarSudoku(sudokuSolucion);
    printSudoku(sudokuSolucion);
    agujerearSudoku(dificultad, sudokuSolucion, sudokuAresolver);//aca se controla que la solucion sea unica
    printSudoku(sudokuAresolver);
    pintarSudoku(sudokuAresolver);
  }

/*Selecciona la dificultad en funcion del boton presionado.*/
  void seleccionDificultad (GtkWidget *btn, GtkWidget *tablero){//,  gpointer   data){
	const gchar *dificultadText = gtk_button_get_label (btn);
	int dificultadSeleccionada = 0;

	if(!strcmp(dificultadText, "Facil")){
	  dificultadSeleccionada = DIFICULTAD_FACIL;
	}
	if(!strcmp(dificultadText, "Medio")){
	  dificultadSeleccionada =DIFICULTAD_MEDIO;
	}
	if(!strcmp(dificultadText, "Dificil")){
	  dificultadSeleccionada = DIFICULTAD_DIFICIL;
	}

	gtk_widget_destroy(tablero);
	jugar(dificultadSeleccionada);
  }

/*Crea la interfaz para el menu principal.*/
  int menuPrincipal(){
    GtkWidget *tablero;

    tablero = gtk_grid_new ();
	gtk_container_add (GTK_CONTAINER (window), tablero);
	
	GtkWidget *espacio = gtk_label_new ("                               ");
	
	gtk_grid_attach (GTK_GRID (tablero), espacio, 0, 0, 1, 1);
	GtkWidget *separador = gtk_label_new ("\nBienvenid@!!!\n\nSeleccione la dificultad :D\n");
	gtk_label_set_justify(separador, GTK_JUSTIFY_CENTER);
	gtk_grid_attach (GTK_GRID (tablero), separador, 1, 0, 1, 1);
	
	gtk_grid_attach (GTK_GRID (tablero), espacio, 0, 1, 1, 1);
	GtkWidget *btnFacil = gtk_button_new_with_label ("Facil");
	g_signal_connect (btnFacil, "clicked", G_CALLBACK (seleccionDificultad), tablero);
    gtk_grid_attach (GTK_GRID (tablero), btnFacil, 1, 1, 2, 1);
	
	gtk_grid_attach (GTK_GRID (tablero), espacio, 0, 2, 1, 1);
	GtkWidget *btnMedio = gtk_button_new_with_label ("Medio");
	g_signal_connect (btnMedio, "clicked", G_CALLBACK (seleccionDificultad), tablero);
    gtk_grid_attach (GTK_GRID (tablero), btnMedio, 1, 2, 2, 1);
	
	gtk_grid_attach (GTK_GRID (tablero), espacio, 0, 3, 1, 1);
	GtkWidget *btnDificil = gtk_button_new_with_label ("Dificil");
	g_signal_connect (btnDificil, "clicked", G_CALLBACK (seleccionDificultad), tablero);
    gtk_grid_attach (GTK_GRID (tablero), btnDificil, 1, 3, 2, 1);
	
	gtk_grid_attach (GTK_GRID (tablero), espacio, 0, 4, 1, 1);
	GtkWidget *espacio2 = gtk_label_new ("                               ");
	gtk_grid_attach (GTK_GRID (tablero), espacio2, 1, 4, 1, 1);
	gtk_grid_attach (GTK_GRID (tablero), espacio, 0, 5, 1, 1);
	GtkWidget *btnSalir = gtk_button_new_with_label ("Salir");
	g_signal_connect (btnSalir, "clicked", G_CALLBACK (destroy), NULL);
    gtk_grid_attach (GTK_GRID (tablero), btnSalir, 1, 5, 2, 1);
	
	gtk_widget_show_all(window);
  }

/*Funcin main del programa.*/  
  int main (int argc, char *argv[]){	

    gtk_init(&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	initialize_window();
	menuPrincipal();

    gtk_main ();

    return 0;
  }