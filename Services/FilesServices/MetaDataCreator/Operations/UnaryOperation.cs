using System;

namespace MetaDataCreator
{
    public class UnaryOperation : IOperation
    {
        private readonly OperationId _operation;

        public UnaryOperation(OperationId operation)
        {
            _operation = operation ?? throw new Exception("Null input");
        }
        
        public OperationId GetOperationId()
        {
            return _operation;
        }

        public Variable GetSecondOperand()
        {
            return null;
        }
    }
}