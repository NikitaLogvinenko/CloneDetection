using CodeMetaData;
using CodeMetaDataConverter;
using CodeMetaDataSerializer;
using System;

namespace CodeMetaDataHandler
{ 
    public static class MetaDataHandler
    {
        public static SerializeType SerializeAndConvert<SerializeType, DtoType>(SourceCodeMetaData metaData, ISerializerCodeMetaData<SerializeType, DtoType> serializer, 
            IMetaDataConverter<DtoType> converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(metaData);

            var dto = converter.ConvertMetaDataToDto(metaData);

            return serializer.Serialize(dto);
        }

        public static SourceCodeMetaData DeserializeAndDeconvert<SerializeType, DtoType>(SerializeType dtoFormat, ISerializerCodeMetaData<SerializeType, DtoType> serializer,
            IMetaDataConverter<DtoType> converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(dtoFormat);

            var dto = serializer.Deserialize(dtoFormat);

            return converter.DeconvertDto(dto);
        }
    }
}
