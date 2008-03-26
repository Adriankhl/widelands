/*
 * Copyright (C) 2008 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "gamecontroller.h"

#include "computer_player.h"
#include "game.h"
#include "playercommand.h"
#include "wlapplication.h"

class SinglePlayerGameController : public GameController {
public:
	SinglePlayerGameController(Widelands::Game* game, bool useai, Widelands::Player_Number local);
	~SinglePlayerGameController();

	void think();
	void sendPlayerCommand(Widelands::PlayerCommand* pc);
	int32_t getFrametime();
	std::string getGameDescription();

	uint32_t realSpeed();
	uint32_t desiredSpeed();
	void setDesiredSpeed(uint32_t speed);

private:
	Widelands::Game& m_game;
	bool m_useai;
	int32_t m_lastframe;
	int32_t m_time;
	uint32_t m_speed; ///< current game speed, in milliseconds per second
	uint32_t m_player_cmdserial;
	Widelands::Player_Number m_local;
	std::vector<Computer_Player *> m_computerplayers;
};

SinglePlayerGameController::SinglePlayerGameController(Widelands::Game* game, bool useai, Widelands::Player_Number local)
: m_game(*game), m_useai(useai), m_local(local)
{
	m_lastframe = WLApplication::get()->get_time();
	m_time = m_game.get_gametime();
	m_speed = 1000;
	m_player_cmdserial = 0;
}

SinglePlayerGameController::~SinglePlayerGameController()
{
	for (uint32_t i = 0; i < m_computerplayers.size(); ++i)
		delete m_computerplayers[i];
	m_computerplayers.clear();
}

void SinglePlayerGameController::think()
{
	int32_t curtime = WLApplication::get()->get_time();
	int32_t frametime = curtime - m_lastframe;
	m_lastframe = curtime;

	// prevent crazy frametimes
	if (frametime < 0)
		frametime = 0;
	else if (frametime > 1000)
		frametime = 1000;

	frametime = (frametime*m_speed)/1000;

	m_time = m_game.get_gametime() + frametime;

	if (m_useai && m_game.is_loaded()) {
		const Widelands::Player_Number nr_players = m_game.map().get_nrplayers();
		iterate_players_existing(p, nr_players, m_game, plr) {
			if (p != m_local) {
				if (p > m_computerplayers.size())
					m_computerplayers.resize(p);
				if (!m_computerplayers[p-1])
					m_computerplayers[p-1] = new Computer_Player(m_game, p);
				m_computerplayers[p-1]->think();
			}
		}
	}
}

void SinglePlayerGameController::sendPlayerCommand(Widelands::PlayerCommand* pc)
{
	pc->set_cmdserial(++m_player_cmdserial);
	m_game.enqueue_command (pc);
}

int32_t SinglePlayerGameController::getFrametime()
{
	return m_time - m_game.get_gametime();
}

std::string SinglePlayerGameController::getGameDescription()
{
	return "singleplayer";
}

uint32_t SinglePlayerGameController::realSpeed()
{
	return m_speed;
}

uint32_t SinglePlayerGameController::desiredSpeed()
{
	return m_speed;
}

void SinglePlayerGameController::setDesiredSpeed(uint32_t speed)
{
	m_speed = speed;
}


GameController* GameController::createSinglePlayer
	(Widelands::Game* game, bool cpls, Widelands::Player_Number local)
{
	return new SinglePlayerGameController(game, cpls, local);
}
