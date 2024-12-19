#ifndef _INC_XE_BASE_DATA_TYPE_H_
#define _INC_XE_BASE_DATA_TYPE_H_
#include "xeCoreConfig.h"

namespace xe
{
	enum class xeAudioCompressSolution :uint32_t
	{
		ERROR	= 0,

		FLAC	= 1,
		OGG		= 2
	};

	// pxiel imgae
	using byte_line = byte_t*;

	enum class xeColorChannel :uint32_t
	{
		BOOL = 1,
		GRAY16 = 2,
		RGB = 3,
		RGBA = 4
	};


	class Testure
	{
	public:
		uint32_t		x, y;
		xeColorChannel	channel;
		uint32_t		channel_byte_size;
		byte_t* pixel_data;
		Testure() = default;
		Testure(uint32_t i_x, uint32_t i_y, xeColorChannel i_channel)
		{
			x = i_x; y = i_y; channel = i_channel; channel_byte_size = 1;
			pixel_data = new byte_t[i_x * i_y * static_cast<uint32_t>(i_channel)];
		}
		~Testure()
		{
			delete[]pixel_data;
		}
		constexpr byte_t* GetData()
		{
			return pixel_data;
		}
		constexpr uint32_t GetLineSize()
		{
			return x * static_cast<uint32_t>(channel) * channel_byte_size;
		}
		constexpr uint32_t GetTestureSize()
		{
			return GetLineSize() * y;
		}
		void GetlineStart(byte_line* dst)
		{
			for (uint32_t i = 0; i < y; i++)
			{
				dst[i] = pixel_data + i * GetLineSize();
			}
		}
	};

	// Audio
	struct Audio
	{
		xeAudioCompressSolution solution;
		size_t _size;
		byte_t* data;
	};

	// 3 channel pixel 
	template<typename T> class vec3
	{
	public:
		T x, y, z;
		vec3() = default;
		bool operator==(const vec3<T>& input)
		{
			if (memcmp((const char*)(&(this->x)), (const char*)(&(input.x)), sizeof(T) * 3))
				return true;
			return false;
		}
		int32_t operator*(const vec3<T>& input)
		{
			return this->x * input.x + this->y * input.y + this->z * input.z;
		}
		vec3<T> operator+(const vec3<T>& input)
		{
			return vec3(this->x + input.x, this->y + input.y, this->z + input.z);
		}
		vec3<T> operator-(const vec3<T>& input)
		{
			return vec3(this->x - input.x, this->y - input.y, this->z - input.z);
		}
		constexpr vec3(T i_x, T i_y, T i_z)
		{
			x = i_x;
			y = i_y;
			z = i_z;
		}
	};

	// 4 channel pixel
	template<typename T> class vec4
	{
	public:
		T n1, n2, n3, n4;
		vec4() = default;
		bool operator==(const vec4<T>& input)
		{
			if (memcmp((const char*)(&(this->n1)), (const char*)(&(input.n1)), sizeof(T) * 3) == 0)
				return true;
			return false;
		}
		int32_t operator*(const vec4<T>& input)
		{
			return this->n1 * input.n1 + this->n2 * input.n2 + this->n3 * input.n3 + this->n4 * input.n4;
		}
		vec4<T> operator+(const vec4<T>& input)
		{
			return vec4(this->n1 + input.n1, this->n2 + input.n2, this->n3 + input.n3, this->n4 + input.n4);
		}
		vec4<T> operator-(const vec4<T>& input)
		{
			return vec4(this->n1 - input.n1, this->n2 - input.n2, this->n3 - input.n3, this->n4 - input.n4);
		}
		constexpr vec4(T i_n1, T i_n2, T i_n3, T i_n4)
		{
			n1 = i_n1;
			n2 = i_n2;
			n3 = i_n3;
			n4 = i_n4;
		}
	};

	using vec4_f32 = vec4<float>;
	using vec4_i8  = vec4<int8_t>;
	using vec4_u8  = vec4<uint8_t>;
	using vec4_i16 = vec4<int16_t>;
	using vec4_u16 = vec4<uint16_t>;
	using vec4_i32 = vec4<int32_t>;
	using vec4_u32 = vec4<uint32_t>;

	using vec3_f32 = vec3<float>;
	using vec3_i8  = vec3<int8_t>;
	using vec3_u8  = vec3<uint8_t>;
	using vec3_i16 = vec3<int16_t>;
	using vec3_u16 = vec3<uint16_t>;
	using vec3_i32 = vec3<int32_t>;
	using vec3_u32 = vec3<uint32_t>;

	class xeFastExtension :public vec4_i8
	{
	public:
		bool operator==(const char* start_str_ptr)
		{
			const uint64_t* external = (uint64_t*)start_str_ptr;
			const uint64_t* internal = (uint64_t*)(&this->n1);
			return *internal == *external;
		}
		xeFastExtension() = default;
		constexpr xeFastExtension(const char str[5])
		{
			n1 = str[0];
			n2 = str[1];
			n3 = str[2];
			n4 = str[3];
		}
	};
}

#endif // _INC_XE_BASE_DATA_TYPE_H_ IS EOF