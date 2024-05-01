using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaData;
using CodeMetaDataConverter;
using CodeMetaDataSerializer;
using System.Net.WebSockets;
using System.Text.Json;
using Newtonsoft.Json;

namespace TestFilesServices
{
    [TestFixture]
    public class CodeMetaDataHandlerTest
    {
        private readonly string str = "not null"; 
        [Test]
        public void SerializerInit()
        {
            var Serializer = new JsonCodeMetaDataSerializer();
        }

        [Test]
        public void SerializeData()
        {
            var serializer = new JsonCodeMetaDataSerializer();

            SourceCodeMetaData metaData = new SourceCodeMetaData();
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            OperationCounter counter2 = new OperationCounter(new UnaryOperation(new OperationId("--")), 4);
            usage.Add(counter2);
            metaData.AddVariable(usage, new Variable("a"));


            var converter = new MetaDataConverter();
            var convertData = converter.ConvertMetaDataToDto(metaData);

            var result = serializer.Serialize(convertData);

            Console.WriteLine(result);

            var res2 = serializer.Deserialize(result);

            Assert.IsNotNull(result);

            var deserialize = serializer.Deserialize(result);

            Assert.IsNotNull(deserialize);

            var deconvert = converter.DeconvertDto(deserialize);

            Assert.IsNotNull(deconvert);

            Assert.True(deconvert.GetSize() == metaData.GetSize());
        }
    }
}
