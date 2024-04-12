using System;

namespace MetaDataCreator
{
    [Serializable]
    public class OperationCounter
    {
        public IOperation Operation { get; }
        public uint UsageNum { get; }
        
        public OperationCounter(IOperation operation, uint usageNum)
        {
            UsageNum = usageNum;

            MetaDataExceptionChecker.CheckForNull(operation);

            Operation = operation;
        }
    }
}