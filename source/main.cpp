#include <thread>
#include <string>
#include <iostream>
#include <thread>         
#include <chrono>
#include "SipTesterConfig.h"

using namespace std;

class SipTesterConfig;
int main()
{
	
	string str = "uas";
	SipTesterConfig* a = SipTesterConfig::instance();
	a->run(str);
	//SipReadConfig a;
	//a.read(path_test_config);
	//XMLParser parser;
	//const char* path = "..\\source\\scenarios\\uas.xml";
	//SipScenario* m = parser.parse(path);
	//if (m!=NULL)
	//	m->run();
	system("pause");
	return 0;
};
