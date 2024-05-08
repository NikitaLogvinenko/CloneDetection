using FileStorageSystem;
using System.ComponentModel;
using FileStorageCreator;
using System.IO;
using System.Reflection;
using CodeMetaData;
using FileStorageSystem.FileId;
using System.Collections.Concurrent;

public class FileStorageSytemView : INotifyPropertyChanged
{
    public StorageSystem _fileStorage;

    private FileStorageSytemView()
    {}

    public static async Task<FileStorageSytemView> CreateAsync(FileInfo fileInfo)
    {
        var result = new FileStorageSytemView();
        result._fileStorage = await FileStorageCreator.FileStorageCreator.CreateFileStorageFromSourceFile(fileInfo);

        return result;
    }

    public ConcurrentDictionary<FileId, SourceCodeMetaData> getSystem
    {
        get { return _fileStorage.FileStorageDictionary; }
    }

    public StorageSystem Get()
    {
        return _fileStorage;
    }

    public event PropertyChangedEventHandler PropertyChanged;

    protected void OnPropertyChanged(string propertyName)
    {
        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
    }
}