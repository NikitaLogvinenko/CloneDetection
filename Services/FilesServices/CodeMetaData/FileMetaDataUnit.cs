using Exceptions;
using System.Collections.Immutable;

namespace CodeMetaData
{
    public sealed class FileMetaDataUnit
    {
        private FunctionId _id;
        private FunctionCodeMetaData _metaData = new();

        public FileMetaDataUnit(FunctionId id, FunctionCodeMetaData funcMetaData)
        {
            ExceptionsChecker.IsNull(id);
            ExceptionsChecker.IsNull(funcMetaData);

            _id = id;
            _metaData = funcMetaData;
        }

        public FunctionId Id => _id;
        public FunctionCodeMetaData MetaData => _metaData;
    }
}
