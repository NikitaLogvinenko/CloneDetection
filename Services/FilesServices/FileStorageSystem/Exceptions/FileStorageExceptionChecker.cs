using System.Collections.Concurrent;

namespace FileStorageSystem.Exceptions
{
    public static class FileStorageExceptionChecker
    {
        public static void IsNotExistFile(FileInfo inputFileInfo)
        {
            if (!inputFileInfo.Exists)
            {
                throw new FileStorageArgumentException("No such file");
            }
        }

        public static void IsNotExistInSystem(ConcurrentDictionary<FileId.FileId, CodeMetaData.SourceCodeMetaData> fileStorageDictionary, FileId.FileId inputFileId)
        {
            if (!fileStorageDictionary.TryGetValue(inputFileId, out var metaData))
            {
                throw new FileStorageArgumentException("No such file in system");
            }
        }

        public static void IsEmptyFile(FileId.FileId inputFileId)
        {
            if (inputFileId.GetId().Length == 0)
            {
                throw new FileStorageArgumentException("Empty input file");
            }
        }
    }
}