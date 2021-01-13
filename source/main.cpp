#include <thread>
#include <string>
#include <iostream>
#include <thread>         
#include <chrono>

#include "XMLParser.h"

using namespace std;


int main()
{
	XMLParser parser;
	const char* path = "..\\source\\scenarios\\uas.xml";
	parser.parse(path);
	//UASSenario();	
	system("pause");
	return 0;
}
