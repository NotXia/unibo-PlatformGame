#include "ArmedEntity.hpp"
#include "settings.h"

ArmedEntity::ArmedEntity(int health, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon) : Entity(health, head_left, head_right, body, position) {
	this->weapon = weapon;
	is_attacking = false;
	weapon_display_counter = 0;
}

Weapon ArmedEntity::getWeapon() {
	return this->weapon;
}

/*
	Restituisce un oggetto Bullet identico al Bullet associato all'oggetto weapon
*/
Bullet ArmedEntity::attack() {
	if (weapon.hasAmmo()) {
		weapon.startShootDelay();
		setCanMove(false);
		resetWeaponDisplay();
		is_attacking = true;

		Bullet out_bullet = weapon.getBullet();
		out_bullet.setPosition(getFrontPosition());
		out_bullet.setDirection(this->direction);

		return out_bullet;
	}
	else {
		return Bullet(Pixel(), 0, 0, direction);
	}

}

void ArmedEntity::reload() {
	weapon.startReloadDelay();
}


/*
	Incrementa weapon_loop_counter di 1.
	Se supera il limite, viene resettato.
*/
void ArmedEntity::incWeaponDisplay() {
	weapon_display_counter++;
	if (weapon_display_counter > WEAPON_DISPLAY_TIME) {
		weapon_display_counter = 0;
	}
}

/*
	Azzera weapon_loop_counter
*/
void ArmedEntity::resetWeaponDisplay() {
	weapon_display_counter = 0;
	is_attacking = false;
}

/*
	Indica se terminare la visualizzazione dell'arma quando il giocatore attacca
*/
bool ArmedEntity::endWeaponDisplay() {
	if (weapon_display_counter >= WEAPON_DISPLAY_TIME && is_attacking) {
		is_attacking = false;
		return true;
	}
	else {
		return false;
	}
}

/*
	Incrementa i vari contatori
*/
void ArmedEntity::incCounters() {
	Entity::incCounters();
	weapon.incReloadDelay();
	weapon.incShootDelay();
	incWeaponDisplay();
}

/*
	Restituisce true se ci sono le condizioni per attaccare
*/
bool ArmedEntity::canAttack() {
	return weapon.hasAmmo() && !weapon.isReloading() && !weapon.isShooting() && !is_attacking;
}

/*
	Restituisce true se ci sono le condizioni per ricaricare
*/
bool ArmedEntity::canReload() {
	return !weapon.isReloading() && !weapon.isShooting() && !is_attacking;
}

/*
	Se il delay per la ricarica � terminato, ricarica l'arma
*/
bool ArmedEntity::hasReloadFinished() {
	if (weapon.canEndReloadDelay()) {
		weapon.endReload();
		return true;
	}
	return false;
}

/*
	Se il delay per lo sparo � terminato, permette di sparare nuovamente
*/
bool ArmedEntity::hasShootDelayFinished() {
	if (weapon.canEndShootDelay()) {
		weapon.endShoot();
		return true;
	}
	return false;
}