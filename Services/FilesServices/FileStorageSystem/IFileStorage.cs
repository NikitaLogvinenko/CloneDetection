using CodeMetaData;

namespace FileStorageSystem
{
    interface IFilesStorage
    {
        public Task<bool> TryAddNewFile(FileId.FileId inputFileId);
        public Task<bool> TryRemoveFile(FileId.FileId inputFileId);
        public Task<FileInfo> GetFile(FileId.FileId inputFileId);
        public Task<FileMetaData> GetCodeMetaDataForFile(FileId.FileId inputFileId);
        public Task<string> GetFileText(FileId.FileId inputFileId);
    }
}