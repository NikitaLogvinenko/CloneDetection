using Exceptions;
using System.Collections.Immutable;

namespace CodeMetaData
{
    public sealed class FileMetaData : ICloneable
    {
        private List<FileMetaDataUnit> _fileMetaData = new();

        public FileMetaData()
        {

        }

        public FileMetaData(List<FileMetaDataUnit> fileMetaData)
        {
            ExceptionsChecker.IsNull(fileMetaData);

            _fileMetaData = fileMetaData;
        }

        public void Add(FileMetaDataUnit fileMetaData)
        {
            ExceptionsChecker.IsNull(fileMetaData);

            _fileMetaData.Add(fileMetaData);
        }

        public void Remove(FileMetaDataUnit fileMetaData)
        {
            ExceptionsChecker.IsNull(_fileMetaData);

            _fileMetaData.Remove(fileMetaData);
        }

        public void TryRemove(FileMetaDataUnit fileMetaData)
        {
            if(fileMetaData == null)
            {
                return;
            }

            _fileMetaData.Remove(fileMetaData);
        }

        public object Clone()
        {
            return new FileMetaData(new List<FileMetaDataUnit>(_fileMetaData));
        }

        public ImmutableList<FileMetaDataUnit> MetaData => _fileMetaData.ToImmutableList();
        public int Count => _fileMetaData.Count;
    }
}
