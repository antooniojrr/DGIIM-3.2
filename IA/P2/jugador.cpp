#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <cmath>



const unsigned int GASTO_W_AGUA = 100;
const unsigned int GASTO_W_BOSQUE = 50;
const unsigned int GASTO_W_AGUA_R = 10;
const unsigned int GASTO_W_BOSQUE_R = 15;
const unsigned int GASTO_W_ARENOSO = 2;
const unsigned int GASTO_R_AGUA = 150;
const unsigned int GASTO_R_BOSQUE = 75;
const unsigned int GASTO_R_AGUA_R = 15;
const unsigned int GASTO_R_BOSQUE_R = 25;
const unsigned int GASTO_R_ARENOSO = 3;
const unsigned int GASTO_L_AGUA = 30;
const unsigned int GASTO_L_BOSQUE = 7;
const unsigned int GASTO_L_AGUA_R = 5;
const unsigned int GASTO_L_BOSQUE_R = 1;
const unsigned int GASTO_L_ARENOSO = 2;
const unsigned int GASTO_sR_AGUA = 10;
const unsigned int GASTO_sR_BOSQUE = 5;
const unsigned int GASTO_sR_AGUA_R = 2;
const unsigned int GASTO_sR_BOSQUE_R = 1;
const unsigned int GASTO_sR_ARENOSO = 1;
const unsigned int GASTO_DEFAULT = 1;


bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char>> &mapa) {
	return (mapa[x.f][x.c] != 'P' && mapa[x.f][x.c] != 'M');
}

bool ColaboradorALaVista( const stateN1 &st) {
	if(abs(st.jugador.f-st.colaborador.f) > 3 || abs(st.jugador.c-st.colaborador.c) > 3)
		return false;
	else {
		bool aLaVista;
		switch(st.jugador.brujula) {
			case norte:
				aLaVista = (abs(st.jugador.c-st.colaborador.c)<=st.jugador.f-st.colaborador.f);
				break;
			case noreste:
				aLaVista = (st.colaborador.f <= st.jugador.f && st.colaborador.c >= st.jugador.c);
				break;
			case este:
				aLaVista = (abs(st.jugador.f-st.colaborador.f)<=st.colaborador.c-st.jugador.c);
				break;
			case sureste:
				aLaVista = (st.colaborador.f >= st.jugador.f && st.colaborador.c >= st.jugador.c);
				break;
			case sur:
				aLaVista = (abs(st.jugador.c-st.colaborador.c)<=st.colaborador.f-st.jugador.f);
				break;
			case suroeste:
				aLaVista = (st.colaborador.f >= st.jugador.f && st.colaborador.c <= st.jugador.c);
				break;
			case oeste:
				aLaVista = (abs(st.jugador.f-st.colaborador.f)<=st.jugador.c-st.colaborador.c);
				break;
			case noroeste:
				aLaVista = (st.colaborador.f <= st.jugador.f && st.colaborador.c <= st.jugador.c);
				break;		
		}
		return aLaVista;
	}
}


ubicacion NextCasilla(const ubicacion &pos) {
	ubicacion next = pos;
	switch(pos.brujula) {
		case norte:
			next.f = pos.f - 1;
			break;
		case noreste:
			next.f = pos.f - 1;
			next.c = pos.c + 1;
			break;
		case este:
			next.c = pos.c + 1;
			break;
		case sureste:
			next.f = pos.f + 1;
			next.c = pos.c + 1;
			break;
		case sur:
			next.f = pos.f + 1;
			break;
		case suroeste:
			next.f = pos.f + 1;
			next.c = pos.c - 1;
			break;
		case oeste:
			next.c = pos.c - 1;
			break;
		case noroeste:
			next.f = pos.f - 1;
			next.c = pos.c - 1;
			break;
	}
	return next;
}

stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char>> &mapa) {
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch(a) {
		case actWALK:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){
					st_result.jugador = sig_ubicacion;
			}
			break;
		
		case actRUN:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)) {
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if(CasillaTransitable(sig_ubicacion2,mapa) &&
						!(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)) {
							st_result.jugador = sig_ubicacion2;
					}
			}
			break;
		
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6)%8);
			break;
		
		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1)%8);
			break;
	}
	return st_result;
}

void actualizaColaborador(stateN1 &st, const vector<vector<unsigned char>> &mapa) {	// Con la última orden dada
	if(st.ultimaOrdenColaborador==act_CLB_TURN_SR || st.ultimaOrdenColaborador==act_CLB_WALK){
		ubicacion sig_ubicacion;
		switch(st.ultimaOrdenColaborador){
			case act_CLB_TURN_SR:
				st.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula + 1)%8);
				break;
			case act_CLB_WALK:
				sig_ubicacion = NextCasilla(st.colaborador);
				if(CasillaTransitable(sig_ubicacion,mapa) &&
					!(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
						st.colaborador = sig_ubicacion;
				}
				break;
		}
	}
}

