#include "Core.h"

#include <iostream>
#include <torch/torch.h>

namespace Core {

	void PrintHelloWorld()
	{
		torch::Tensor tensor = torch::rand({ 2, 3 });
		std::cout << tensor << std::endl;
		std::cout << "Hello World!\n";
		std::cin.get();
	}

}