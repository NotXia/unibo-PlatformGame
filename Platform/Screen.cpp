﻿#include "Screen.hpp"
#include <iostream>
#include "colors.h"
using namespace std;

const int GAMEBAR_OFFSET = 3;
const int GAMEBAR_PADDING = 4;

const int WEAPON_WIDTH = 15;
const int WEAPON_HEIGHT = 1;

const int TEXTBOX_HEIGHT = 5;
const int TEXTBOX_MIN_WIDTH = 15;

Screen::Screen() {
	this->console = GetStdHandle(STD_OUTPUT_HANDLE);
	this->hp_x = 1;
	this->hp_y = (GAME_HEIGHT + 2) + GAMEBAR_OFFSET;
	this->weapon_x = 7 + (MAX_LIFE * 2 - 1) + GAMEBAR_PADDING;
	this->weapon_y = this->hp_y;
	this->textBox_x = this->weapon_x + (WEAPON_WIDTH+2) + GAMEBAR_PADDING;
	this->textBox_y = GAME_HEIGHT + 2;
	hideCursor();
}

/*
	Prende in input un intero che rappresenta il colore
	Imposta il colore dei caratteri successivi in quello indicato come parametro
*/
void Screen::setColor(int color) {
	SetConsoleTextAttribute(console, color);
}

/*
	Imposta il colore dei caratteri successivi a quello di default della console
*/
void Screen::resetColor() {
	SetConsoleTextAttribute(console, LIGHTGREY_BLACK);
}

/*
	Prende in input due interi che rappresentano le coordinate
	Muove il cursore a tali coordinate
*/
void Screen::moveCursor(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(console, coord);
}

void Screen::hideCursor() {
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(console, &cursorInfo);
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = false; 
	SetConsoleCursorInfo(console, &cursorInfo);
}

/*
	Inizializza il template del gioco
*/
void Screen::init() {
	system("cls");

	// Area di gioco
	cout <<char(201); // ╔
	for (int i=0; i<GAME_WIDTH; i++) {
		cout <<char(205); // ═
	}
	cout <<char(187) <<endl; // ╗
	for (int i=0; i<GAME_HEIGHT; i++) {
		cout <<char(186); // ║
		moveCursor(GAME_WIDTH+1, 1+i);
		cout <<char(186) <<endl; // ║
	}
	cout <<char(200); // ╚
	for (int i=0; i<GAME_WIDTH; i++) {
		cout <<char(205); // ═
	}
	cout <<char(188); // ╝


	// Barra della vita
	moveCursor(hp_x, hp_y);
	cout <<"HP ";
	setColor(RED_BLACK);
	moveCursor(7, hp_y);
	for (int i=0; i<MAX_LIFE-1; i++) {
		cout <<char(3) <<" "; // ♥
	}
	cout <<char(3); // ♥
	resetColor();


	// Punteggio
	moveCursor(1, hp_y + 2);
	cout <<"Punti 0";


	// Area selettore arma
	moveCursor(weapon_x, weapon_y);
	cout <<char(218); // ┌
	for (int i=0; i<WEAPON_WIDTH; i++) {
		cout <<char(196); // ─
	}
	cout <<char(191); // ┐
	for (int i=0; i<WEAPON_HEIGHT; i++) {
		weapon_y++;
		moveCursor(weapon_x, weapon_y);
		cout <<char(179); // │
		moveCursor(weapon_x+WEAPON_WIDTH+1, weapon_y);
		cout <<char(179); // │
	}
	moveCursor(weapon_x, weapon_y+1);
	cout <<char(192); // ┌
	for (int i=0; i<WEAPON_WIDTH; i++) {
		cout <<char(196); // ─
	}
	cout <<char(217); // ┘


	// Text box
	int textBox_height = 2*GAMEBAR_OFFSET + 3;
	int textBox_width = GAME_WIDTH-textBox_x;
	if (textBox_width < TEXTBOX_MIN_WIDTH) {
		textBox_width = TEXTBOX_MIN_WIDTH;
	}

	moveCursor(textBox_x, textBox_y);
	cout <<char(218); // ┌
	for (int i=0; i<textBox_width; i++) {
		cout <<char(196); // ─
	}
	cout <<char(191); // ┐
	for (int i=0; i<textBox_height-2; i++) {
		textBox_y++;
		moveCursor(textBox_x, textBox_y);
		cout <<char(179); // │
		moveCursor(textBox_x+textBox_width+1, textBox_y);
		cout <<char(179); // │
	}
	moveCursor(textBox_x, textBox_y+1);
	cout <<char(192); // ┌
	for (int i=0; i<textBox_width; i++) {
		cout <<char(196); // ─
	}
	cout <<char(217); // ┘
	
	textBox_y = textBox_y - textBox_height + 2;
}

/*
	Prende in input una matrice di Pixel
	Stampa la matrice nell'area di gioco
*/
void Screen::write_game_area(Pixel terrain[][GAME_HEIGHT]) {
	for (int i=0; i<GAME_HEIGHT; i++) {
		moveCursor(1, 1+i);
		for (int j=0; j<GAME_WIDTH; j++) {
			setColor(terrain[j][i].getColor());
			cout <<terrain[j][i].getValue();
		}
	}
	resetColor();
}

/*
	Prende in input un oggetto di tipo Entity
	Inserisce l'entità nell'area di gioco rivolto verso sinistra
*/
void Screen::write_entity_left(Entity entity) {
	// Corpo
	moveCursor(entity.getBodyPosition().getX(), entity.getBodyPosition().getY());
	setColor(entity.getBody().getColor());
	cout <<entity.getBody().getValue();
	// Testa
	moveCursor(entity.getHeadPosition().getX(), entity.getHeadPosition().getY());
	setColor(entity.getHeadLeft().getColor());
	cout <<entity.getHeadLeft().getValue();
}

/*
	Prende in input un oggetto di tipo Entity
	Inserisce l'entità nell'area di gioco rivolto verso destra
*/
void Screen::write_entity_right(Entity entity) {
	// Corpo
	moveCursor(entity.getBodyPosition().getX(), entity.getBodyPosition().getY());
	setColor(entity.getBody().getColor());
	cout <<entity.getBody().getValue();
	// Testa
	moveCursor(entity.getHeadPosition().getX(), entity.getHeadPosition().getY());
	setColor(entity.getHeadRight().getColor());
	cout <<entity.getHeadRight().getValue();
}

void Screen::resetTerrain(Pixel terrain[][GAME_HEIGHT], Position position) {
	moveCursor(position.getX(), position.getY());
	setColor(terrain[position.getX()-1][position.getY()-1].getColor());
	cout <<terrain[position.getX()-1][position.getY()-1].getValue();
}

void Screen::write_textbox(const char string[]) {
	int start_x = textBox_x+1;
	int start_y = textBox_y+1;

	moveCursor(start_x, start_y);
	setColor(LIGHTGREY_BLACK);
	cout <<string;
}