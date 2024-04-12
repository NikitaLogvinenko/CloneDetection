using System;
using System.IO;
using MakeMetaData;
using NUnit.Framework;
using MetaDataCreator;

namespace TestMetaData
{
    [TestFixture]
    public class OperationIdTest
    {
        [Test]
        public void Create()
        {
            OperationId id = new OperationId("add");
            
            Assert.Throws<MetaDataArgumentException>(() =>
            {
                var operationId = new OperationId(null);
            });
        }
        
        [Test]
        public void Compare()
        {
            string name = "add";
            OperationId id = new OperationId(name);
            
            Assert.True(id.GetId() == name);
        }
        
        
    }

    [TestFixture]
    public class UnaryOperationTest
    {
        [Test]
        public void Create()
        {
            OperationId id = new OperationId("++");
            UnaryOperation operation = new UnaryOperation(id);
            
            Assert.Throws<MetaDataArgumentException>(() =>
            {
                UnaryOperation operation1 = new UnaryOperation(null);
            });
        }
        
        [Test]
        public void GetId()
        {
            OperationId id = new OperationId("++");
            UnaryOperation operation = new UnaryOperation(id);
            
            Assert.True(id.GetId() == operation.GetOperationId()?.GetId());
        }
        
        [Test]
        public void Operand()
        {
            OperationId id = new OperationId("++");
            UnaryOperation operation = new UnaryOperation(id);
            
            Assert.True(null == operation.GetSecondOperand());
        }
    }

    [TestFixture]
    public class BinaryOperationTest
    {
        [Test]
        public void Create()
        {
            OperationId id = new OperationId("++");
            Variable variable = new Variable("a");
            BinaryOperation operation = new BinaryOperation(id, variable);
            
            Assert.Throws<MetaDataArgumentException>(() =>
            {
                BinaryOperation operation1 = new BinaryOperation(null, null);
                BinaryOperation operation2 = new BinaryOperation(id, null);
                BinaryOperation operation3 = new BinaryOperation(null, variable);
            });
        }
        
        [Test]
        public void GetId()
        {
            OperationId id = new OperationId("++");
            Variable variable = new Variable("a");
            BinaryOperation operation = new  BinaryOperation(id, variable);
            
            Assert.True(id.GetId() == operation.GetOperationId()?.GetId());
        }
        
        [Test]
        public void Operand()
        {
            OperationId id = new OperationId("++");
            Variable variable = new Variable("a");
            BinaryOperation operation = new BinaryOperation(id, variable);
            
            Assert.True(variable.VariableId == operation.GetSecondOperand()?.VariableId);
        }
    }
    
    [TestFixture]
    public class VariableUsageTest
    {
        [Test]
        public void Create()
        {
            VariableUsage usage = new VariableUsage();
        }
        
        [Test]
        public void Add()
        {
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            
            usage.Add(counter);
            Assert.True(usage.GetSize() == 1);
        }
        
        [Test]
        public void Remove()
        {
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            
            usage.Add(counter);
            Assert.True(usage.GetSize() == 1);
            
            usage.Remove(counter);
            Assert.True(usage.GetSize() == 0);
        }
        
        [Test]
        public void TryRemoveAndAdd()
        {
            VariableUsage usage = new VariableUsage();
            
            usage.TryAdd(null);
            Assert.True(usage.GetSize() == 0);
            
            usage.TryRemove(null);
            Assert.True(usage.GetSize() == 0);
        }
    }

    [TestFixture]
    public class TestMetaData
    {
        [Test]
        public void MetaDataTestInitZeroParam()
        {
            MetaData metaData = new MetaData();
        }

        [Test]
        public void MetaDataInitUsages()
        {
            MetaData metaData = new MetaData();
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage, new Variable("a"));
        }

        [Test]
        public void MetaDataAdd()
        {
            MetaData metaData = new MetaData();
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage, new Variable("a"));
            VariableUsage usage1 = new VariableUsage();
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage1, new Variable("a"));

            Assert.True(metaData.GetSize() == 2);
        }

        [Test]
        public void MetaDataRemove()
        {
            MetaData metaData = new MetaData();

            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage, new Variable("a"));

            VariableUsage usage1 = new VariableUsage();
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage1, new Variable("u"));

            Assert.True(metaData.GetSize() == 2);

            metaData.RemoveVariable(usage);

            Assert.True(metaData.GetSize() == 1);

            Assert.Throws<MetaDataArgumentException>(() => metaData.RemoveVariableByName(new Variable("gg")));

            metaData.RemoveVariableByName(new Variable("u"));

            Assert.True(metaData.GetSize() == 0);
        }

        [Test]
        public void MetaDataSerializeDeserialize()
        {
            MetaData metaData = new MetaData();

            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage, new Variable("a"));

            VariableUsage usage1 = new VariableUsage();
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            metaData.AddVariable(usage1, new Variable("u"));

            const string filePath = "out.txt";

            Stream stream = new FileStream(filePath, FileMode.Create, FileAccess.Write);

            MetaDataSerializer.Serialize(stream, metaData);

            stream.Close();

            Stream streamRead = new FileStream(filePath, FileMode.Open, FileAccess.Read);

            MetaData newOne = MetaDataSerializer.Deserialize(streamRead);

            Assert.AreEqual(metaData.GetSize(), newOne.GetSize());
        }
    }
}