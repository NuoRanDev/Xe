#ifndef _INC_LAZY_MAT_H_
#define _INC_LAZY_MAT_H_

#include "LazyImgConfig.h"
#include <iostream>

namespace lazyimg
{
	class LAZY_API_EXPORT ImgMat;

	constexpr byte DataType(byte byte_size, byte color_number)
	{
		return (byte_size << 4) + color_number;
	}

	// 3 channel pixel 
	template<typename T> struct vec3
	{
		T x, y, z;
		constexpr vec3(T i_x, T i_y, T i_z)
		{
			x = i_x;
			y = i_y;
			z = i_z;
		}
	};

	// 4 channel pixel
	template<typename T> struct vec4
	{
		T c1, c2, c3, c4;
		constexpr vec4(T i_c1, T i_c2, T i_c3, T i_c4)
		{
			c1 = i_c1;
			c2 = i_c2;
			c3 = i_c3;
			c4 = i_c4;
		}
	};

	class ImgMat
	{
	private:

		// pixel data
		byte* ppix_data;

		// all size
		size_t _this_mat_size;

		// line ptr
		void** start_line_ptr;

		// gengral init
		inline void GengralInit(byte type);

		// get line start ptr
		inline void FindStartPtr();

	public:

		// default init
		ImgMat() = default;

		// init image
		// eg : ImgMat(DataType(U8, C3), 1280, 720) -> 3 color channel 1280(w) x 720(h) empty image
		ImgMat(byte type, uint32_t w, uint32_t h);

		// eg : ImgMat(DataType(U8, C3), 1280, 720, <any*> ptr) -> 3 color channel 1280(w) x 720(h) image , The data from ptr
		ImgMat(byte type, uint32_t w, uint32_t h, const void* i_pdata);

		// Copy ImgMat
		ImgMat(ImgMat& src);

		// Release Data
		void Release();

		// copy to byte stream ptr
		void ByteBlockCopy(byte* pByteBlock, size_t _size);

		// create ImgMat
		void Create(byte type, uint32_t w, uint32_t h);

		// ptr to ImgMat
		void Create(byte type, uint32_t w, uint32_t h, const void* i_pdata);

		// Get image data start pionter
		template <typename T> T* at()
		{
			return (T*)ppix_data;
		}

		// Get image data offset
		template <typename T> T* GetPixel(size_t offset)
		{
			T* output = (T*)ppix_data;
			return output + offset;
		}

		// get line
		template<typename T> T* Xs(uint32_t n_y)
		{
			byte* output = ppix_data + (channel * bit_depth * n_y * x);
			return (T*)output;
		}

		// get all line
		template<typename T> T** AllGetLineStartPtr()
		{
			return (T**) start_line_ptr;
		}

		template<typename T> T** GetLineStartPtr(uint32_t offset)
		{
			T** output = (T**)start_line_ptr;
			return output + offset;
		}

		template<typename T> T* GetLineStartPixel(uint32_t offset)
		{
			T** output = (T**)start_line_ptr;
			return output[offset];
		}

		// Destory
		~ImgMat();

		// W and H
		uint32_t x, y;

		// RGB XYZ LAB 
		byte color_type;

		// color channel
		byte channel;

		// pixel size
		byte bit_depth;
	};
}

#endif // _INC_LAZY_MAT_H_ IS EOF