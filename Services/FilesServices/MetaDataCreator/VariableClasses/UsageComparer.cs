using System;
using System.Collections.Generic;

namespace MetaDataCreator
{
    [Serializable]
    public class UsageComparer : IEqualityComparer<VariableUsage>
    {
        public bool Equals(VariableUsage x, VariableUsage y)
        {
            return ReferenceEquals(x, y);
        }

        public int GetHashCode(VariableUsage obj)
        {
            return obj.GetHashCode();
        }
    }
}