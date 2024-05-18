using CodeMetaData;
using CodeMetaDataConverter;
using CodeMetaDataSerializer;

namespace CodeMetaDataHandler
{
    public static class MetaDataHandler
    {
        public static SerializeType SerializeAndConvert<SerializeType>(FunctionCodeMetaData metaData, ISerializerCodeMetaData<SerializeType> serializer, 
            IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(metaData);
            Exceptions.ExceptionsChecker.IsNull(converter);

            var dto = converter.ConvertFuncMetaDataToDto(metaData);

            return serializer.SerializeFuncMetaData(dto);
        }

        public static FunctionCodeMetaData DeserializeAndDeconvert<SerializeType>(SerializeType dtoFormat, ISerializerCodeMetaData<SerializeType> serializer,
            IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(dtoFormat);
            Exceptions.ExceptionsChecker.IsNull(converter);

            var dto = serializer.DeserializeFuncMetaData(dtoFormat);

            return converter.DeconvertFuncMetaDataDto(dto);
        }

        public static SerializeType SerializeAndConvertFileMetaData<SerializeType>(FileMetaData metaData, 
            ISerializerCodeMetaData<SerializeType> serializer, IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(metaData);
            Exceptions.ExceptionsChecker.IsNull(converter);

            var dto = converter.ConvertFileMetaDataToDto(metaData);

            return serializer.SerializeFileMetaData(dto);
        }

        public static FileMetaData DeserializeAndConvertFileMetaData<SerializeType>(SerializeType dtoFormat,
            ISerializerCodeMetaData<SerializeType> serializer, IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(dtoFormat);
            Exceptions.ExceptionsChecker.IsNull(converter);

            var dto = serializer.DeserializeFileMetaData(dtoFormat);

            return converter.DeconvertFileMetaDataDto(dto);
        }
    }
}
