#ifndef DAVE_LIB_INITRD_RD_H
#define DAVE_LIB_INITRD_RD_H

#include <stdint.h>

namespace dave { namespace initrd
{
	typedef uint8_t u8;
	typedef uint32_t u32;
	typedef unsigned uint;

	template<typename = void>
	class Entry
	{
		typedef Entry<> Self;

		public:
			bool is_directory( ) const { return mChildren; }
			const Self *begin( ) const { return (const Self *) (((const u8 *) this) + mChildren); }
			const Self *end( ) const { return begin() + mSize; }
			const u8 *content( ) const { return is_directory() ? nullptr : (((const u8 *) this) + mContent); }
			u32 size( ) const { return is_directory() ? 0 : mSize; }
			uint children( ) const { return is_directory() ? mSize : 0; }
			const char *name( ) const { return ((const char *) this) + mName; }

			const Self *find(const char *) const;

		private:
			Entry( );
			~Entry( );

			u32 mName;
			u32 mChildren;
			u32 mContent;
			u32 mSize;
	} __attribute__((packed));

	typedef Entry<> Node;

	static_assert(sizeof(Node) == 16);

	namespace detail
	{
		inline bool streq(const char *i1, const char *i2, const char *i)
		{
			for(; i1 != i2 ; ++i1, ++i)
			{
				if(*i1 != *i)
					return false;
			}

			return true;
		}

		inline const char *snext(const char *s, char c)
		{
			while(*s && *s != c) ++s;

			return s;
		}
	}

	template<typename T>
	const Node *Entry<T>::find(const char *path) const
	{
		if(!*path)
			return is_directory() ? nullptr : this;

		const char *i = detail::snext(path, '/');

		for(auto i1 = begin(), i2 = end() ; i1 != i2 ; ++i1)
		{
			if(detail::streq(path, i, i1->name()))
			{
				return *i ? i1->find(i + 1) : i1;
			}
		}

		return nullptr;
	}
}}

#endif

