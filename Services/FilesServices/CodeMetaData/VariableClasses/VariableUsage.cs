using System.Collections;
using CodeMetaData.Exceptions;
using CodeMetaData.Operations;
using Exceptions;

namespace CodeMetaData.VariableClasses
{
    [Serializable]
    public sealed class VariableUsage : IEnumerable, ICloneable
    {
        private readonly List<OperationCounter> _usageCount = new();
        
        public VariableUsage()
        {}
        
        private VariableUsage(List<OperationCounter> usageCount)
        {
            ExceptionsChecker.IsNull(usageCount);

            _usageCount = usageCount;
        }

        public void Add(OperationCounter operationCounter)
        {
            ExceptionsChecker.IsNull(operationCounter);

            _usageCount.Add(operationCounter);
        }

        public void Remove(OperationCounter operationCounter)
        {
            ExceptionsChecker.IsNull(operationCounter);

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