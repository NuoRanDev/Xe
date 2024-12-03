
using System.Text;

class Test
{
    static int Main(string[] args)
    {
        XeCsharpAPI.XeIO.XeFileStreamIO mmap_test = new XeCsharpAPI.XeIO.XeFileStreamIO();
        mmap_test.Open("C:\\Users\\Wunuo\\Desktop\\文本文档l.txt");
        Console.WriteLine(mmap_test.ReadU8String(0, 33));
        mmap_test.Close();
        return 0;
    }
}

