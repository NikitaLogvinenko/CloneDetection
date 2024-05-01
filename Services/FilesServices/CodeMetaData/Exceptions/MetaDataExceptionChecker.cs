using CodeMetaData.VariableClasses;
using Microsoft.VisualBasic;

namespace CodeMetaData.Exceptions
{
    public static class MetaDataExceptionChecker
    {
        public static void IsKeyAvailableToAdd(VariableUsage obj, Dictionary<VariableUsage, Variable> metaData)
        {
            if (metaData.ContainsKey(obj))
            {
                throw new MetaDataArgumentException("Already such key in Dictionary");
            }
        }

        public static void IsKeyAvailableToRemove(VariableUsage obj, Dictionary<VariableUsage, Variable> metaData)
        {
            if (!metaData.ContainsKey(obj))
            {
                throw new MetaDataArgumentException("No such key in Dictionary");
            }
        }
        public static void IsNullUsageNum(uint UsageNum)
        {
            if(UsageNum == 0) 
            {
                throw new MetaDataArgumentException("Usage num cant be 0");
            }
        }
    }
}