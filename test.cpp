#include <iostream>
#include <vector>
#include <fstream>
#include <iterator>

#include "lib/rd.h"

using namespace dave::initrd;

void process(const Node *n, uint indent)
{
	for(uint i = 0 ; i < indent ; ++i)
	{
		std::cout << "  ";
	}

	std::cout << "/" << n->name() << " (" << (n->is_directory() ? "D" : "F") << "): ";

	if(n->is_directory())
	{
		std::cout << n->children() << " children." << std::endl;

		for(const auto& c : *n)
		{
			process(&c, indent + 1);
		}
	}
	else
	{
		std::cout << n->size() << "B" << std::endl;
	}
}

int main(int argc, char **argv)
{
	std::vector<std::string> args{argv + 1, argv + argc};

	std::ifstream fin(args.at(0), std::ios::binary);
	std::vector<u8> content(std::istreambuf_iterator<char>(fin), {});

	process((const Node *) &content.at(0), 0);

	return 0;
}

