using System.Collections;
using CodeMetaData.Exceptions;
using CodeMetaData.VariableClasses;
using Exceptions;

namespace CodeMetaData
{
    [Serializable]
    public sealed class FunctionCodeMetaData : IEnumerable<KeyValuePair<VariableUsage, Variable>>, ICloneable
    {
        private readonly Dictionary<VariableUsage, Variable> _metaData = new(new UsageComparer());

        public FunctionCodeMetaData()
        {

        }
        public FunctionCodeMetaData(Dictionary<VariableUsage, Variable> metaData)
        {
            _metaData = metaData;
        }

        public int GetSize()
        {
            return _metaData.Count;
        }

        public void AddVariable(VariableUsage usage, Variable variable)
        {
            ExceptionsChecker.IsNull(usage);
            ExceptionsChecker.IsNull(variable);
            MetaDataExceptionChecker.IsKeyAvailableToAdd(usage, _metaData);
            
            _metaData.Add(usage, variable);
        }

        public void RemoveVariable(VariableUsage key)
        {
            ExceptionsChecker.IsNull(key);
            MetaDataExceptionChecker.IsKeyAvailableToRemove(key, _metaData);

            _metaData.Remove(key);
        }

        public void TryRemoveVariable(VariableUsage key)
        {
            if(_metaData.ContainsKey(key))
            {
                return;
            }
            _metaData.Remove(key);
        }

        public void RemoveVariableByName(Variable var)
        {
            ExceptionsChecker.IsNull(var);
            
            var searchingKey = _metaData.FirstOrDefault(x => x.Value.VariableId == var.VariableId);

            ExceptionsChecker.IsNull(searchingKey);

            RemoveVariable(searchingKey.Key);
        }

        public bool HasUsages(VariableUsage key)
        {
            ExceptionsChecker.IsNull(key);

            return _metaData.ContainsKey(key);
        }
        
        public IEnumerator<KeyValuePair<VariableUsage, Variable>> GetEnumerator()
        {
            return _metaData.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        public object Clone() => new FunctionCodeMetaData(_metaData);
    }
}