#include "arkanoPiLib.h"

int ladrillos_basico[NUM_FILAS_DISPLAY][NUM_COLUMNAS_DISPLAY] = {
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
};

int vidas = MAX_VIDAS;

void MuestraMensajeInicial(){
	piLock (STD_IO_BUFFER_KEY);
	printf("\n ****************\n ***BIENVENIDO*** \n **************** \n" );
	printf("\n Tienes %d vidas \n -Izquierda: tecla A \n -Derecha:   tecla D\n -Salir:     tecla F\n", vidas);
	piUnlock (STD_IO_BUFFER_KEY);
}

//------------------------------------------------------
// FUNCIONES DE VISUALIZACION (ACTUALIZACION DEL OBJETO PANTALLA QUE LUEGO USARA EL DISPLAY)
//------------------------------------------------------


void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial) {
	int i, j = 0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = p_pantalla_inicial->matriz[i][j];
		}
	}

	return;
}

void ReseteaPantalla (tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = 0;
		}
	}
}


//------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET
//------------------------------------------------------

void InicializaLadrillos(tipo_pantalla *p_ladrillos) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_ladrillos->matriz[i][j] = ladrillos_basico[i][j];
		}
	}
}

void InicializaPelota(tipo_pelota *p_pelota) {
	// Aleatorizamos la posicion inicial de la pelota
	p_pelota->x = rand() % NUM_COLUMNAS_DISPLAY;
	p_pelota->y = 2 + rand() % (NUM_FILAS_DISPLAY-2); // 2 evita que aparezca encima de ladrillos y para que no empiece demasiado pegada al suelo de la pantalla

	// Pelota inicialmente en el centro de la pantalla
	//p_pelota->x = NUM_COLUMNAS_DISPLAY/2 - 1;
	//p_pelota->y = NUM_FILAS_DISPLAY/2 -1 ;

	InicializaPosiblesTrayectorias(p_pelota);

	// Trayectoria inicial
	//p_pelota->trayectoria.xv = 0;
	//p_pelota->trayectoria.yv = 1;
	CambiarDireccionPelota(p_pelota, rand() % p_pelota->num_posibles_trayectorias);
}

void InicializaPelota2(tipo_pelota *p_pelota2) {
	// Aleatorizamos la posicion inicial de la pelota
	p_pelota2->x = rand() % NUM_COLUMNAS_DISPLAY;
	p_pelota2->y = 2 + rand() % (NUM_FILAS_DISPLAY-2); // 2 evita que aparezca encima de ladrillos y para que no empiece demasiado pegada al suelo de la pantalla

	// Pelota inicialmente en el centro de la pantalla
	//p_pelota->x = NUM_COLUMNAS_DISPLAY/2 - 1;
	//p_pelota->y = NUM_FILAS_DISPLAY/2 -1 ;

	InicializaPosiblesTrayectorias(p_pelota2);

	// Trayectoria inicial
	//p_pelota->trayectoria.xv = 0;
	//p_pelota->trayectoria.yv = 1;
	CambiarDireccionPelota(p_pelota2, rand() % p_pelota2->num_posibles_trayectorias);
}

void InicializaPala(tipo_pala *p_pala) {
	// Pala inicialmente en el centro de la pantalla
	p_pala->x = NUM_COLUMNAS_DISPLAY/2 - p_pala->ancho/2;
	p_pala->y = NUM_FILAS_DISPLAY - 1;
	p_pala->ancho = NUM_COLUMNAS_PALA;
	p_pala->alto = NUM_FILAS_PALA;
}

