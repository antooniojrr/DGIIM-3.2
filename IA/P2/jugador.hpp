#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>


struct stateN0{
	ubicacion jugador;
	ubicacion colaborador;
	Action ultimaOrdenColaborador;

	bool operator==(const stateN0 &x) const{
		return (jugador == x.jugador && colaborador.c == x.colaborador.c && colaborador.f == x.colaborador.f);
	}
};

struct nodeN0{
  stateN0 st;
  list<Action> secuencia;

  bool operator==(const nodeN0 &n) const {
    return (st == n.st);
  }

  bool operator<(const nodeN0 &b) const {
    if(st.jugador.f < b.st.jugador.f) 
      return true;
    
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c)
      return true;
    
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula)
      return true;
    
    else
      return false;
  }
};

struct stateN1{
	ubicacion jugador;
	ubicacion colaborador;
	Action ultimaOrdenColaborador;

	bool operator==(const stateN1 &x) const{
		return (jugador == x.jugador && colaborador == x.colaborador && ultimaOrdenColaborador == x.ultimaOrdenColaborador);
	}

  stateN1& operator=(const stateN0 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    return *this;
  }

  stateN1(stateN0 &x){
    *this=x;
  }

  stateN1(){}
};

struct nodeN1{
  stateN1 st;
  list<Action> secuencia;

  bool operator==(const nodeN1 &n) const {
    return (st == n.st);
  }

  bool operator<(const nodeN1 &b) const {
    if(st.jugador.f < b.st.jugador.f) 
      return true;
    
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c)
      return true;
    
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula)
      return true;
    
    else if(st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula == b.st.jugador.brujula) {
      if(st.colaborador.f < b.st.colaborador.f)
        return true;
      else if(st.colaborador.f == b.st.colaborador.f && st.colaborador.c < b.st.colaborador.c)
        return true;
      else if(st.colaborador.f == b.st.colaborador.f && st.colaborador.c == b.st.colaborador.c && st.colaborador.brujula < b.st.colaborador.brujula )
        return true;
      else 
        return false;
    }
    else
      return false;
  }
};

struct stateN2{
	ubicacion jugador;
	ubicacion colaborador;
	Action ultimaOrdenColaborador;
  int bateria;
  bool bikini;
  bool zapatillas;

  static const unsigned int GASTO_W_AGUA = 100;
  static const unsigned int GASTO_W_BOSQUE = 50;
  static const unsigned int GASTO_W_AGUA_R = 10;
  static const unsigned int GASTO_W_BOSQUE_R = 15;
  static const unsigned int GASTO_W_ARENOSO = 2;
  static const unsigned int GASTO_R_AGUA = 150;
  static const unsigned int GASTO_R_BOSQUE = 75;
  static const unsigned int GASTO_R_AGUA_R = 15;
  static const unsigned int GASTO_R_BOSQUE_R = 25;
  static const unsigned int GASTO_R_ARENOSO = 3;
  static const unsigned int GASTO_L_AGUA = 30;
  static const unsigned int GASTO_L_BOSQUE = 7;
  static const unsigned int GASTO_L_AGUA_R = 5;
  static const unsigned int GASTO_L_BOSQUE_R = 1;
  static const unsigned int GASTO_L_ARENOSO = 2;
  static const unsigned int GASTO_sR_AGUA = 10;
  static const unsigned int GASTO_sR_BOSQUE = 5;
  static const unsigned int GASTO_sR_AGUA_R = 2;
  static const unsigned int GASTO_sR_BOSQUE_R = 1;
  static const unsigned int GASTO_sR_ARENOSO = 1;
  static const unsigned int GASTO_DEFAULT = 1;

	bool operator==(const stateN2 &x) const{
		return (jugador == x.jugador && colaborador.c == x.colaborador.c && colaborador.f == x.colaborador.f && bateria == x.bateria && bikini == x.bikini && zapatillas == x.zapatillas);
	}

  stateN2(){
    bateria = 3000;
    bikini = false;
    zapatillas = false;
  }

  stateN2& operator=(const stateN0 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    return *this;
  }

  stateN2& operator=(const stateN2 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    bateria = x.bateria;
    bikini = x.bikini;
    zapatillas = x.zapatillas;

    return *this;
  }

  stateN2(const stateN0 &x) {
    *this = x;
    bateria = 3000;
    bikini = false;
    zapatillas = false;
  }
  // Revisar zapatillas y bikini
  bool operator<(const stateN2 &st) const {
    
    if(jugador.f < st.jugador.f) 
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c < st.jugador.c)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula < st.jugador.brujula)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula) {
      if(!bikini && st.bikini)
        return true;
      else if( bikini == st.bikini && !zapatillas && st.zapatillas)
        return true;
      else 
        return false;      
    }
      
    else
      return false;
  }
};

struct nodeN2{
  stateN2 st;
  list<Action> secuencia;

  bool operator==(const nodeN2 &n) const {
    return (st == n.st);
  }

  nodeN2& operator=(const nodeN2 &n) {
    st = n.st;
    secuencia = n.secuencia;
    return *this;
  }

