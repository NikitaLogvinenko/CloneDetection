namespace CodeMetaData.Exceptions
{
    public sealed class MetaDataArgumentException : ArgumentException
    {
        public MetaDataArgumentException(string message): base(message)
        {}
    }
}