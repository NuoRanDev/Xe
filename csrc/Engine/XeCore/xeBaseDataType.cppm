export module xe.Core.xeBaseDataType;

import xe.Core.xeOrdinals;

namespace xe
{
	export enum class xeAudioCompressSolution :xeUint8
	{
		ERROR = 0,

		FLAC = 1,
		OGG = 2
	};

	// pxiel imgae
	export using byte_line = xeByte*;

	export enum class xeColorChannel :xeUint32
	{
		BOOL = 1,
		GRAY16 = 2,
		RGB = 3,
		RGBA = 4
	};


	export struct TestureEncodedData
	{
		xeColorChannel encodesolution;
		xeSize _size;
		xeByte* data;
	};

	// Audio
	export struct AudioEncodedData
	{
		xeAudioCompressSolution solution;
		xeSize _size;
		xeByte* data;
	};

	// 2 channel pixel 
	template<typename T> class vec2
	{
	public:
		T x, y;
		vec2() = default;
		bool operator==(const vec2<T>& input)
		{
			if (std::memcmp((const char*)(&(this->x)), (const char*)(&(input.x)), sizeof(T) * 2))
				return true;
			return false;
		}
		xeInt64 operator*(const vec2<T>& input)
		{
			return this->x * input.x + this->y * input.y;
		}
		vec2<T> operator+(const vec2<T>& input)
		{
			return vec2(this->x + input.x, this->y + input.y);
		}
		vec2<T> operator-(const vec2<T>& input)
		{
			return vec2(this->x - input.x, this->y - input.y);
		}
		constexpr vec2(T i_x, T i_y)
		{
			x = i_x;
			y = i_y;
		}
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
		xeInt64 operator*(const vec3<T>& input)
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
		xeInt64 operator*(const vec4<T>& input)
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

	export using vec4_f32	= vec4<float>;
	export using vec4_i8	= vec4<xeInt8>;
	export using vec4_u8	= vec4<xeUint8>;
	export using vec4_i16	= vec4<xeInt16>;
	export using vec4_u16	= vec4<xeUint16>;
	export using vec4_i32	= vec4<xeInt32>;
	export using vec4_u32	= vec4<xeUint32>;

	export using vec3_f32	= vec3<float>;
	export using vec3_i8	= vec3<xeInt8>;
	export using vec3_u8	= vec3<xeUint8>;
	export using vec3_i16	= vec3<xeInt16>;
	export using vec3_u16	= vec3<xeUint16>;
	export using vec3_i32	= vec3<xeInt32>;
	export using vec3_u32	= vec3<xeUint32>;

	export using vec2_f32 = vec2<float>;
	export using vec2_i8 = vec2<xeInt8>;
	export using vec2_u8 = vec2<xeUint8>;
	export using vec2_i16 = vec2<xeInt16>;
	export using vec2_u16 = vec2<xeUint16>;
	export using vec2_i32 = vec2<xeInt32>;
	export using vec2_u32 = vec2<xeUint32>;

	export class xeFastExtension :public vec4_i8
	{
	public:
		bool operator==(const char* start_str_ptr)
		{
			const xeUint64* external = (xeUint64*)start_str_ptr;
			const xeUint64* internal = (xeUint64*)(&this->n1);
			return *internal == *external;
		}
		xeFastExtension() = default;
		constexpr xeFastExtension(const char str[4]) :vec4_i8(str[0], str[1], str[2], str[3]) {}
	};
	export bool is4Extension(const char* main_csharp_str, int64_t len_1, xeFastExtension _4_extension)
	{
		return _4_extension == (main_csharp_str + len_1 - 1);
	}
}