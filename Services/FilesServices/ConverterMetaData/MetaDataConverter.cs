using CodeMetaData;
using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;

namespace CodeMetaDataConverter
{
    public sealed class MetaDataConverter : IMetaDataConverter<CodeMetaDataDto>
    {
        public CodeMetaDataDto ConvertMetaDataToDto(SourceCodeMetaData metaData)
        {
            Exceptions.ExceptionsChecker.IsNull(metaData);

            var dto = new CodeMetaDataDto();

            Parallel.ForEach(metaData, item =>
            {
                List<OperationCountDto> DtoCounter = [];
                string variable = new("");
                foreach (var keyItem in item.Key)
                {
                    variable = item.Value.VariableId;
                    string operationId = keyItem.Operation.GetOperationId().GetId();
                    string num = keyItem.UsageNum.ToString();
                    string operand = new("");

                    if (keyItem.Operation is BinaryOperation operation)
                    {
                        operand = operation.GetSecondOperand().VariableId;
                    }
                    DtoCounter.Add(new(operationId, num, operand));
                }

                lock (dto)
                {
                    var added = new VariableCountDto();
                    added.OperationDto = DtoCounter;
                    added.Variable = variable;
                    dto.dict.Add(added);
                }
            });

            return dto;
        }

        public SourceCodeMetaData DeconvertDto(CodeMetaDataDto metaDataDto)
        {
            Exceptions.ExceptionsChecker.IsNull(metaDataDto);

            SourceCodeMetaData metaData = new SourceCodeMetaData();

            Parallel.ForEach(metaDataDto.dict, item =>
            {
                VariableUsage usage = new();
                foreach (var keyItem in item.OperationDto)
                {
                    if (!string.IsNullOrEmpty(keyItem.Operand))
                    {
                        usage.Add(new OperationCounter(new BinaryOperation(new OperationId(keyItem.OperationId),
                            new Variable(keyItem.Operand)), Convert.ToUInt32(keyItem.CountNum)));
                        continue;
                    }
                    usage.Add(new OperationCounter(new UnaryOperation(
                        new OperationId(keyItem.OperationId)), Convert.ToUInt32(keyItem.CountNum)));
                }
                lock (metaData)
                {
                    metaData.AddVariable(usage, new Variable(item.Variable));
                }
            });
            return metaData;
        }
    }
}
