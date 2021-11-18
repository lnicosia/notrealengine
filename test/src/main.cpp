
#include "notrealengine.hpp"

#include <string>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

int		main(int argc, char **argv)
{
	using namespace std::chrono_literals;
	nre::SDLWindow win("test");

	std::this_thread::sleep_for(2000ms);
}
