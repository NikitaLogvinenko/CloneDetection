using FileStorageSystem.Exceptions;
using FileStorageSystem.FileId;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestFilesServices
{
    [TestFixture]
    public class TestsFileId
    {
        private readonly FileInfo _fileInfo = new FileInfo("21345/Vinichenko.txt");
        private readonly FileInfo _fileInfo2 = new FileInfo("21345/Logvinenko.txt");
        private readonly FileInfo _fileInfo3 = new FileInfo("dfdf");

        [Test]
        public void TestInitNoException()
        {
            FileId id = new(_fileInfo);
            FileId id1 = new(_fileInfo2);
        }
        [Test]
        public void TestInitException()
        {
            Assert.Throws<FileStorageArgumentException>(() => new FileId(_fileInfo3));
        }

        [Test]
        public void TestEquals()
        {
            var id = new FileId(_fileInfo);
            var id1 = new FileId(_fileInfo);

            Assert.True(id.Equals(id1));

            ConcurrentDictionary<FileId, string> dict = new ConcurrentDictionary<FileId, string>();
            dict.TryAdd(id, "hell");
            Assert.True(dict.TryGetValue(id1, out _));
            Assert.True(dict.TryGetValue(id, out _));
        }

    }
}
