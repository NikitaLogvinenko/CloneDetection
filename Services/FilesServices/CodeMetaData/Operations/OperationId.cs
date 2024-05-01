using CodeMetaData.Exceptions;
using Exceptions;

namespace CodeMetaData.Operations
{
    public sealed class OperationId
    {
        private readonly string _operationId;

        public OperationId(string operationName)
        {
            ExceptionsChecker.IsNullOrEmptyString(operationName);

            _operationId = operationName;
        }

        public string GetId()
        {
            return _operationId;
        }
    }
}