void InicializaPosiblesTrayectorias(tipo_pelota *p_pelota) {
	p_pelota->num_posibles_trayectorias = 0;
	p_pelota->posibles_trayectorias[ARRIBA_IZQUIERDA].xv = -1;
	p_pelota->posibles_trayectorias[ARRIBA_IZQUIERDA].yv = -1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ARRIBA].xv = 0;
	p_pelota->posibles_trayectorias[ARRIBA].yv = -1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ARRIBA_DERECHA].xv = 1;
	p_pelota->posibles_trayectorias[ARRIBA_DERECHA].yv = -1;
	p_pelota->num_posibles_trayectorias++;

	p_pelota->posibles_trayectorias[ABAJO_DERECHA].xv = 1;
	p_pelota->posibles_trayectorias[ABAJO_DERECHA].yv = 1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ABAJO].xv = 0;
	p_pelota->posibles_trayectorias[ABAJO].yv = 1;
	p_pelota->num_posibles_trayectorias++;
	p_pelota->posibles_trayectorias[ABAJO_IZQUIERDA].xv = -1;
	p_pelota->posibles_trayectorias[ABAJO_IZQUIERDA].yv = 1;
	p_pelota->num_posibles_trayectorias++;

	//p_pelota->posibles_trayectorias[IZQUIERDA].xv = -1;
	//p_pelota->posibles_trayectorias[IZQUIERDA].yv = 0;
	//p_pelota->num_posibles_trayectorias++;
	//p_pelota->posibles_trayectorias[DERECHA].xv = 1;
	//p_pelota->posibles_trayectorias[DERECHA].yv = 0;
	//p_pelota->num_posibles_trayectorias++;
}

void InicializaPosiblesTrayectorias2(tipo_pelota *p_pelota2) {
	p_pelota2->num_posibles_trayectorias = 0;
	p_pelota2->posibles_trayectorias[ARRIBA_IZQUIERDA].xv = -1;
	p_pelota2->posibles_trayectorias[ARRIBA_IZQUIERDA].yv = -1;
	p_pelota2->num_posibles_trayectorias++;
	p_pelota2->posibles_trayectorias[ARRIBA].xv = 0;
	p_pelota2->posibles_trayectorias[ARRIBA].yv = -1;
	p_pelota2->num_posibles_trayectorias++;
	p_pelota2->posibles_trayectorias[ARRIBA_DERECHA].xv = 1;
	p_pelota2->posibles_trayectorias[ARRIBA_DERECHA].yv = -1;
	p_pelota2->num_posibles_trayectorias++;

	p_pelota2->posibles_trayectorias[ABAJO_DERECHA].xv = 1;
	p_pelota2->posibles_trayectorias[ABAJO_DERECHA].yv = 1;
	p_pelota2->num_posibles_trayectorias++;
	p_pelota2->posibles_trayectorias[ABAJO].xv = 0;
	p_pelota2->posibles_trayectorias[ABAJO].yv = 1;
	p_pelota2->num_posibles_trayectorias++;
	p_pelota2->posibles_trayectorias[ABAJO_IZQUIERDA].xv = -1;
	p_pelota2->posibles_trayectorias[ABAJO_IZQUIERDA].yv = 1;
	p_pelota2->num_posibles_trayectorias++;
}

void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			p_pantalla->matriz[i][j] = p_ladrillos->matriz[i][j];
		}
    }
}

void PintaPala(tipo_pala *p_pala, tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	for(i=0;i<NUM_FILAS_PALA;i++) {
		for(j=0;j<NUM_COLUMNAS_PALA;j++) {
			if (( (p_pala->y+i >= 0) && (p_pala->y+i < NUM_FILAS_DISPLAY) ) &&
				( (p_pala->x+j >= 0) && (p_pala->x+j < NUM_COLUMNAS_DISPLAY) ))
				p_pantalla->matriz[p_pala->y+i][p_pala->x+j] = 1;
		}
	}
}

void PintaPelota(tipo_pelota *p_pelota, tipo_pantalla *p_pantalla) {
	if( (p_pelota->x >= 0) && (p_pelota->x < NUM_COLUMNAS_DISPLAY) ) {
		if( (p_pelota->y >= 0) && (p_pelota->y < NUM_FILAS_DISPLAY) ) {
			p_pantalla->matriz[p_pelota->y][p_pelota->x] = 8;
		}
		else {
			printf("\n\nPROBLEMAS!!!! posicion y=%d de la pelota INVALIDA!!!\n\n", p_pelota->y);
			fflush(stdout);
		}
	}
	else {
		printf("\n\nPROBLEMAS!!!! posicion x=%d de la pelota INVALIDA!!!\n\n", p_pelota->x);
		fflush(stdout);
	}
}

