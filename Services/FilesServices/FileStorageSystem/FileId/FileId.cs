using Exceptions;
using FileStorageSystem.Exceptions;

namespace FileStorageSystem.FileId
{
    public sealed class FileId : IEquatable<FileId>
    {
        private readonly String _id;
    
        public FileId(FileInfo inputFileInfo)
        {
            ExceptionsChecker.IsNull(inputFileInfo);
            FileStorageExceptionChecker.IsNotExistFile(inputFileInfo);

            _id = inputFileInfo.FullName;
        }

        public String GetId()
        {
            return _id;
        }

        public override bool Equals(object obj)
        {
            ExceptionsChecker.IsNull(obj);

            return obj is FileId fileId ? _id.Equals(fileId._id) : false;
        }

        public bool Equals(FileId other)
        {
            ExceptionsChecker.IsNull(other);

            return _id.Equals(other._id);
        }

        public override int GetHashCode()
        {
            return _id.GetHashCode();
        }
    }
}