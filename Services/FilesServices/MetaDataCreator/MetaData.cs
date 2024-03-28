using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;

namespace MetaDataCreator
{
    public class MetaData : IEnumerable<KeyValuePair<VariableUsage, Variable>>
    {
        private readonly Dictionary<VariableUsage, Variable> _metaData =
            new Dictionary<VariableUsage, Variable>(new UsageComparer());

        private int _currentIndex = -1;

        public int GetSize()
        {
            return _metaData.Count;
        }

        public void AddVariable(VariableUsage usage, Variable variable)
        {
            MetaDataExceptionChecker.CheckForNull(usage);
            MetaDataExceptionChecker.CheckForNull(variable);

            if (_metaData.ContainsKey(usage))
            {
                throw new Exception("Already have such key");
            }
            
            _metaData.Add(usage, variable);
        }

        public void RemoveVariable(VariableUsage key)
        {
            MetaDataExceptionChecker.CheckForNull(key);

            if (!_metaData.ContainsKey(key))
            {
                throw new ArgumentException("No such key");
            }
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