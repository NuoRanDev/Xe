using System;
using System.IO;
using System.IO.MemoryMappedFiles;
using System.Runtime.InteropServices;

namespace xe.AssetsPackings
{
    enum CompressSolution : uint
    {
        NONE = 0,
		ZSTD = 1,
	};

    enum AssetFileType :ulong
    {
        IMAGE_ASSET_FILE_HEADER = 297548205385,
        AUDIO_ASSET_FILE_HEADER = 340531631425,
        VIDEO_ASSET_FILE_HEADER = 340464519510,
        TEXT_ASSET_FILE_HEADER = 1415071060,
        MODEL_ASSET_FILE_HEADER = 327579619149,
        SHADER_ASSET_FILE_HEADER = 90457451350099
    };

    class AssetsPacking
    {
        public AssetsPacking(CompressSolution cmp_solution, AssetFileType AssetFileType)
        {
            unsafe
            {
                cpp_class = CreateAssetsPacking((uint)cmp_solution, (ulong)AssetFileType);
                if (cpp_class == null)
                {
                    throw new Exception("Failed to create AssetsPacking c++ instance.");
                }
            }
            asset_file_path = null;
            mmap_file = null;
            accessor = null;
            file_offset = 0;
            mmap_file_size = 0;
        }

        public void AddAsset(string block_name, byte[] need_packing_data)
        {
            bool state = true;
            unsafe
            {
                fixed (byte* p_need_packing_data = &need_packing_data[0])
                {
                    state = AssetsPackingAddAsset(cpp_class, block_name, p_need_packing_data, (ulong)need_packing_data.Length);
                }
            }
            if (!state)
            {
                throw new Exception("Failed to add asset");
            }
        }

        public void WriteAsset(string file_path)
        {
            asset_file_path = file_path;

            OpenFileFunction open_cb = new OpenFileFunction(OpenFile);
            var popen_cb = Marshal.GetFunctionPointerForDelegate(open_cb);

            unsafe
            {
                WriteFileFunction write_cb = new WriteFileFunction(WriteFile);
                nint pwrite_cb = Marshal.GetFunctionPointerForDelegate(write_cb);

                CloseFileFunction close_cb = new CloseFileFunction(CloseFile);
                nint pclose_cb = Marshal.GetFunctionPointerForDelegate(close_cb);

                AssetsPackingWrite(cpp_class, popen_cb, pwrite_cb, pclose_cb);

                GC.KeepAlive(popen_cb);
                GC.KeepAlive(pwrite_cb);
                GC.KeepAlive(pclose_cb);
            }
        }

        [DllImport("AssetsPacking", EntryPoint = "CreateAssetsPacking", CallingConvention = CallingConvention.StdCall)]
        unsafe private extern static void* CreateAssetsPacking(uint cmp_solution, ulong AssetFileType);


        [DllImport("AssetsPacking", EntryPoint = "AssetsPackingAddAsset", CallingConvention = CallingConvention.StdCall)]
        unsafe private extern static bool AssetsPackingAddAsset(void* self, string block_name, byte* input_data, ulong data_size);

        [DllImport("AssetsPacking", EntryPoint = "AssetsPackingWrite", CallingConvention = CallingConvention.StdCall)]
        unsafe private extern static bool AssetsPackingWrite(void* self, nint open_cb, nint write_cb, nint close_cb);

        [DllImport("AssetsPacking", EntryPoint = "AssetsPackingReleaseAsset", CallingConvention = CallingConvention.StdCall)]
        unsafe private extern static void AssetsPackingReleaseAsset(void* cpp_class);

        private delegate bool OpenFileFunction(ulong file_size);

        private unsafe delegate bool WriteFileFunction(byte* block_data, ulong block_size);

        private delegate void CloseFileFunction();

        private unsafe void* cpp_class;

        private string? asset_file_path;

        private MemoryMappedFile? mmap_file;

        private MemoryMappedViewAccessor? accessor;

        private unsafe byte* file_ptr;

        private ulong file_offset;

        private ulong mmap_file_size;

        private bool OpenFile(ulong file_size)
        {
            if (asset_file_path != null)
            {
                mmap_file = MemoryMappedFile.CreateFromFile(asset_file_path, FileMode.OpenOrCreate, "xeAsssetPacking", (long)file_size);
            }
            else return false;
            accessor = mmap_file.CreateViewAccessor(0, (long)mmap_file_size);
            if (!accessor.CanWrite)
            {
                Console.WriteLine("File can't write");
                return false;
            }
            unsafe
            {
                accessor.SafeMemoryMappedViewHandle.AcquirePointer(ref file_ptr);
                if (file_ptr == null)
                {
                    return false;
                }
            }
            mmap_file_size = file_size;
            return true;
        }

        private unsafe bool WriteFile(byte* block_data, ulong block_size)
        {
            unsafe
            {
                Buffer.MemoryCopy(block_data, file_ptr + file_offset, block_size, block_size);
                file_offset += block_size;
            }
            return true;
        }
        /*
        unsafe void PrintPtr(byte* bp, ulong n)
        {
            for (ulong i = 0; i < n; ++i)
                Console.Write(bp[i] + " ");
            Console.WriteLine();
        }*/

        private void CloseFile()
        {
            unsafe
            {
                if (file_ptr != null)
                {
                    accessor.SafeMemoryMappedViewHandle.ReleasePointer();
                }
            }
            accessor.Dispose();
            mmap_file.Dispose();
        }

        ~AssetsPacking()
        {
            unsafe
            {
                AssetsPackingReleaseAsset(cpp_class);
                cpp_class = null;
            }
        }
    }
}
