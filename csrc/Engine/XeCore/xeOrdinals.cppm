export module xe.Core.xeOrdinals;

import std;

namespace xe
{
	export using xeInt8			= std::int8_t;
	export using xeInt16		= std::int16_t;
	export using xeInt32		= std::int32_t;
	export using xeInt64		= std::int64_t;

	export using xeUint8		= std::uint8_t;
	export using xeUint16		= std::uint16_t;
	export using xeUint32		= std::uint32_t;
	export using xeUint64		= std::uint64_t;

	export using xeUnicode		= char32_t;
	export using xeSize			= size_t;
	export using xeOffsize		= xeSize;
	export using xeDataStart	= void*;
	export using xeByte			= xeUint8;

	export using xeU8cstr		= char8_t;

	export using xeAnyType		= void*;
}