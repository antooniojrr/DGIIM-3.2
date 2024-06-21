#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"
using namespace std;

struct state{
  int fil;
  int col;
  Orientacion brujula;
};

struct casilla{
  int fil;
  int col;
};

class ComportamientoJugador : public Comportamiento{

  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size){
      // Constructor de la clase
      // Dar el valor inicial a las variables de estado
      last_action = actIDLE;
      current_state.brujula = norte;
      current_state.fil = 99;
      current_state.col = 99;
      girar_derecha = false;
      girar_izquierda = false;
      seguir_recto = false;
      bien_situado = false;
      bikini = false;
      zapatillas = false;
      predet_action = actIDLE;
      atrapado = false;
      turnos_escape = 0;
      posXcol = posXfil = -1;
      hay_predet = false;
      turnos_quieto = 0;
      casilla_obj.brujula = norte;
      casilla_obj.col = casilla_obj.fil = -1;
      bordes = false;
      for ( int i = 0; i < 16; i++)
        nuevos[i] = false;
      
      for( int i = 0; i < TAM_VISITADAS; i++) {
        visitadas[i].fil = -1;
        visitadas[i].col = -1;
      }
      visitadas_iter = 0;
    }

    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    Action think(Sensores sensores);
    int interact(Action accion, int valor);

  private:
  // Declarar aquí las variables de estado
    Action last_action;
    state current_state;
    bool girar_derecha;
    bool girar_izquierda;
    bool seguir_recto;
    bool bien_situado;
    bool bikini;
    bool zapatillas;
    Action predet_action;
    bool hay_predet;
    bool nuevos[16];
    bool atrapado;
    unsigned int turnos_escape;
    int posXcol;
    int posXfil;
    int turnos_quieto;
    state casilla_obj;
    bool bordes;
    static const int TAM_VISITADAS = 50;
    casilla visitadas[TAM_VISITADAS];
    unsigned int visitadas_iter;
};
#endif
