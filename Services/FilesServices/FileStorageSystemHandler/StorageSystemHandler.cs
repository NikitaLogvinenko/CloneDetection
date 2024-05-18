using FileStorageSystem;
using FileStorageSerializer;
using FileStorageConverter;
using CodeMetaDataConverter;

namespace FileStorageSystemHandler
{
    public static class StorageSystemHandler<SerializeType>
    {
        public static SerializeType SerializeAndConvert(StorageSystem system, IFileStorageSystemSerializer<SerializeType> serializer, IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(converter);
            Exceptions.ExceptionsChecker.IsNull(system);

            var dto = StorageSystemConverter.ConvertStorageSystem(system, converter);

            return serializer.Serialize(dto);
        }

        public static StorageSystem DeserializeAndDeconvert(SerializeType dtoFormat, IFileStorageSystemSerializer<SerializeType> serializer, IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(converter);
            Exceptions.ExceptionsChecker.IsNull(dtoFormat);

            var dto = serializer.Deserialize(dtoFormat);

            return StorageSystemConverter.DeconvertStorageSystem(dto, converter);
        }

        public static void SaveToStream(StreamWriter writer, StorageSystem system, IFileStorageSystemSerializer<SerializeType> serializer, IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(converter);
            Exceptions.ExceptionsChecker.IsNull(system);

            var serializeSystem = SerializeAndConvert(system, serializer, converter);

            writer.Write(serializeSystem);
        }

        public static StorageSystem ReadFromStream(StreamReader reader, IFileStorageSystemSerializer<SerializeType> serializer, IMetaDataConverter converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(converter);

            string data = reader.ReadLine();

            Exceptions.ExceptionsChecker.IsNull(data);

            var deserializedData = (SerializeType)Convert.ChangeType(data, typeof(SerializeType));

            Exceptions.ExceptionsChecker.IsNull(deserializedData);

            return DeserializeAndDeconvert(deserializedData, serializer, converter);
        }
    }
}
