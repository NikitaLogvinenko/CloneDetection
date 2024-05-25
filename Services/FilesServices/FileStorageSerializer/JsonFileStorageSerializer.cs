using Exceptions;
using FileStorageConverter;
using Newtonsoft.Json;

namespace FileStorageSerializer
{
    public sealed class JsonFileStorageSerializer : IFileStorageSystemSerializer<string>
    {
        public StorageSystemDto Deserialize(string systemSerializeFormat)
        {
            ExceptionsChecker.IsNullOrEmptyString(systemSerializeFormat);

            StorageSystemDto dto = JsonConvert.DeserializeObject<StorageSystemDto>(systemSerializeFormat);

            ExceptionsChecker.IsNull(dto);

            return dto;
        }

        public string Serialize(StorageSystemDto systemDto)
        {
            ExceptionsChecker.IsNull(systemDto);

            return JsonConvert.SerializeObject(systemDto);
        }
    }
}
