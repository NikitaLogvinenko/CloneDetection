using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeMetaDataConverter
{
    public class FileMetaDataUnitDto
    {
        [JsonProperty]
        public string Id;
        [JsonProperty]
        public FunctionMetaDataDto FuncDto = new();

        public FileMetaDataUnitDto() 
        { }   
    }
}
