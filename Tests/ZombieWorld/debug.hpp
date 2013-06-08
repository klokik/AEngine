#include <iostream>
#include <string>

void debugMsg(std::string msg)
{
	std::cout<<msg<<std::endl;
}

#define NOT_IMPLEMENTED 0x1337

class ZombieException
{
public:
	int code;

	ZombieException(int code, std::string msg)
	{
		debugMsg("An Exception was thrown: " + msg);
		this->code = code;
	}
};