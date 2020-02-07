#include <vector>

#include "write.h"

namespace dave { namespace initrd {

namespace
{
	struct Impl
	{
		Impl(const entry *);

		void build( );
		uint add(const entry *);

		std::vector<std::pair<u32, const entry *>> nodes;
		std::vector<u8> disk;
	};

	Impl::Impl(const entry *root)
	{
		nodes.emplace_back(1, root);

		add(root);

		build();
	}

	uint Impl::add(const entry *e)
	{
		uint r = nodes.size();

		for(const auto& c : e->children)
		{
			nodes.emplace_back(0, c.second);
		}

		for(uint i = 0 ; i < e->children.size() ; ++i)
		{
			auto& c{nodes.at(r + i)};

			c.first = add(c.second);
		}

		return r;
	}

	void Impl::build(void)
	{
		uint disk_size = nodes.size() * sizeof(Node);
		uint widx = disk_size;

		for(const auto& i : nodes)
		{
			disk_size += i.second->name.size() + 1;
			disk_size += i.second->children.size() + 1;
			disk_size += i.second->size;
		}

		std::cout << nodes.size() << " nodes, totaling " << disk_size << "B" << std::endl;

		disk.resize(disk_size);

		for(uint i = 0 ; i < nodes.size() ; ++i)
		{
			auto *node = nodes.at(i).second;
			uint children_base = nodes.at(i).first;
			uint base = i * sizeof(Node);
			u32 *self = (u32 *) &disk.at(base);
			u8 *data = &disk.at(widx);

//			std::cout << "node " << i << " @" << base << ":" << std::endl;

			self[0] = self[1] = self[2] = self[3] = 0;

			self[0] = widx - base;

			for(auto *name = node->name.data() ; true ; ++name)
			{
				*data++ = *name;
				++widx;

				if(!*name) break;
			}

			if(node->children.empty())
			{
				self[2] = widx - base;
				self[3] = node->size;

				data = &disk.at(widx);

				for(uint j = 0 ; j < node->size ; ++j)
				{
					data[j] = node->content[j];
				}

				widx += node->size;
			}
			else
			{
				self[1] = children_base * sizeof(Node) - base;
				self[3] = node->children.size();
			}

//			std::cout << "  name @" << self[0] << " \"" << node->name << "\"" << std::endl;
//			std::cout << "  children @" << self[1] << std::endl;
//			std::cout << "  content @" << self[2] << std::endl;
//			std::cout << "  size " << self[3] << std::endl;
//			std::cout << "  continuing @" << widx << std::endl;
		}
	}
}

void write(std::ostream& os, const entry *e)
{
	Impl self{e};

	os.write((const char *) &self.disk.at(0), self.disk.size());
}

}}

