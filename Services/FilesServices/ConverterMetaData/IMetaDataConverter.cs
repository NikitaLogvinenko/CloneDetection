using CodeMetaData;

namespace CodeMetaDataConverter
{
    public interface IMetaDataConverter
    {
        public FunctionMetaDataDto ConvertFuncMetaDataToDto(FunctionCodeMetaData metaData);

        public FunctionCodeMetaData DeconvertFuncMetaDataDto(FunctionMetaDataDto metaDataDto);

        public FileMetaData DeconvertFileMetaDataDto(FileMetaDataDto dto);

        public FileMetaDataDto ConvertFileMetaDataToDto(FileMetaData fileMetaData);
    }
}
