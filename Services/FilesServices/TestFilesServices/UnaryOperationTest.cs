using CodeMetaData.Operations;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestFilesServices
{
    [TestFixture]
    public class UnaryOperationTest
    {
        [Test]
        public void Create()
        {
            OperationId id = new OperationId("++");
            UnaryOperation operation = new UnaryOperation(id);

            Assert.Throws<Exceptions.Argument>(() =>
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
        }
    }

}
