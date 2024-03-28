namespace MetaDataCreator
{
    public static class MetaDataExceptionChecker
    {
        public static void CheckForNull(object obj)
        {
            if (obj == null)
            {
                throw new MetaDataArgumentException("Input cant be null");
            }
        }
    }
}