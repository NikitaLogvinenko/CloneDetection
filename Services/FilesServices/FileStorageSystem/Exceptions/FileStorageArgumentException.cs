namespace FileStorageSystem.Exceptions
{
    public class FileStorageArgumentException : ArgumentException
    {
        public FileStorageArgumentException(string message): base(message)
        {}
    }
}