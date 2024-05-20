using CLR;
using CodeMetaData;
using FileStorageConverter;
using FileStorageSerializer;
using FileStorageSystem.FileId;
using FileStorageSystemHandler;

namespace TestClrProject
{
    public class Tests
    {
        [SetUp]
        public void Setup()
        {
        }

        [Test]
        public void TestProccesFunctionsCodeMetaData()
        {
            FileInfo info = new("C:\\Users\\User\\Project\\CloneDetection\\TestClrProject\\bin\\Debug\\net8.0-windows7.0\\cmcd\\CloneDetection\\config.txt");

            string init_file = "out.txt";

            var fileMetaData = CMCDFacadeWrapper.ProccesFunctionsCodeMetaData(info);

            FileStorageSystem.StorageSystem system = new();

            _ = system.AddFileWithMetaData(new FileId(new FileInfo(init_file)), fileMetaData);

            StreamWriter writer = new StreamWriter("out.txt");

            var serializer = new JsonFileStorageSerializer();

            var converter = new CodeMetaDataConverter.MetaDataConverter();

            StorageSystemHandler<string>.SaveToStream(writer, system, serializer, converter);

            writer.Close();

            StreamReader reader = new StreamReader(init_file);

            var system_1 = StorageSystemHandler<string>.ReadFromStream(reader, serializer, converter);

            Assert.True(system.FileStorageDictionary.Count == system_1.FileStorageDictionary.Count);
            
            reader.Close();
        }
    }
}