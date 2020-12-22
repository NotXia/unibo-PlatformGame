#pragma once
#include <windows.h>
#include "settings.h"
#include "Pixel.hpp"
#include "Entity.hpp"
#include "Map.hpp"
#include "EnemyList.hpp"

// Imposta il template del gioco
class Screen {
	protected:
		const int GAMEBAR_OFFSET = 3;
		const int GAMEBAR_PADDING = 4;

		const int WEAPON_WIDTH = 20;
		const int WEAPON_HEIGHT = 1;

		const int TEXTBOX_HEIGHT = 5;
		const int TEXTBOX_MIN_WIDTH = 15;

		const char MONEY_SYMBOL = char(207);

		const int WEAPONBOX_ROTATION_SPEED = 10000;


		HANDLE console;

		int hp_x, hp_y;
		int weapon_x, weapon_y;
		int textBox_x, textBox_y;
		int weaponbox_width, ammobox_width;

		int start_index;
		char weaponbox_text[STRING_LEN];
		bool need_rotate;
		int rotation_counter;

		/*
			Prende in input un intero che rappresenta il colore 
			Imposta il colore dei caratteri successivi in quello indicato come parametro
		*/
		void setColor(int color);

		/*
			Imposta il colore dei caratteri successivi a quello di default della console
		*/
		void resetColor();

		/*
			Prende in input due interi che rappresentano le coordinate
			Muove il cursore a tali coordinate
		*/
		void moveCursor(int x, int y);
		void hideCursor();


	public:
		Screen();

		/*
			Inizializza il template del gioco
		*/
		void init();

		/*
			Prende in input una mappa
			Stampa l'area di gioco
		*/
		void write_game_area(Map *map);

		/*
			Prende in input un Pixel e una Position.
			Imposta il pixel nella posizione indicata.
		*/
		void write_at(Pixel pixel, Position position);

		/*
			Prende in input un oggetto di tipo Entity
			Inserisce l'entit� nell'area di gioco
		*/
		void write_entity(Entity entity);

		/*
			Prende in input una posizione e la mappa.
			Imposta in posizione position il valore previsto dalla mappa.
		*/
		void resetTerrain(Map *map, Position position);

		/*
			Prende in input una stringa.
			Inserisce la stringa nell'area di testo (in basso a destra).
		*/
		void write_textbox(const char string[]);

		/*
			Prende in input un oggetto EnemyList.
			Stampa sullo schermo tutti i nemici
		*/
		void write_enemies(EnemyList list);

		/*
			Prende in input un intero
			Aggiorna la quantit� di soldi visualizzata
		*/
		void write_money(int money);

		/*
			Prende in input un intero
			Aggiorna la quantit� di punti visualizzata
		*/
		void write_points(int points);

		/*
			Prende in input una stringa
			Scrive il parametro nell'area del nome dell'arma. Se la stringa � troppo lunga, vengono inizializzati i parametri per la rotazione
		*/
		void write_weaponbox(char name[]);

		/*
			Incrementa di 1 rotation_counter
		*/
		void incWeaponboxRotateCounter();

		/*
			Restituisce true se rotation_counter ha raggiunto WEAPONBOX_ROTATION_SPEED e need_rotate � true
		*/
		bool canRotateWeaponbox();

		/*
			Incrementa start_index e stampa nell'area dell'arma la stringa weaponbox_text a partire da quell'indice.
			Se start_index supera la dimensione della stringa, ricomincia da capo.
		*/
		void rotate_weaponbox();

		/*
			Prende in input un intero.
			Aggiorna la quantit� di munizioni visualizzata
		*/
		void write_ammobox(int ammo);


};