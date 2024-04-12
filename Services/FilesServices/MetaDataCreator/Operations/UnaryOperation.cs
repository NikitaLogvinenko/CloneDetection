using System;

namespace MetaDataCreator
{
    public class UnaryOperation : IOperation
    {
        private readonly OperationId _operation;

        public UnaryOperation(OperationId operation)
        {
            MetaDataExceptionChecker.CheckForNull(operation);

            _operation = operation;
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