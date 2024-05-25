using CodeMetaDataConverter;
using Newtonsoft.Json;

namespace FileStorageConverter
{
    public sealed class FileStorageElementDto
    {
        [JsonProperty]
        public string Id { get; set; }
        [JsonProperty]
        public FileMetaDataDto dto { get; set; }

        public FileStorageElementDto() 
        {}
    }
}
