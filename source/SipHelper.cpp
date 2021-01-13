#include "SipHelper.h"
#include <sstream>
std::string SipHelper::generateHexString(int length)
{

	std::stringstream ss;
	for (int i = 0; i < length; ++i)
	{
		ss << std::hex << ((rand() % 16));
	}
	std::string res(ss.str());
	return res;

}