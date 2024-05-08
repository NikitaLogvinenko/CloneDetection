using System;
using System.IO;
using System.Threading.Tasks;
using FileStorageSystem;
using FileStorageSystem.Exceptions;
using FileStorageSystem.FileId;

namespace FileStorageCreator
{
    public static class FileStorageCreator
    {
        public static async Task<StorageSystem> CreateFileStorageFromSourceFile(FileInfo initFileInfo)
        {
            StorageSystem fileSystem = new StorageSystem();
            
            Exceptions.ExceptionsChecker.IsNull(initFileInfo);
            FileStorageExceptionChecker.IsNotExistFile(initFileInfo);

            FileStream stream = new FileStream(initFileInfo.FullName, FileMode.Open, FileAccess.Read, FileShare.Read);
            var reader = new StreamReader(stream);
            int i = 0;
            while ((await reader.ReadLineAsync()) is { } filePath)
            {
                FileStorageExceptionChecker.IsNotExistFile(new FileInfo(filePath));

                await fileSystem.TryAddNewFile(new FileId(new FileInfo(filePath)));
                Console.WriteLine(i++);
            }

            return fileSystem;
        }
    }
}