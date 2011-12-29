/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/harray.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>
#include <xal/AudioManager.h>
#include <xal/Player.h>

#include "Playlist.h"

namespace xal
{
	Playlist::Playlist(bool repeatAll) : enabled(true), playing(false), index(-1)
	{
		this->repeatAll = repeatAll;
	}
	
	Playlist::~Playlist()
	{
		this->clear();
	}

	bool Playlist::isPaused()
	{
		return (this->index >= 0 && this->index < this->players.size() && this->players[this->index]->isPaused());
	}
	
	harray<hstr> Playlist::getSoundNames()
	{
		harray<hstr> result;
		foreach (Player*, it, this->players)
		{
			result += (*it)->getName();
		}
		return result;
	}

	Player* Playlist::getCurrentPlayer()
	{
		return (this->isPlaying() ? this->players[this->index] : NULL);
	}
	
	void Playlist::update()
	{
		if (this->enabled)
		{
			if (this->players.size() == 0 || !this->playing || this->index < 0)
			{
				return;
			}
			if (this->repeatAll)
			{
				if (!this->players[this->index]->isPlaying())
				{
					this->index = (this->index + 1) % this->players.size();
					this->players[this->index]->play();
				}
			}
			else if (this->index < this->players.size())
			{
				if (!this->players[this->index]->isPlaying())
				{
					this->index++;
					if (this->index < this->players.size())
					{
						this->players[this->index]->play();
					}
					else
					{
						this->playing = false;
					}
				}
			}
			else
			{
				this->playing = false;
			}
		}
	}
	
	void Playlist::clear()
	{
		if (this->enabled)
		{
			this->stop();
			foreach (Player*, it, this->players)
			{
				xal::mgr->destroyPlayer(*it);
			}
			this->players.clear();
			this->index = -1;
		}
	}
	
	void Playlist::queueSound(chstr name)
	{
		this->players += xal::mgr->createPlayer(name);
		this->index = hmax(this->index, 0);
	}
	
	void Playlist::queueSounds(harray<hstr> names)
	{
		foreach (hstr, it, names)
		{
			this->players += xal::mgr->createPlayer(*it);
		}
		this->index = hmax(this->index, 0);
	}
	
	void Playlist::play(float fadeTime)
	{
		if (this->enabled)
		{
			if (this->players.size() == 0 || this->playing)
			{
				return;
			}
			if (this->index >= this->players.size())
			{
				this->index = 0;
			}
			this->players[this->index]->play(fadeTime);
			this->playing = true;
		}
	}
	
	void Playlist::stop(float fadeTime)
	{
		if (this->enabled)
		{
			if (this->playing)
			{
				this->players[this->index]->stop(fadeTime);
			}
			this->playing = false;
		}
	}
	
	void Playlist::pause(float fadeTime)
	{
		if (this->enabled)
		{
			if (this->playing)
			{
				this->players[this->index]->pause(fadeTime);
			}
			this->playing = false;
		}
	}

	void Playlist::shuffle()
	{
		if (this->enabled)
		{
			if (!this->playing && this->players.size() > 1)
			{
				xal::Player* player = (this->index >= 0 && this->index < this->players.size() ? this->players[index] : NULL);
				this->players.randomize();
				if (player != NULL)
				{
					this->index = this->players.index_of(player);
				}
			}
		}
	}

	void Playlist::reset()
	{
		if (this->enabled)
		{
			this->stop();
			this->index = 0;
		}
	}
	
}
