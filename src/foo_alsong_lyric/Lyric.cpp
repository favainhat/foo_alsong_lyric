/*
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

#include "stdafx.h"

#include "Lyric.h"
#include "SoapHelper.h"
#include "pugixml/pugixml.hpp"
#include "regex"

Lyric::Lyric()
{

}

Lyric::Lyric(const char *raw)
{
	m_Lyric = raw;
	//Split("\r\n");
	Split("\n");
}

Lyric::~Lyric()
{

}

vector<string> split(const string& i_str, const string& i_delim)
{
	vector<string> result;
	size_t startIndex = 0;

	for (size_t found = i_str.find(i_delim); found != string::npos; found = i_str.find(i_delim, startIndex))
	{
		result.emplace_back(i_str.begin() + startIndex, i_str.begin() + found);
		startIndex = found + i_delim.size();
	}
	if (startIndex != i_str.size())
		result.emplace_back(i_str.begin() + startIndex, i_str.end());
	return result;
}

DWORD Lyric::Split(const char *Delimiter)
{
	int i;

	m_LyricLines.clear();

	std::string deli = std::string(Delimiter);
	vector<string> lyrics = split(m_Lyric, deli);
	for (i = 0; i< lyrics.size(); i++) {
		std::regex e{ R"(\[(\d\d):(\d\d).(\d\d)\](.*))" };

		std::smatch sm;
		std::regex_search(lyrics[i], sm, e);

		if (sm.size() != 5) {
			continue;
		}
		int minutes = stoi(sm[1]);
		int sec = stoi(sm[2]);
		int cent = stoi(sm[3]);
		int total = ((minutes * 60) + sec) * 1000 + cent * 10;
		int time = total / 10;
		std::string temp = sm[4];
		m_LyricLines.push_back(LyricLine(time, temp));

	}
	//temporay workaround! if m_LyricLines is not empty add blank line.
	if (m_LyricLines.size() > 0) {
		m_LyricLines.insert(m_LyricLines.begin(), LyricLine(0, ""));
	}
	m_LyricIterator = m_LyricLines.begin();

	return S_OK;
}

void Lyric::Clear()
{
	m_Title.clear();
	m_Album.clear();
	m_Artist.clear();
	m_Registrant.clear();
	m_Lyric.clear();
	m_LyricLines.clear();
	m_LyricIterator = m_LyricLines.begin();
}

std::vector<LyricLine>::const_iterator Lyric::GetIteratorAt(unsigned int time) const
{
	std::vector<LyricLine>::const_iterator it;
	for(it = m_LyricLines.begin(); it != m_LyricLines.end() && it->time < time; it ++);
	
	return it;
}

int Lyric::IsValidIterator(std::vector<LyricLine>::const_iterator it) const
{
	try
	{
		if(!it._Ptr)
			return false;
		return std::find(m_LyricLines.begin(), m_LyricLines.end(), *it) != m_LyricLines.end();
	}
	catch(...)
	{
		return false;
	}
}