  bool operator<(const nodeN2 &b) const {

    if(st.bateria < b.st.bateria)
      return true;
    else if(st.bateria == b.st.bateria && !st.bikini && b.st.bikini)
      return true;
    else if(st.bateria == b.st.bateria && st.bikini == b.st.bikini && !st.zapatillas && b.st.zapatillas)
      return true;
    /*else if(st.bateria == b.st.bateria && st.jugador.f < b.st.jugador.f) 
      return true;
    
    else if(st.bateria == b.st.bateria && st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c)
      return true;
    
    else if(st.bateria == b.st.bateria && st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula)
      return true;*/
    
    else
      return false;
  }

};



struct stateN3{
  ubicacion jugador;
	ubicacion colaborador;
	Action ultimaOrdenColaborador;
  int bateria;
  bool bikini_j;
  bool zapatillas_j;
  bool bikini_c;
  bool zapatillas_c;

  stateN3(){
    bateria = 3000;
    bikini_j = false;
    zapatillas_j = false;
    bikini_c = false;
    zapatillas_c = false;
    ultimaOrdenColaborador = act_CLB_STOP;
  }

  stateN3& operator=(const stateN0 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    return *this;
  }

  /*stateN3& operator=(const stateN2 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    bikini_j = x.bikini;
    zapatillas_j = x.zapatillas;
    return *this;
  }

  stateN3& operator=(const stateN1 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    return *this;
  }*/

  /*stateN3& operator=(const stateN3 &x) {
    jugador = x.jugador;
    colaborador = x.colaborador;
    ultimaOrdenColaborador = x.ultimaOrdenColaborador;
    bikini_c = x.bikini_c;
    bikini_j = x.bikini_j;
    zapatillas_c = x.zapatillas_c;
    zapatillas_j = x.zapatillas_j;
    bateria = x.bateria;
  }*/

  stateN3(const stateN0 &x) {
    *this = x;
    bateria = 3000;
    bikini_j = false;
    zapatillas_j = false;
    bikini_c = false;
    zapatillas_c = false;

  }


  bool operator<(const stateN3 &st) const {
    if(jugador.f < st.jugador.f) 
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c < st.jugador.c)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula < st.jugador.brujula)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && colaborador.f < st.colaborador.f)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && colaborador.f == st.colaborador.f && colaborador.c < st.colaborador.c)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && colaborador.f == st.colaborador.f && colaborador.c == st.colaborador.c && colaborador.brujula < st.colaborador.brujula)
      return true;
    
    else if(jugador.f == st.jugador.f && jugador.c == st.jugador.c && jugador.brujula == st.jugador.brujula && colaborador.f == st.colaborador.f && colaborador.c == st.colaborador.c && colaborador.brujula == st.colaborador.brujula) {
      if( bikini_c < st.bikini_c)
        return true;
      else if(bikini_c == st.bikini_c && zapatillas_c < st.zapatillas_c)
        return true;
      else if(bikini_c == st.bikini_c && zapatillas_c == st.zapatillas_c && bikini_j < st.bikini_j)
        return true;
      else if(bikini_c == st.bikini_c && zapatillas_c == st.zapatillas_c && bikini_j == st.bikini_j && zapatillas_j < st.zapatillas_j)
        return true;
      else
        return false;
    }
    else
      return false;
  }

  bool operator==(const stateN3 &x) const{
		return (jugador == x.jugador && colaborador == x.colaborador && bikini_j == x.bikini_j && zapatillas_j == x.zapatillas_j && zapatillas_c == x.zapatillas_c && bikini_c == x.bikini_c);
	}

};

struct nodeN3 {
  stateN3 st;
  int estimacion_gasto;
  list<Action> secuencia;

  bool operator==(const nodeN3 &n) const {
    return (st == n.st);
  }

  nodeN3& operator=(const nodeN3 &n) {
    st = n.st;
    secuencia = n.secuencia;
    return *this;
  }

  bool operator<(const nodeN3 &b) const {
    if(st.bateria - estimacion_gasto < b.st.bateria - b.estimacion_gasto)
      return true;
    
    /*else if(st.bateria - estimacion_gasto == b.st.bateria - b.estimacion_gasto) {
      if( !st.bikini_c && b.st.bikini_c) 
        return true;
      else if( st.bikini_c == b.st.bikini_c && !st.zapatillas_c && b.st.zapatillas_c)
        return true;
      else if( st.bikini_c == b.st.bikini_c && st.zapatillas_c == b.st.zapatillas_c && !st.bikini_j && b.st.bikini_j )
        return true;
      else if( st.bikini_c == b.st.bikini_c && st.zapatillas_c == b.st.zapatillas_c && st.bikini_j == b.st.bikini_j && !st.zapatillas_j && b.st.zapatillas_j)
        return true;

      else return false;
      
    }*/
    /*else if(st.bateria == b.st.bateria && st.jugador.f < b.st.jugador.f) 
      return true;
    
    else if(st.bateria == b.st.bateria && st.jugador.f == b.st.jugador.f && st.jugador.c < b.st.jugador.c)
      return true;
    
    else if(st.bateria == b.st.bateria && st.jugador.f == b.st.jugador.f && st.jugador.c == b.st.jugador.c && st.jugador.brujula < b.st.jugador.brujula)
      return true;*/
    else
      return false;
  }
};

class ComportamientoJugador : public Comportamiento {
  public:
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
      hayPlan = false;
    }
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      // Inicializar Variables de Estado
      hayPlan = false;
    }
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){}
    ~ComportamientoJugador(){}

    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);

    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:
    // Declarar Variables de Estado
    list<Action> plan;
    bool hayPlan;
    stateN0 c_state;
    ubicacion goal;
    

};

#endif
