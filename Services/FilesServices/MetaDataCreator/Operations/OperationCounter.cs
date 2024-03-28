using System;

namespace MetaDataCreator
{
    public class OperationCounter
    {
        public IOperation Operation { get; }
        public int UsageNum { get; }
        
        public OperationCounter(IOperation operation, int usageNum)
        {
            UsageNum = usageNum;
            Operation = operation ?? throw new Exception("Null Input");
        }
    }
}