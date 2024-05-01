using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestFilesServices
{
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

        [Test] public void Foreach() 
        {
            VariableUsage usage = new VariableUsage();
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 4);
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);
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

}
