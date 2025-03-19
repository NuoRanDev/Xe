export module xe.xeTesture.xeTestureDecode;

import std;

import xe.xeCore.xeBaseDataType;

import xe.xeTesture.xeTestureCore;

namespace xe
{
	export std::unique_ptr<Testure> DeCodeFile(std::unique_ptr<TestureFile> File);
}