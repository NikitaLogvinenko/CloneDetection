using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileStorageConverter
{
    public class FileStorageElementDto<DtoType>
    {
        [JsonProperty]
        public string Id { get; set; }
        [JsonProperty]
        public DtoType dto { get; set; }

        public FileStorageElementDto() 
        {}
    }
}
