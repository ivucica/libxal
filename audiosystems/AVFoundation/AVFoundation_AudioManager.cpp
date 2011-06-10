/// @file
/// @author  Ivan Vucica
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an implementation of the AudioManager for iOS's AVFoundation.

#if 1

#ifdef __APPLE__
#include <TargetConditionals.h>
#endif

#if TARGET_OS_IPHONE
#include <string.h>

#include <hltypes/exception.h>
#include <hltypes/hdir.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#include "Buffer.h"
#include "Category.h"
#include "AVFoundation_AudioManager.h"
#include "AVFoundation_Player.h"
#include "xal.h"

namespace xal
{
	AVFoundation_AudioManager::AVFoundation_AudioManager(chstr systemName, unsigned long backendId, bool threaded, float updateTime, chstr deviceName) :
		AudioManager(systemName, backendId, threaded, updateTime, deviceName)
	{
		xal::log("initializing AVFoundation");
		this->enabled = true;
	}

	AVFoundation_AudioManager::~AVFoundation_AudioManager()
	{
		xal::log("destroying AVFoundation");
	}
	
	Player* AVFoundation_AudioManager::_createAudioPlayer(Sound* sound, Buffer* buffer)
	{
		return new AVFoundation_Player(sound, buffer);
	}
	

}
#endif

#endif