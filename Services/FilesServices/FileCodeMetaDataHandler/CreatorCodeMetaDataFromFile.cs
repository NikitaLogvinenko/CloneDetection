using CodeMetaData;

namespace FileCodeMetaDataHandler
{
    public static class CreatorCodeMetaDataFromFile
    {
        public static async Task<SourceCodeMetaData> MakeCodeMetaDataFromSourceFile(string id)
        { 
            return await Task.Run(() => new SourceCodeMetaData());
        }
    }
}