bool actualizaColaboradorN3(stateN3 &st, const vector<vector<unsigned char>> &mapa) {
	if(st.ultimaOrdenColaborador==act_CLB_TURN_SR || st.ultimaOrdenColaborador==act_CLB_WALK){
		ubicacion sig_ubicacion;
		switch(st.ultimaOrdenColaborador){
			case act_CLB_TURN_SR:
				st.colaborador.brujula = static_cast<Orientacion>((st.colaborador.brujula + 1)%8);
				return true;
				break;
			case act_CLB_WALK:
				sig_ubicacion = NextCasilla(st.colaborador);
				if(CasillaTransitable(sig_ubicacion,mapa) &&
					!(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
						st.colaborador = sig_ubicacion;
						return true;
				}
				else if(mapa[sig_ubicacion.f][sig_ubicacion.c] == 'P')
					return false;

				else
					return true;
				break;
		}
	}
	else return true;
}

stateN1 applyColab(const Action &a, const stateN1 &st, const vector<vector<unsigned char>> &mapa) {
	stateN1 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch(a) {
		case actWALK:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){
					st_result.jugador = sig_ubicacion;
			}
			actualizaColaborador(st_result,mapa);
			break;
		
		case actRUN:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)) {
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if(CasillaTransitable(sig_ubicacion2,mapa) &&
						!(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)) {
							st_result.jugador = sig_ubicacion2;
					}
			}

			actualizaColaborador(st_result,mapa);
			break;
		
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6)%8);

			actualizaColaborador(st_result,mapa);
			break;
		
		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1)%8);

			actualizaColaborador(st_result,mapa);
			break;
		
		case act_CLB_WALK:
			if(ColaboradorALaVista(st)){
				st_result.ultimaOrdenColaborador = act_CLB_WALK;
				sig_ubicacion = NextCasilla(st.colaborador);
				if(CasillaTransitable(sig_ubicacion,mapa) &&
					!(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
						st_result.colaborador = sig_ubicacion;
				}	
			}
			else actualizaColaborador(st_result,mapa);
			break;
		
		case act_CLB_TURN_SR:
			if(ColaboradorALaVista(st)){
				st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
				st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 1)%8);
			}
			else actualizaColaborador(st_result,mapa);
			break;
		
		case act_CLB_STOP:
			if(ColaboradorALaVista(st)){
				st_result.ultimaOrdenColaborador = act_CLB_STOP;
			}
			else actualizaColaborador(st_result,mapa);
			break;
		
		case actIDLE:
			actualizaColaborador(st_result,mapa);
			break;
	}
	return st_result;
}

int actualizaBateriaN2(const Action &a, const stateN2 &st, const vector<vector<unsigned char>> &mapa) {
	int gasto_bat = 0;
	switch(a) {
		case actWALK:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini)
						gasto_bat = st.GASTO_W_AGUA;
					else
						gasto_bat = st.GASTO_W_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas)
						gasto_bat = st.GASTO_W_BOSQUE;
					else
						gasto_bat = st.GASTO_W_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = st.GASTO_W_ARENOSO;
					break;
				default:
					gasto_bat = st.GASTO_DEFAULT;
					break;
			}
			break;
		
		case actRUN:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini)
						gasto_bat = st.GASTO_R_AGUA;
					else
						gasto_bat = st.GASTO_R_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas)
						gasto_bat = st.GASTO_R_BOSQUE;
					else
						gasto_bat = st.GASTO_R_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = st.GASTO_R_ARENOSO;
					break;
				default:
					gasto_bat = st.GASTO_DEFAULT;
					break;
			}
			break;
		
		case actTURN_L:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini)
						gasto_bat = st.GASTO_L_AGUA;
					else
						gasto_bat = st.GASTO_L_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas)
						gasto_bat = st.GASTO_L_BOSQUE;
					else
						gasto_bat = st.GASTO_L_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = st.GASTO_L_ARENOSO;
					break;
				default:
					gasto_bat = st.GASTO_DEFAULT;
					break;
			}
			break;

		case actTURN_SR:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini)
						gasto_bat = st.GASTO_sR_AGUA;
					else
						gasto_bat = st.GASTO_sR_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas)
						gasto_bat = st.GASTO_sR_BOSQUE;
					else
						gasto_bat = st.GASTO_sR_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = st.GASTO_sR_ARENOSO;
					break;
				default:
					gasto_bat = st.GASTO_DEFAULT;
					break;
			}

			break;
	}
	return st.bateria - gasto_bat;
}

bool ColaboradorALaVistaN3(const stateN3 &st) {
	if(abs(st.jugador.f-st.colaborador.f) > 3 || abs(st.jugador.c-st.colaborador.c) > 3)
		return false;
	else {
		bool aLaVista;
		switch(st.jugador.brujula) {
			case norte:
				aLaVista = (abs(st.jugador.c-st.colaborador.c)<=st.jugador.f-st.colaborador.f);
				break;
			case noreste:
				aLaVista = (st.colaborador.f <= st.jugador.f && st.colaborador.c >= st.jugador.c);
				break;
			case este:
				aLaVista = (abs(st.jugador.f-st.colaborador.f)<=st.colaborador.c-st.jugador.c);
				break;
			case sureste:
				aLaVista = (st.colaborador.f >= st.jugador.f && st.colaborador.c >= st.jugador.c);
				break;
			case sur:
				aLaVista = (abs(st.jugador.c-st.colaborador.c)<=st.colaborador.f-st.jugador.f);
				break;
			case suroeste:
				aLaVista = (st.colaborador.f >= st.jugador.f && st.colaborador.c <= st.jugador.c);
				break;
			case oeste:
				aLaVista = (abs(st.jugador.f-st.colaborador.f)<=st.jugador.c-st.colaborador.c);
				break;
			case noroeste:
				aLaVista = (st.colaborador.f <= st.jugador.f && st.colaborador.c <= st.jugador.c);
				break;		
		}
		return aLaVista;
	}
}

int gastoBateriaColab(const stateN3 &st, const vector<vector<unsigned char>> &mapa) {
	int gasto_bat = 0;
	switch(st.ultimaOrdenColaborador) {
		case act_CLB_WALK:
			switch(mapa[st.colaborador.f][st.colaborador.c]){
				case 'A':
					if(!st.bikini_c)
						gasto_bat = GASTO_W_AGUA;
					else
						gasto_bat = GASTO_W_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_c)
						gasto_bat = GASTO_W_BOSQUE;
					else
						gasto_bat = GASTO_W_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_W_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
			}

			break;
		
		case act_CLB_TURN_SR:
			switch(mapa[st.colaborador.f][st.colaborador.c]){
				case 'A':
					if(!st.bikini_c)
						gasto_bat = GASTO_sR_AGUA;
					else
						gasto_bat = GASTO_sR_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_c)
						gasto_bat = GASTO_sR_BOSQUE;
					else
						gasto_bat = GASTO_sR_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_sR_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
			}
			break;
		
		default:
			gasto_bat = 0;
			break;
	}
	return gasto_bat;
}

