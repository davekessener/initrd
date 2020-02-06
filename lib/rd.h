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
}}

#endif

