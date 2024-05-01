using System;
using FileStorageSystem;
using FileStorageSerializer;
using FileStorageConverter;
using CodeMetaDataConverter;

namespace FileStorageSystemHandler
{
    public static class StorageSystemHandler<SerializeType, DtoType>
    {
        public static SerializeType SerializeAndConvert(StorageSystem metaData, IFileStorageSystemSerializer<SerializeType, DtoType> serializer, IMetaDataConverter<DtoType> converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(metaData);

            var dto = StorageSystemConverter<DtoType>.ConvertStorageSystem(metaData, converter);

            return serializer.Serialize(dto);
        }

        public static StorageSystem DeserializeAndDeconvert(SerializeType dtoFormat, IFileStorageSystemSerializer<SerializeType, DtoType> serializer, IMetaDataConverter<DtoType> converter)
        {
            Exceptions.ExceptionsChecker.IsNull(serializer);
            Exceptions.ExceptionsChecker.IsNull(dtoFormat);

            var dto = serializer.Deserialize(dtoFormat);

            return StorageSystemConverter<DtoType>.DeconvertStorageSystem(dto, converter);
        }
    }
}
