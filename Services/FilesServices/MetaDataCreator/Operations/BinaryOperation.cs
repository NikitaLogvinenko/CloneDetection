using System;

namespace MetaDataCreator
{
    public class BinaryOperation : IOperation
    {
        private readonly OperationId _operation;
        private readonly Variable _operandSecond;

        public BinaryOperation(OperationId operation, Variable second)
        {
            if (operation == null || second == null)
            {
                throw new Exception("Null input");
            }

            _operation = operation;
            _operandSecond = second;
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