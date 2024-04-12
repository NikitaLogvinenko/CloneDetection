using System;

namespace MetaDataCreator
{
    [Serializable]
    public class Variable
    {
        public string VariableId { get; }

        public Variable(string variableId)
        {
            MetaDataExceptionChecker.CheckForNull(variableId);

            VariableId = variableId;
        }
    }
}