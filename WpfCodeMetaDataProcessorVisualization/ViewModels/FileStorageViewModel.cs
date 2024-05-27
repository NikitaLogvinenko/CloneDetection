using FileStorageSystem;
using System.ComponentModel;
using System.IO;
using CodeMetaData;
using FileStorageSystem.FileId;
using System.Runtime.CompilerServices;
using System.Collections.Immutable;
using CodeMetaDataConverter;
using FileStorageSerializer;

public sealed class FileStorageSytemViewModel : INotifyPropertyChanged
{
    private StorageSystem _fileStorage;

    public ImmutableDictionary<FileId, FileMetaData> GetSystem => _fileStorage.FileStorageDictionary;

    private FileStorageSytemViewModel(StorageSystem system)
    { 
        _fileStorage = system;

        OnPropertyChanged(nameof(GetSystem));
    }

    public static async Task<FileStorageSytemViewModel> CreateAsync(FileInfo fileInfo)
    {
        using StreamReader reader = new(fileInfo.FullName);
        JsonFileStorageSerializer serializer = new JsonFileStorageSerializer();
        MetaDataConverter converter = new MetaDataConverter();

        var result = new FileStorageSytemViewModel(FileStorageSystemHandler.StorageSystemHandler<string>.ReadFromStream(reader, serializer, converter));

        reader.Close();

        return result;
    }

    //public async void Add(string fileName)
    //{
    //    var _ = await _fileStorage.TryAddNewFile(new FileId(new FileInfo(fileName)));

    //    OnPropertyChanged(nameof(GetSystem));
    //}

    public async Task<bool> AddWithMetaData(FileId fileName, FileMetaData metaData)
    {
        return await _fileStorage.TryAddFileWithMetaData(fileName, metaData);

        //OnPropertyChanged(nameof(GetSystem));
    }

    public async Task<bool> Remove(FileId fileid)
    {
        return await _fileStorage.TryRemoveFile(fileid);

        //OnPropertyChanged(nameof(GetSystem));
    }

    public async Task<FileMetaData> GetMetaData(FileId fileid)
    {
        return await _fileStorage.GetCodeMetaDataForFile(fileid);
    }

    public async Task<string> GetFileText(FileId fileId)
    {
        return await _fileStorage.GetFileText(fileId);
    }

    public List<FileId> GetFullCompareCanditates(FileMetaData metaData, float param)
    {
        if(metaData == null) 
        {
            Exceptions.ExceptionsChecker.IsNull(metaData);
        }

        var keys = _fileStorage.FileStorageDictionary.Keys.ToList();

        foreach(var item in _fileStorage)
        {
            if (metaData.Equals(item.Value) || !CodeMetaDataComparator.ComparerMetaData.CheckEquationOfFileMetaData(metaData, item.Value, param))
            {
                keys.Remove(item.Key);
            }
        }

        return keys;
    }

    public void Save(StreamWriter writer)
    {
        JsonFileStorageSerializer serializer = new JsonFileStorageSerializer();
        MetaDataConverter converter = new MetaDataConverter();
        
        FileStorageSystemHandler.StorageSystemHandler<string>.SaveToStream(writer, _fileStorage, serializer, converter);
    }

    public event PropertyChangedEventHandler PropertyChanged;

    protected void OnPropertyChanged([CallerMemberName] string property = "")
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
    }
}