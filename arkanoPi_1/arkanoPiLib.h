
#ifndef _ARKANOPILIB_H_
#define _ARKANOPILIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ledDisplay.h"
#include "softTone.h"

enum t_direccion {
	ARRIBA_IZQUIERDA,
	ARRIBA,
	ARRIBA_DERECHA,
	ABAJO_DERECHA,
	ABAJO,
	ABAJO_IZQUIERDA,
	IZQUIERDA, // NO PERMITIDA
	DERECHA, // NO PERMITIDA
};

// CONSTANTES DEL JUEGO
#define NUM_COLUMNAS_PALA 	3
#define NUM_FILAS_PALA 		1
#define MAX_NUM_TRAYECTORIAS 8
#define MAX_VIDAS 5

typedef struct {
  int ancho;
  int alto;
  int x;
  int y;
} tipo_pala;

typedef struct {
  int xv;
  int yv;
} tipo_trayectoria;

typedef struct {
  tipo_trayectoria posibles_trayectorias[MAX_NUM_TRAYECTORIAS];
  int num_posibles_trayectorias;
  tipo_trayectoria trayectoria;
  int x;
  int y;
} tipo_pelota;

typedef struct {
  tipo_trayectoria posibles_trayectorias[MAX_NUM_TRAYECTORIAS];
  int num_posibles_trayectorias;
  tipo_trayectoria trayectoria;
  int x;
  int y;
} tipo_pelota2;

typedef struct {
	tipo_pantalla *p_pantalla; // Esta es nuestra pantalla de juego (matriz 10x7 de labo)
	tipo_pantalla ladrillos;
	tipo_pala pala;
	tipo_pelota pelota;
	tipo_pelota2 pelota2;
	tmr_t* tmr_actualizacion_juego;

} tipo_arkanoPi;

void MuestraMensajeInicial();
//------------------------------------------------------------------------
// FUNCIONES DE INICIALIZACION / RESET  DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------------------------

void InicializaLadrillos(tipo_pantalla *p_ladrillos);
void InicializaPelota(tipo_pelota *p_pelota);
void InicializaPelota2(tipo_pelota *p_pelota2);
void InicializaPala(tipo_pala *p_pala);
void InicializaPosiblesTrayectorias(tipo_pelota *p_pelota);
void InicializaPosiblesTrayectorias2(tipo_pelota *p_pelota2);
void InicializaArkanoPi(tipo_arkanoPi *p_arkanoPi, int debug);
void ResetArkanoPi(tipo_arkanoPi *p_arkanoPi);
void ReseteaMatriz(tipo_pantalla *p_pantalla);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA GESTION DEL JUEGO
//------------------------------------------------------
void CambiarDireccionPelota(tipo_pelota *p_pelota, enum t_direccion direccion);
void ActualizaPosicionPala(tipo_pala *p_pala, enum t_direccion direccion);
void ActualizaPosicionPelota (tipo_pelota *p_pelota);
int CompruebaReboteLadrillo (tipo_arkanoPi *p_arkanoPi);
int CompruebaReboteParedesVerticales (tipo_arkanoPi arkanoPi);
int CompruebaReboteTecho (tipo_arkanoPi arkanoPi);
int CompruebaRebotePala (tipo_arkanoPi arkanoPi);
int CompruebaFallo (tipo_arkanoPi arkanoPi);
int CompruebaReboteLadrillo2 (tipo_arkanoPi *p_arkanoPi);
int CompruebaReboteParedesVerticales2 (tipo_arkanoPi arkanoPi);
int CompruebaReboteTecho2 (tipo_arkanoPi arkanoPi);
int CompruebaRebotePala2 (tipo_arkanoPi arkanoPi);
int CompruebaFallo2 (tipo_arkanoPi arkanoPi);
int CalculaLadrillosRestantes(tipo_pantalla *p_ladrillos);

//------------------------------------------------------
// PROCEDIMIENTOS PARA LA VISUALIZACION DEL JUEGO
//------------------------------------------------------
void PintaMensajeInicialPantalla (tipo_pantalla *p_pantalla, tipo_pantalla *p_pantalla_inicial);
void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla);
void PintaLadrillos(tipo_pantalla *p_ladrillos, tipo_pantalla *p_pantalla);
void PintaPala(tipo_pala *p_pala, tipo_pantalla *p_pantalla);
void PintaPelota(tipo_pelota *p_pelota, tipo_pantalla *p_pantalla);
void PintaPelota2(tipo_pelota *p_pelota2, tipo_pantalla *p_pantalla);
void ActualizaPantalla(tipo_arkanoPi* p_arkanoPi);

//------------------------------------------------------
// FUNCIONES DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
int CompruebaBotonPulsado (fsm_t* this);
int CompruebaMovimientoArriba (fsm_t* this);
int CompruebaMovimientoAbajo (fsm_t* this);
int CompruebaMovimientoIzquierda (fsm_t* this);
int CompruebaMovimientoDerecha (fsm_t* this);
int CompruebaTimeoutActualizacionJuego (fsm_t* this);
int CompruebaFinalJuego (fsm_t* this);

//------------------------------------------------------
// FUNCIONES DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------
void InicializaJuego (fsm_t* this);
void MuevePalaIzquierda (fsm_t* this);
void MuevePalaDerecha (fsm_t* this);
void ActualizarJuego (fsm_t* this);
void FinalJuego (fsm_t* this);
void ReseteaJuego (fsm_t* this);

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------
void tmr_actualizacion_juego_isr(union sigval value);

#endif /* _ARKANOPILIB_H_ */
