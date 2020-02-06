#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <map>
#include <vector>

#include "lib/rd.h"

#include "write.h"

using namespace dave::initrd;

int main(int argc, char **argv)
{
	if(argc <= 2)
	{
		std::cerr << "usage: " << argv[0] << " out-file.ext in-file_1.ext..." << std::endl;

		return 1;
	}

	std::vector<std::string> args{argv + 2, argv + argc};
	std::string ofn{argv[1]};

	std::cout << "saving to " << ofn << std::endl;

	entry root;

	for(const auto& fn : args)
	{
		std::cout << "adding " << fn << std::endl;

		std::vector<std::string> parts;

		auto i1 = fn.begin(), i2 = fn.end();

		for(auto i = i1 ; i != i2 ; ++i)
		{
			if(*i == '/')
			{
				parts.emplace_back(i1, i);
				i1 = ++i;
			}
		}

		if(i1 != i2)
		{
			parts.emplace_back(i1, i2);
		}

		auto *cdir = &root;

		for(auto i1 = parts.begin(), i2 = parts.end() ; i1 != i2 ; )
		{
			const auto& p{*i1};

			++i1;

			if(p == ".") continue;

			bool is_file = (i1 == i2);

			auto n = std::find_if(cdir->children.begin(), cdir->children.end(), [&](const auto& e) { return e.second->name == p; });

			if(n == cdir->children.end())
			{
				cdir = (cdir->children[p] = new entry);

				cdir->name = p;
			}
			else
			{
				cdir = n->second;
			}

			if(is_file)
			{
				std::ifstream fin(fn, std::ios::binary);
				std::vector<u8> *c = new std::vector<u8>(std::istreambuf_iterator<char>(fin), {});

				cdir->content = &c->at(0);
				cdir->size = c->size();
			}
		}
	}

	std::ofstream fout(ofn, std::ios::binary);

	write(fout, &root);

	return 0;
}

