#include "AppConfig.h"
#include "./Common/tinyxml/tinystr.h"
#include "./Common/tinyxml/tinyxml.h"

CAppConfig::CAppConfig(void)
{
}


CAppConfig::~CAppConfig(void)
{
}

void CAppConfig::GetServer(Server& server)
{
	TiXmlDocument doc("app.config");
	bool bOk = doc.LoadFile();
	if (!bOk)
	{
		return;
	}

	TiXmlElement* root = doc.RootElement();
	TiXmlElement* serverNode = root->FirstChildElement();

	TiXmlElement* protocolNode = serverNode->FirstChildElement();
	server.strProtocol = protocolNode->FirstChild()->Value();

	TiXmlElement* hostNode = protocolNode->NextSiblingElement();
	server.strHost = hostNode->FirstChild()->Value();

	TiXmlElement* portNode = hostNode->NextSiblingElement();
	server.strPort = portNode->FirstChild()->Value();
}