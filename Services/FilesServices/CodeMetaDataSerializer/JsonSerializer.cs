using CodeMetaData;
using CodeMetaDataConverter;
using Exceptions;
using Newtonsoft.Json;
using static System.Runtime.InteropServices.JavaScript.JSType;
using Newtonsoft.Json.Linq;

namespace CodeMetaDataSerializer
{
    public class JsonCodeMetaDataSerializer : ISerializerCodeMetaData<string, CodeMetaDataDto>
    {

        public string Serialize(CodeMetaDataDto metaDataDto)
        {
            ExceptionsChecker.IsNull(metaDataDto);

            return JsonConvert.SerializeObject(metaDataDto);
        }

        public CodeMetaDataDto Deserialize(string metaDataJsonFormat) 
        {
            ExceptionsChecker.IsNullOrEmptyString(metaDataJsonFormat);

            CodeMetaDataDto metaDataDto = JsonConvert.DeserializeObject<CodeMetaDataDto>(metaDataJsonFormat);

            ExceptionsChecker.IsNull(metaDataDto);

            return metaDataDto;
        }
    }
}
