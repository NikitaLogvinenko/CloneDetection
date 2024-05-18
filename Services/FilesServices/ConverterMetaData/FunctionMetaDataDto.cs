using System.Text.Json;
using Newtonsoft.Json;

namespace CodeMetaDataConverter
{
    public sealed class FunctionMetaDataDto
    {
        [JsonProperty]
        public List<VariableCountDto> Dict { get; set; } = [];

        public FunctionMetaDataDto()
        { }
    }
}
