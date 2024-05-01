using CodeMetaData.Exceptions;
using Exceptions;

namespace CodeMetaData.VariableClasses
{
    [Serializable]
    public class Variable
    {
        public string VariableId { get; }

        public Variable(string variableId)
        {
            ExceptionsChecker.IsNullOrEmptyString(variableId);

            VariableId = variableId;
        }
    }
}