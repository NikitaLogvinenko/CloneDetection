using System.Collections.Concurrent;
using System.Collections.Generic;
using FileStorageSystem;
using FileStorageSystem.Exceptions;
using FileStorageSystem.FileId;
using NUnit.Framework;

namespace TestFilesServices
{
    [TestFixture]
    public class TestsFileStorage
    {
        private readonly string _filePath = "System.txt";
        private readonly string _filePath2 = "System1.txt";
        private readonly FileInfo _fileInfo = new FileInfo("21345/Vinichenko.txt");
        private readonly FileInfo _fileInfo2 = new FileInfo("21345/Logvinenko.txt");
        private readonly FileInfo _fileInfo3 = new FileInfo("21345/Vileiko.txt");
        [Test]
        public void InitException()
        {
            Assert.ThrowsAsync<Exceptions.Argument>(() => FileStorageCreator.CreateFileStorageFromSourceFile(""));
            Assert.ThrowsAsync<Exceptions.Argument>(() => FileStorageCreator.CreateFileStorageFromSourceFile(null));
        }

        [Test]
        public void Init()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile(_filePath);
            system.Wait();
        }

        [Test]
        public void Contains()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile(_filePath);
            system.Wait();

            FileId id = new FileId(_fileInfo);
            FileId id2 = new FileId(_fileInfo2);
            FileId id3 = new FileId(_fileInfo);

            Assert.True(id.Equals(id3));

            Assert.True(system.Result.ContainsKey(id).Result);
            Assert.True(system.Result.ContainsKey(id2).Result);
        }

        [Test]
        public void Remove()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile(_filePath);
            system.Wait();

            var state = system.Result.TryRemoveFile(new FileId(_fileInfo));
            state.Wait();
            //Assert.True(state.Result);
            Console.WriteLine(system.Result.GetSize());
            Assert.True(system.Result.GetSize() == 3);

            state = system.Result.TryRemoveFile(new FileId(_fileInfo2));

            Assert.True(state.Result);
            Assert.True(system.Result.GetSize() == 2);
        }

        [Test]
        public void Add()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile(_filePath2);

            var state = system.Result.TryAddNewFile(new FileId(_fileInfo3));

            Assert.True(system.Result.GetSize() == 5);
        }

        [Test]
        public void Read()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile(_filePath2);

            Assert.True("1234" == system.Result.GetFileText(new FileId(_fileInfo)).Result);

            var state = system.Result.TryRemoveFile(new FileId(_fileInfo));

            Assert.ThrowsAsync<FileStorageArgumentException>(() => system.Result.GetFileText(new FileId(_fileInfo)));

            Assert.True(system.Result.GetSize() == 3);
        }

        [Test]
        public void GetMetaData()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile(_filePath2);

            Assert.True(system.Result.GetCodeMetaDataForFile(new FileId(_fileInfo)) != null);
        }
    }
}