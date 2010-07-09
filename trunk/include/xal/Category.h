/************************************************************************************\
This source file is part of the KS(X) audio library                                  *
For latest info, see http://libatres.sourceforge.net/                                *
**************************************************************************************
Copyright (c) 2010 Kresimir Spes (kreso@cateia.com), Boris Mikic                     *
*                                                                                    *
* This program is free software; you can redistribute it and/or modify it under      *
* the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php   *
\************************************************************************************/
#ifndef XAL_CATEGORY_H
#define XAL_CATEGORY_H

#include <hltypes/hstring.h>
#include "xalExport.h"

namespace xal
{
	class xalExport Category
	{
	public:
		Category(chstr name);
		~Category();
		
		hstr getName() { return this->name; }
		float getGain() { return this->gain; }
		void setGain(float value) { this->gain = value; }
		
	protected:
		hstr name;
		float gain;
		
	};

}
#endif
