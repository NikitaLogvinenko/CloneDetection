namespace MetaDataCreator
{
    public class OperationId
    {
        private readonly string _operationId;

        public OperationId(string operationName)
        {
            MetaDataExceptionChecker.CheckForNull(operationName);

            _operationId = operationName;
        }

        public string GetId()
        {
            return _operationId;
        }
    }
}