void PintaPelota2(tipo_pelota *p_pelota2, tipo_pantalla *p_pantalla) {
	if( (p_pelota2->x >= 0) && (p_pelota2->x < NUM_COLUMNAS_DISPLAY) ) {
		if( (p_pelota2->y >= 0) && (p_pelota2->y < NUM_FILAS_DISPLAY) ) {
			p_pantalla->matriz[p_pelota2->y][p_pelota2->x] = 8;
		}
		else {
			printf("\n\nPROBLEMAS!!!! posicion y=%d de la pelota INVALIDA!!!\n\n", p_pelota2->y);
			fflush(stdout);
		}
	}
	else {
		printf("\n\nPROBLEMAS!!!! posicion x=%d de la pelota INVALIDA!!!\n\n", p_pelota2->x);
		fflush(stdout);
	}
}

void ActualizaPantalla(tipo_arkanoPi* p_arkanoPi) {

    // Borro toda la pantalla
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));

    // Pinta los ladrillos
	PintaLadrillos(
		(tipo_pantalla*)(&(p_arkanoPi->ladrillos)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

    // Pinta la pala
	PintaPala(
		(tipo_pala*)(&(p_arkanoPi->pala)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

	// Pinta la pelota
	PintaPelota(
		(tipo_pelota*)(&(p_arkanoPi->pelota)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));

	PintaPelota2(
		(tipo_pelota2*)(&(p_arkanoPi->pelota2)),
		(tipo_pantalla*)(p_arkanoPi->p_pantalla));
	
	/*if (debug) {
		printf("\nDESPUES DE PintaPelota\n");
		fflush(stdout);
		PintaPantallaPorTerminal((tipo_pantalla*)(p_arkanoPi->p_pantalla));
	}*/
}

void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi, int debug) {
	ResetArkanoPi(p_arkanoPi);
	ActualizaPantalla(p_arkanoPi);

}

void ResetArkanoPi(tipo_arkanoPi *p_arkanoPi) {
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));
	InicializaLadrillos((tipo_pantalla*)(&(p_arkanoPi->ladrillos)));
	InicializaPelota((tipo_pelota*)(&(p_arkanoPi->pelota)));
	InicializaPelota2((tipo_pelota2*)(&(p_arkanoPi->pelota2)));
	InicializaPala((tipo_pala*)(&(p_arkanoPi->pala)));
}

void CambiarDireccionPelota(tipo_pelota *p_pelota, enum t_direccion direccion) {
	if((direccion < 0)||(direccion > p_pelota->num_posibles_trayectorias)) {
		printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
		return;
	}
	else {
		p_pelota->trayectoria.xv = p_pelota->posibles_trayectorias[direccion].xv;
		p_pelota->trayectoria.yv = p_pelota->posibles_trayectorias[direccion].yv;
	}
}

void CambiarDireccionPelota2(tipo_pelota2 *p_pelota2, enum t_direccion direccion) {
	if((direccion < 0)||(direccion > p_pelota2->num_posibles_trayectorias)) {
		printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
		return;
	}
	else {
		p_pelota2->trayectoria.xv = p_pelota2->posibles_trayectorias[direccion].xv;
		p_pelota2->trayectoria.yv = p_pelota2->posibles_trayectorias[direccion].yv;
	}
}

void ActualizaPosicionPala(tipo_pala *p_pala, enum t_direccion direccion) {
	switch (direccion) {
		case DERECHA:
			// Dejamos que la pala rebase parcialmente el límite del area de juego
			if( p_pala->x + 1 + p_pala->ancho <= NUM_COLUMNAS_DISPLAY + 2 )
				p_pala->x = p_pala->x + 1;
			break;
		case IZQUIERDA:
			// Dejamos que la pala rebase parcialmente el límite del area de juego
			if( p_pala->x - 1 >= -2)
					p_pala->x = p_pala->x - 1;
			break;
		default:
			printf("[ERROR!!!!][direccion NO VALIDA!!!!][%d]", direccion);
			break;
	}
}

void ActualizaPosicionPelota (tipo_pelota *p_pelota) {
	p_pelota->x += p_pelota->trayectoria.xv;
	p_pelota->y += p_pelota->trayectoria.yv;
}

void ActualizaPosicionPelota2 (tipo_pelota2 *p_pelota2) {
	p_pelota2->x += p_pelota2->trayectoria.xv;
	p_pelota2->y += p_pelota2->trayectoria.yv;
}

