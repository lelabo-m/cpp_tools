#include <iostream>
#include "LibManager.hh"

int main()
{
	LibManager	l;

	try
	{
	}
	catch (LoadingException* e)
	{
		std::cout << "Exception : " + std::string(e->what()) << std::endl;
	}
	return 0;
}