using System;
using System.IO;
using System.Threading.Tasks;

namespace FilesServices
{
    public static class FileStorageCreator
    {
        public static async Task<StorageSystem> CreateFileStorageFromSourceFile(string initFileName)
        {
            StorageSystem fileSystem = new StorageSystem();
            
            FileStorageExceptionChecker.CheckForNull(initFileName);
            FileStorageExceptionChecker.CheckExistFile(initFileName);

            FileStream stream = new FileStream(initFileName, FileMode.Open, FileAccess.Read, FileShare.Read);
            var reader = new StreamReader(stream);

            while ((await reader.ReadLineAsync()!)! is { } filePath)
            {
                FileStorageExceptionChecker.CheckExistFile(filePath);

                await fileSystem.AddNewFile(new FileId(filePath))!;
            }

            return fileSystem;
        }
    }
}