int CompruebaReboteLadrillo (tipo_arkanoPi *p_arkanoPi) {
	int p_posible_ladrillo_x = 0;
	int p_posible_ladrillo_y = 0;

	p_posible_ladrillo_x = p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv;

	if ( ( p_posible_ladrillo_x < 0) || ( p_posible_ladrillo_x >= NUM_COLUMNAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_x = %d!!!\n\n", p_posible_ladrillo_x);
		fflush(stdout);
		exit(-1);
	}

	p_posible_ladrillo_y = p_arkanoPi->pelota.y + p_arkanoPi->pelota.trayectoria.yv;

	if ( ( p_posible_ladrillo_y < 0) || ( p_posible_ladrillo_y >= NUM_FILAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_y = %d!!!\n\n", p_posible_ladrillo_y);
		fflush(stdout);
	}

	if(p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] > 0 ) {
		// La pelota ha entrado en el area de ladrillos
		// y descontamos el numero de golpes que resta para destruir el ladrillo
		p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] = p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] - 1;

		softToneWrite(GPIO_TONO, 500);

		return 1;
	}
	return 0;
}

int CompruebaReboteParedesVerticales (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= NUM_COLUMNAS_DISPLAY) ||
		(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < 0) ) {
		// La pelota rebota contra la pared derecha o izquierda
		softToneWrite(GPIO_TONO, 1000);

		return 1;
	}
	return 0;
}

int CompruebaReboteTecho (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < 0) {
		// La pelota rebota contra la pared derecha o izquierda
		softToneWrite(GPIO_TONO, 1000);

		return 1;
	}
	return 0;
}

int CompruebaRebotePala (tipo_arkanoPi arkanoPi) {
	if(arkanoPi.pelota.trayectoria.yv > 0) { // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv >= arkanoPi.pala.x ) &&
			(arkanoPi.pelota.x + arkanoPi.pelota.trayectoria.xv < arkanoPi.pala.x + NUM_COLUMNAS_PALA)) {
				if ((arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= arkanoPi.pala.y) &&
					(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv < arkanoPi.pala.y + NUM_FILAS_PALA)) {
					softToneWrite(GPIO_TONO, 1000);

					return 1;
				}
		}
	}
	return 0;
}

int CompruebaFallo (tipo_arkanoPi arkanoPi) {
	// Comprobamos si no hemos conseguido devolver la pelota
	if(arkanoPi.pelota.y + arkanoPi.pelota.trayectoria.yv >= NUM_FILAS_DISPLAY) {
		// Hemos fallado
		return 1;
	}
	return 0;
}

//COMPRUEBAS PARA LA SEGUNDA PELOTA

int CompruebaReboteLadrillo2 (tipo_arkanoPi *p_arkanoPi) {
	int p_posible_ladrillo_x = 0;
	int p_posible_ladrillo_y = 0;

	p_posible_ladrillo_x = p_arkanoPi->pelota2.x + p_arkanoPi->pelota2.trayectoria.xv;

	if ( ( p_posible_ladrillo_x < 0) || ( p_posible_ladrillo_x >= NUM_COLUMNAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_x = %d!!!\n\n", p_posible_ladrillo_x);
		fflush(stdout);
		exit(-1);
	}

	p_posible_ladrillo_y = p_arkanoPi->pelota2.y + p_arkanoPi->pelota2.trayectoria.yv;

	if ( ( p_posible_ladrillo_y < 0) || ( p_posible_ladrillo_y >= NUM_FILAS_DISPLAY ) ) {
		printf("\n\nERROR GRAVE!!! p_posible_ladrillo_y = %d!!!\n\n", p_posible_ladrillo_y);
		fflush(stdout);
	}

	if(p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] > 0 ) {
		// La pelota ha entrado en el area de ladrillos
		// y descontamos el numero de golpes que resta para destruir el ladrillo
		p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] = p_arkanoPi->ladrillos.matriz[p_posible_ladrillo_y][p_posible_ladrillo_x] - 1;

		softToneWrite(GPIO_TONO, 500);

		return 1;
	}
	return 0;
}

int CompruebaReboteParedesVerticales2 (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if((arkanoPi.pelota2.x + arkanoPi.pelota2.trayectoria.xv >= NUM_COLUMNAS_DISPLAY) ||
		(arkanoPi.pelota2.x + arkanoPi.pelota2.trayectoria.xv < 0) ) {
		// La pelota rebota contra la pared derecha o izquierda
		softToneWrite(GPIO_TONO, 1000);


		return 1;
	}
	return 0;
}

