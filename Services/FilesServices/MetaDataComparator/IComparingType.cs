using System.Collections;

namespace CodeMetaDataComparator
{
    public interface IComparingType : IEnumerable, ICloneable
    {
        int GetSize();
        void Remove<Type>(Type toDeleteSecond);
    }
}
