using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CodeMetaData;
using CodeMetaDataConverter;



namespace CodeMetaDataSerializer
{
    public interface ISerializerCodeMetaData<SerializeType, DtoType>
    {
        public SerializeType Serialize(DtoType metaData);

        public DtoType Deserialize(SerializeType metaDataSerializeFormat);
    }
}
