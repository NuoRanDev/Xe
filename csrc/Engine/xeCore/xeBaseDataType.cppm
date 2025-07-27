export module xe.Core.xeBaseDataType;

import xe.Core.xeOrdinals;
import xe.Core.xeAlloc;

namespace xe
{
	export enum class xeAudioCompressSolution :xeUint64
	{
		ERROR = 0,

		FLAC = 1,
		OGG = 2
	};

	// pxiel imgae
	export using byte_line = xeByte*;

	export enum class xeColorChannel :xeUint64
	{
		BOOL = 1,
		GRAY16 = 2,
		RGB = 3,
		RGBA = 4
	};


	export class TestureEncodedData
	{
	public:
		const xeU8cstr*		name;
		const xeSize		name_size;
		xeColorChannel		encodesolution;
		xeSize				size;
		xeByte*				data;
		TestureEncodedData(xeByte* i_data, xeSize i_size, xeColorChannel i_encodesolution, const xeU8cstr* i_name, const xeSize i_name_size):
			name_size(i_name_size)
		{
			data = i_data;
			size = i_size;
			encodesolution = i_encodesolution;
			name = i_name;
		}
		~TestureEncodedData()
		{
			xeFree(data);
		}
	};

	// Audio
	export class AudioEncodedData
	{
	public:
		const xeU8cstr*			name;
		const xeSize			name_size;
		xeAudioCompressSolution encodesolution;
		xeByte*					cur_ptr;
		xeSize					size;
		xeByte*					data;
		AudioEncodedData(xeByte* i_data, xeSize i_size, xeAudioCompressSolution i_encodesolution, const xeU8cstr* i_name, const xeSize i_name_size):
			name_size(i_name_size)
		{
			data = i_data;
			size = i_size;
			encodesolution = i_encodesolution;
			name = i_name;
			cur_ptr = data;
		}
		~AudioEncodedData()
		{
			//xeFree(data);
		}
	};

	// 2 channel pixel 
	template<typename T> class Vec2
	{
	public:
		T x, y;
		Vec2() = default;
		bool operator==(const Vec2<T>& input)
		{
			if (std::memcmp((const char*)(&(this->x)), (const char*)(&(input.x)), sizeof(T) * 2))
				return true;
			return false;
		}
		xeInt64 operator*(const Vec2<T>& input)
		{
			return this->x * input.x + this->y * input.y;
		}
		Vec2<T> operator+(const Vec2<T>& input)
		{
			return Vec2(this->x + input.x, this->y + input.y);
		}
		Vec2<T> operator-(const Vec2<T>& input)
		{
			return Vec2(this->x - input.x, this->y - input.y);
		}
		constexpr Vec2(T i_x, T i_y)
		{
			x = i_x;
			y = i_y;
		}
	};

	// 3 channel pixel 
	template<typename T> class Vec3
	{
	public:
		T x, y, z;
		Vec3() = default;
		bool operator==(const Vec3<T>& input)
		{
			if (memcmp((const char*)(&(this->x)), (const char*)(&(input.x)), sizeof(T) * 3))
				return true;
			return false;
		}
		xeInt64 operator*(const Vec3<T>& input)
		{
			return this->x * input.x + this->y * input.y + this->z * input.z;
		}
		Vec3<T> operator+(const Vec3<T>& input)
		{
			return Vec3(this->x + input.x, this->y + input.y, this->z + input.z);
		}
		Vec3<T> operator-(const Vec3<T>& input)
		{
			return Vec3(this->x - input.x, this->y - input.y, this->z - input.z);
		}
		constexpr Vec3(T i_x, T i_y, T i_z)
		{
			x = i_x;
			y = i_y;
			z = i_z;
		}
	};

	// 4 channel pixel
	template<typename T> class Vec4
	{
	public:
		T n1, n2, n3, n4;
		Vec4() = default;
		bool operator==(const Vec4<T>& input)
		{
			if (memcmp((const char*)(&(this->n1)), (const char*)(&(input.n1)), sizeof(T) * 3) == 0)
				return true;
			return false;
		}
		xeInt64 operator*(const Vec4<T>& input)
		{
			return this->n1 * input.n1 + this->n2 * input.n2 + this->n3 * input.n3 + this->n4 * input.n4;
		}
		Vec4<T> operator+(const Vec4<T>& input)
		{
			return Vec4(this->n1 + input.n1, this->n2 + input.n2, this->n3 + input.n3, this->n4 + input.n4);
		}
		Vec4<T> operator-(const Vec4<T>& input)
		{
			return Vec4(this->n1 - input.n1, this->n2 - input.n2, this->n3 - input.n3, this->n4 - input.n4);
		}
		constexpr Vec4(T i_n1, T i_n2, T i_n3, T i_n4)
		{
			n1 = i_n1;
			n2 = i_n2;
			n3 = i_n3;
			n4 = i_n4;
		}
	};

	export using Vec4_f32	= Vec4<float>;
	export using Vec4_i8	= Vec4<xeInt8>;
	export using Vec4_u8	= Vec4<xeUint8>;
	export using Vec4_i16	= Vec4<xeInt16>;
	export using Vec4_u16	= Vec4<xeUint16>;
	export using Vec4_i32	= Vec4<xeInt32>;
	export using Vec4_u32	= Vec4<xeUint32>;

	export using Vec3_f32	= Vec3<float>;
	export using Vec3_i8	= Vec3<xeInt8>;
	export using Vec3_u8	= Vec3<xeUint8>;
	export using Vec3_i16	= Vec3<xeInt16>;
	export using Vec3_u16	= Vec3<xeUint16>;
	export using Vec3_i32	= Vec3<xeInt32>;
	export using Vec3_u32	= Vec3<xeUint32>;

	export using Vec2_f32 = Vec2<float>;
	export using Vec2_i8 = Vec2<xeInt8>;
	export using Vec2_u8 = Vec2<xeUint8>;
	export using Vec2_i16 = Vec2<xeInt16>;
	export using Vec2_u16 = Vec2<xeUint16>;
	export using Vec2_i32 = Vec2<xeInt32>;
	export using Vec2_u32 = Vec2<xeUint32>;

	export class xeFastExtension :public Vec4_i8
	{
	public:
		bool operator==(const xeU8cstr* start_str_ptr)
		{
			const xeUint64* external = (xeUint64*)start_str_ptr;
			const xeUint64* internal = (xeUint64*)(&this->n1);
			return *internal == *external;
		}
		xeFastExtension() = default;
		constexpr xeFastExtension(const char str[4]) :Vec4_i8(str[0], str[1], str[2], str[3]) {}
	};
	export bool is4Extension(const xeU8cstr* main_csharp_str, int64_t len_1, xeFastExtension _4_extension)
	{
		return _4_extension == (main_csharp_str + len_1 - 1);
	}

	template<typename T>
	concept Pixel = requires
	{
		std::same_as<T, Vec3_u8> && 
		std::same_as<T, Vec3_u16>&&
		std::same_as<T, Vec3_u32>&&
		std::same_as<T, Vec4_u8> &&
		std::same_as<T, Vec4_u16>&&
		std::same_as<T, Vec4_u32>;
	};
}