using System.IO;

namespace FilesServices
{
    public static class FileStorageExceptionChecker
    {
        public static void CheckForNull(object obj)
        {
            if (obj == null)
            {
                throw new FileStorageArgumentException("FileId cant be Null");
            }
        }

        public static void CheckExistFile(string initFileName)
        {
            if (!File.Exists(initFileName))
            {
                throw new FileStorageArgumentException("No such file");
            }
        }

        public static void CheckExistInSystem(bool flag)
        {
            if (!flag)
            {
                throw new FileStorageArgumentException("No such file in system");
            }
        }
    }
}