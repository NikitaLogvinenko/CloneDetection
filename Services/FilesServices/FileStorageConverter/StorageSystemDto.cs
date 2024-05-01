using CodeMetaDataConverter;
using Newtonsoft.Json;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FileStorageConverter
{
    public class StorageSystemDto<DtoType>
    {
        [JsonProperty]
        public List<FileStorageElementDto<DtoType>> list { get; set; } = new();
        
        public StorageSystemDto() { }
    }
}
