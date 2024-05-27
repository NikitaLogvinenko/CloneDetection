using FileStorageSystem;
using CodeMetaDataConverter;
using FileStorageSystem.FileId;

namespace FileStorageConverter
{
    public static class StorageSystemConverter
    {
        public static StorageSystemDto ConvertStorageSystem(StorageSystem system, IMetaDataConverter converter)
        {
            StorageSystemDto dto = new();

            foreach (var item in system)
            {
                var added = new FileStorageElementDto();
                added.Id = item.Key.Id;
                added.dto = converter.ConvertFileMetaDataToDto(item.Value);

                dto.list.Add(added);
            }

            return dto;
        }

        public static StorageSystem DeconvertStorageSystem(StorageSystemDto systemDto, IMetaDataConverter converter)
        {
            StorageSystem system = new();

            foreach (var item in systemDto.list)
            {
                _ = system.TryAddFileWithMetaData(new FileId(new FileInfo(item.Id)), converter.DeconvertFileMetaDataDto(item.dto));
            }

            return system;
        }
    }
}
