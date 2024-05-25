namespace CodeMetaDataComparator.Exceptions
{
    public class ComparatorInputArgumentException : ArgumentException
    {
        public ComparatorInputArgumentException(string message): base(message)
        {}
    }
    
    public class ComparatorInputParamException : ArgumentException
    {
        public ComparatorInputParamException(string message): base(message)
        {}
    }
}