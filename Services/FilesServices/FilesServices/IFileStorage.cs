using System.IO;
using System.Threading.Tasks;

namespace FilesServices
{
    interface IFilesStorage
    {
        public Task<bool> AddNewFile(FileId.FileId inputFileId);
        public Task RemoveFile(FileId.FileId inputFileId);
        public Task<FileInfo>  GetFile(FileId.FileId inputFileId);
        public Task<bool> GetMetaDataForFile(FileId.FileId inputFileId);
        public Task<string> GetFileText(FileId.FileId inputFileId);
    }
}