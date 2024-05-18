using CodeMetaDataConverter;
using Exceptions;
using Newtonsoft.Json;

namespace CodeMetaDataSerializer
{
    public sealed class JsonCodeMetaDataSerializer : ISerializerCodeMetaData<string>
    {

        public string SerializeFuncMetaData(FunctionMetaDataDto metaDataDto)
        {
            ExceptionsChecker.IsNull(metaDataDto);

            return JsonConvert.SerializeObject(metaDataDto);
        }

        public FunctionMetaDataDto DeserializeFuncMetaData(string metaDataJsonFormat) 
        {
            ExceptionsChecker.IsNullOrEmptyString(metaDataJsonFormat);

            FunctionMetaDataDto metaDataDto = JsonConvert.DeserializeObject<FunctionMetaDataDto>(metaDataJsonFormat);

            ExceptionsChecker.IsNull(metaDataDto);

            return metaDataDto;
        }

        public string SerializeFileMetaData(FileMetaDataDto metaDataDto)
        {
            ExceptionsChecker.IsNull(metaDataDto);

            return JsonConvert.SerializeObject(metaDataDto);
        }

        public FileMetaDataDto DeserializeFileMetaData(string metaDataJsonFormat)
        {
            ExceptionsChecker.IsNullOrEmptyString(metaDataJsonFormat);

            FileMetaDataDto metaDataDto = JsonConvert.DeserializeObject<FileMetaDataDto>(metaDataJsonFormat);

            ExceptionsChecker.IsNull(metaDataDto);

            return metaDataDto;
        }
    }
}
