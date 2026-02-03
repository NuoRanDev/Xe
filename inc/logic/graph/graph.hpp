#ifndef _INC_GRAPH_HPP_
#define _INC_GRAPH_HPP_

#include "type/xeOrdinals.hpp"
#include "log/xeLogOutput.hpp"
#include "memory/xeAlloc.hpp"

#include "image/xeImage.hpp"

#include <limits>

namespace xe
{
	template<typename T>
	concept SignePixel = requires(T)
	{
		std::convertible_to<T, uint16_t> || std::convertible_to<T, uint8_t>;
	};

	template<SignePixel T ,IMG_FORMAT U> class Graph
	{
	public:

		constexpr Graph() noexcept
		{
			static_assert ((U != IMG_FORMAT::GRAY_U8 || U != IMG_FORMAT::GRAY_U16), "Can't match data");
			index_max = std::numeric_limits<T>::max();
			index_min = std::numeric_limits<T>::min();
			x = 0, y = 0;
		}

		bool load_from_img(Image& bit_map) noexcept
		{
			if (is_true_img_type(bit_map))
			{
				uint64_t size = sizeof(T);
				x = bit_map.get_width();
				y = bit_map.get_height();
				data = xe_malloc<T>(size * x * y);
				return true;
			}
			XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
				"xeCore",
				"Image format is not supported in Graph");
			return false;
		}

		void create_empty(uint32_t i_x, uint32_t i_y) noexcept
		{
			uint64_t size = sizeof(T);
			x = i_x, y = i_y;
			data = xe::xe_malloc<T>(size * x * y);
		}
		
		~Graph()
		{
			xe_free(data);
		}

		T unsafe_at(uint32_t i_x, uint32_t i_y) noexcept { return data[i_y * x + i_x]; }

		bool at(T& in, uint32_t i_x, uint32_t i_y) noexcept
		{
			if(not (0 < i_x || i_x < x))
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
					"xeCore",
					"X index out of range in Graph");
				return false;
			}
			if(not (0 < i_y || i_y < y))
			{
				XE_WARNING_OUTPUT(XE_TYPE_NAME_OUTPUT::APP,
					"xeCore",
					"Y index out of range in Graph");
				return false;
			}
			in = data[i_y * x + i_x];
			return true;
		}

	private:

		bool is_true_img_type(const Image& img) noexcept
		{
			return img.get_format() == allow_format;
		}
		
		const IMG_FORMAT allow_format = U;

		uint32_t x, y;

		T* data;
		
		T index_max;

		T index_min;
	};

	using GraphBit8 = Graph<uint8_t, IMG_FORMAT::GRAY_U8>;

	using GraphBit16 = Graph<uint16_t, IMG_FORMAT::GRAY_U16>;

} // namespace xe is end

#endif // _INC_GRAPH_HPP_ IS EOF