#include "teclado_TL04.h"

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	/*static int gpio_col[4]={GPIO_KEYBOARD_COL_1,GPIO_KEYBOARD_COL_2,GPIO_KEYBOARD_COL_3,GPIO_KEYBOARD_COL_4};  // array de columnas
	static int gpio_row[4]={GPIO_KEYBOARD_ROW_1,GPIO_KEYBOARD_ROW_2,GPIO_KEYBOARD_ROW_3,GPIO_KEYBOARD_ROW_4};   // array de filas
	void *fila[4] = {teclado_fila_1_isr, teclado_fila_2_isr, teclado_fila_3_isr, teclado_fila_4_isr};*/
	int i=0;
	for (i=0; i<4; i++){
		pinMode(teclado.columnas[i], OUTPUT);
		digitalWrite(teclado.columnas[i], LOW);
	}
	int j=0;
	for (j=0; j<4; j++){
		pinMode(teclado.filas[j], INPUT);
		pullUpDnControl (teclado.filas[j], PUD_DOWN );  // todas las filas inicialmente a pull down
		wiringPiISR(teclado.filas[j], INT_EDGE_RISING, teclado.rutinas_ISR[j]); //array de rutinas de interrupción, //Filas del principio a completar con las del teclado
		// detectan flanco de subida
		p_teclado->debounceTime[j]=0;
	}

	softToneCreate(GPIO_TONO); //inicializamos el PIN de tono

	p_teclado->tmr_duracion_columna=tmr_new(timer_duracion_columna_isr);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);
    fflush(stdout);

}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno
	// ...
	switch(columna){
		case COLUMNA_1:
			//pinMode(GPIO_KEYBOARD_COL_1,OUTPUT);
			digitalWrite(GPIO_KEYBOARD_COL_1, HIGH);
			digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
			break;
		case COLUMNA_2:
			//pinMode(GPIO_KEYBOARD_COL_2,OUTPUT);
			digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_2, HIGH);
			digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
			break;
		case COLUMNA_3:
			//pinMode(GPIO_KEYBOARD_COL_3,OUTPUT);
			digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_3, HIGH);
			digitalWrite(GPIO_KEYBOARD_COL_4, LOW);
			break;
		case COLUMNA_4:
			//pinMode(GPIO_KEYBOARD_COL_4,OUTPUT);
			digitalWrite(GPIO_KEYBOARD_COL_1, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_2, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_3, LOW);
			digitalWrite(GPIO_KEYBOARD_COL_4, HIGH);
			break;
	}
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);


	// A completar por el alumno...
	// ...
	piLock(KEYBOARD_KEY);
	result=(p_teclado->flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock(KEYBOARD_KEY);

	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);


	// A completar por el alumno
	// ...
	piLock(KEYBOARD_KEY);
	result = (p_teclado->flags & FLAG_TECLA_PULSADA);
	piUnlock(KEYBOARD_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------
int fila, columna;
void TecladoExcitaColumna (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(KEYBOARD_KEY);
	p_teclado->flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);

	p_teclado->columna_actual = p_teclado->columna_actual + 1;

	if (p_teclado->columna_actual > COLUMNA_4) {
		p_teclado->columna_actual = COLUMNA_1;
	}

	ActualizaExcitacionTecladoGPIO(p_teclado->columna_actual);

	piUnlock (KEYBOARD_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(KEYBOARD_KEY);
	p_teclado->flags &= (~FLAG_TECLA_PULSADA);

	piLock (SYSTEM_FLAGS_KEY);
	flags |= FLAG_BOTON;  // para iniciar el juego con cualquier tecla
	piUnlock (SYSTEM_FLAGS_KEY);

	switch(p_teclado->teclaPulsada.col){
	case COLUMNA_1:
		if(p_teclado->teclaPulsada.row == FILA_4){
			//printf("\nKeypress A!!!\n");
			//fflush(stdout);
			piLock(SYSTEM_FLAGS_KEY);
			flags |= FLAG_MOV_IZQUIERDA;
			piUnlock(SYSTEM_FLAGS_KEY);
			p_teclado->teclaPulsada.row= -1;
			p_teclado->teclaPulsada.col= -1;

			softToneWrite(GPIO_TONO, 750);

		}break;

	case COLUMNA_3:
		if(p_teclado->teclaPulsada.row == FILA_4){
			printf("\nKeypress B!!!\n");
			//fflush(stdout);
			p_teclado->teclaPulsada.row= -1;
			p_teclado->teclaPulsada.col= -1;
		} break;
	case COLUMNA_4:
			if(p_teclado->teclaPulsada.row == FILA_2){
				//printf("\nKeypress D!!!\n");
				//fflush(stdout);
				piLock(SYSTEM_FLAGS_KEY);
				flags |= FLAG_MOV_DERECHA;
				piUnlock(SYSTEM_FLAGS_KEY);
				p_teclado->teclaPulsada.row= -1;
				p_teclado->teclaPulsada.col= -1;
				softToneWrite(GPIO_TONO, 750);

			}else if(p_teclado->teclaPulsada.row == FILA_4){
				printf("\n\nSaliste del juego!!\n\n");
				fflush(stdout);
				exit(0);
			}break;

	default:
		break;
	}
	piUnlock (KEYBOARD_KEY);
}


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void) {
	// A completar por el alumno


	if (millis() < (teclado.debounceTime[FILA_1])) {
		teclado.debounceTime[FILA_1] = (millis() + DEBOUNCE_TIME) ;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_1;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_1] = millis() + DEBOUNCE_TIME;

}

void teclado_fila_2_isr (void) {
	// A completar por el alumno

	if (millis() < (teclado.debounceTime[FILA_2])) {
		teclado.debounceTime[FILA_2] = (millis() + DEBOUNCE_TIME) ;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_2;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_2] = millis() + DEBOUNCE_TIME;
}

void teclado_fila_3_isr (void) {
	// A completar por el alumno

	if (millis() < (teclado.debounceTime[FILA_3])) {
		teclado.debounceTime[FILA_3] = (millis() + DEBOUNCE_TIME) ;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_3;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_3] = millis() + DEBOUNCE_TIME;
}

void teclado_fila_4_isr (void) {
	// A completar por el alumno

	if (millis() < (teclado.debounceTime[FILA_4])) {
		teclado.debounceTime[FILA_4] = (millis() + DEBOUNCE_TIME) ;
		return;
	}

	piLock(KEYBOARD_KEY);
	teclado.teclaPulsada.row = FILA_4;
	teclado.teclaPulsada.col = teclado.columna_actual;

	teclado.flags |= FLAG_TECLA_PULSADA;
	piUnlock (KEYBOARD_KEY);

	teclado.debounceTime[FILA_4] = millis() + DEBOUNCE_TIME;
}

void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	piLock (KEYBOARD_KEY);
	teclado.flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock (KEYBOARD_KEY);
}
