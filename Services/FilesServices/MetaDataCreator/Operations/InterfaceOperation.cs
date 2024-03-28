using System;

namespace MetaDataCreator
{
    public interface IOperation
    {
        public OperationId GetOperationId();
        public Variable GetSecondOperand();
    }
}