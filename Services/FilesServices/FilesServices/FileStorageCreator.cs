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
            
            if (!File.Exists(initFileName))
            {
                throw new ArgumentException("No such input file");
            }

            FileStream stream = new FileStream(initFileName, FileMode.Open);
            var reader = new StreamReader(stream);

            while ((await reader.ReadLineAsync()!)! is { } filePath)
            {
                if (!File.Exists(filePath))
                {
                    throw new ArgumentException("No such file from dictionary");
                }

                await fileSystem.AddNewFile(new FileId(filePath))!;
            }

            return fileSystem;
        }
    }
}