using FileStorageConverter;

namespace FileStorageSerializer
{
    public interface IFileStorageSystemSerializer<SerializeType, DtoType>
    {
        public SerializeType Serialize(StorageSystemDto<DtoType> system);

        public StorageSystemDto<DtoType> Deserialize(SerializeType systemSerializeFormat);
    }
}
