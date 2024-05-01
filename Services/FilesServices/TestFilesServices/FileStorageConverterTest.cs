using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaData;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FileStorageSystem;
using FileStorageSystem.FileId;
using FileStorageConverter;
using CodeMetaDataConverter;

namespace TestFilesServices
{
    [TestFixture]
    public class FileStorageConverterTest
    {
        [Test]
        public void Convert()
        {
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

            var deconvert = StorageSystemConverter<CodeMetaDataDto>.DeconvertStorageSystem(dto, converter);

            Assert.True(system.GetSize() == deconvert.GetSize());
        }
    }
}
