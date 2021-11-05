
#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
	{0,0,0,0,0,0,0},
	{0,1,1,0,1,0,0},
	{0,1,1,0,0,1,0},
	{0,0,0,0,0,1,0},
	{0,0,0,0,0,1,0},
	{0,1,1,0,0,1,0},
	{0,1,1,0,1,0,0},
	{0,0,0,0,0,0,0},
	}
};

tipo_pantalla pantalla_final = {
	.matriz = {
	{0,0,0,0,0,0,0},
	{0,0,1,0,0,1,0},
	{0,1,1,0,1,0,0},
	{0,0,0,0,1,0,0},
	{0,0,0,0,1,0,0},
	{0,1,1,0,1,0,0},
	{0,0,1,0,0,1,0},
	{0,0,0,0,0,0,0},
	}
};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	/*static int led_col[3]={GPIO_LED_DISPLAY_COL_1,GPIO_LED_DISPLAY_COL_2,GPIO_LED_DISPLAY_COL_3};   // array de columnas
	static int led_row[7]={GPIO_LED_DISPLAY_ROW_1,GPIO_LED_DISPLAY_ROW_2,GPIO_LED_DISPLAY_ROW_3,GPIO_LED_DISPLAY_ROW_4,GPIO_LED_DISPLAY_ROW_5,GPIO_LED_DISPLAY_ROW_6,GPIO_LED_DISPLAY_ROW_7};   // array de filas
	*/

	for (int i=0;i<3;i++){
		pinMode(led_display->pines_control_columnas[i],OUTPUT); //LOW, columna actual
		digitalWrite(led_display->pines_control_columnas[i], LOW); //NUEVO
	}
	for (int i=0;i<7;i++){
		pinMode(led_display->filas[i],OUTPUT);//apagafilas()
		ApagaFilas(led_display);
	}

	led_display->tmr_refresco_display=tmr_new(timer_refresco_display_isr);

	tmr_startms((tmr_t*)(led_display->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	// A completar por el alumno...
	//static int led_row[7]={GPIO_LED_DISPLAY_ROW_1,GPIO_LED_DISPLAY_ROW_2,GPIO_LED_DISPLAY_ROW_3,GPIO_LED_DISPLAY_ROW_4,GPIO_LED_DISPLAY_ROW_5,GPIO_LED_DISPLAY_ROW_6,GPIO_LED_DISPLAY_ROW_7};   // array de filas

	for(int i=0; i<7;i++) {
		digitalWrite(led_display->filas[i], LOW);
	}
}

void ExcitaColumnas(int columna) {

	switch(columna) {
		// A completar por el alumno...
		case 0:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
			break;
		case 1:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
			break;
		case 2:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
			break;
		case 3:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, LOW);
			break;
		case 4:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
		case 5:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
		case 6:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
		case 7:
			digitalWrite(GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite(GPIO_LED_DISPLAY_COL_3, HIGH);
			break;
	}
}

void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...

	ApagaFilas(led_display);



	ExcitaColumnas(led_display->p_columna);

	for (int i=0; i< NUM_FILAS_DISPLAY; i++){
		if (led_display->pantalla.matriz[i][led_display->p_columna] == 0) {
			digitalWrite(led_display->filas[i],HIGH);
		}
	}

}

void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla) {
	int i=0, j=0;

	printf("\n[PANTALLA]\n");
	fflush(stdout);
	for(j=0;j<NUM_FILAS_DISPLAY;j++) {
		for(i=0;i<NUM_COLUMNAS_DISPLAY;i++) {
			printf("%d", p_pantalla->matriz[j][i]);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
	}
	fflush(stdout);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	piLock(MATRIX_KEY);
	result=(p_ledDisplay->flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock(MATRIX_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);
	//int valor = LOW;

	// A completar por el alumno...
	piLock(MATRIX_KEY);
	p_ledDisplay->flags &= (~FLAG_TIMEOUT_COLUMNA_DISPLAY);
	p_ledDisplay->p_columna++;
	if(p_ledDisplay->p_columna > NUM_COLUMNAS_DISPLAY-1) {
		p_ledDisplay->p_columna = 0;
		}

	ActualizaLedDisplay(&led_display);
	piUnlock (MATRIX_KEY);

	tmr_startms((tmr_t*)(p_ledDisplay->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);


}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	// A completar por el alumno...
	piLock (MATRIX_KEY);
	led_display.flags |= FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock (MATRIX_KEY);
}
