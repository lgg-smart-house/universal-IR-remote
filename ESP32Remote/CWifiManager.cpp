#include "CWifiManager.h"

CWifiManager::CWifiManager(uint16_t _port, CBaseEventInterface * _basePointer) : server(_port), CBaseEventInterface(_basePointer)
{
	
}


CWifiManager::~CWifiManager()
{
}

void CWifiManager::init()
{
	WiFi.mode(WIFI_AP);

	WiFi.softAP("IRControl");

	WiFi.softAPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

	Serial.println(WiFi.localIP());

	bClientConnected = false;

	server.begin();
}

void CWifiManager::loop()
{
	//eventParser();

	if (!bClientConnected)
	{
		client = server.available();

		if (client)
		{
			bClientConnected = true;
			bClientConnectionNeedsUpdate = false;

			iLastClientConnectionUpdateTime = millis();

			Serial.println("got client");
		}
	}
	else
	{
		eventParser();

		/*if (bClientConnectionNeedsUpdate && millis() - iClientConnectionUpdateTimoutStartTime >= CLIENT_TEST_CONNECTION_TIMEOUT)
		{
			client.stop();

			bClientConnected = false;

			Serial.println("disconnect");
		}

		if (millis() - iLastClientConnectionUpdateTime >= CLIENT_TEST_CONNECTION_INTERVAL && !bClientConnectionNeedsUpdate)
		{
			client.print("requestUpdate");

			bClientConnectionNeedsUpdate = true;
			iClientConnectionUpdateTimoutStartTime = millis();
		}*/
	}
}


void CWifiManager::eventParser()
{
	/*if (!client.available())
		return;*/

	String readData,
		   buffer,
		   eventName,
		   eventData;

	int currentIndex = 0,
		state = 0,
		tempIndex,
		readReturn;

	//readData = client.readString();

	//char charBuffer[2048];

	/*readReturn = client.read_adv((uint8_t*) charBuffer, 2047);

	if (readReturn == -1)
		return;

	if (readReturn == -999)
	{
		client.stop();

		bClientConnected = false;

		Serial.println("disconnect");

		return;
	}*/

	//charBuffer[readReturn] = '\0';


	//readData = String(std::String(charBuffer, readReturn).c_str());

	//readData.reserve(readReturn);

	readData = client.read_adv(state);

	if (state <= 0)
	{ 	
		if (state == -999)
		{
			client.stop();

			bClientConnected = false;

			Serial.println("disconnect");
		}

		return;
	}

	bClientConnectionNeedsUpdate = false;

	while (true)
	{
		if (currentIndex == -1)
			break;
			

		tempIndex = readData.indexOf(';', currentIndex + 1);

		if (tempIndex == -1)
			buffer = readData.substring(currentIndex + 1).c_str();
		else
			buffer = readData.substring(currentIndex + 1, tempIndex).c_str();

		eventName = buffer.substring(0, buffer.indexOf(':'));

		eventData = buffer.substring(buffer.indexOf(':') + 1);

		currentIndex = tempIndex;

		//test = eventList[0]->_event;

		client.print(raiseEvent(eventName, eventData));
	}

	iLastClientConnectionUpdateTime = millis();
}