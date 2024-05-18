using CodeMetaData;

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

        public static void IsZeroSizeMetaData(FunctionCodeMetaData metaData)
        {
            if(metaData.GetSize() == 0)
            {
                throw new ComparatorInputArgumentException("Zero Size Meta Data");
            }
        }
    }
}