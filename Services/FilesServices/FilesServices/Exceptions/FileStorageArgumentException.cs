using System;

namespace FilesServices
{
    public class FileStorageArgumentException : ArgumentException
    {
        public FileStorageArgumentException(string message): base(message)
        {}
    }
}