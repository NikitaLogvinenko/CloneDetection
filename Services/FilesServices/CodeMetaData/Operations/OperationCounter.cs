using CodeMetaData.Exceptions;
using Exceptions;

namespace CodeMetaData.Operations
{
    [Serializable]
    public sealed class OperationCounter
    {
        public IOperation Operation { get; }
        public uint UsageNum { get; }
        
        public OperationCounter(IOperation operation, uint usageNum)
        {
            ExceptionsChecker.IsNull(operation);

            UsageNum = usageNum;
            Operation = operation;
        }
    }
}