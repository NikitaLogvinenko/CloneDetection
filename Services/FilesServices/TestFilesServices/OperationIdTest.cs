using CodeMetaData.Operations;

namespace TestFilesServices
{
    [TestFixture]
    public class OperationIdTest
    {
        [Test]
        public void Create()
        {
            OperationId id = new OperationId("add");

            Assert.Throws<Exceptions.Argument>(() =>
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
}
