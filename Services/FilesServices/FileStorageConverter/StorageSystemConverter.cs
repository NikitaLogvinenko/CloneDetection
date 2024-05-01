using FileStorageSystem;
using CodeMetaDataConverter;
using FileStorageSystem.FileId;

namespace FileStorageConverter
{
    public static class StorageSystemConverter<DtoType>
    {
        public static StorageSystemDto<DtoType> ConvertStorageSystem(StorageSystem system, IMetaDataConverter<DtoType> converter)
        {
            StorageSystemDto<DtoType> dto = new();

            foreach (var item in system)
            {
                var added = new FileStorageElementDto<DtoType>();
                added.Id = item.Key.GetId();
                added.dto = converter.ConvertMetaDataToDto(item.Value);

                dto.list.Add(added);
            }

            return dto;
        }

        public static StorageSystem DeconvertStorageSystem(StorageSystemDto<DtoType> systemDto, IMetaDataConverter<DtoType> converter)
        {
            StorageSystem system = new();

            foreach (var item in systemDto.list)
            {
                _ = system.AddFileWithMetaData(new FileId(new FileInfo(item.Id)), converter.DeconvertDto(item.dto));
            }

            return system;
        }
    }
}
