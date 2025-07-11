using System.Runtime.InteropServices;

namespace xe.Core
{
    public static class CoreClrOutput
    {
        [DllImport("Engine", EntryPoint = "XE_ERROR_OUTPUT", CallingConvention = CallingConvention.StdCall)]
        public extern static bool XE_ERROR_OUTPUT(string str);

        [DllImport("Engine", EntryPoint = "XE_WARNING_OUTPUT", CallingConvention = CallingConvention.StdCall)]
        public extern static bool XE_WARNING_OUTPUT(string str);

        [DllImport("Engine", EntryPoint = "XE_INFO_OUTPUT", CallingConvention = CallingConvention.StdCall)]
        public extern static bool XE_INFO_OUTPUT(string str);
    }
}
