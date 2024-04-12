using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace MetaDataCreator
{
    [Serializable]
    public class MetaData : IEnumerable<KeyValuePair<VariableUsage, Variable>>
    {
        private readonly Dictionary<VariableUsage, Variable> _metaData =
            new Dictionary<VariableUsage, Variable>(new UsageComparer());

        public int GetSize()
        {
            return _metaData.Count;
        }

        public void AddVariable(VariableUsage usage, Variable variable)
        {
            MetaDataExceptionChecker.CheckForNull(usage);
            MetaDataExceptionChecker.CheckForNull(variable);
            MetaDataExceptionChecker.CheckKeyAddAvailability(usage, _metaData);
            
            _metaData.Add(usage, variable);
        }

        public void RemoveVariable(VariableUsage key)
        {
            MetaDataExceptionChecker.CheckForNull(key);
            MetaDataExceptionChecker.CheckKeyRemoveAvailability(key, _metaData);

            _metaData.Remove(key);
        }

        public void RemoveVariableByName(Variable var)
        {
            MetaDataExceptionChecker.CheckForNull(var);
            
            VariableUsage searchingKey = _metaData.FirstOrDefault(x => x.Value.VariableId == var.VariableId).Key;
            
            RemoveVariable(searchingKey);
        }

        public bool HasSuchUsages(VariableUsage key)
        {
            MetaDataExceptionChecker.CheckForNull(key);

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
    }
}