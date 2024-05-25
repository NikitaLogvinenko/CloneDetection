using CodeMetaData.VariableClasses;

namespace CodeMetaData.Operations
{
    public interface IOperation
    {
        public OperationId GetOperationId();
        //public Variable GetSecondOperand();
    }
}