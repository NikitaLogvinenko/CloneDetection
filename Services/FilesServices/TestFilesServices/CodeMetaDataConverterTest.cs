using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaData;
using CodeMetaDataConverter;
using CodeMetaDataHandler;
using CodeMetaDataSerializer;

namespace TestFilesServices
{
    [TestFixture]
    public class CodeMetaDataConverterTest
    {
        [Test]
        public void CreateHandlerParts()
        {
            var serializer = new JsonCodeMetaDataSerializer();
            var converter = new MetaDataConverter();
        }
        [Test]
        public void ConvertElement()
        {
            SourceCodeMetaData metaData = new SourceCodeMetaData();
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            metaData.AddVariable(usage, new Variable("a"));
                

            var converter = new MetaDataConverter();
            var convertData = converter.ConvertMetaDataToDto(metaData);

            Assert.IsNotNull(convertData);
        }
        [Test]
        public void ConvertDeconvertOneElement()
        {
            SourceCodeMetaData metaData = new();
            VariableUsage usage = [];
            OperationCounter counter = new(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);

            metaData.AddVariable(usage, new Variable("a"));

            var converter = new MetaDataConverter();
            var convertData = converter.ConvertMetaDataToDto(metaData);

            Assert.IsNotNull(convertData);

            var deconvert = converter.DeconvertDto(convertData);

            Assert.IsNotNull(deconvert);

            Assert.That(deconvert.GetSize(), Is.EqualTo(1));

            Assert.That(deconvert.GetSize(), Is.EqualTo(metaData.GetSize()));
        }
        [Test]
        public void ConvertAndDeconvertFewElements()
        {
            SourceCodeMetaData metaData = new();
            VariableUsage usage = [];
            OperationCounter counter = new(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            OperationCounter counter1 = new(new UnaryOperation(new OperationId("--")), 4);
            usage.Add(counter1);
            OperationCounter counter2 = new(new BinaryOperation(new OperationId("+"), new("alpha")), 4);
            usage.Add(counter2);

            metaData.AddVariable(usage, new Variable("betta"));

            VariableUsage usage2 = [];
            OperationCounter counter3 = new(new UnaryOperation(new OperationId("++")), 1);
            usage2.Add(counter3);
            OperationCounter counter4 = new(new UnaryOperation(new OperationId("--")), 2);
            usage2.Add(counter4);
            OperationCounter counter5 = new(new BinaryOperation(new OperationId("-"), new("gamma")), 1);
            usage2.Add(counter5);

            metaData.AddVariable(usage2, new Variable("etta"));

            var converter = new MetaDataConverter();
            var convertData = converter.ConvertMetaDataToDto(metaData);

            Assert.IsNotNull(convertData);

            var deconvert = converter.DeconvertDto(convertData);

            Assert.IsNotNull(deconvert);

            Assert.That(deconvert.GetSize(), Is.EqualTo(2));

            Assert.That(deconvert.GetSize(), Is.EqualTo(metaData.GetSize()));
        }
    }
}
