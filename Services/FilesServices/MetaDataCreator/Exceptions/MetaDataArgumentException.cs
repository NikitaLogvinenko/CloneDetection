using System;

namespace MetaDataCreator
{
    public class MetaDataArgumentException : ArgumentException
    {
        public MetaDataArgumentException(string message): base(message)
        {}
    }
}