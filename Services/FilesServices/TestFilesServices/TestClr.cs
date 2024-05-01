using CLR;

namespace TestFilesServices
{
    [TestFixture]
    public class TestClr
    {
        [Test]
        public void Similar()
        {
            AlreadyCSharp class1 = new AlreadyCSharp(10);

            Assert.True(class1.GetA() == 10);
            Assert.True(class1.GetName().ToString() == "test");
        }
    }
}
