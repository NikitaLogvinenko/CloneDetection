using CodeMetaData.Exceptions;
using CodeMetaData.VariableClasses;
using Exceptions;

namespace CodeMetaData.Operations
{
    public sealed class UnaryOperation : IOperation
    {
        private readonly OperationId _operation;

        public UnaryOperation(OperationId operation)
        {
            ExceptionsChecker.IsNull(operation);

            _operation = operation;
        }
        
        public OperationId GetOperationId()
        {
            return _operation;
        }

        //public Variable GetSecondOperand()
        //{
        //    return null;
        //}
    }
}