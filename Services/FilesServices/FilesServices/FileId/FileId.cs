using System;

namespace FilesServices
{
    public class FileId
    {
        private readonly string _id;
    
        public FileId(string inputFilePath)
        {
            FileStorageExceptionChecker.CheckForNull(inputFilePath);
            _id = inputFilePath;
        }

        public string GetId()
        {
            return _id;
        }

        public override bool Equals(object obj)
        {
            FileStorageExceptionChecker.CheckForNull(obj);
            if (obj is FileId fileId) return _id == fileId._id;
            return false;
        }

        protected bool Equals(FileId other)
        {
            FileStorageExceptionChecker.CheckForNull(other);
            return other != null && _id == other._id;
        }

        public override int GetHashCode()
        {
            return (_id != null ? _id.GetHashCode() : 0);
        }
    }
}