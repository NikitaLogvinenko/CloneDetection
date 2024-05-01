using Exceptions;
using FileStorageSystem.Exceptions;

namespace FileStorageSystem
{
    public static class FileStorageCreator
    {
        public static async Task<StorageSystem> CreateFileStorageFromSourceFile(string initFileName)
        {
            StorageSystem fileSystem = new StorageSystem();

            ExceptionsChecker.IsNullOrEmptyString(initFileName);
            //FileStorageExceptionChecker.IsNotExistFile();

            FileStream stream = new FileStream(initFileName, FileMode.Open, FileAccess.Read, FileShare.Read);
            var reader = new StreamReader(stream);

            while (await reader.ReadLineAsync() is { } filePath)
            {
                var info = new FileInfo(filePath);

                FileStorageExceptionChecker.IsNotExistFile(info);
                await fileSystem.TryAddNewFile(new FileId.FileId(info));
            }

            await stream.DisposeAsync();

            return fileSystem;
        }
    }
}