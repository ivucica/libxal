/************************************************************************************\
This source file is part of the KS(X) audio library                                  *
For latest info, see http://libatres.sourceforge.net/                                *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef XAL_SOURCE_H
#define XAL_SOURCE_H

#include "xalExport.h"

#include "Sound.h"

namespace xal
{
	class SoundBuffer;
	
	class xalExport Source : public Sound
	{
	public:
		Source(SoundBuffer* sound);
		~Source();

		void update(float k);
		
		Sound* play(float fadeTime = 0.0f, bool looping = false);
		void stop(float fadeTime = 0.0f);
		void pause(float fadeTime = 0.0f);
		
		void unbind();
		
		unsigned int getSourceId() { return this->sourceId; }
		void setSourceId(unsigned int value) { this->sourceId = value; }
		float getSampleOffset() { return this->sampleOffset; }
		unsigned int getBuffer();
		SoundBuffer* getSound() { return this->sound; }
		bool isBound() { return this->bound; }
		float getGain() { return this->gain; }
		void setGain(float value);
		bool isLooping() { return this->looping; }
		bool isPlaying();
		bool isFading();
		bool isFadingIn();
		bool isFadingOut();
		bool isPaused();
		
	protected:
		unsigned int sourceId;
		float gain;
		bool looping;
		bool paused;
		float fadeSpeed;
		float fadeTime;
		float sampleOffset;
		bool bound;
		SoundBuffer* sound;
		
		void _stop(float fadeTime, bool pause = false);
		
	};

}
#endif