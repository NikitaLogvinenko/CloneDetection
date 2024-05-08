using System.Collections.Concurrent;
using CodeMetaData;
using FileStorageSystem.Exceptions;
using Exceptions;
using FileCodeMetaDataHandler;
using System.Collections;

namespace FileStorageSystem
{
    public sealed class StorageSystem : IFilesStorage, IEnumerable<KeyValuePair<FileId.FileId, SourceCodeMetaData>>
    {
        public ConcurrentDictionary<FileId.FileId, SourceCodeMetaData> FileStorageDictionary { get; } = new();
        private readonly SemaphoreSlim _semaphoreSlim = new SemaphoreSlim(1, 1);

        public int GetSize()
        {
            return FileStorageDictionary.Count;
        }
        
        public async Task<bool> TryAddNewFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);
            FileStorageExceptionChecker.IsEmptyFile(inputFileId);

            var metaData = await CreatorCodeMetaDataFromFile.MakeCodeMetaDataFromSourceFile(inputFileId.GetId());

            return await Task.Run(() => FileStorageDictionary.TryAdd(inputFileId, metaData));
        }
        
        public async Task<bool> TryRemoveFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);

            return await Task.Run(() => FileStorageDictionary.TryRemove(inputFileId, out _));
        }

        public async Task<FileInfo> GetFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);

            await _semaphoreSlim.WaitAsync();
            FileStorageExceptionChecker.IsNotExistInSystem(FileStorageDictionary, inputFileId);
            _semaphoreSlim.Release();

            return await Task.Run(() => new FileInfo(inputFileId.GetId()));
        }

        public async Task<SourceCodeMetaData> GetCodeMetaDataForFile(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);
            FileStorageExceptionChecker.IsEmptyFile(inputFileId);

            var metaData = await Task.Run(() => FileStorageDictionary.GetValueOrDefault(inputFileId));

            ExceptionsChecker.IsNull(metaData);

            return metaData;
        }

        public async Task<bool> ContainsKey(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);

            return await Task.Run(() => FileStorageDictionary.ContainsKey(inputFileId));
        }

        public async Task<string> GetFileText(FileId.FileId inputFileId)
        {
            ExceptionsChecker.IsNull(inputFileId);
            FileStorageExceptionChecker.IsEmptyFile(inputFileId);

            await _semaphoreSlim.WaitAsync();
            FileStorageExceptionChecker.IsNotExistInSystem(FileStorageDictionary, inputFileId);
            _semaphoreSlim.Release();

            var reader = new StreamReader(inputFileId.GetId());
                
            return await reader.ReadToEndAsync()!;
        }
        
        public async Task<bool> AddFileWithMetaData(FileId.FileId inputFileId, SourceCodeMetaData metaData)
        {
            ExceptionsChecker.IsNull(inputFileId);
            ExceptionsChecker.IsNull(metaData);

            return await Task.Run(() => FileStorageDictionary.TryAdd(inputFileId, metaData));
        }

        public IEnumerator<KeyValuePair<FileId.FileId, SourceCodeMetaData>> GetEnumerator()
        {
            return FileStorageDictionary.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}