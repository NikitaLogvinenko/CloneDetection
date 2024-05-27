using System.Collections.Concurrent;
using CodeMetaData;
using FileStorageSystem.Exceptions;
using Exceptions;
using System.Collections;
using System.Collections.Immutable;
using CLR;

namespace FileStorageSystem
{
    public sealed class StorageSystem : IFilesStorage, IEnumerable<KeyValuePair<FileId.FileId, FileMetaData>>
    {
        private ConcurrentDictionary<FileId.FileId, FileMetaData> _fileStorage = new();

        private readonly SemaphoreSlim _semaphoreSlim = new SemaphoreSlim(1, 1);

        public ImmutableDictionary<FileId.FileId, FileMetaData> FileStorageDictionary => _fileStorage.ToImmutableDictionary();

        public int GetSize()
        {
            return _fileStorage.Count;
        }
        
        public async Task<bool> TryAddNewFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);
            FileStorageExceptionChecker.IsEmptyFile(inputFileId);

            //var metaData = CMCDFacadeWrapper.ProccesFunctionsCodeMetaData(new FileInfo(inputFileId.Id));

            var metaData = new CodeMetaData.FileMetaData();

            ExceptionsChecker.IsNull(metaData);

            return await Task.Run(() => _fileStorage.TryAdd(inputFileId, metaData));
        }
        
        public async Task<bool> TryRemoveFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);

            return await Task.Run(() => _fileStorage.TryRemove(inputFileId, out _));
        }

        public async Task<FileInfo> GetFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);

            await _semaphoreSlim.WaitAsync();
            FileStorageExceptionChecker.IsNotExistInSystem(_fileStorage, inputFileId);
            _semaphoreSlim.Release();

            return await Task.Run(() => new FileInfo(inputFileId.Id));
        }

        public async Task<FileMetaData> GetCodeMetaDataForFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);
            FileStorageExceptionChecker.IsEmptyFile(inputFileId);

            var metaData = await Task.Run(() => _fileStorage.GetValueOrDefault(inputFileId));

            ExceptionsChecker.IsNull(metaData);

            return metaData;
        }

        public async Task<bool> ContainsKey(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);

            return await Task.Run(() => _fileStorage.ContainsKey(inputFileId));
        }

        public async Task<string> GetFileText(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);
            FileStorageExceptionChecker.IsEmptyFile(inputFileId);

            await _semaphoreSlim.WaitAsync();
            FileStorageExceptionChecker.IsNotExistInSystem(_fileStorage, inputFileId);
            _semaphoreSlim.Release();

            using StreamReader reader = new StreamReader(inputFileId.Id);
                
            return await reader.ReadToEndAsync();
        }
        
        public async Task<bool> TryAddFileWithMetaData(FileId.FileId inputFileId, FileMetaData metaData)
        {
            ExceptionsChecker.IsNull(inputFileId);
            ExceptionsChecker.IsNull(metaData);

            return await Task.Run(() => _fileStorage.TryAdd(inputFileId, metaData));
        }

        public IEnumerator<KeyValuePair<FileId.FileId, FileMetaData>> GetEnumerator()
        {
            return _fileStorage.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}