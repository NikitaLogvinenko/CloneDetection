using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;

namespace TestFilesServices
{
    [TestFixture]
    public class BinaryOperationTest
    {
        [Test]
        public void Create()
        {
            OperationId id = new OperationId("++");
            Variable variable = new Variable("a");
            BinaryOperation operation = new BinaryOperation(id, variable);

            Assert.Throws<Exceptions.Argument>(() =>
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
            BinaryOperation operation = new BinaryOperation(id, variable);

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

}
