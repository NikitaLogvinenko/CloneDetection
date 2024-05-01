using CodeMetaData.Exceptions;
using CodeMetaData.VariableClasses;
using Exceptions;

namespace CodeMetaData.Operations
{
    public sealed class BinaryOperation : IOperation
    {
        private readonly OperationId _operation;
        private readonly Variable _operandSecond;

        public BinaryOperation(OperationId operation, Variable secondOperand)
        {
            ExceptionsChecker.IsNull(operation);
            ExceptionsChecker.IsNull(secondOperand);

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