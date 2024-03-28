using System;
using System.Collections.Generic;

namespace FilesServices
{
    class FileIdComparer : IEqualityComparer<FileId>
    {
        public bool Equals(FileId x, FileId y)
        {
            FileStorageExceptionChecker.CheckForNull(x);
            FileStorageExceptionChecker.CheckForNull(y);

            return x.GetId() == y.GetId();
        }

        public int GetHashCode(FileId obj)
        {
            return obj.GetHashCode();
        }
    }
}