int actualizaBateriaN3(const Action &a, const stateN3 &st, const vector<vector<unsigned char>> &mapa) {
	int gasto_bat = 0;
	switch(a) {
		case actWALK:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini_j)
						gasto_bat = GASTO_W_AGUA;
					else
						gasto_bat = GASTO_W_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_j)
						gasto_bat = GASTO_W_BOSQUE;
					else
						gasto_bat = GASTO_W_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_W_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
			}
			gasto_bat += gastoBateriaColab(st,mapa);

			break;
		
		case actRUN:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini_j)
						gasto_bat = GASTO_R_AGUA;
					else
						gasto_bat = GASTO_R_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_j)
						gasto_bat = GASTO_R_BOSQUE;
					else
						gasto_bat = GASTO_R_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_R_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
			}
			gasto_bat += gastoBateriaColab(st,mapa);
			break;
		
		case actTURN_L:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini_j)
						gasto_bat = GASTO_L_AGUA;
					else
						gasto_bat = GASTO_L_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_j)
						gasto_bat = GASTO_L_BOSQUE;
					else
						gasto_bat = GASTO_L_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_L_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
			}
			gasto_bat += gastoBateriaColab(st,mapa);
			break;

		case actTURN_SR:
			switch(mapa[st.jugador.f][st.jugador.c]){
				case 'A':
					if(!st.bikini_j)
						gasto_bat = GASTO_sR_AGUA;
					else
						gasto_bat = GASTO_sR_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_j)
						gasto_bat = GASTO_sR_BOSQUE;
					else
						gasto_bat = GASTO_sR_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_sR_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
			}
			gasto_bat += gastoBateriaColab(st,mapa);	

			break;
		
		case actIDLE:
			gasto_bat = gastoBateriaColab(st,mapa);
			break;
		
		case act_CLB_WALK:
			if(ColaboradorALaVistaN3(st)) {
				switch(mapa[st.colaborador.f][st.colaborador.c]){
				case 'A':
					if(!st.bikini_c)
						gasto_bat = GASTO_W_AGUA;
					else
						gasto_bat = GASTO_W_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_c)
						gasto_bat = GASTO_W_BOSQUE;
					else
						gasto_bat = GASTO_W_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_W_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
				}
			}
			else {
				gasto_bat = gastoBateriaColab(st,mapa);
			}
			break;
		
		case act_CLB_TURN_SR:
			if(ColaboradorALaVistaN3(st)) {
				switch(mapa[st.colaborador.f][st.colaborador.c]){
				case 'A':
					if(!st.bikini_c)
						gasto_bat = GASTO_sR_AGUA;
					else
						gasto_bat = GASTO_sR_AGUA_R;
					break;
				case 'B':
					if(!st.zapatillas_c)
						gasto_bat = GASTO_sR_BOSQUE;
					else
						gasto_bat = GASTO_sR_BOSQUE_R;
					break;
				case 'T':
					gasto_bat = GASTO_sR_ARENOSO;
					break;
				default:
					gasto_bat = GASTO_DEFAULT;
					break;
				}
			}
			else {
				gasto_bat = gastoBateriaColab(st,mapa);
			}
			break;
		
		case act_CLB_STOP:
			if(ColaboradorALaVistaN3(st)){
				gasto_bat = 0;
			}
			else {
				gasto_bat = gastoBateriaColab(st,mapa);
			}
			break;
	}
	return st.bateria - gasto_bat;
}

stateN2 applyN2(const Action &a, const stateN2 &st, const vector<vector<unsigned char>> &mapa) {
	stateN2 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	switch(a) {
		case actWALK:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

					st_result.bateria = actualizaBateriaN2(actWALK,st_result,mapa);
					
					st_result.jugador = sig_ubicacion;
			}
			break;
		
		case actRUN:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)) {
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if(CasillaTransitable(sig_ubicacion2,mapa) &&
						!(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)) {

							st_result.bateria = actualizaBateriaN2(actRUN,st_result,mapa);

							st_result.jugador = sig_ubicacion2;

							/*switch(mapa[sig_ubicacion2.f][sig_ubicacion2.c]){
								case 'K':
									st_result.bikini = true;
									st_result.zapatillas = false;
									break;
								case 'D':
									st_result.zapatillas = true;
									st_result.bikini = false;
									break;
							}*/
					}
			}
			break;
		
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6)%8);
			st_result.bateria = actualizaBateriaN2(actTURN_L,st_result,mapa);
			break;
		
		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1)%8);
			st_result.bateria = actualizaBateriaN2(actTURN_SR,st_result,mapa);
			break;
		
		case actIDLE:
			if(mapa[st_result.jugador.f][st_result.jugador.c] == 'X') {
				st_result.bateria = (st_result.bateria+10 < 3000) ? st_result.bateria+10 : 3000;
			}
	}

	switch(mapa[st_result.jugador.f][st_result.jugador.c]){
		case 'K':
			st_result.bikini = true;
			st_result.zapatillas = false;
			break;
		case 'D':
			st_result.zapatillas = true;
			st_result.bikini = false;
			break;
	}

	return st_result;
}

