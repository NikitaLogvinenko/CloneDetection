using CodeMetaDataConverter;

namespace CodeMetaDataSerializer
{
    public interface ISerializerCodeMetaData<SerializeType>
    {
        public SerializeType SerializeFuncMetaData(FunctionMetaDataDto metaData);

        public FunctionMetaDataDto DeserializeFuncMetaData(SerializeType metaDataSerializeFormat);

        public SerializeType SerializeFileMetaData(FileMetaDataDto metaDataDto);

        public FileMetaDataDto DeserializeFileMetaData(SerializeType metaDataJsonFormat);
    }
}
