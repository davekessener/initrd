#ifndef DAVE_INITRD_WRITE_H
#define DAVE_INITRD_WRITE_H

#include <iostream>
#include <map>

#include "lib/rd.h"

namespace dave
{
	namespace initrd
	{
		struct entry
		{
			std::string name = "";
			std::map<std::string, entry *> children;
			u32 size = 0;
			u8 *content = nullptr;
		};

		void write(std::ostream&, const entry *);

		extern uint initrd_log_verbosity;
	}
}

#endif

