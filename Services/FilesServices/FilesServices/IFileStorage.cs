using System.IO;
using System.Threading.Tasks;
using MetaDataCreator;

namespace FilesServices
{
    interface IFilesStorage
    {
        public Task<bool> AddNewFile(FileId inputFileId);
        public Task RemoveFile(FileId inputFileId);
        public Task<FileInfo>  GetFile(FileId inputFileId);
        public Task<bool> GetMetaDataForFile(FileId inputFileId);
        public Task<string> GetFileText(FileId inputFileId);
    }
}