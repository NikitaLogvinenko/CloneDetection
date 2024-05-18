using Newtonsoft.Json;

namespace CodeMetaDataConverter
{
    public class FileMetaDataDto
    {
        [JsonProperty]
        public List<FileMetaDataUnitDto> FileDto = new();

        public FileMetaDataDto() { }
    }
}
