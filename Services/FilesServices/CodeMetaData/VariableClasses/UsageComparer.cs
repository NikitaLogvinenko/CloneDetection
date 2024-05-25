namespace CodeMetaData.VariableClasses
{
    [Serializable]
    public sealed class UsageComparer : IEqualityComparer<VariableUsage>
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