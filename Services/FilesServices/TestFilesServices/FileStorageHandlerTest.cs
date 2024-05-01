using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaData;
using CodeMetaDataConverter;
using FileStorageConverter;
using FileStorageSerializer;
using FileStorageSystem.FileId;
using FileStorageSystem;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestFilesServices
{
    [TestFixture]
    public class FileStorageHandlerTest
    {
        [Test]
        public void SerializerInit()
        {
            JsonFileStorageSerializer<CodeMetaDataDto> serializer = new JsonFileStorageSerializer<CodeMetaDataDto>();
        }

        [Test]
        public void SerializeSystem()
        {
            JsonFileStorageSerializer<CodeMetaDataDto> serializer = new JsonFileStorageSerializer<CodeMetaDataDto>();

            SourceCodeMetaData metaData = new SourceCodeMetaData();

            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter1);
            metaData.AddVariable(usage, new Variable("a"));

            StorageSystem system = new StorageSystem();

            system.AddFileWithMetaData(new FileId(new FileInfo("TestFile.txt")), metaData);

            MetaDataConverter converter = new MetaDataConverter();
            var dto = StorageSystemConverter<CodeMetaDataDto>.ConvertStorageSystem(system, converter);

            var serialize = serializer.Serialize(dto);

            Console.WriteLine(serialize);

            var deserialize = serializer.Deserialize(serialize);

            var deconvert = StorageSystemConverter<CodeMetaDataDto>.DeconvertStorageSystem(deserialize, converter);

            Assert.True(system.GetSize() == deconvert.GetSize());
        }
    }
}
