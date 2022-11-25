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

#include "stdafx.h"

#include "SoapHelper.h"
#include "Socket.h"

SoapHelper::SoapHelper()
{
	pugi::xml_node envelope = m_Document.append_child();
	envelope.set_name("SOAP-ENV:Envelope");
	envelope.append_attribute("xmlns:SOAP-ENV").set_value("http://www.w3.org/2003/05/soap-envelope");
	envelope.append_attribute("xmlns:SOAP-ENC").set_value("http://www.w3.org/2003/05/soap-encoding");
	envelope.append_attribute("xmlns:xsi").set_value("http://www.w3.org/2001/XMLSchema-instance");
	envelope.append_attribute("xmlns:xsd").set_value("http://www.w3.org/2001/XMLSchema");
	envelope.append_attribute("xmlns:ns2").set_value("ALSongWebServer/Service1Soap");
	envelope.append_attribute("xmlns:ns1").set_value("ALSongWebServer");
	envelope.append_attribute("xmlns:ns3").set_value("ALSongWebServer/Service1Soap12");
	envelope.append_child().set_name("SOAP-ENV:Body");

	m_Body = envelope.child("SOAP-ENV:Body");
}

void SoapHelper::SetMethod(const char *MethodName)
{
	pugi::xml_node method = m_Body.append_child();
	method.set_name(MethodName);
	m_Query = method.append_child();
	m_Query.set_name("ns1:encData");
	m_Query.append_child(pugi::node_pcdata).set_value("8582df6473c019a3186a2974aa1e034ae1b2bbb2e7c99575aadc475fcddd997d74bbc1ce3d50b9900282903ee9eb60ae8c5bbf27484441bacb41ecf9128402696641655ff38c2cbbf3c81396034a883af2d82e0545ec32170bddc7c141208e7255e367e5b5ebd81750226856f5405ec3ad7b6f8600c32c2718c4c525bfe34666");
	m_Query = method.append_child();
	m_Query.set_name("ns1:stQuery");
	m_MethodName = MethodName;
}
void SoapHelper::AddParameter(const char *ParameterName, const char *value)
{
	pugi::xml_node param = m_Query.append_child();
	param.set_name(ParameterName);
	param.append_child(pugi::node_pcdata).set_value(value);
}

boost::shared_ptr<pugi::xml_document> SoapHelper::Execute()
{
	std::stringstream str;
	pugi::xml_writer_stream writer(str);
	m_Document.save(writer, "", pugi::format_raw);

	Socket s("lyrics.alsong.co.kr", 80);
	char buf[255];

	CHAR Header[] = "POST /alsongwebservice/service1.asmx HTTP/1.1\r\n"
		"Host: lyrics.alsong.co.kr\r\n"
		"User-Agent: gSOAP/2.7\r\n"
		"Content-Type: application/soap+xml; charset=utf-8\r\n"
		"Content-Length: %d\r\n"
		"Connection: close\r\n"
		"SOAPAction: \"ALSongWebServer/%s\"\r\n\r\n";

	wsprintfA(buf, Header, str.str().length(), &*(boost::find_first(m_MethodName, "ns1:")).begin() + 4);

	s.Send(buf, lstrlenA(buf));
	s.Send(str.str().c_str(), str.str().length());

	std::vector<char> data = s.ReceiveUntilEOF();
	if(data.size() == 0)
		throw SoapReceiveException();

	boost::shared_ptr<pugi::xml_document> ret = boost::shared_ptr<pugi::xml_document>(new pugi::xml_document());
	ret->load(&*boost::find_first(data, "\r\n\r\n").begin());

	return ret;
}
