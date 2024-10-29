#include "decompress/private/private_Decompress.h"
#include "lzma/LzmaDec.hpp"

namespace xe
{
	// compress by lzma
	bool DecompressLZMA(byte_t* input, byte_t* output, void* start, uint64_t compress_data_offset, uint64_t _not_compress_size)
	{
		pTestData = "DEADBEEF|askjd9827x34bnzkj#%wioeruxo3b84nxijlhwqdzhwerzu39b87r#_3b9p78bznor83y4fr";
		size_t nTestDataLen = _not_compress_size;
		unsigned char pPropsBuf[LZMA_PROPS_SIZE];
		size_t nPropsBufLen = LZMA_PROPS_SIZE;
		ELzmaStatus Status;
		SRes rc;
		int err = 0;
		memset(pCompBuf, 0, COMP_BUFLEN);
		memset(pUnCompBuf, 0, UNCOMP_BUFLEN); /* Initialize compressor (aka encoder) properties... */

		LzmaEncProps_Init(&stProps);


		/* Decompress compressed data from previous step */
		   //nCompBufLen += LZMA_PROPS_SIZE;
		nUnCompBufLen = nTestDataLen;
		rc = LzmaDecode(pUnCompBuf, &nUnCompBufLen, pCompBuf + LZMA_PROPS_SIZE, &compress_data_offset, pPropsBuf, nPropsBufLen, LZMA_FINISH_ANY, &Status, &stAllocator);

		if (rc != SZ_OK)
		{
			printf("\nLZMA decompression failed (rc=%d, status=%d).\n", rc, Status);
			err = 1;
		}

		if ((nUnCompBufLen != nTestDataLen) || memcmp(pTestData, pUnCompBuf, nTestDataLen))
		{
			printf("Compression and/or decompression failed!\n");
			printf("\tInput data length [nTestDataLen] : %d\n", nTestDataLen);
			printf("\tCompressed data length [nCompBufLen] : %d\n", nCompBufLen);
			printf("\tUncompressed data length [nUnCompBufLen]: %d\n", nUnCompBufLen);

			if (memcmp(pTestData, pUnCompBuf, nTestDataLen))
				printf("\tpTestData and pUnCompBuf contain different data!\n");
			else
			{
				printf("\ndest_len:%d\n,dest_data:%s\n,src_src:%s\n", nUnCompBufLen, pUnCompBuf, pTestData);
			}

			err = 1;
		}

		printf("\ndest_len:%d\n,dest_data:%s\n,src_src:%s\n", nUnCompBufLen, pUnCompBuf, pTestData);

		if (!err)
			printf("\nOK!\n");

		return 0;
		return true;
	}
	// compress by zstd
	bool DecompressZSTD(byte_t* input, byte_t* output, void* start, uint64_t compress_data_offset, uint64_t _not_compress_size)
	{
		return true;
	}
	// not compress
	bool DecompressNone(byte_t* input, byte_t* output, void* start, uint64_t offset)
	{
		return true;
	}
}