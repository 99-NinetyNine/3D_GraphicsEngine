
#include <iostream>
#include "Project.h"

int main()
{
	Engine engine;

	if (engine.ConstructConsole(256, 176, 4, 4))
	{

		engine.Start();

	}

}