stateN3 applyN3(const Action &a, const stateN3 &st, const vector<vector<unsigned char>> &mapa) {
	stateN3 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2;
	bool vivo;
	switch(a) {
		case actWALK:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)){

					st_result.bateria = actualizaBateriaN3(actWALK,st_result,mapa);
					
					st_result.jugador = sig_ubicacion;
					vivo = actualizaColaboradorN3(st_result,mapa);
					if(!vivo) return st;
			}			
			break;
		
		case actRUN:
			sig_ubicacion = NextCasilla(st.jugador);
			if(CasillaTransitable(sig_ubicacion,mapa) &&
				!(sig_ubicacion.f == st.colaborador.f && sig_ubicacion.c == st.colaborador.c)) {
					sig_ubicacion2 = NextCasilla(sig_ubicacion);
					if(CasillaTransitable(sig_ubicacion2,mapa) &&
						!(sig_ubicacion2.f == st.colaborador.f && sig_ubicacion2.c == st.colaborador.c)) {

							st_result.bateria = actualizaBateriaN3(actRUN,st_result,mapa);

							st_result.jugador = sig_ubicacion2;
							vivo = actualizaColaboradorN3(st_result,mapa);
							if(!vivo) return st;
							/*switch(mapa[sig_ubicacion2.f][sig_ubicacion2.c]){
								case 'K':
									st_result.bikini = true;
									st_result.zapatillas = false;
									break;
								case 'D':
									st_result.zapatillas = true;
									st_result.bikini = false;
									break;
							}*/
					}
					
			}		
			break;
		
		case actTURN_L:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 6)%8);
			st_result.bateria = actualizaBateriaN3(actTURN_L,st_result,mapa);
			vivo = actualizaColaboradorN3(st_result,mapa);
			if(!vivo) return st;
			break;
		
		case actTURN_SR:
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula + 1)%8);
			st_result.bateria = actualizaBateriaN3(actTURN_SR,st_result,mapa);
			vivo = actualizaColaboradorN3(st_result,mapa);
			if(!vivo) return st;
			break;
		
		case actIDLE:
			/*if(mapa[st_result.jugador.f][st_result.jugador.c] == 'X') {
				st_result.bateria = (st_result.bateria+10 < 3000) ? st_result.bateria+10 : 3000;
			}*/
			st_result.bateria = actualizaBateriaN3(actIDLE,st_result,mapa);
			vivo = actualizaColaboradorN3(st_result,mapa);
			if(!vivo) return st;
			break;

		// Revisar caerse por precipicio
		case act_CLB_WALK:
			if(ColaboradorALaVistaN3(st)){
				sig_ubicacion = NextCasilla(st.colaborador);
				if(CasillaTransitable(sig_ubicacion,mapa) &&
					!(sig_ubicacion.f == st.jugador.f && sig_ubicacion.c == st.jugador.c)){
						st_result.bateria = actualizaBateriaN3(act_CLB_WALK,st_result,mapa);
						st_result.colaborador = sig_ubicacion;
						st_result.ultimaOrdenColaborador = act_CLB_WALK;
				}	
			}
			break;
		
		case act_CLB_TURN_SR:
			if(ColaboradorALaVistaN3(st)){
				st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;
				st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 1)%8);
				st_result.bateria = actualizaBateriaN3(act_CLB_TURN_SR,st_result,mapa);
			}
			break;
		
		case act_CLB_STOP:
			if(ColaboradorALaVistaN3(st)){
				st_result.ultimaOrdenColaborador = act_CLB_STOP;
			}
			break;
	}

	switch(mapa[st_result.jugador.f][st_result.jugador.c]){
		case 'K':
			st_result.bikini_j = true;
			st_result.zapatillas_j = false;
			break;
		case 'D':
			st_result.zapatillas_j = true;
			st_result.bikini_j = false;
			break;
	}

	switch(mapa[st_result.colaborador.f][st_result.colaborador.c]){
		case 'K':
			st_result.bikini_c = true;
			st_result.zapatillas_c = false;
			break;
		case 'D':
			st_result.zapatillas_c = true;
			st_result.bikini_c = false;
			break;
	}

	return st_result;
}

bool Find(const stateN0 &item, const list<stateN0> &lista){
	auto it = lista.begin();
	while(it != lista.end() && !((*it) == item))
		it++;
	return(!(it == lista.end()));
}

bool AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final,
						const vector<vector<unsigned char>> &mapa) 
{
	stateN0 current_state = inicio;
	list<stateN0> frontier;
	list<stateN0> explored;
	frontier.push_back(current_state);
	bool SolutionFound = (current_state.jugador.f == final.f &&
						  current_state.jugador.c == final.c);
	
	while(!frontier.empty() && !SolutionFound) {
		frontier.pop_front();
		explored.push_back(current_state);

		// actWALK
		stateN0 child_walk = apply(actWALK, current_state, mapa);
		if(child_walk.jugador.f == final.f && child_walk.jugador.c == final.c){
			current_state = child_walk;
			SolutionFound = true;
		}
		else if(!Find(child_walk,frontier) && !Find(child_walk,explored)){
			frontier.push_back(child_walk);
		}

		//actRUN
		if(!SolutionFound){
			stateN0 child_run = apply(actRUN, current_state, mapa);
			if(child_run.jugador.f == final.f && child_run.jugador.c == final.c){
				current_state = child_run;
				SolutionFound = true;
			}
			else if(!Find(child_run,frontier) && !Find(child_run,explored)){
				frontier.push_back(child_run);
			}

		}

		// Giros
		if(!SolutionFound) {
			stateN0 child_turnl = apply(actTURN_L,current_state,mapa);
			if(!Find(child_turnl,frontier) && !Find(child_turnl,explored)) {
				frontier.push_back(child_turnl);
			}

			stateN0 child_turnsr = apply(actTURN_SR,current_state,mapa);
			if(!Find(child_turnsr,frontier) && !Find(child_turnsr,explored)) {
				frontier.push_back(child_turnsr);
			}
		}

		if(!SolutionFound && !frontier.empty())
			current_state = frontier.front();
	}
	return SolutionFound;
}

void AnulaMatriz(vector<vector<unsigned char>> &matriz) {
	for(int i = 0; i < matriz.size(); i++)
		for(int j = 0 ; j < matriz[0].size(); j++)
			matriz[i][j] = 0;
}

void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan) {
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin();
	while(it!=plan.end()){
		if((*it != act_CLB_WALK) && (*it != act_CLB_TURN_SR) && (*it != act_CLB_STOP))
		{
			switch(cst.ultimaOrdenColaborador)
			{
				case act_CLB_WALK:
					cst.colaborador = NextCasilla(cst.colaborador);
					mapaConPlan[cst.colaborador.f][cst.colaborador.c]=2;
					break;
				case act_CLB_TURN_SR:
					cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
					break;
			}
		}

		switch(*it)
		{
			case actRUN:
				cst.jugador = NextCasilla(cst.jugador);
				mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
				cst.jugador = NextCasilla(cst.jugador);
				mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
				break;
			case actWALK:
				cst.jugador = NextCasilla(cst.jugador);
				mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
				break;
			case actTURN_SR:
				cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
				break;
			case actTURN_L:
				cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
				break;
			case act_CLB_WALK:
				cst.colaborador = NextCasilla(cst.colaborador);
				cst.ultimaOrdenColaborador = act_CLB_WALK;
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			case act_CLB_TURN_SR:
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
				break;
			case act_CLB_STOP:
				cst.ultimaOrdenColaborador = act_CLB_STOP;
				break;
		}
		it++;
	}
}

