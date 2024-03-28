namespace MetaDataComparator.Exceptions
{
    public static class ComparatorExceptionChecker
    {
        public static void CheckForNull(object obj)
        {
            if (obj == null)
            {
                throw new ComparatorInputArgumentException("Input cant be null");
            }
        }
        
        public static void CheckForZero(int param)
        {
            if (param == 0)
            {
                throw new ComparatorInputParamException("Param cant be 0");
            }
        }
        
        public static void CheckForRange(float param)
        {
            if (param <= 0 || param >= 1)
            {
                throw new ComparatorInputParamException("Param must be 0 - 1");
            }
        }
    }
}