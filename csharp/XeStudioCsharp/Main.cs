namespace XeStudioCsharp
{
    class XeStudioMain
    {
        public static int Main()
        {
            var packing = new AssetsPacking(CompressSolution.ZSTD, AssetFileType.IMAGE_ASSET_FILE_HEADER);

            byte[] byte_i = File.ReadAllBytes(@"C:\Users\root\Desktop\k1.png");


            packing.AddAsset("test.vec4", byte_i);
            packing.WriteAsset(@"C:\Users\root\Desktop\k1.xea");
            return 0;
        }
    }
}