bool Find(const stateN0 &item, const list<nodeN0> &lista){
	auto it = lista.begin();
	while(it != lista.end() && !(it->st == item))
	it++;

	return (!(it == lista.end()));
}

void PintaPlan(const list<Action> &plan) {
	auto it = plan.begin();
	while(it != plan.end())
	{
		if(*it==actWALK){
			cout << "W ";
		}
		else if(*it==actRUN){
			cout << "R ";
		}
		else if(*it==actTURN_SR){
			cout << "r ";
		}
		else if(*it==actTURN_L){
			cout << "L ";
		}
		else if(*it==act_CLB_WALK){
			cout << "cW ";
		}
		else if(*it==act_CLB_TURN_SR){
			cout << "cr ";
		}
		else if(*it==act_CLB_STOP){
			cout << "cS ";
		}
		else if(*it==actIDLE){
			cout << "I ";
		}
		else{
			cout << "_ ";
		}
		it++;
	}
	cout << " (" << plan.size() << " acciones)\n";
}

list<Action> AnchuraSoloJugador_V2(const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa){
	nodeN0 current_node;
	list<nodeN0> frontier;
	list<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f &&
							current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while(!frontier.empty() && !SolutionFound){
		frontier.pop_front();
		explored.push_back(current_node);

		//Hijo walk
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK,current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if(child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c){
			current_node = child_walk;
			SolutionFound = true;
		}
		else if(!Find(child_walk.st, frontier) && !Find(child_walk.st,explored)){
			frontier.push_back(child_walk);
		}

		if(!SolutionFound){
			//Hijo run
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN,current_node.st,mapa);
			child_run.secuencia.push_back(actRUN);
			if(child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if(!Find(child_run.st, frontier) && !Find(child_run.st,explored)){
				frontier.push_back(child_run);
			}
		}

		if(!SolutionFound){
			//Hijos giros

			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L,current_node.st,mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(!Find(child_turnl.st,frontier) && !Find(child_turnl.st,explored)){
				frontier.push_back(child_turnl);
			}

			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR,current_node.st,mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if(!Find(child_turnsr.st,frontier) && !Find(child_turnsr.st,explored)){
				frontier.push_back(child_turnsr);
			}
		}

		if(!SolutionFound and !frontier.empty())
			current_node = frontier.front();
	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
	
}

list<Action> AnchuraSoloJugador_V3(const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa){
	nodeN0 current_node;
	list<nodeN0> frontier;
	set<nodeN0> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.jugador.f == final.f &&
							current_node.st.jugador.c == final.c);
	frontier.push_back(current_node);

	while(!frontier.empty() && !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		//Hijo walk
		nodeN0 child_walk = current_node;
		child_walk.st = apply(actWALK,current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if(child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c){
			current_node = child_walk;
			SolutionFound = true;
		}
		else if(explored.find(child_walk) == explored.end()){
			frontier.push_back(child_walk);
		}

		if(!SolutionFound){
			//Hijo run
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN,current_node.st,mapa);
			child_run.secuencia.push_back(actRUN);
			if(child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if(explored.find(child_run) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		if(!SolutionFound){
			//Hijos giros

			nodeN0 child_turnl = current_node;
			child_turnl.st = apply(actTURN_L,current_node.st,mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}

			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR,current_node.st,mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if(explored.find(child_turnsr) == explored.end()){
				frontier.push_back(child_turnsr);
			}
		}

		if(!SolutionFound and !frontier.empty()) {
			current_node = frontier.front();
			while(!frontier.empty() && explored.find(current_node) != explored.end()) {
				frontier.pop_front();
				if(!frontier.empty()){
					current_node = frontier.front();
				}
			}
		}

	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
	
}

list<Action> AnchuraConColaborador(const stateN1 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa){
	nodeN1 current_node;
	list<nodeN1> frontier;
	set<nodeN1> explored;
	list<Action> plan;
	current_node.st = inicio;
	bool SolutionFound = (current_node.st.colaborador.f == final.f &&
							current_node.st.colaborador.c == final.c);
	frontier.push_back(current_node);

	while(!frontier.empty() && !SolutionFound){
		frontier.pop_front();
		explored.insert(current_node);

		// Hijo idle
		nodeN1 child_idle = current_node;
		child_idle.st = applyColab(actIDLE,current_node.st,mapa);
		child_idle.secuencia.push_back(actIDLE);
		if(child_idle.st.colaborador.f == final.f && child_idle.st.colaborador.c == final.c){
			current_node = child_idle;
			SolutionFound = true;
		}
		else if(explored.find(child_idle) == explored.end()){
			frontier.push_back(child_idle);
		}

		if(!SolutionFound){
			//Hijo walk
			nodeN1 child_walk = current_node;
			child_walk.st = applyColab(actWALK,current_node.st, mapa);
			child_walk.secuencia.push_back(actWALK);
			if(child_walk.st.colaborador.f == final.f && child_walk.st.colaborador.c == final.c){
				current_node = child_walk;
				SolutionFound = true;
			}
			else if(explored.find(child_walk) == explored.end()){
				frontier.push_back(child_walk);
			}
		}

		if(!SolutionFound){
			//Hijo run
			nodeN1 child_run = current_node;
			child_run.st = applyColab(actRUN,current_node.st,mapa);
			child_run.secuencia.push_back(actRUN);
			if(child_run.st.colaborador.f == final.f && child_run.st.colaborador.c == final.c){
				current_node = child_run;
				SolutionFound = true;
			}
			else if(explored.find(child_run) == explored.end()){
				frontier.push_back(child_run);
			}
		}

		//Hijos giros
		if(!SolutionFound){
			nodeN1 child_turnl = current_node;
			child_turnl.st = applyColab(actTURN_L,current_node.st,mapa);
			child_turnl.secuencia.push_back(actTURN_L);
			if(child_turnl.st.colaborador.f == final.f && child_turnl.st.colaborador.c == final.c){
				current_node = child_turnl;
				SolutionFound = true;
			}
			else if(explored.find(child_turnl) == explored.end()){
				frontier.push_back(child_turnl);
			}

			
		}

		if(!SolutionFound){
			nodeN1 child_turnsr = current_node;
			child_turnsr.st = applyColab(actTURN_SR,current_node.st,mapa);
			child_turnsr.secuencia.push_back(actTURN_SR);
			if(child_turnsr.st.colaborador.f == final.f && child_turnsr.st.colaborador.c == final.c){
				current_node = child_turnsr;
				SolutionFound = true;
			}
			else if(explored.find(child_turnsr) == explored.end()){
				frontier.push_back(child_turnsr);
			}
		}

		if(!SolutionFound && ColaboradorALaVista(current_node.st)) {
			// Hijo clb_walk
			nodeN1 child_clb_walk = current_node;
			child_clb_walk.st = applyColab(act_CLB_WALK,current_node.st,mapa);
			child_clb_walk.secuencia.push_back(act_CLB_WALK);
			if(child_clb_walk.st.colaborador.f == final.f && child_clb_walk.st.colaborador.c == final.c ) {
				current_node = child_clb_walk;
				SolutionFound = true;
			}
			else if(explored.find(child_clb_walk) == explored.end()){
				frontier.push_back(child_clb_walk);
			}
		}

		if(!SolutionFound && ColaboradorALaVista(current_node.st)) {
			// Hijo clb_turnsr
			nodeN1 child_clb_turnsr = current_node;
			child_clb_turnsr.st = applyColab(act_CLB_TURN_SR,current_node.st,mapa);
			child_clb_turnsr.secuencia.push_back(act_CLB_TURN_SR);
			if(child_clb_turnsr.st.colaborador.f == final.f && child_clb_turnsr.st.colaborador.c == final.c ) {
				current_node = child_clb_turnsr;
				SolutionFound = true;
			}
			else if(explored.find(child_clb_turnsr) == explored.end()){
				frontier.push_back(child_clb_turnsr);
			}
		}

		if(!SolutionFound && ColaboradorALaVista(current_node.st)) {
			// Hijo clb_stop
			nodeN1 child_clb_stop = current_node;
			child_clb_stop.st = applyColab(act_CLB_STOP,current_node.st,mapa);
			child_clb_stop.secuencia.push_back(act_CLB_STOP);
			
			if(explored.find(child_clb_stop) == explored.end()){
				frontier.push_back(child_clb_stop);
			}
		}

		if(!SolutionFound and !frontier.empty()) {
			current_node = frontier.front();
			while(!frontier.empty() && explored.find(current_node) != explored.end()) {
				frontier.pop_front();
				if(!frontier.empty()){
					current_node = frontier.front();
				}
			}
		}

	}

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Encontrado un plan: ";
		PintaPlan(current_node.secuencia);
	}

	return plan;
	
}

/*list<Action> AnchuraSoloJugadorN2(const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa){
	nodeN2 current_node;
	nodeN2 best_sol;
	best_sol.st.bateria = -1;
	const stateN2 ini(inicio);
	priority_queue<nodeN2> frontier;
	set<nodeN2> explored;
	list<Action> plan;
	current_node.st = ini;
	bool SolutionFound = (current_node.st.jugador.f == final.f &&
							current_node.st.jugador.c == final.c);
	frontier.push(current_node);

	while(!frontier.empty() && current_node.st.bateria > best_sol.st.bateria){
		frontier.pop();
		explored.insert(current_node);

		//Hijo walk
		nodeN2 child_walk = current_node;
		child_walk.st = applyN2(actWALK,current_node.st, mapa);
		child_walk.secuencia.push_back(actWALK);
		if(child_walk.st.jugador.f == final.f && child_walk.st.jugador.c == final.c){
			if(!SolutionFound){
				best_sol = child_walk;
				SolutionFound = true;
			}
			else{
				if(child_walk.st.bateria > best_sol.st.bateria)
					best_sol = child_walk;
			}
		}
		else if(explored.find(child_walk) == explored.end())
			frontier.push(child_walk);
		
		//Hijo run
		nodeN2 child_run = current_node;
		child_run.st = applyN2(actRUN,current_node.st,mapa);
		child_run.secuencia.push_back(actRUN);
		if(child_run.st.jugador.f == final.f && child_run.st.jugador.c == final.c){
			if(!SolutionFound) {
				best_sol = child_run;
				SolutionFound = true;
			}
			else {
				if( child_run.st.bateria > best_sol.st.bateria)
					best_sol = child_run;
			}
			
		}
		else if(explored.find(child_run) == explored.end()) {
			frontier.push(child_run);
		}
		
		//Hijos giros

		nodeN2 child_turnl = current_node;
		child_turnl.st = applyN2(actTURN_L,current_node.st,mapa);
		child_turnl.secuencia.push_back(actTURN_L);
		if(explored.find(child_turnl) == explored.end()){
			frontier.push(child_turnl);
		}

		nodeN2 child_turnsr = current_node;
		child_turnsr.st = applyN2(actTURN_SR,current_node.st,mapa);
		child_turnsr.secuencia.push_back(actTURN_SR);
		if(explored.find(child_turnsr) == explored.end()){
			frontier.push(child_turnsr);
		}

		// Hijo idle
		if(mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'X' && current_node.st.bateria < 3000) {
			nodeN2 child_idle = current_node;
			child_idle.st = applyN2(actIDLE,current_node.st,mapa);
			child_idle.secuencia.push_back(actIDLE);
			if(explored.find(child_idle) == explored.end()){
				frontier.push(child_idle);
			}
		}

		if(!frontier.empty()) {
			current_node = frontier.top();
			while(!frontier.empty() && explored.find(current_node) != explored.end() ) {
				frontier.pop();
				if(!frontier.empty()){
					current_node = frontier.top();
				}
			}
		}

	}

	if(SolutionFound){
		plan = best_sol.secuencia;
		cout << "Bateria: " << best_sol.st.bateria << "\n";
		cout << "Encontrado un plan: ";
		PintaPlan(plan);
	}

	return plan;
	
}*/

list<Action> DijkstraN2(const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa){
	nodeN2 current_node;
	const stateN2 ini(inicio);
	priority_queue<nodeN2> frontier;
	set<stateN2> explored;
	list<Action> plan;
	current_node.st = ini;

	switch(mapa[current_node.st.jugador.f][current_node.st.jugador.c]){
		case 'K':
			current_node.st.bikini = true;
			current_node.st.zapatillas = false;
			break;
		case 'D':
			current_node.st.zapatillas = true;
			current_node.st.bikini = false;
			break;
	}

	bool SolutionFound = (current_node.st.jugador.f == final.f &&
							current_node.st.jugador.c == final.c);
	frontier.push(current_node);
	while(!SolutionFound && !frontier.empty()) {
		frontier.pop();
		explored.insert(current_node.st);

		// Hijo idle
		if(mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'X' && current_node.st.bateria < 3000) {
			nodeN2 child_idle = current_node;
			child_idle.st = applyN2(actIDLE,current_node.st,mapa);
			child_idle.secuencia.push_back(actIDLE);
			
			if(explored.find(child_idle.st) == explored.end()){
				frontier.push(child_idle);
			}
		}
		
		//Hijo walk
		nodeN2 child_walk = current_node;
		child_walk.st = applyN2(actWALK,current_node.st,mapa);
		child_walk.secuencia.push_back(actWALK);
		if(explored.find(child_walk.st) == explored.end() && child_walk.st.bateria >= 0){
			frontier.push(child_walk);
		}

		// Hijo run
		nodeN2 child_run = current_node;
		child_run.st = applyN2(actRUN,current_node.st,mapa);
		child_run.secuencia.push_back(actRUN);
		if(explored.find(child_run.st) == explored.end() && child_run.st.bateria >= 0){
			frontier.push(child_run);
		}

		// Hijos giros
		nodeN2 child_turnl = current_node;
		child_turnl.st = applyN2(actTURN_L,current_node.st,mapa);
		child_turnl.secuencia.push_back(actTURN_L);
		if(explored.find(child_turnl.st) == explored.end() && child_turnl.st.bateria >= 0){
			frontier.push(child_turnl);
		}

		nodeN2 child_turnsr = current_node;
		child_turnsr.st = applyN2(actTURN_SR,current_node.st,mapa);
		child_turnsr.secuencia.push_back(actTURN_SR);
		if(explored.find(child_turnsr.st) == explored.end() && child_turnsr.st.bateria >= 0){
			frontier.push(child_turnsr);
		}

		if(!frontier.empty()){
			current_node = frontier.top();
			SolutionFound = (current_node.st.jugador.f == final.f &&
							current_node.st.jugador.c == final.c);
			while(!frontier.empty() && explored.find(current_node.st) != explored.end()) {
				frontier.pop();
				current_node = frontier.top();
				SolutionFound = (current_node.st.jugador.f == final.f &&
							current_node.st.jugador.c == final.c);
			}
		}
		
	}
	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Bateria: " << current_node.st.bateria << "\n";
		cout << "Encontrado un plan: ";
		PintaPlan(plan);
	}

	return plan;
}

int distancia(const ubicacion &ub1, const ubicacion &ub2) {
    int distancia = ((ub1.f-ub2.f)*(ub1.f-ub2.f) > (ub1.c-ub2.c)*(ub1.c-ub2.c)) ? abs(ub1.f-ub2.f) : abs(ub1.c-ub2.c);
    return distancia;
}

float heuristicaN3( const stateN3 &st, const ubicacion &meta) {
	/*int dist = ( distancia(st.jugador,st.colaborador) > 3 ) ? distancia(st.jugador,st.colaborador) : 0;
	int dist2 = distancia(st.colaborador, meta);
	if (dist2 == 0)
		return 0;
	else
		return (dist2 + dist )*3;*/
	return distancia(st.colaborador,meta)*2;
}

list<Action> ArbolN3(const stateN0 &inicio, const ubicacion &final,
									const vector<vector<unsigned char>> &mapa) {
	nodeN3 current_node;
	const stateN3 ini(inicio);
	current_node.st = ini;
	current_node.estimacion_gasto = heuristicaN3(current_node.st,final);
	priority_queue<nodeN3> frontier;
	set<stateN3> explored;
	list<Action> plan;

	switch(mapa[current_node.st.jugador.f][current_node.st.jugador.c]){
		case 'K':
			current_node.st.bikini_j = true;
			current_node.st.zapatillas_j = false;
			break;
		case 'D':
			current_node.st.zapatillas_j = true;
			current_node.st.bikini_j = false;
			break;
	}

	switch(mapa[current_node.st.colaborador.f][current_node.st.colaborador.c]){
		case 'K':
			current_node.st.bikini_c = true;
			current_node.st.zapatillas_c = false;
			break;
		case 'D':
			current_node.st.zapatillas_c = true;
			current_node.st.bikini_c = false;
			break;
	}
	
	bool SolutionFound = (current_node.st.colaborador.f == final.f &&
							current_node.st.colaborador.c == final.c);
	frontier.push(current_node);

	while(!SolutionFound && !frontier.empty()) {
		frontier.pop();
		explored.insert(current_node.st);
		cout << "Explorados: "<< explored.size() << endl;
		cout << "Frontier: " << frontier.size() << endl;
		SolutionFound = (current_node.st.colaborador.f == final.f &&
							current_node.st.colaborador.c == final.c);
		
		// Genero hijos clb solo si esta a la vista el colaborador
		if(ColaboradorALaVistaN3(current_node.st)) {
			// Hijo clb_walk
			if(current_node.st.ultimaOrdenColaborador != act_CLB_WALK) {
				nodeN3 child_clb_walk = current_node;
				child_clb_walk.st = applyN3(act_CLB_WALK,current_node.st,mapa);
				child_clb_walk.secuencia.push_back(act_CLB_WALK);
				child_clb_walk.estimacion_gasto = heuristicaN3(child_clb_walk.st,final);
				
				if(explored.find(child_clb_walk.st) == explored.end() && child_clb_walk.st.bateria - child_clb_walk.estimacion_gasto >= 0){
					frontier.push(child_clb_walk);
					
				}
			}
			
			// Hijo clb_turnsr
			if(current_node.st.ultimaOrdenColaborador != act_CLB_TURN_SR) {
				nodeN3 child_clb_turnsr = current_node;
				child_clb_turnsr.st = applyN3(act_CLB_TURN_SR,current_node.st,mapa);
				child_clb_turnsr.secuencia.push_back(act_CLB_TURN_SR);
				child_clb_turnsr.estimacion_gasto = heuristicaN3(child_clb_turnsr.st,final);

				if(explored.find(child_clb_turnsr.st) == explored.end() && child_clb_turnsr.st.bateria - child_clb_turnsr.estimacion_gasto >= 0){
					frontier.push(child_clb_turnsr);
					
				}
			}
			
			// Hijo clb_stop
			if(current_node.st.ultimaOrdenColaborador != act_CLB_STOP) {
				nodeN3 child_clb_stop = current_node;
				child_clb_stop.st = applyN3(act_CLB_STOP,current_node.st,mapa);
				child_clb_stop.secuencia.push_back(act_CLB_STOP);
				child_clb_stop.estimacion_gasto = heuristicaN3(child_clb_stop.st,final);

				if(explored.find(child_clb_stop.st) == explored.end() && child_clb_stop.st.bateria - child_clb_stop.estimacion_gasto >= 0){
					frontier.push(child_clb_stop);
					
				}
			}
		}

		// Hijo idle
		if(current_node.st.ultimaOrdenColaborador != act_CLB_STOP || mapa[current_node.st.jugador.f][current_node.st.jugador.c] == 'X' ){
			nodeN3 child_idle = current_node;
			child_idle.st = applyN3(actIDLE,current_node.st,mapa);
			child_idle.secuencia.push_back(actIDLE);
			child_idle.estimacion_gasto = heuristicaN3(child_idle.st,final);
			
			if(explored.find(child_idle.st) == explored.end() && child_idle.st.bateria - child_idle.estimacion_gasto >= 0){
				frontier.push(child_idle);
			}
		}
		
		//Hijo walk
		if(CasillaTransitable(NextCasilla(current_node.st.jugador),mapa)){
			nodeN3 child_walk = current_node;
			child_walk.st = applyN3(actWALK,current_node.st,mapa);
			child_walk.secuencia.push_back(actWALK);
			child_walk.estimacion_gasto = heuristicaN3(child_walk.st,final);
			if(explored.find(child_walk.st) == explored.end() && child_walk.st.bateria - child_walk.estimacion_gasto >= 0){
				frontier.push(child_walk);
				
			}
		}

		// Hijos giros
		nodeN3 child_turnl = current_node;
		child_turnl.st = applyN3(actTURN_L,current_node.st,mapa);
		child_turnl.secuencia.push_back(actTURN_L);
		child_turnl.estimacion_gasto = heuristicaN3(child_turnl.st,final);
		if(explored.find(child_turnl.st) == explored.end() && child_turnl.st.bateria - child_turnl.estimacion_gasto >= 0){
			frontier.push(child_turnl);
			
		}
		
		nodeN3 child_turnsr = current_node;
		child_turnsr.st = applyN3(actTURN_SR,current_node.st,mapa);
		child_turnsr.secuencia.push_back(actTURN_SR);
		child_turnsr.estimacion_gasto = heuristicaN3(child_turnsr.st,final);
		if(explored.find(child_turnsr.st) == explored.end() && child_turnsr.st.bateria - child_turnsr.estimacion_gasto >= 0){
			frontier.push(child_turnsr);
			
		}
		
		// Hijo run
		nodeN3 child_run = current_node;
		child_run.st = applyN3(actRUN,current_node.st,mapa);
		child_run.secuencia.push_back(actRUN);
		child_run.estimacion_gasto = heuristicaN3(child_run.st,final);
		if(explored.find(child_run.st) == explored.end() && child_run.st.bateria - child_run.estimacion_gasto >= 0){
			frontier.push(child_run);
			
		}
	

		if(!frontier.empty() && !SolutionFound){
			current_node = frontier.top();
			SolutionFound = (current_node.st.colaborador.f == final.f &&
							current_node.st.colaborador.c == final.c);
			
			while(!frontier.empty() && explored.find(current_node.st) != explored.end() && !SolutionFound) {
				frontier.pop();
				current_node = frontier.top();
				SolutionFound = (current_node.st.colaborador.f == final.f &&
							current_node.st.colaborador.c == final.c);
			}
		}
	}
	

	if(SolutionFound){
		plan = current_node.secuencia;
		cout << "Bateria " << current_node.st.bateria << endl;
		cout << "Encontrado un plan: ";
		PintaPlan(plan);
	}

	return plan;
}

list<Action> AvanzaASaltosDeCaballo(){
	list<Action> secuencia;
	secuencia.push_back(actWALK);
	secuencia.push_back(actWALK);
	secuencia.push_back(actTURN_L);
	secuencia.push_back(actWALK);
	return secuencia;
}
// Este es el método principal que se piden en la practica.
// Tiene como entrada la información de los sensores y devuelve la acción a realizar.
// Para ver los distintos sensores mirar fichero "comportamiento.hpp"
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;
	if(sensores.nivel != 4) {
		if(!hayPlan) {
			// Invocar método de busqueda
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;
			switch(sensores.nivel){
				case 0: plan = AnchuraSoloJugador_V3(c_state, goal, mapaResultado);
					break;
				
				case 1:
					plan = AnchuraConColaborador(c_state,goal,mapaResultado);
					break;

				case 2:
					plan = DijkstraN2(c_state,goal,mapaResultado);
					break;
				
				case 3:
					plan = ArbolN3(c_state,goal,mapaResultado);
					break;
			}
			if(plan.size() > 0) {
				VisualizaPlan(c_state,plan);
				hayPlan = true;
			}
		}
		if(hayPlan and plan.size() > 0) {
			accion = plan.front();
			plan.pop_front();
		}
		if(plan.size()==0) {
			cout << "Se completo el plan\n";
			hayPlan = false;	
		}
	}
	
	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}