int CompruebaReboteTecho2 (tipo_arkanoPi arkanoPi) {
	// Comprobamos si la nueva posicion de la pelota excede los limites de la pantalla
	if(arkanoPi.pelota2.y + arkanoPi.pelota2.trayectoria.yv < 0) {
		// La pelota rebota contra la pared derecha o izquierda
		softToneWrite(GPIO_TONO, 1000);

		return 1;
	}
	return 0;
}

int CompruebaRebotePala2 (tipo_arkanoPi arkanoPi) {
	if(arkanoPi.pelota2.trayectoria.yv > 0) { // Esta condicion solo tiene sentido si la pelota va hacia abajo en la pantalla
		if ((arkanoPi.pelota2.x + arkanoPi.pelota2.trayectoria.xv >= arkanoPi.pala.x ) &&
			(arkanoPi.pelota2.x + arkanoPi.pelota2.trayectoria.xv < arkanoPi.pala.x + NUM_COLUMNAS_PALA)) {
				if ((arkanoPi.pelota2.y + arkanoPi.pelota2.trayectoria.yv >= arkanoPi.pala.y) &&
					(arkanoPi.pelota2.y + arkanoPi.pelota2.trayectoria.yv < arkanoPi.pala.y + NUM_FILAS_PALA)) {
					softToneWrite(GPIO_TONO, 1000);
					return 1;
				}
		}
	}
	return 0;
}

int CompruebaFallo2 (tipo_arkanoPi arkanoPi) {
	// Comprobamos si no hemos conseguido devolver la pelota
	if(arkanoPi.pelota2.y + arkanoPi.pelota2.trayectoria.yv >= NUM_FILAS_DISPLAY) {
		// Hemos fallado
		return 1;
	}
	return 0;
}

