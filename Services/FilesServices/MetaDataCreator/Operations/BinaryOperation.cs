using System;

namespace MetaDataCreator
{
    public class BinaryOperation : IOperation
    {
        private readonly OperationId _operation;
        private readonly Variable _operandSecond;

        public BinaryOperation(OperationId operation, Variable secondOperand)
        {
            MetaDataExceptionChecker.CheckForNull(operation);
            MetaDataExceptionChecker.CheckForNull(secondOperand);

            _operation = operation;
            _operandSecond = secondOperand;
        }
        
        public OperationId GetOperationId()
        {
            return _operation;
        }

        public Variable GetSecondOperand()
        {
            return _operandSecond;
        }
    }
}