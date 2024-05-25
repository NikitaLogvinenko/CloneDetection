using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeMetaDataConverter
{
    public sealed class VariableCountDto
    {
        [JsonProperty]
        public string Variable { get; set; }
        [JsonProperty]
        public List<OperationCountDto> OperationDto { get; set; }
        public VariableCountDto() { }
    }
}
