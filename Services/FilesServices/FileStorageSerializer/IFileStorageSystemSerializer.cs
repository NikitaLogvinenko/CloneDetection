using FileStorageConverter;

namespace FileStorageSerializer
{
    public interface IFileStorageSystemSerializer<SerializeType>
    {
        public SerializeType Serialize(StorageSystemDto system);

        public StorageSystemDto Deserialize(SerializeType systemSerializeFormat);
    }
}
