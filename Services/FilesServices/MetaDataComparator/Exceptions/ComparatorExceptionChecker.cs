namespace CodeMetaDataComparator.Exceptions
{
    public static class ComparatorExceptionChecker
    {   
        public static void IsZeroValueParametr(int param)
        {
            if (param == 0)
            {
                throw new ComparatorInputParamException("Param cant be 0");
            }
        }
        
        public static void IsParametrInAvailableRange(float param)
        {
            if (param <= 0 || param >= 1)
            {
                throw new ComparatorInputParamException("Param must be 0 - 1");
            }
        }
    }
}