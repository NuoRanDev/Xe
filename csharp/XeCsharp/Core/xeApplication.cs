using System.Runtime.InteropServices;

namespace xe.Core
{
    class Application
    {
        [DllImport("Engine", EntryPoint = "LaodApplication", CallingConvention = CallingConvention.StdCall)]
        private extern static unsafe bool LaodApplication();

        [DllImport("Engine", EntryPoint = "DestroyApplication", CallingConvention = CallingConvention.StdCall)]
        private extern static unsafe int DestroyApplication();

        private static readonly Application instance = new Application();
        private Application() 
        {
            bool is_success = true;
            var cmds_in = Environment.GetCommandLineArgs();
            unsafe
            {
                is_success = LaodApplication();
                if (!is_success) { throw new Exception("Init application failed!"); }
            }
        }

        public static Application Instance
        {
            get
            {
                return instance;
            }
        }

        ~Application()
        {
            DestroyApplication();
        }
    }
}