int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos) {
	int i=0, j=0;
	int numLadrillosRestantes;

	numLadrillosRestantes = 0;

	for(i=0;i<NUM_FILAS_DISPLAY;i++) {
		for(j=0;j<NUM_COLUMNAS_DISPLAY;j++) {
			if(p_ladrillos->matriz[i][j] != 0) {
				numLadrillosRestantes++;
			}
		}
	}

	return numLadrillosRestantes;
}

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaBotonPulsado (fsm_t* this) {
	int result = 0;

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaMovimientoIzquierda(fsm_t* this) {
	int result = 0;

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_IZQUIERDA);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaMovimientoDerecha(fsm_t* this) {
	int result = 0;

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_MOV_DERECHA);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTimeoutActualizacionJuego (fsm_t* this) {
	int result = 0;

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego(fsm_t* this) {
	int result = 0;

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_FIN_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int boton_pulsado(fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

int timeout(fsm_t* this) {
	int result = 0;

	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}
//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

// void InicializaJuego (void): funcion encargada de llevar a cabo
// la oportuna inicialización de toda variable o estructura de datos
// que resulte necesaria para el desarrollo del juego.

void InicializaJuego(fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	/*TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);*/

	/*piLock (STD_IO_BUFFER_KEY);
	printf("\n ****************\n ***BIENVENIDO*** \n **************** \n" );
	printf("\n -Izquierda: tecla A \n -Derecha:   tecla D\n -Salir:     tecla F\n");
	piUnlock (STD_IO_BUFFER_KEY);
	*/

	// A completar por el alumno
	piLock (SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_BOTON;
	piUnlock (SYSTEM_FLAGS_KEY);

	InicializaArkanoPi(p_arkanoPi,0);
	//PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, TIMEOUT);

	//InicializaTeclado(p_teclado);

	pseudoWiringPiEnableDisplay(1);
}

// void MuevePalaIzquierda (void): funcion encargada de ejecutar
// el movimiento hacia la izquierda contemplado para la pala.
// Debe garantizar la viabilidad del mismo mediante la comprobación
// de que la nueva posición correspondiente a la pala no suponga
// que ésta rebase o exceda los límites definidos para el área de juego
// (i.e. al menos uno de los leds que componen la raqueta debe permanecer
// visible durante todo el transcurso de la partida).

void MuevePalaIzquierda (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_MOV_IZQUIERDA;
	piUnlock (SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_arkanoPi->pala), IZQUIERDA);

	/*piLock(KEYBOARD_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock(STD_IO_BUFFER_KEY);

	piLock(STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY);*/
}

// void MuevePalaDerecha (void): función similar a la anterior
// encargada del movimiento hacia la derecha.

void MuevePalaDerecha (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_MOV_DERECHA;
	piUnlock (SYSTEM_FLAGS_KEY);

	ActualizaPosicionPala(&(p_arkanoPi->pala), DERECHA);

	/*piLock(KEYBOARD_KEY);
	ActualizaPantalla(p_arkanoPi);
	piUnlock(STD_IO_BUFFER_KEY);

	piLock(STD_IO_BUFFER_KEY);
	PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock(STD_IO_BUFFER_KEY);*/
}

// void ActualizarJuego (void): función encargada de actualizar la
// posición de la pelota conforme a la trayectoria definida para ésta.
// Para ello deberá identificar los posibles rebotes de la pelota para,
// en ese caso, modificar su correspondiente trayectoria (los rebotes
// detectados contra alguno de los ladrillos implicarán adicionalmente
// la eliminación del ladrillo). Del mismo modo, deberá también
// identificar las situaciones en las que se dé por finalizada la partida:
// bien porque el jugador no consiga devolver la pelota, y por tanto ésta
// rebase el límite inferior del área de juego, bien porque se agoten
// los ladrillos visibles en el área de juego.

void ActualizarJuego (fsm_t* this) {
	tipo_arkanoPi* p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	if(CompruebaReboteParedesVerticales(*p_arkanoPi)) {
			p_arkanoPi->pelota.trayectoria.xv = -(p_arkanoPi->pelota.trayectoria.xv);}

	else if(CompruebaReboteTecho (*p_arkanoPi)) {
			p_arkanoPi->pelota.trayectoria.yv = -(p_arkanoPi->pelota.trayectoria.yv);
	        p_arkanoPi->pelota.trayectoria.xv = -(p_arkanoPi->pelota.trayectoria.xv);}
	else if(CompruebaFallo (*p_arkanoPi)) {
		piLock (SYSTEM_FLAGS_KEY);
		flags |= FLAG_FIN_JUEGO;
		piUnlock (SYSTEM_FLAGS_KEY);
		return;
	}else if (CompruebaRebotePala (*p_arkanoPi)) {
		switch(p_arkanoPi->pelota.x + p_arkanoPi->pelota.trayectoria.xv - p_arkanoPi->pala.x) {
		case 0:
			CambiarDireccionPelota(&(p_arkanoPi->pelota),ARRIBA_IZQUIERDA);
			break;
		case 1:
			CambiarDireccionPelota(&(p_arkanoPi->pelota), ARRIBA);
			break;
		case 2:
			CambiarDireccionPelota(&(p_arkanoPi->pelota), ARRIBA_DERECHA);
			break;
		}
	}
	else if (CompruebaReboteLadrillo(p_arkanoPi)) {
		p_arkanoPi->pelota.trayectoria.yv = -(p_arkanoPi->pelota.trayectoria.yv);

	if(CalculaLadrillosRestantes(&(p_arkanoPi->ladrillos))<= 0) {
		printf("Has ganado !!");

		piLock(STD_IO_BUFFER_KEY);
		ActualizaPantalla (p_arkanoPi);
		piUnlock(STD_IO_BUFFER_KEY);

		piLock (STD_IO_BUFFER_KEY);
		//PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
		piUnlock (STD_IO_BUFFER_KEY);

		p_arkanoPi->tmr_actualizacion_juego = tmr_new(tmr_actualizacion_juego_isr);
		tmr_startms(p_arkanoPi->tmr_actualizacion_juego, TIMEOUT);

		piLock (SYSTEM_FLAGS_KEY);
		flags |= FLAG_FIN_JUEGO;
		piUnlock (SYSTEM_FLAGS_KEY);

		return;
	}
	}
	/*
	 * Aquí comienza para la segunda pelota
	 */
	if(CompruebaReboteParedesVerticales2(*p_arkanoPi)) {
			p_arkanoPi->pelota2.trayectoria.xv = -(p_arkanoPi->pelota2.trayectoria.xv);}

	else if(CompruebaReboteTecho2 (*p_arkanoPi)) {
			p_arkanoPi->pelota2.trayectoria.yv = -(p_arkanoPi->pelota2.trayectoria.yv);
	        p_arkanoPi->pelota2.trayectoria.xv = -(p_arkanoPi->pelota2.trayectoria.xv);}
	else if(CompruebaFallo2 (*p_arkanoPi)) {
		piLock (SYSTEM_FLAGS_KEY);
		flags |= FLAG_FIN_JUEGO;
		piUnlock (SYSTEM_FLAGS_KEY);
		return;
	}else if (CompruebaRebotePala2 (*p_arkanoPi)) {
		switch(p_arkanoPi->pelota2.x + p_arkanoPi->pelota2.trayectoria.xv - p_arkanoPi->pala.x) {
		case 0:
			CambiarDireccionPelota2(&(p_arkanoPi->pelota2),ARRIBA_IZQUIERDA);
			break;
		case 1:
			CambiarDireccionPelota2(&(p_arkanoPi->pelota2), ARRIBA);
			break;
		case 2:
			CambiarDireccionPelota2(&(p_arkanoPi->pelota2), ARRIBA_DERECHA);
			break;
		}
	}
	else if (CompruebaReboteLadrillo2(p_arkanoPi)) {
		p_arkanoPi->pelota2.trayectoria.yv = -(p_arkanoPi->pelota2.trayectoria.yv);
	}

	ActualizaPosicionPelota (&(p_arkanoPi->pelota));
	ActualizaPosicionPelota2 (&(p_arkanoPi->pelota2));

	piLock(STD_IO_BUFFER_KEY);
	ActualizaPantalla (p_arkanoPi);
	piUnlock(STD_IO_BUFFER_KEY);

	piLock (STD_IO_BUFFER_KEY);
	//PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

	//p_arkanoPi->tmr_actualizacion_juego = tmr_new(tmr_actualizacion_juego_isr);
	tmr_startms(p_arkanoPi->tmr_actualizacion_juego, TIMEOUT);
}

// void FinalJuego (void): función encargada de mostrar en la ventana de
// terminal los mensajes necesarios para informar acerca del resultado del juego.
void FinalJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_FIN_JUEGO;
	piUnlock (SYSTEM_FLAGS_KEY);

    pseudoWiringPiEnableDisplay(0);
	ReseteaPantalla((tipo_pantalla*)(p_arkanoPi->p_pantalla));

	piLock (STD_IO_BUFFER_KEY);
	//PintaPantallaPorTerminal(p_arkanoPi->p_pantalla);
	piUnlock (STD_IO_BUFFER_KEY);

	piLock (STD_IO_BUFFER_KEY);
	printf("\n\nEl juego ha acabado");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	piLock (STD_IO_BUFFER_KEY);
	if(vidas >0){
		vidas--;
		printf("\n\nTe quedan %d vidas \n\nPulse A o D para continuar \n", vidas);
		flags |= FLAG_BOTON;
	}else{
		printf("\n\nNo te quedan vidas \n\nPulse F para salir \n");
		vidas=MAX_VIDAS;
	}
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);


}

//void ReseteaJuego (void): función encargada de llevar a cabo la
// reinicialización de cuantas variables o estructuras resulten
// necesarias para dar comienzo a una nueva partida.

void ReseteaJuego (fsm_t* this) {
	tipo_arkanoPi *p_arkanoPi;
	p_arkanoPi = (tipo_arkanoPi*)(this->user_data);

	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	flags &= ~FLAG_BOTON;
	piUnlock (SYSTEM_FLAGS_KEY);
}

void encender (fsm_t* this) {

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_BOTON);
	piUnlock (SYSTEM_FLAGS_KEY);

	//digitalWrite (GPIO_LIGHT, 1);
	tmr_startms((tmr_t*)(this->user_data), TIMEOUT);
}

void apagar (fsm_t* this) {

	piLock (SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_TIMER_JUEGO);
	piUnlock (SYSTEM_FLAGS_KEY);

	//digitalWrite (GPIO_LIGHT, 0);
}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void tmr_actualizacion_juego_isr(union sigval value) {
	// A completar por el alumno

	piLock (SYSTEM_FLAGS_KEY);
	flags |= FLAG_TIMER_JUEGO;
	piUnlock (SYSTEM_FLAGS_KEY);
}


