#include "Bullet.hpp"
#include "settings.h"

Bullet::Bullet(Pixel texture, int damage, int range, Position position, bool direction) {
	this->textureLeft = texture;
	this->textureRight = texture;
	this->damage = damage;
	this->range = range;
	this->position = position;
	this->direction = direction;
	this->refreshTime = 1500;
	this->currRefresh = 0;
	this->hostile = false;
}

Bullet::Bullet(Pixel textureLeft, Pixel textureRight, int damage, int range, Position position, bool direction) {
	this->textureLeft = textureLeft;
	this->textureRight = textureRight;
	this->damage = damage;
	this->range = range;
	this->position = position;
	this->direction = direction;
	this->refreshTime = 1500;
	this->currRefresh = 0;
	this->hostile = false;
}

Pixel Bullet::getTexture() {
	if (direction == BULLET_LEFT) {
		return textureLeft;
	}
	else {
		return textureRight;
	}
}
int Bullet::getDamage() {
	return damage;
}
int Bullet::getRange() {
	return range;
}
void Bullet::setHostile(bool hostile) {
	this->hostile = hostile;
}
bool Bullet::isHostile() {
	return this->hostile;
}
Position Bullet::getPosition() {
	return this->position;
}
void Bullet::setPosition(Position position) {
	this->position = position;
}
void Bullet::setDirection(bool direction) {
	this->direction = direction;
}

/*
	Restituisce true se il range � maggiore di 0, false altrimenti
*/
bool Bullet::canTravel() {
	return range > 0;
}

/*
	Se range � maggiore di 0: decrementa range e muove la posizione di 1 (in base all direzione).
	In caso la posizione dovesse superare i limiti, range viene impostato ad un valore nullo.
	Restituisce true se il range � maggiore di 0, false altrimenti.
*/
bool Bullet::travel() {
	if (canTravel()) {
		range--;
		if (direction == BULLET_LEFT) {
			if (position.getX() == 1) {
				range = -1;
			}
			else {
				position.setX(position.getX()-1);
			}
		}
		else {
			if (position.getX() == GAME_WIDTH) {
				range = -1;
			}
			else {
				position.setX(position.getX()+1);
			}
		}
	}

	return canTravel();
}

/*
	Indica se � possibile aggiornare lo stato del Bullet
*/
bool Bullet::canRefresh() {
	return currRefresh >= refreshTime;
}

/*
	Incrementa  currRefresh di 1. Se supera refreshTime, viene azzerato
*/
void Bullet::incRefresh() {
	currRefresh++;
	if (currRefresh > refreshTime) {
		currRefresh = 0;
	}
}

/*
	Imposta range ad un valore negativo e restituisce il danno del proiettile
*/
int Bullet::hit() {
	range = -1;
	return damage;
}