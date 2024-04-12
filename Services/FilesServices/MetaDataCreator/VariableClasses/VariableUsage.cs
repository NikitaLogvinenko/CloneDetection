using System;
using System.Collections;
using System.Collections.Generic;

namespace MetaDataCreator
{
    [Serializable]
    public class VariableUsage : IEnumerable, ICloneable
    {
        private readonly List<OperationCounter> _usageCount = new List<OperationCounter>();
        
        public VariableUsage()
        {}
        
        private VariableUsage(List<OperationCounter> usageCount)
        {
            MetaDataExceptionChecker.CheckForNull(usageCount);

            _usageCount = usageCount;
        }

        public void Add(OperationCounter operationCounter)
        {
            MetaDataExceptionChecker.CheckForNull(operationCounter);

            _usageCount.Add(operationCounter);
        }

        public void Remove(OperationCounter operationCounter)
        {
            MetaDataExceptionChecker.CheckForNull(operationCounter);

            _usageCount.Remove(operationCounter);
        }

        public void TryAdd(OperationCounter operationCounter)
        {
            if (operationCounter == null)
            {
                return;
            }
            _usageCount.Add(operationCounter);
        }

        public void TryRemove(OperationCounter operationCounter)
        {
            if (operationCounter == null)
            {
                return;
            }
            _usageCount.Remove(operationCounter);
        }
        
        public int GetSize()
        {
            return _usageCount.Count;
        }
        
        public IEnumerator<OperationCounter> GetEnumerator()
        {
            return _usageCount.GetEnumerator();
        }
        
        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator()!;
        }

        public object Clone() => new VariableUsage(new List<OperationCounter>(_usageCount));
    }
}