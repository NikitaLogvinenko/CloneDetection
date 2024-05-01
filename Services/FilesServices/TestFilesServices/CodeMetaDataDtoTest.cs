using CodeMetaDataSerializer;
using CodeMetaDataConverter;

namespace TestFilesServices
{
    [TestFixture]
    public class OperationCountDtoTest
    {
        private readonly string testString = "not null";
        private readonly string emptyString = "";
        [Test]
        public void InitNotNull()
        {
            OperationCountDto dto = new(testString, testString, testString);
            OperationCountDto dto1 = new(testString, testString, emptyString);
        }

        [Test]
        public void InitNull()
        {
            Assert.Throws<Exceptions.Argument>(() => new OperationCountDto(null, testString, testString));
            Assert.Throws<Exceptions.Argument>(() => new OperationCountDto(testString, null, testString));
            Assert.Throws<Exceptions.Argument>(() => new OperationCountDto(testString, testString, null));
            Assert.Throws<Exceptions.Argument>(() => new OperationCountDto(emptyString, testString, testString));
            Assert.Throws<Exceptions.Argument>(() => new OperationCountDto(testString, emptyString, testString));
        }
    }
}
