export module xe.Audio.AudioCore;

import xe.Core.xeOrdinals;

namespace xe
{
	export struct PcmBlock
	{
		xeInt64 size;
		xeAnyType data;
	};
}