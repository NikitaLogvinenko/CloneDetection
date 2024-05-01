using System.IO;
using CodeMetaData;
using CodeMetaData.Exceptions;
using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using NUnit.Framework;

namespace TestFilesServices
{
    [TestFixture]
    public class TestMetaData
    {
        [Test]
        public void MetaDataTestInitZeroParam()
        {
            SourceCodeMetaData metaData = new SourceCodeMetaData();
        }

        [Test]
        public void MetaDataInitUsages()
        {
            SourceCodeMetaData metaData = new SourceCodeMetaData();
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            metaData.AddVariable(usage, new Variable("a"));
        }

        [Test]
        public void MetaDataAdd()
        {
            SourceCodeMetaData metaData = new SourceCodeMetaData();
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            metaData.AddVariable(usage, new Variable("a"));
            VariableUsage usage1 = new VariableUsage();
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage1.Add(counter1);
            metaData.AddVariable(usage1, new Variable("a"));

            Assert.True(metaData.GetSize() == 2);
        }

        [Test]
        public void MetaDataRemove()
        {
            SourceCodeMetaData metaData = new SourceCodeMetaData();

            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage.Add(counter);
            metaData.AddVariable(usage, new Variable("a"));

            VariableUsage usage1 = new VariableUsage();
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            usage1.Add(counter1);
            metaData.AddVariable(usage1, new Variable("u"));

            Assert.True(metaData.GetSize() == 2);

            metaData.RemoveVariable(usage);

            Assert.True(metaData.GetSize() == 1);

            Assert.Throws<Exceptions.Argument>(() => metaData.RemoveVariableByName(new Variable("gg")));

            metaData.RemoveVariableByName(new Variable("u"));

            Assert.True(metaData.GetSize() == 0);
        }
    }
}