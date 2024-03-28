using System;
using System.Collections;
using System.Collections.Generic;

namespace MetaDataCreator
{
    public class VariableUsage : IEnumerable, ICloneable
    {
        private readonly List<OperationCounter> _usageCount = new List<OperationCounter>();
        
        public VariableUsage()
        {}
        
        private VariableUsage(List<OperationCounter> usageCount)
        {
            _usageCount = usageCount ?? throw new Exception("Null Input");
        }

        public void Add(OperationCounter operationCounter)
        {
            if (operationCounter == null)
            {
                throw new Exception("Null input");
            }
            _usageCount.Add(operationCounter);
        }

        public void Remove(OperationCounter operationCounter)
        {
            if (operationCounter == null)
            {
                throw new Exception("Null input");
            }
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