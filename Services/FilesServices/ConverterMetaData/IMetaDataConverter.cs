using CodeMetaData;

namespace CodeMetaDataConverter
{
    public interface IMetaDataConverter<DtoType>
    {
        public DtoType ConvertMetaDataToDto(SourceCodeMetaData metaData);

        public SourceCodeMetaData DeconvertDto(DtoType metaDataDto);
    }
}
