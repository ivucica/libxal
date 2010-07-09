/************************************************************************************\
This source file is part of the KS(X) audio library                                  *
For latest info, see http://libatres.sourceforge.net/                                *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#include <hltypes/hstring.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#ifndef __APPLE__
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include "AudioManager.h"
#include "Category.h"
#include "Sound.h"
#include "Source.h"
#include "DummySound.h"


/*
metoda za loadanje zvukova koja vraca harray loadanih zvukova
prefixi loadanih zvukova za keyeve u mapi
*/

xal::AudioManager* audioMgr;

namespace xal
{
/******* GLOBAL ********************************************************/
	
	void xal_writelog(chstr text)
	{
		printf("%s\n", text.c_str());
	}
	
	void (*gLogFunction)(chstr) = xal_writelog;
	
	ALCdevice* gDevice = NULL;
	ALCcontext* gContext = NULL;

/******* CONSTRUCT / DESTRUCT ******************************************/
	AudioManager::AudioManager(chstr deviceName)
	{
		if (deviceName == "Dummy")
		{
			this->deviceName = "Dummy";
			return;
		}
		// init OpenAL
		this->logMessage("Initializing OpenAL");	
		ALCdevice* currentDevice = alcOpenDevice(deviceName.c_str());
		if (alcGetError(currentDevice) != ALC_NO_ERROR)
		{
			return;
		}
		this->deviceName = alcGetString(currentDevice, ALC_DEVICE_SPECIFIER);
		this->logMessage("Audio device: " + this->deviceName);
		ALCcontext* currentContext = alcCreateContext(currentDevice, NULL);
		if (alcGetError(currentDevice) != ALC_NO_ERROR)
		{
			return;
		}
		alcMakeContextCurrent(currentContext);
		if (alcGetError(currentDevice) != ALC_NO_ERROR)
		{
			return;
		}
		ALuint sources[XAL_MAX_SOURCES];
		alGenSources(XAL_MAX_SOURCES, sources);
		for (int i = 0; i < XAL_MAX_SOURCES; i++)
		{
			this->sources[i] = new xal::Source(sources[i]);
		}
		gDevice = currentDevice;
		gContext = currentContext;
		this->deviceName = deviceName;
	}

	AudioManager::~AudioManager()
	{
		this->logMessage("Destroying OpenAL");
		if (gDevice)
		{
			ALuint id;
			for (int i = 0; i < XAL_MAX_SOURCES; i++)
			{
				this->sources[i]->stop();
				id = this->sources[i]->getId();
				alDeleteSources(1, &id);
				delete this->sources[i];
			}
			std::map<hstr, Sound*>::iterator it = this->sounds.begin();
			for (;it != this->sounds.end(); it++)
			{
				delete it->second;
			}
			alcMakeContextCurrent(NULL);
			alcDestroyContext(gContext);
			alcCloseDevice(gDevice);
		}
	}
	
/******* CONSTRUCT / DESTRUCT ******************************************/

	void AudioManager::logMessage(chstr message)
	{
		gLogFunction(message);
	}
	
	float AudioManager::getCategoryGain(chstr name)
	{
		if (this->categories.find(name) == this->categories.end())
		{
			return 1.0f;
		}
		return this->categories[name]->getGain();
	}

	void AudioManager::setCategoryGain(chstr name, float gain)
	{
		this->categories[name]->setGain(gain);
		for (int i = 0; i < XAL_MAX_SOURCES; i++)
		{
			if (this->sources[i]->getSound() != NULL)
			{
				alSourcef(this->sources[i]->getId(), AL_GAIN, this->sources[i]->getGain() * gain);
			}
		}
	}

	void AudioManager::update(float k)
	{
		for (int i = 0; i < XAL_MAX_SOURCES; i++)
		{
			this->sources[i]->update(k);
		}
	}

	Source* AudioManager::allocateSource()
	{
		for (int i = 0; i < XAL_MAX_SOURCES; i++)
		{
			if (!this->sources[i]->hasSound())
			{
				return this->sources[i];
			}
		}
		this->logMessage("AudioManager: unable to allocate audio source!");
		return NULL;
	}

	Sound* AudioManager::getSound(chstr name)
	{
		if (this->sounds.find(name) == this->sounds.end())
		{
			return NULL;
		}
		return this->sounds[name];
	}
	
	Sound* AudioManager::loadSound(chstr filename, chstr category, chstr prefix)
	{
		if (!gDevice)
		{
			return NULL;
		}
		Sound* sound = new Sound(filename, category);
		if (!sound->load())
		{
			return NULL;
		}
		this->sounds[prefix + sound->getName()] = sound;
		return sound;
	}

	harray<hstr> AudioManager::loadPath(chstr path, chstr prefix)
	{
		//2DO - implement
		/*
		harray<hstr> result;
		if (!gDevice)
		{
			return result;
		}
		
		
		
		Sound* sound = new Sound(filename, category);
		if (!sound->load())
		{
			return NULL;
		}
		this->sounds[sound->getName()] = sound;
		return result;
		*/
	}

	void AudioManager::unloadSound(Sound* sound)
	{
		std::map<hstr,Sound*>::iterator it = this->sounds.begin();
		for (;it != this->sounds.end(); it++)
		{
			if (it->second == sound)
			{
				this->sounds.erase(it);
				delete it->second;
				break;
			}
		}
	}

/******* GLOBAL ********************************************************/

	void setLogFunction(void (*function)(chstr))
	{
		gLogFunction = function;
	}
	
	void init(chstr deviceName)
	{
		audioMgr = new AudioManager(deviceName);
	}
	
	void destroy()
	{
		delete audioMgr;
	}
		
}