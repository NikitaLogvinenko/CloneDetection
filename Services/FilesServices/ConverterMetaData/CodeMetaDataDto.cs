using System.Collections;
using System.Text.Json;
using Exceptions;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using JsonConverter = Newtonsoft.Json.JsonConverter;

namespace CodeMetaDataConverter
{
    public sealed class CodeMetaDataDto
    {
        [JsonProperty]
        public List<VariableCountDto> dict { get; set; } = [];

        public CodeMetaDataDto()
        { }
    }
}
