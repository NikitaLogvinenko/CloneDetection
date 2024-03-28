using System;

namespace MetaDataCreator
{
    public class OperationId
    {
        private readonly string _operationId;

        public OperationId(string operationName)
        {
            _operationId = operationName ?? throw new Exception("Null input");
        }

        public string GetId()
        {
            return _operationId;
        }
    }
}