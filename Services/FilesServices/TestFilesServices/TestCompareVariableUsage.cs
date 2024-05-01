using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaDataComparator;

namespace TestFilesServices
{
    [TestFixture]
    public class TestCompareUsages
    {
        [Test]
        public void UsagesNullInput()
        {
            Assert.Throws<Exceptions.Argument>(() => ComparerMetaData.CompareUsages(null, null));
        }

        [Test]
        public void UsagesNotZero()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);

            VariableUsage variableUsage = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();

            variableUsage.Add(counter);
            variableUsage2.Add(counting2);

            Assert.True(Math.Abs(ComparerMetaData.CompareUsages(variableUsage, variableUsage2) - 0.6) < 0.001);
        }

        [Test]
        public void UsagesZero()
        {
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);
            VariableUsage variableUsage1 = new VariableUsage();
            VariableUsage variableUsage2 = [counting2];

            Assert.True(ComparerMetaData.CompareUsages(variableUsage1, variableUsage2) == 0);
        }

        [Test]
        public void FewElements()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);
            OperationCounter counting3 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);
            OperationCounter counting4 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);

            VariableUsage variableUsage = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();

            variableUsage.Add(counter);
            variableUsage2.Add(counting2);
            variableUsage.Add(counting3);
            variableUsage2.Add(counting4);

            Assert.True(Math.Abs(ComparerMetaData.CompareUsages(variableUsage, variableUsage2) - 0.8) < 0.001);
        }
    }
}
