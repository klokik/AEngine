#include <iostream>

#include "world.hpp"

using namespace std;
using namespace Zombie;


int main(int argc, char **argv)
{
	debugMsg("Welcome to Zombie World!");

	int seed=3425;	//random
	ZombieWorld world;
	world.generate(seed,"world1");

	world.start();
	world.stop();
	world.waitEnd();

	world.saveState("world1");

	debugMsg("Good luck!");

	return 0;
}