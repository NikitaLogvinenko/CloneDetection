using CodeMetaData;
using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;

namespace CodeMetaDataConverter
{
    public sealed class MetaDataConverter : IMetaDataConverter
    {
        public FunctionMetaDataDto ConvertFuncMetaDataToDto(FunctionCodeMetaData metaData)
        {
            Exceptions.ExceptionsChecker.IsNull(metaData);

            var dto = new FunctionMetaDataDto();

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
                    dto.Dict.Add(added);
                }
            });

            return dto;
        }

        public FunctionCodeMetaData DeconvertFuncMetaDataDto(FunctionMetaDataDto metaDataDto)
        {
            Exceptions.ExceptionsChecker.IsNull(metaDataDto);

            FunctionCodeMetaData metaData = new FunctionCodeMetaData();

            Parallel.ForEach(metaDataDto.Dict, item =>
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

        public FileMetaDataDto ConvertFileMetaDataToDto(FileMetaData fileMetaData)
        {
            var functionsMetaData = fileMetaData.MetaData;
            var metaDataDto = new FileMetaDataDto();

            foreach(var it in functionsMetaData) 
            {
                var unit = new FileMetaDataUnitDto();
                unit.FuncDto = ConvertFuncMetaDataToDto(it.MetaData);
                unit.Id = it.Id.Id;

                metaDataDto.FileDto.Add(unit);
            }

            return metaDataDto;
        }

        public FileMetaData DeconvertFileMetaDataDto(FileMetaDataDto dto)
        {
            var fileDto = dto.FileDto;

            var fileMetaData = new FileMetaData();

            foreach(var it in fileDto)
            {
                var funcMetaData = DeconvertFuncMetaDataDto(it.FuncDto);
                fileMetaData.Add(new FileMetaDataUnit(new FunctionId(it.Id), funcMetaData));
            }    

            return fileMetaData;
        }
    }
}
