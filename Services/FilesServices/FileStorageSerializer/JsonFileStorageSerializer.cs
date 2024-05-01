using Exceptions;
using FileStorageConverter;
using Newtonsoft.Json;

namespace FileStorageSerializer
{
    public class JsonFileStorageSerializer<DtoType> : IFileStorageSystemSerializer<string, DtoType>
    {
        public StorageSystemDto<DtoType> Deserialize(string systemSerializeFormat)
        {
            ExceptionsChecker.IsNullOrEmptyString(systemSerializeFormat);

            StorageSystemDto<DtoType> dto = JsonConvert.DeserializeObject<StorageSystemDto<DtoType>>(systemSerializeFormat);

            ExceptionsChecker.IsNull(dto);

            return dto;
        }

        public string Serialize(StorageSystemDto<DtoType> systemDto)
        {
            ExceptionsChecker.IsNull(systemDto);

            return JsonConvert.SerializeObject(systemDto);
        }
    }
}
