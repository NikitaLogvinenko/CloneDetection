namespace CodeMetaData.Exceptions
{
    public class MetaDataArgumentException : ArgumentException
    {
        public MetaDataArgumentException(string message): base(message)
        {}
    }
}