﻿/*
* foo_alsong_lyric														
* Copyright (C) 2007-2010 Inseok Lee <dlunch@gmail.com>
*
* This library is free software; you can redistribute it and/or modify it 
* under the terms of the GNU Lesser General Public License as published 
* by the Free Software Foundation; version 2.1 of the License.
*
* This library is distributed in the hope that it will be useful, 
* but WITHOUT ANY WARRANTY; without even the implied warranty of 
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU Lesser General Public License for more details.
*
* You can receive a copy of the GNU Lesser General Public License from 
* http://www.gnu.org/
*/

#pragma once

#include "Lyric.h"
#include "EncodingFunc.h"

class LRCLyric : public Lyric
{
private:
	wchar_t m_filename[255];
public:
	LRCLyric(std::string rawlyric, std::wstring filename)
	{
		m_Lyric.assign(rawlyric);
		//Split("\r\n");
		Split("\n");

		lstrcpy(m_filename, filename.c_str());
	}

	virtual int GetInternalID() const
	{
		return (int)m_filename;
	}
};