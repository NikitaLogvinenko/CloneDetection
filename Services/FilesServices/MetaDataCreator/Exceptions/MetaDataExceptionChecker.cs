using System.Collections.Generic;

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

        public static void CheckKeyAddAvailability(VariableUsage obj, Dictionary<VariableUsage, Variable> metaData)
        {
            if (metaData.ContainsKey(obj))
            {
                throw new MetaDataArgumentException("Already such key in Dictionary");
            }
        }

        public static void CheckKeyRemoveAvailability(VariableUsage obj, Dictionary<VariableUsage, Variable> metaData)
        {
            if (!metaData.ContainsKey(obj))
            {
                throw new MetaDataArgumentException("No such key in Dictionary");
            }
        }
    }
}