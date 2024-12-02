using System.Runtime.InteropServices;
using System.Text;

namespace XeCsharpAPI
{
    namespace XeIO
    {

        class XeFileStreamIO
        {
            #region PInvokse
            [DllImport("XeIO.dll", EntryPoint = "GetMmapClassObj", CallingConvention = CallingConvention.StdCall)]
            static private extern IntPtr GetMmapClassObj();

            [DllImport("XeIO.dll", EntryPoint = "OpenFile", CallingConvention = CallingConvention.StdCall)]
            static private extern bool OpenFile(IntPtr class_obj, string file_path);

            [DllImport("XeIO", EntryPoint = "MemcpyOut", CallingConvention = CallingConvention.StdCall)]
            static private extern unsafe bool MemcpyOut(IntPtr class_obj, byte* pdata, int start, int offset);

            [DllImport("XeIO.dll", EntryPoint = "DeleteMmapClassObj", CallingConvention = CallingConvention.StdCall)]
            static private extern void DeleteMmapClassObj(IntPtr class_ptr);
            #endregion PInvokse

            #region CLASS_OBJ
            private IntPtr MmapfstreamPtr;
            #endregion CLASS_OBJ
            public XeFileStreamIO()
            {
                MmapfstreamPtr = GetMmapClassObj();
            }
            public bool Open(string file_path)
            {
                return OpenFile(MmapfstreamPtr, file_path);
            }
            public byte[] ReadByte(int start, int offset)
            {
                byte[] dst = new byte[offset];
                if (offset > int.MaxValue || start > int.MaxValue)
                {
                    throw new ArgumentException("OUT OF C SHARP ALLOC!!!");
                }
                unsafe
                {
                    byte* pdata = (byte*)Marshal.AllocHGlobal(offset);
                    if (!MemcpyOut(MmapfstreamPtr, pdata, start, offset)) 
                    {
                        throw new ArgumentException("READ FAILD");
                    }
                    Marshal.Copy((IntPtr)pdata, dst, 0, offset);
                    Marshal.FreeHGlobal((IntPtr)pdata);
                }
                return dst;
            }
            public string ReadU8String(int start, int light)
            {
                string out_str = Encoding.UTF8.GetString(ReadByte(start, light));
                return out_str;
            }
            public void Close()
            {
                DeleteMmapClassObj(MmapfstreamPtr);
            }
            ~XeFileStreamIO()
            {
                Close();
            }
        }
    }
}
