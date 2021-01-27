#include "Boss.hpp"
#include <cstdlib>
#include "Map.hpp"

const Pixel STUNNED_BOSS_HEAD_LEFT_TEXTURE = Pixel('?', STUNNED_BOSS_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel STUNNED_BOSS_HEAD_RIGHT_TEXTURE = Pixel('?', STUNNED_BOSS_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel STUNNED_BOSS_BODY_TEXTURE = Pixel(ENEMY_BODY, STUNNED_BOSS_BODY_COLOR_FG, BACKGROUND_DEFAULT, true);

const Pixel BOSS_HEAD_LEFT_TEXTURE = Pixel(ENEMY_HEAD_LEFT, ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel BOSS_HEAD_RIGHT_TEXTURE = Pixel(ENEMY_HEAD_RIGHT, ANGRY_ENEMY_HEAD_COLOR_FG, BACKGROUND_DEFAULT, true);
const Pixel BOSS_BODY_TEXTURE = Pixel(ENEMY_BODY, ANGRY_ENEMY_BODY_COLOR_FG, BACKGROUND_DEFAULT, true);

Boss::Boss(int health, int points, int money, Pixel head_left, Pixel head_right, Pixel body, Position position, Weapon weapon, int type, int ability_num, int ability_max) :
	Enemy(health, points, money, head_left, head_right, body, position, weapon) {
	this->type = type;
	this->phase = 0;
	this->max_phase = 5;
	this->ability_num = ability_num;
	this->ability_max = ability_max;
	this->down_time = AnimationTimer(45);
	this->pause_time = AnimationTimer(20);
	this->refresh = AnimationTimer(1500);

	default_visualRange = GAME_WIDTH;
	visualRange = GAME_WIDTH;
	max_jump_height = GAME_HEIGHT;

}

Position Boss::getBody2Position() {
	Position out = Enemy::getBodyPosition();
	out.setY(out.getY()-1);
	return out;
}

Position Boss::getHeadPosition() {
	Position out = Enemy::getHeadPosition();
	out.setY(out.getY()-1);
	return out;
}

int Boss::getType() {
	return type;
}

int Boss::getAbilityNum() {
	return ability_num;
}
void Boss::setAbilityNum(int ability_num) {
	if (ability_num > ability_max) {
		ability_num = ability_max;
	}
	this->ability_num = ability_num;
}

int Boss::getPhase() {
	return phase;
}

/*
	Prende in input un oggetto Position.
	Restituisce true se il boss � presente in quella posizione
*/
bool Boss::existsAt(Position position) {
	return getHeadPosition().equals(position) || getBodyPosition().equals(position) || getBody2Position().equals(position);
}

/*
	Incrementa phase di 1.
	Se supera max_phase, azzera phase.
*/
void Boss::nextPhase() {
	phase++;
	if (phase > max_phase) {
		phase = 0;
	}
}

/*
	Prende in input un oggetto Map e un oggetto Player.
	Restituisce il codice dell'azione da eseguire
*/
int Boss::getAction(Map *map, Player player) {
	int action_code = ACTION_DO_NOTHING;
	int weapon_range = 0;

	/* Boss SUMMONER */
	if (type == BOSS_SUMMONER) {
		if (phase == 0) {					// Fase 0: Pausa
			if (pause_time.limit()) {
				nextPhase();
			}
			pause_time.incTimer();
		}
		else if (phase == 1) {				// Fase 1: Attacco: evoca dei nemici
			action_code = ACTION_ATTACK;
			setAbilityNum(ability_num+1);
			nextPhase();
		}
		else if (phase == 2) {				// Fase 2: Va avanti e indietro
			if (map->getEnemyList().size() == 0) {
				nextPhase();
			}
			else if (!map->isSolidAt(getBodyFrontPosition())) {
				if (direction == DIRECTION_LEFT) {
					action_code = ACTION_GO_LEFT;
				}
				else {
					action_code = ACTION_GO_RIGHT;
				}
			}
			else if (map->isSolidAt(getBodyFrontPosition())) {
				if (direction == DIRECTION_LEFT) {
					action_code = ACTION_GO_RIGHT;
				}
				else {
					action_code = ACTION_GO_LEFT;
				}
			}
		}
		else if (phase == 3) {				// Fase 3: Nemici sconfitti -> Cade + inizio timer down_time
			action_code = ACTION_FALL;
			down_time.reset();
			nextPhase();
			this->body = STUNNED_BOSS_BODY_TEXTURE;
			this->head_left = STUNNED_BOSS_HEAD_LEFT_TEXTURE;
			this->head_right = STUNNED_BOSS_HEAD_RIGHT_TEXTURE;
		}
		else if (phase == 4) {				// Fase 4: Stordito
			action_code = ACTION_DO_NOTHING;
			down_time.incTimer();
			if (down_time.limit()) {
				this->body = BOSS_BODY_TEXTURE;
				this->head_left = BOSS_HEAD_LEFT_TEXTURE;
				this->head_right = BOSS_HEAD_RIGHT_TEXTURE;
				nextPhase();
			}
		}
		else if (phase == 5) {				// Fase 5: Fine stordimento -> Salta
			action_code = ACTION_JUMP;
			pause_time.reset();
			nextPhase();
		}
	}
	/* Boss MAGE */
	else if (type == BOSS_MAGE) {
		if (phase == 0) {					// Fase 0: Pausa
			if (pause_time.limit()) {
				nextPhase();
			}
			pause_time.incTimer();
		}
		else if (phase == 1) {				// Fase 1: Attacco: Spara sfere di fuoco 
			if (canReload() && !weapon.hasAmmo()) {
				setAbilityNum(ability_num + 1);
				weapon.setAmmo(ability_num);
				reload();
				nextPhase();
			}
			else {
				action_code = ACTION_ATTACK;
			}
		}
		else if (phase == 2) {				// Fase 2: Sta fermo
			action_code = ACTION_DO_NOTHING;
			if (map->getBulletList().sizeHostile() == 0) {
				nextPhase();
			}
		}
		else if (phase == 3) {				// Fase 3: Non ci sono pi� proiettili -> Va a terra + inizio timer down_time
			action_code = ACTION_DO_NOTHING;
			position = Position((GAME_WIDTH/2-30)+rand()%61, 0);
			down_time.reset();
			nextPhase();
			this->body = STUNNED_BOSS_BODY_TEXTURE;
			this->head_left = STUNNED_BOSS_HEAD_LEFT_TEXTURE;
			this->head_right = STUNNED_BOSS_HEAD_RIGHT_TEXTURE;
		}
		else if (phase == 4) {				// Fase 4: Stordito
			action_code = ACTION_DO_NOTHING;
			down_time.incTimer();
			if (down_time.limit()) {
				nextPhase();
				this->body = BOSS_BODY_TEXTURE;
				this->head_left = BOSS_HEAD_LEFT_TEXTURE;
				this->head_right = BOSS_HEAD_RIGHT_TEXTURE;
			}
		}
		else if (phase == 5) {				// Fase 5: Fine stordimento -> Si teletrasporta a DX o a SX
			action_code = ACTION_DO_NOTHING;
			int x_pos[] = { 1, GAME_WIDTH-2 };
			int gen = rand() % 2;

			if (gen == 0) {
				goRight();
			}
			else {
				goLeft();
			}

			setPosition(Position(x_pos[gen], 0));
			pause_time.reset();
			nextPhase();
		}
	}
	/* Boss MELEE */
	else if (type == BOSS_MELEE) {
		if (phase == 0) {					// fase 0: Pausa
			if (pause_time.limit()) {
				nextPhase();
			}
			pause_time.incTimer();
		}
		else if (phase == 1) {				// Fase 1: Corre + se tocca player, lo lancia
			if (getBodyFrontPosition().equals(player.getBodyPosition())) {
				action_code = ACTION_ATTACK;
			}
			else if (direction == DIRECTION_LEFT) {
				action_code = ACTION_GO_LEFT;
				if (getBodyFrontPosition().getX() == 1) {
					nextPhase();
				}
			}
			else {
				action_code = ACTION_GO_RIGHT;
				if (getBodyFrontPosition().getX() == GAME_WIDTH-2) {
					nextPhase();
				}
			}
		}
		else if (phase == 2) {				// Fase 2: Finito di correre -> Rimane fermo + inizio timer down_time
			action_code = ACTION_DO_NOTHING;
			down_time.reset();
			nextPhase();
			this->body = STUNNED_BOSS_BODY_TEXTURE;
			this->head_left = STUNNED_BOSS_HEAD_LEFT_TEXTURE;
			this->head_right = STUNNED_BOSS_HEAD_RIGHT_TEXTURE;
		}
		else if (phase == 3) {				// Fase 3: Stordito
			down_time.incTimer();
			if (down_time.limit()) {
				nextPhase();
				this->body = BOSS_BODY_TEXTURE;
				this->head_left = BOSS_HEAD_LEFT_TEXTURE;
				this->head_right = BOSS_HEAD_RIGHT_TEXTURE;
			}
		}
		else if (phase == 4) {				// Fase 4: Fine stordimento -> Si gira
			setAbilityNum(getAbilityNum()+1);
			if (direction == DIRECTION_LEFT) {
				goRight();
			}
			else {
				goLeft();
			}
			pause_time.reset();
			nextPhase();
		}
		else if (phase == 5) {				// Fase 5: Skip
			nextPhase();
		}
	}

	return action_code;
}

/*
	Incrementa i vari contatori per le animazioni
*/
void Boss::incCounters() {
	Enemy::incCounters();
}