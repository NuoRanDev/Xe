export module xe.Core.Unit;

import xe.Core.xeOrdinals;
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif


namespace xe
{
	export namespace Storage
	{
		constexpr xeSize KiB = 1024llu;
		constexpr xeSize MiB = 1024llu * KiB;
		constexpr xeSize GiB = 1024llu * MiB;
		constexpr xeSize TiB = 1024llu * GiB;
	}
}