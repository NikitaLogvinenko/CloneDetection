﻿using System;
using System.Collections.Generic;
using System.Collections.Concurrent;
using System.IO;
using System.Linq;
using System.Threading;
using MetaDataCreator;
using System.Threading.Tasks;

namespace FilesServices
{
    public sealed class StorageSystem : IFilesStorage
    {
        private readonly ConcurrentDictionary<FileId, MetaData> _fileSystem =
            new ConcurrentDictionary<FileId, MetaData>();

        public int GetSize()
        {
            return _fileSystem!.Count;
        }
        
        public Task<bool> AddNewFile(FileId inputFileId)
        {
            FileStorageExceptionChecker.CheckForNull(inputFileId);
            
            return Task.FromResult(_fileSystem.TryAdd(inputFileId, FileHandler.CreateMetaDataFromSourceFile(inputFileId.GetId())));
        }
        
        public Task RemoveFile(FileId inputFileId)
        {
            FileStorageExceptionChecker.CheckForNull(inputFileId);
            
            _fileSystem!.TryRemove(inputFileId, out _);
            return Task.CompletedTask;
        }

        public Task<FileInfo> GetFile(FileId inputFileId)
        {
            FileStorageExceptionChecker.CheckForNull(inputFileId);
            
            var result = _fileSystem.TryGetValue(inputFileId, out var metaData);
            
            if (!result)
            {
                throw new ArgumentException("No such file");
            }

            return Task.FromResult(new FileInfo(inputFileId.GetId()));
        }

        public Task<bool> GetMetaDataForFile(FileId inputFileId)
        {
            FileStorageExceptionChecker.CheckForNull(inputFileId);
            
            return Task.FromResult(_fileSystem!.TryGetValue(inputFileId, out var metaData));
        }

        public bool HasSuchKey(FileId inputFileId)
        {
            FileStorageExceptionChecker.CheckForNull(inputFileId);
            
            return _fileSystem.TryGetValue(inputFileId, out var metaData);
        }
        
        public async Task<string> GetFileText(FileId inputFileId)
        {
            FileStorageExceptionChecker.CheckForNull(inputFileId);
            
            if (!_fileSystem.TryGetValue(inputFileId, out var metaData))
            {
                throw new ArgumentException("No such file");
            }
                
            var reader = new StreamReader(inputFileId.GetId()!);
                
            return await reader.ReadToEndAsync()!;
        }
        
    }
}