using System.IO;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using MetaDataCreator;

namespace MakeMetaData
{
    public static class MetaDataSerializer
    {
        public static void Serialize(Stream writer, MetaData metaData)
        {
            MetaDataExceptionChecker.CheckForNull(metaData);
            MetaDataExceptionChecker.CheckForNull(writer);

            IFormatter formatter = new BinaryFormatter();

            formatter.Serialize(writer, metaData);
        }

        public static MetaData Deserialize(Stream reader)
        {
            MetaDataExceptionChecker.CheckForNull(reader);

            IFormatter formatter = new BinaryFormatter();

            return (MetaData)formatter.Deserialize(reader);
        }
    }
}
