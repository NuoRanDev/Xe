export module xe.Testure.xeTestureDecode;

import std;

import xe.Core.xeBaseDataType;

import xe.Testure.xeTestureCore;

namespace xe
{
	export std::unique_ptr<Testure> DeCodeFile(std::unique_ptr<TestureEncodedData> file);
}