#include "../Comportamientos_Jugador/jugador.hpp"
#include <iostream>
using namespace std;

void PonerTerrenoEnMatriz( const vector<unsigned char> &terreno, const state &st, vector<vector<unsigned char>> &matriz, bool *nuevos) {
	matriz[st.fil][st.col] = terreno[0];
	int index = 0;
	switch(st.brujula){
		case norte:
			for(int i = 0 ; i < 4; i++ ) {
				for(int j = - i ; j <= i ; j++) {
					if(matriz[st.fil-i][st.col+j] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil-i][st.col+j] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case noreste:
			for(int i = 0 ; i < 4; i++ ) {
				for (int j = 0; j <= i; j++ ) {
					if(matriz[st.fil-i][st.col+j] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil-i][st.col+j] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
				for (int j = i-1; j >= 0; j-- ) {
					if(matriz[st.fil-j][st.col+i] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil-j][st.col+i] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case este:
			for(int i = 0 ; i < 4; i++ ) {
				for(int j = - i ; j <= i ; j++) {
					if (matriz[st.fil+j][st.col+i] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil+j][st.col+i] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case sureste:
			for(int i = 0 ; i < 4; i++ ) {
				for (int j = 0; j <= i; j++ ) {
					if (matriz[st.fil+j][st.col+i] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil+j][st.col+i] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
				for (int j = i-1; j >= 0; j-- ) {
					if (matriz[st.fil+i][st.col+j] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil+i][st.col+j] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case sur:
			for(int i = 0 ; i < 4; i++ ) {
				for(int j = - i ; j <= i ; j++) {
					if (matriz[st.fil+i][st.col-j] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil+i][st.col-j] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case suroeste:
			for(int i = 0 ; i < 4; i++ ) {
				for (int j = 0; j <= i; j++ ) {
					if (matriz[st.fil+i][st.col-j] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil+i][st.col-j] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
				for (int j = i-1; j >= 0; j-- ) {
					if (matriz[st.fil+j][st.col-i] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil+j][st.col-i] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case oeste:
			for(int i = 0 ; i < 4; i++ ) {
				for(int j = - i ; j <= i ; j++) {
					if (matriz[st.fil-j][st.col-i] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil-j][st.col-i] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
		
		case noroeste:
			for(int i = 0 ; i < 4; i++ ) {
				for (int j = 0; j <= i; j++ ) {
					if (matriz[st.fil-j][st.col-i] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil-j][st.col-i] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
				for (int j = i-1; j >= 0; j-- ) {
					if (matriz[st.fil-i][st.col-j] != terreno[index] && terreno[index] != '?') {
						matriz[st.fil-i][st.col-j] = terreno[index];
						nuevos[index] = true;
					} else nuevos[index] = false;
					index++;
				}
			}
			break;
	}
}

bool transitable(unsigned char terreno, unsigned char agente, bool bikini, bool zapatillas) {
	bool casilla = ( terreno=='T' || terreno=='S' || terreno=='G' || terreno =='K' || terreno =='D' );
	bool cas_esp = (terreno=='B' && zapatillas) || (terreno=='A' && bikini);
	bool agentes = (agente=='_');

	return (casilla || cas_esp) && agentes;
}

bool transit_correr(unsigned char terreno1, unsigned char terreno2, unsigned char agente1, unsigned char agente2, bool bikini, bool zapatillas) {
	bool intermedio = ( terreno1 != 'P' && terreno1 != 'M') && ( agente1 == '_');
	bool final = transitable(terreno2,agente2,bikini,zapatillas);
	return intermedio && final;
}

bool transit_urgente(unsigned char terreno, unsigned char agente) {
	bool casilla = ( terreno != 'M' && terreno != 'P');
	bool agentes = ( agente == '_');
	return casilla && agente;
}

int casillaALaVista(vector<unsigned char> terreno, unsigned char casilla) {
	int pos = -1;
	for (int i = 1; i < 15 && pos == -1; i++) {
		if (terreno[i] == casilla) pos = i;
	}
	return pos;
}

bool rodeado(vector<vector<unsigned char>> &matriz, state &st, bool zapatillas, bool bikini) {
	bool resp = true;
	for( int i = -1; i < 2 && resp; i++) {
		for(int j = -1; j < 2 && resp; j++) {
			if ( i != 0 || j != 0) {
				resp = false;
				if (!bikini)
					resp = matriz[st.fil+i][st.col+j] == 'A';
				if (!zapatillas)
					resp = (resp || matriz[st.fil+i][st.col+j] == 'B');
				resp = resp || (matriz[st.fil+i][st.col+j] == 'P') || (matriz[st.fil+i][st.col+j] == 'M');
			}
		}
	}
	return resp;
}

int irA(int fil, int col, state &st) {
	if ( fil > st.fil) {
		if ( col > st.col) {
			switch(st.brujula) {
				case norte: case noreste: case noroeste: case este:
					return 15;
				case oeste: case suroeste: case sur:
					return 9;
				case sureste:
					return 12;
			}
		}
		else if (col == st.col) {
			switch(st.brujula) {
				case norte: case noreste: case este: case sureste:
					return 15;
				case oeste: case noroeste: case suroeste:
					return 9;
				case sur:
					return 12;
			}

		}

		else {
			switch(st.brujula) {
				case sur: case sureste: case este: case noreste:
					return 15;
				case norte: case noroeste: case oeste:
					return 9;
				case suroeste:
					return 12;
			}

		}
	}

	else if (fil == st.fil) {
		if ( col > st.col) {
			switch(st.brujula) {
				case norte: case noreste: case noroeste: case oeste:
					return 15;
				case sur: case suroeste: case sureste:
					return 9;
				case este:
					return 12;
			}
		}
		else if (col == st.col)
			return 0;

		else {
			switch(st.brujula) {
				case sur: case sureste: case este: case suroeste:
					return 15;
				case norte: case noroeste: case noreste:
					return 9;
				case oeste:
					return 12;
			}

		}
	}

	else {
		
		if ( col > st.col) {
			switch(st.brujula) {
				case norte: case noroeste: case este: case suroeste:
					return 15;
				case sur: case sureste: case oeste:
					return 9;
				case noreste:
					return 12;
			}
		}
		else if (col == st.col) {
			switch(st.brujula) {
				case noroeste: case oeste: case suroeste: case sur:
					return 15;
				case sureste: case este: case noreste:
					return 9;
				case norte:
					return 12;
			}

		}

		else {
			switch(st.brujula) {
				case oeste: case suroeste: case sur: case sureste:
					return 15;
				case este: case noreste: case norte:
					return 9;
				case noroeste:
					return 12;
			}

		}


	}
	
}

bool nuevoDelante(vector<vector<unsigned char>> &mapaResultado, state &st) {
	if(st.col != -1 && st.fil != -1) {
		bool nuevo = false;
		int index, index2;
		switch(st.brujula) {
			case norte:
				index = st.fil;
				while(index >= 0 && !nuevo) {
					if(mapaResultado[index][st.col] == '?')
						nuevo = true;
					else if(st.col > 0) {
						if(mapaResultado[index][st.col-1] == '?')
						nuevo = true;
					}
					else if(st.col < mapaResultado[0].size()-1) {
						if(mapaResultado[index][st.col+1] == '?')
						nuevo = true;
					}
					index--;
				}
				break;
			case sur:
				index = st.fil;
				while(index < mapaResultado.size() && !nuevo) {
					if(mapaResultado[index][st.col] == '?')
							nuevo = true;
					else if(st.col > 0) {
						if(mapaResultado[index][st.col-1] == '?')
							nuevo = true;
					}
					else if(st.col < mapaResultado[0].size()-1) {
						if(mapaResultado[index][st.col+1] == '?')
							nuevo = true;
					}
					index++;
				}
				break;
			case oeste:
				index = st.col;
				while(index >= 0 && !nuevo) {
					if(mapaResultado[st.fil][index] == '?')
						nuevo = true;
					else if(st.fil > 0) {
						if(mapaResultado[st.fil-1][index] == '?')
							nuevo = true;
					}
					else if(st.fil < mapaResultado.size()-1) {
						if(mapaResultado[st.fil+1][index] == '?')
							nuevo = true;
					}
					index--;
				}
				break;
			case este:
				index = st.col;
				while(index < mapaResultado[0].size() && !nuevo) {
					if(mapaResultado[st.fil][index] == '?')
						nuevo = true;
					else if(st.fil > 0) {
						if(mapaResultado[st.fil-1][index] == '?')
							nuevo = true;
					}
					else if(st.fil < mapaResultado.size()-1) {
						if(mapaResultado[st.fil+1][index] == '?')
							nuevo = true;
					}
					index++;
				}
				break;
			
			case noroeste:
				index = st.fil;
				index2 = st.col;
				while(index >= 0 && index2 >= 0 && !nuevo) {
					if(mapaResultado[index][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index+1][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index][index2+1] == '?')
						nuevo = true;
					else {
						index--;
						index2--;
					}
				}
				break;
			case noreste:
				index = st.fil;
				index2 = st.col;
				while(index >= 0 && index2 < mapaResultado[0].size() && !nuevo) {
					if(mapaResultado[index][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index+1][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index][index2-1] == '?')
						nuevo = true;
					else {
						index--;
						index2++;
					}
				}
				break;
			case sureste:
				index = st.fil;
				index2 = st.col;
				while(index < mapaResultado.size() && index2 < mapaResultado[0].size() && !nuevo) {
					if(mapaResultado[index][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index-1][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index][index2-1] == '?')
						nuevo = true;
					else {
						index++;
						index2++;
					}
				}
				break;
			case suroeste:
				index = st.fil;
				index2 = st.col;
				while(index < mapaResultado.size() && index2 >= 0 && !nuevo) {
					if(mapaResultado[index][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index-1][index2] == '?')
						nuevo = true;
					else if(mapaResultado[index][index2+1] == '?')
						nuevo = true;	
					else {
						index++;
						index2--;
					}
				}
				break;

		}

		return nuevo;
	}

	return true;
}

casilla buscarCasillaNueva(vector<vector<unsigned char>> &mapaResultado, state &st) {
	casilla buscada;
	buscada.col = buscada.fil = -1;
	int radio = 1;
	bool dentro_mapa = true, salir = false;;
	while(dentro_mapa && !salir) {
		dentro_mapa = false;

		if(st.fil-radio >= 0) {
			for(int i = st.col-radio; i<st.col+radio+1 && !salir; i++) {
				if (i >= 0 && i < mapaResultado[0].size()) {
					dentro_mapa = true;
					if (mapaResultado[st.fil-radio][i]=='?') {
						buscada.fil = st.fil-radio;
						buscada.col = i;
						salir = true;
					}
				}
			}
		}
		
		if(st.fil+radio < mapaResultado.size()) {
			for(int i = st.col-radio; i < st.col+radio+1 && !salir; i++) {
				if (i >= 0 && i < mapaResultado[0].size()) {
					dentro_mapa = true;
					if (mapaResultado[st.fil+radio][i]=='?') {
						buscada.fil = st.fil+radio;
						buscada.col = i;
						salir = true;
					}
				}
			}
		}

		if(st.col-radio >= 0) {
			for(int i = st.fil-radio; i < st.fil+radio+1 && !salir; i++) {
				if (i >= 0 && i < mapaResultado.size()) {
					dentro_mapa = true;
					if (mapaResultado[i][st.col-radio]=='?') {
						buscada.fil = i;
						buscada.col = st.col-radio;
						salir = true;
					}
				}
			}
		}

		if(st.col+radio < mapaResultado[0].size()) {
			for(int i = st.fil-radio; i < st.fil+radio+1 && !salir; i++) {
				if (i >= 0 && i < mapaResultado.size()) {
					dentro_mapa = true;
					if (mapaResultado[i][st.col+radio]=='?') {
						buscada.fil = i;
						buscada.col = st.col+radio;
						salir = true;
					}
				}
			}
		}
		radio++;
	}

	return buscada;
}

casilla coordenadasDe(unsigned int index, state &st) {
	int altura = 0;
	int ancho = 0;
	casilla coordenadas;
	if ( index < 4) {
		altura = 1;
		ancho = index - 2;
	}
	else if ( index < 9) {
		altura = 2;
		ancho = index - 6;
	}
	else {
		altura = 3;
		ancho = index - 12;
	}

	switch(st.brujula) {
		case norte:
			coordenadas.fil = st.fil-altura;
			coordenadas.col = st.col+ancho;
			break;
		case noreste:
			if(ancho <= 0) {
				coordenadas.fil = st.fil-altura;
				coordenadas.col = st.col+ancho+altura;
			}
			else {
				coordenadas.col = st.col+altura;
				coordenadas.fil = st.fil-altura+ancho;
			}
			break;
		case este:
			coordenadas.fil = st.fil+ancho;
			coordenadas.col = st.col+altura;
			break;
		case sureste:
			if(ancho <= 0) {
				coordenadas.fil = st.fil+altura+ancho;
				coordenadas.col = st.col+altura;
			}
			else {
				coordenadas.col = st.col+altura-ancho;
				coordenadas.fil = st.fil+altura;
			}
			break;
		case sur:
			coordenadas.fil = st.fil+altura;
			coordenadas.col = st.col-ancho;
			break;
		case suroeste:
			if(ancho <= 0) {
				coordenadas.fil = st.fil+altura;
				coordenadas.col = st.col-altura-ancho;
			}
			else {
				coordenadas.col = st.col-altura;
				coordenadas.fil = st.fil+altura-ancho;
			}
			break;
		case oeste:
			coordenadas.fil = st.fil-ancho;
			coordenadas.col = st.col-altura;
			break;
		case noroeste:
			if(ancho <= 0) {
				coordenadas.fil = st.fil-altura-ancho;
				coordenadas.col = st.col-altura;
			}
			else {
				coordenadas.col = st.col-altura+ancho;
				coordenadas.fil = st.fil-altura;
			}
			break;
	}
	return coordenadas;
}

bool estaEn(casilla pos, casilla *ar, int iter, const int TAM_AR) {
	bool esta = false;
	int i = (iter+1)%TAM_AR;
	while(!esta && i != iter) {
		if(ar[i].fil == pos.fil && ar[i].col == pos.col)
			esta = true;
		else
			i = (i+1)%TAM_AR;
	}
	return esta;
}

Action ComportamientoJugador::think(Sensores sensores)
{
	int buscado = -1;
	Action accion = actIDLE;

	if(!bordes) {
		int alto = mapaResultado.size();
		int ancho = mapaResultado[0].size();
		for(int i = 0; i < alto; i++) {
			mapaResultado[i][0] = 'P';
			mapaResultado[i][1] = 'P';
			mapaResultado[i][2] = 'P';

			mapaResultado[i][ancho-1] = 'P';
			mapaResultado[i][ancho-2] = 'P';
			mapaResultado[i][ancho-3] = 'P';
		}

		for(int i = 3; i < ancho-3; i++) {
			mapaResultado[0][i] = 'P';
			mapaResultado[1][i] = 'P';
			mapaResultado[2][i] = 'P';

			mapaResultado[alto-1][i] = 'P';
			mapaResultado[alto-2][i] = 'P';
			mapaResultado[alto-3][i] = 'P';
		}
		bordes = true;
	}

	if(sensores.nivel == 0 && !bien_situado) {
		bien_situado = true;
		current_state.brujula = sensores.sentido;
		current_state.col = sensores.posC;
		current_state.fil = sensores.posF;
	}

	// Actualización de las variables de estado
	if (!sensores.colision && !sensores.reset) {
		switch(last_action){

			case actTURN_SR:
				current_state.brujula = static_cast<Orientacion>((current_state.brujula+1)%8);
				turnos_quieto++;
				if(bien_situado)
					atrapado = rodeado(mapaResultado,current_state,zapatillas,bikini);
				else {
					atrapado = ( turnos_quieto >= 4);
				}
				break;

			case actTURN_L:
				current_state.brujula = static_cast<Orientacion>((current_state.brujula+6)%8);
				turnos_quieto++;
				if(bien_situado)
					atrapado = rodeado(mapaResultado,current_state,zapatillas,bikini);
				else {
					atrapado = ( turnos_quieto >= 4);
				}
				break;

			case actWALK:
				switch(current_state.brujula){
					case norte:		current_state.fil--;	break;
					case noreste:	current_state.fil--;	current_state.col++;	break;
					case este:		current_state.col++;	break;
					case sureste:	current_state.fil++;	current_state.col++;	break;
					case sur:		current_state.fil++;	break;
					case suroeste:	current_state.fil++;	current_state.col--;	break;
					case oeste:		current_state.col--;	break;
					case noroeste:	current_state.fil--;	current_state.col--;	break;
				}
				turnos_quieto = 0;
				visitadas[visitadas_iter].fil = current_state.fil;
				visitadas[visitadas_iter].col = current_state.col;
				visitadas_iter = (visitadas_iter+1)%TAM_VISITADAS; 
				break;

			case actRUN:
				switch(current_state.brujula){
					case norte:		current_state.fil-=2;	break;
					case noreste:	current_state.fil-=2;	current_state.col+=2;	break;
					case este:		current_state.col+=2;	break;
					case sureste:	current_state.fil+=2;	current_state.col+=2;	break;
					case sur:		current_state.fil+=2;	break;
					case suroeste:	current_state.fil+=2;	current_state.col-=2;	break;
					case oeste:		current_state.col-=2;	break;
					case noroeste:	current_state.fil-=2;	current_state.col-=2;	break;
				}
				turnos_quieto = 0;
				visitadas[visitadas_iter].fil = current_state.fil;
				visitadas[visitadas_iter].col = current_state.col;
				visitadas_iter = (visitadas_iter+1)%TAM_VISITADAS;
				break;
		}
	}

	if(sensores.reset) {
		bien_situado = false;
		bikini = false;
		zapatillas = false;
		predet_action = actIDLE;
		hay_predet = false;
	}

	if(atrapado) {
		turnos_escape = 1;
		atrapado = false;
	}

	switch(sensores.terreno[0]) {
		case 'G':
			if (!bien_situado){
				current_state.fil = sensores.posF;
				current_state.col= sensores.posC;
				current_state.brujula = sensores.sentido;
				bien_situado = true;
			}
			break;

		case 'K':
			if(!bikini)
				bikini = true;
			break;
		
		case 'D':
			if(!zapatillas)
				zapatillas = true;
			break;
		
		case 'X':
			if(bien_situado) {
				posXcol = current_state.col;
				posXfil = current_state.fil;
			}
			if(sensores.bateria < 750) {
				predet_action = actIDLE;
				hay_predet = true;
			}
			break;
		
		case 'B':
			if(!zapatillas) {
				turnos_escape = 1;
			}
			break;
		
		case 'A':
			if(!bikini) {
				turnos_escape = 1;
			}
			break;
	}

	if (bien_situado) {
		//mapaResultado[current_state.fil][current_state.col] = sensores.terreno[0];
		PonerTerrenoEnMatriz(sensores.terreno,current_state,mapaResultado,nuevos);
	} else {
		buscado = casillaALaVista(sensores.terreno, 'G');
	}
	if (!bikini && buscado == -1)
		buscado = casillaALaVista(sensores.terreno, 'K');
	if (!zapatillas && buscado == -1)
		buscado = casillaALaVista(sensores.terreno, 'D');
	
	if( sensores.bateria < 100 && posXcol != -1 && buscado == -1 && bien_situado) 	// Batería baja
		buscado = irA(posXfil,posXcol,current_state);
	
	int charger = casillaALaVista(sensores.terreno, 'X');
	if(charger!=-1) {
		casilla cargador = coordenadasDe(charger,current_state);
		posXcol = cargador.col;
		posXfil = cargador.fil;
	}
	
	/*if(buscado == -1 && casilla_obj.col != -1)
		buscado = irA(casilla_obj.fil,casilla_obj.col,current_state);*/
	// HACER PROTOCOLO DE ESCAPE DE CASA: entrandoencasa/saliendodecasa

	// Decisión de la siguiente acción

	/*if (nuevos[12]||nuevos[11]||nuevos[13]||nuevos[2])			// En función de donde se ha descubierto territorio
		seguir_recto = true;
	else if (nuevos[14] || nuevos[15]||nuevos[7])
		girar_derecha = true;
	else if(nuevos[9] || nuevos[10]||nuevos[5])
		girar_izquierda = true;*/

	if(hay_predet) {	// Accion solicitada por el think anterior
		if(predet_action != actWALK || transit_urgente(sensores.terreno[2],sensores.agentes[2])) {
			accion = predet_action;
		}
		predet_action = actIDLE;
		hay_predet = false;
	}
	else if (turnos_escape > 0) {
		if(transitable(sensores.terreno[1],sensores.agentes[1],bikini,zapatillas) && transit_urgente(sensores.terreno[2],sensores.agentes[2])) {
			accion = actWALK;
			turnos_escape--;
			predet_action = actTURN_L;
			hay_predet = true;
		}
		else if(transitable(sensores.terreno[1],sensores.agentes[1],bikini,zapatillas)) {
			accion = actTURN_L;
			predet_action = actTURN_SR;
			hay_predet = true;
		}
		else if(transitable(sensores.terreno[3],sensores.agentes[3],bikini,zapatillas)) {
			accion = actTURN_SR;
		}
		else if(transit_urgente(sensores.terreno[2],sensores.agentes[2])) {
			accion = actWALK;
			turnos_escape--;
		}
		else {
			accion = actTURN_SR;
		}
	}
	else if (buscado == -1 && bien_situado) { // Movimiento en funcion de donde se ha descubierto territorio
		if ( seguir_recto && transitable(sensores.terreno[2],sensores.agentes[2],bikini,zapatillas)){
			accion = actWALK;
			seguir_recto = false;
		}
		else if (seguir_recto && transit_correr(sensores.terreno[2],sensores.terreno[6],sensores.agentes[2],sensores.agentes[6],bikini,zapatillas)) {
			accion = actRUN;
			seguir_recto = false;
		}
		else if( girar_derecha ) {
			accion = actTURN_SR;
			girar_derecha = false;
		}
		else if ( girar_izquierda ) {
			accion = actTURN_L;
			girar_izquierda = false;
		}
		else {		// En caso de todo el terreno detectado conocido

			if(nuevoDelante(mapaResultado,current_state)){
				if(transitable(sensores.terreno[2],sensores.agentes[2],bikini,zapatillas) && 
					!estaEn(coordenadasDe(2,current_state),visitadas,visitadas_iter,TAM_VISITADAS)) {
					accion = actWALK;
				}
				else if(transit_correr(sensores.terreno[2],sensores.terreno[6],sensores.agentes[2],sensores.agentes[6],bikini,zapatillas) &&
					!estaEn(coordenadasDe(6,current_state),visitadas,visitadas_iter,TAM_VISITADAS)) {
					accion = actRUN;
				}
				else if(transitable(sensores.terreno[3],sensores.agentes[3],bikini,zapatillas)&&
					!estaEn(coordenadasDe(3,current_state),visitadas,visitadas_iter,TAM_VISITADAS)) {
					accion = actTURN_SR;
					predet_action = actWALK;
					hay_predet = true;
				}
				else if(transitable(sensores.terreno[1],sensores.agentes[1],bikini,zapatillas)&&
					!estaEn(coordenadasDe(1,current_state),visitadas,visitadas_iter,TAM_VISITADAS)) {
					accion = actTURN_L;
					predet_action = actTURN_SR;
					hay_predet = true;
				}
				else {
					if(transitable(sensores.terreno[2],sensores.agentes[2],bikini,zapatillas)) {
						accion = actWALK;
					}
					else if(transit_correr(sensores.terreno[2],sensores.terreno[6],sensores.agentes[2],sensores.agentes[6],bikini,zapatillas)) {
						accion = actRUN;
					}
					else if(transitable(sensores.terreno[3],sensores.agentes[3],bikini,zapatillas)) {
						accion = actTURN_SR;
						predet_action = actWALK;
						hay_predet = true;
					}
					else if(transitable(sensores.terreno[1],sensores.agentes[1],bikini,zapatillas)) {
						accion = actTURN_L;
						predet_action = actTURN_SR;
						hay_predet = true;
					}
					else {
						if(transit_urgente(sensores.terreno[2],sensores.agentes[2])) {
							accion = actWALK;
						}
						else {
							accion = actTURN_SR;
						}
					}
				}
			}
			else {				
				if(transitable(sensores.terreno[2],sensores.agentes[2],bikini,zapatillas) && rand()%2 == 0) {
					accion = actWALK;
				}
				else {
					if(rand()%2==0)
						accion = actTURN_SR;
					else
						accion = actTURN_L;
				}
			}
			/*if ( (nuevoDelante(mapaResultado,current_state) || last_action == actTURN_L || last_action == actTURN_SR) 
				&& transitable(sensores.terreno[2],sensores.agentes[2],bikini,zapatillas)){
				accion = actWALK;
			}
			else if ((nuevoDelante(mapaResultado,current_state) || last_action == actTURN_L || last_action == actTURN_SR)
				&& transit_correr(sensores.terreno[2],sensores.terreno[6],sensores.agentes[2],sensores.agentes[6],bikini,zapatillas)){
				accion = actRUN;
			}
			else if (transitable(sensores.terreno[3],sensores.agentes[3],bikini,zapatillas))
				accion = actTURN_SR;
			else if(transitable(sensores.terreno[1],sensores.agentes[1],bikini,zapatillas)) {
				accion = actTURN_L;
				predet_action = actTURN_SR;
			}
			else if (!transit_urgente(sensores.terreno[1],sensores.agentes[1])&&!transit_urgente(sensores.terreno[2],sensores.agentes[2])&&
				!transit_urgente(sensores.terreno[3],sensores.agentes[3])) {		
				accion = actTURN_L;
				predet_action = actWALK;
			}
			/*else if(rand()%2 == 0){
				accion = actTURN_L;
			}
			else {
				accion = actTURN_L;
			}*/
		}
	}
	else if (buscado != -1){ // Casilla buscada
		switch(buscado) {
			case 1: case 4: case 9:
				accion = actTURN_L;
				break;
			
			case 3: case 8: case 15:
				accion = actTURN_SR;
				break;
			case 2: case 6: case 12:
				if(transit_urgente(sensores.terreno[2],sensores.agentes[2]))
					accion = actWALK;
				else if(sensores.agentes[2] == 'a' && last_action != actIDLE)
					accion = actIDLE;
				else {
					accion = actTURN_L;
					predet_action = actTURN_L;	// actWALK
					hay_predet = true;
				}
				break;
			case 5: case 10: case 11:
				if(transit_urgente(sensores.terreno[2],sensores.agentes[2]))
					accion = actWALK;
				else {
					accion = actTURN_L;
					predet_action = actWALK;
					hay_predet = true;
				}
				break;
			case 7: case 13: case 14:
				if(transit_urgente(sensores.terreno[2],sensores.agentes[2]))
					accion = actWALK;
				else {
					accion = actTURN_SR;
					predet_action = actWALK;
					hay_predet = true;
				}
				break;
			case 0:
				accion = actIDLE;
				break;

		}
	}
	else {		// !bien_situado && buscado == -1
		/*if(transitable(sensores.terreno[3],sensores.agentes[3],bikini,zapatillas)) {
			accion = actTURN_SR;
			predet_action = actWALK;
			hay_predet = true;
		}
		else if(transitable(sensores.terreno[1],sensores.agentes[1],bikini,zapatillas)){
			accion = actTURN_L;
			predet_action = actTURN_SR;
			hay_predet = true;
		}*/
		if(transitable(sensores.terreno[2],sensores.agentes[2],bikini,zapatillas)) {
			accion = actWALK;
		}
		else if(transit_correr(sensores.terreno[2],sensores.terreno[6],sensores.agentes[2],sensores.agentes[6],bikini,zapatillas)) {
			accion = actRUN;
		}
		else {
			if(rand()%2==0)
				accion = actTURN_SR;
			else
				accion = actTURN_L;
		}
	}

	last_action = accion;

	// Mostrar el valor de los sensores
	/*cout << "Posicion: fila " << sensores.posF << " columna " << sensores.posC;
	switch (sensores.sentido)
	{
		case norte:	  cout << " Norte\n";	break;
		case noreste: cout << " Noreste\n";	break;
		case este:    cout << " Este\n";	break;
		case sureste: cout << " Sureste\n";	break;
		case sur:     cout << " Sur\n";	break;
		case suroeste:cout << " Suroeste\n";	break;
		case oeste:   cout << " Oeste\n";	break;
		case noroeste:cout << " Noroeste\n";	break;
	}
	cout << "Terreno: ";
	for (int i=0; i<sensores.terreno.size(); i++)
		cout << sensores.terreno[i];

	cout << "  Agentes: ";
	for (int i=0; i<sensores.agentes.size(); i++)
		cout << sensores.agentes[i];

	cout << "\nColision: " << sensores.colision;
	cout << "  Reset: " << sensores.reset;
	cout << "  Vida: " << sensores.vida << endl<< endl;*/

	return accion;
}

int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}