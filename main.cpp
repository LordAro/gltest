#include <stdexcept>
#include <iostream>

#include "video.h"

int main()
{
	try {
		Video vid;

		vid.Loop();
	} catch (const std::runtime_error &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

    return 0;
}
