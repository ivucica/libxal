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

#include "AudioManager.h"
#include "SimpleSound.h"

#include <iostream>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#ifndef __APPLE__
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#include "Endianess.h"

namespace xal
{
/******* CONSTRUCT / DESTRUCT ******************************************/

	SimpleSound::SimpleSound(chstr filename, chstr category, chstr prefix) :
		buffer(0), SoundBuffer(filename, category, prefix)
	{
	}

	SimpleSound::~SimpleSound()
	{
		this->stopAll();
		if (this->buffer != 0)
		{
			alDeleteBuffers(1, &this->buffer);
		}
	}
	
/******* METHODS *******************************************************/

	bool SimpleSound::_loadOgg()
	{
		xal::mgr->logMessage("XAL: Loading ogg sound " + this->filename);
		vorbis_info *info;
		OggVorbis_File oggStream;
		if (ov_fopen((char*)this->filename.c_str(), &oggStream) != 0)
		{
			xal::mgr->logMessage("Ogg: Error opening file!");
			return false;
		}
		alGenBuffers(1, &this->buffer);
		info = ov_info(&oggStream, -1);
		unsigned long length = ov_pcm_total(&oggStream, -1) * info->channels * 2; // always 16 bit data
		unsigned char *data = new unsigned char[length];
		bool result = false;
		if (data != NULL)
		{
			int section;
			unsigned long size = length;
			unsigned char *buffer = data;
			int read;
			while (size > 0)
			{
				int read = ov_read(&oggStream, (char*)buffer, size, 0, 2, 1, &section);
				if (read == 0)
				{
					length -= size;
					break;
				}
				size -= read;
				buffer += read;
			}

#ifdef __BIG_ENDIAN__
			for (uint16_t* p = (uint16_t*)data; (unsigned char*)p < buffer; p++)
			{
				XAL_NORMALIZE_ENDIAN(*p);
			}
#endif	
			alBufferData(this->buffer, (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, length, info->rate);
			this->duration = ((float)length) / (info->rate * info->channels * 2);
			delete [] data;
			result = true;
		}
		else
		{
			xal::mgr->logMessage("Ogg: Could not allocate ogg buffer.");
		}
		ov_clear(&oggStream);
		return result;
	}

}