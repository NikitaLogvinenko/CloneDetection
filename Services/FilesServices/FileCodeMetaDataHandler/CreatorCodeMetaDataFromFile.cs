using CodeMetaData;

namespace FileCodeMetaDataHandler
{
    public static class CreatorCodeMetaDataFromFile
    {
        public static async Task<SourceCodeMetaData> MakeCodeMetaDataFromSourceFile(String id)
        {
            return await Task.Run(() => new SourceCodeMetaData());
        }
    }
}
