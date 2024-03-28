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
    }
}