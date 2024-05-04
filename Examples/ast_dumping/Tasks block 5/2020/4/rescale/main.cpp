#include <iostream>
#include <crtdbg.h>
#include "imageResizeCon.h"

int main(int argc, char** argv)
{
	if (argc == 6)
	{
		auto imageResize = ImageResizeCon(argv[1], argv[2], atof(argv[3]), atof(argv[4]), argv[5]);
		if(imageResize.resize())
		{
			imageResize.write();
		}
	}
	std::cout <<"Leaks: "<< _CrtDumpMemoryLeaks() << "\n";
	return 0;
}
