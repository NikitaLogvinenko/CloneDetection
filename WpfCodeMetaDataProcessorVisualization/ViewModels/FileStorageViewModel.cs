using FileStorageSystem;
using System.ComponentModel;
using System.IO;
using CodeMetaData;
using FileStorageSystem.FileId;
using System.Runtime.CompilerServices;
using System.Collections.Immutable;

public sealed class FileStorageSytemViewModel : INotifyPropertyChanged
{
    private StorageSystem _fileStorage;

    private FileStorageSytemViewModel(StorageSystem system)
    { 
        _fileStorage = system;

        OnPropertyChanged(nameof(GetSystem));
    }

    public static async Task<FileStorageSytemViewModel> CreateAsync(FileInfo fileInfo)
    {
        var result = new FileStorageSytemViewModel(await FileStorageCreator.FileStorageCreator.CreateFileStorageFromSourceFile(fileInfo));

        return result;
    }

    public ImmutableDictionary<FileId, SourceCodeMetaData> GetSystem => _fileStorage.FileStorageDictionary;

    public async void Add(string fileName)
    {
        var _ = await _fileStorage.TryAddNewFile(new FileId(new FileInfo(fileName)));

        OnPropertyChanged(nameof(GetSystem));
    }

    public SourceCodeMetaData GetMetaData(string fileName)
    {
       _fileStorage.FileStorageDictionary.TryGetValue(new FileId(new FileInfo(fileName)), out SourceCodeMetaData metaData);

        return metaData;
    }

    public List<FileId> GetFullCompareCanditates(SourceCodeMetaData metaData, float param)
    {
        if(metaData == null) 
        {
            Exceptions.ExceptionsChecker.IsNull(metaData);
        }

        var keys = _fileStorage.FileStorageDictionary.Keys.ToList();

        foreach(var item in _fileStorage)
        {
            if(!CodeMetaDataComparator.ComparerMetaData.CheckEquationOfMetaData(metaData, item.Value, param))
            {
                keys.Remove(item.Key);
            }
        }

        return keys;
    }

    public event PropertyChangedEventHandler PropertyChanged;

    protected void OnPropertyChanged([CallerMemberName] string property = "")
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
    }
}