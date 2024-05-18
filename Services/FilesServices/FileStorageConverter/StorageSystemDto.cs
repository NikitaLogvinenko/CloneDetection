using Newtonsoft.Json;

namespace FileStorageConverter
{
    public sealed class StorageSystemDto
    {
        [JsonProperty]
        public List<FileStorageElementDto> list { get; set; } = new();
        
        public StorageSystemDto() { }
    }
}
