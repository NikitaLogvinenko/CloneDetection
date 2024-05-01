using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaDataComparator;

namespace TestFilesServices
{
    [TestFixture]
    public class TestCompareOperationCounting
    {
        [Test]
        public void Similar()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counter1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);

            Assert.That(Math.Abs(ComparerMetaData.CompareOperationCounters(counter, counter1) - 1), Is.LessThan(0.001));
        }

        [Test]
        public void ZeroUsage()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counter2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 0);

            Assert.That(ComparerMetaData.CompareOperationCounters(counter, counter2), Is.EqualTo(0));
        }

        [Test]
        public void NotZeroUsage()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counter2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);

            Assert.That(Math.Abs(ComparerMetaData.CompareOperationCounters(counter, counter2) - 0.6), Is.LessThan(0.001));
        }
        [Test]
        public void UnaryAndBinary()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counter2 = new OperationCounter(new BinaryOperation(new OperationId("+"), new Variable("a")), 0);

            Assert.That(ComparerMetaData.CompareOperationCounters(counter, counter2), Is.EqualTo(0));
        }
    }
}
