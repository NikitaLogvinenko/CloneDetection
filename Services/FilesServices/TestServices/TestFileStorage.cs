using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using NUnit.Framework;
using FilesServices;

namespace TestFileStorage
{
    [TestFixture]
    public class TestsFileId
    {
        [Test]
        public void TestInitException()
        {
            FileId id = new FileId("dark");
            FileId id1 = new FileId("dark");

            List<FileId> list = new List<FileId> {id};
            Assert.True(list.Contains(id1));
        }
    }

    [TestFixture]
    public class TestsFileStorage
    {
        [Test]
        public void InitException()
        {
            Assert.ThrowsAsync<ArgumentException>(() => FileStorageCreator.CreateFileStorageFromSourceFile(""));
            Assert.ThrowsAsync<ArgumentException>(() => FileStorageCreator.CreateFileStorageFromSourceFile("Add"));
        }
        
        [Test]
        public void Init()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile("System.txt");
            system.Wait();
        }

        [Test]
        public void Contains()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile("System.txt");
            system.Wait();
            
            Assert.True(system.Result.HasSuchKey(new FileId("21345/Vinichenko.txt")));
            Assert.True(system.Result.HasSuchKey(new FileId("21345/Logvinenko.txt")));
        }
        
        [Test]
        public void Remove()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile("System.txt");
            system.Wait();
            
            system.Result.RemoveFile(new FileId("21345/Vinichenko.txt"));
            Assert.True(system.Result.GetSize() == 3);
            
            system.Result.RemoveFile(new FileId("21345/Logvinenko.txt"));
            Assert.True(system.Result.GetSize() == 2);
        }
        
        [Test]
        public void Add()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile("System1.txt");
            
            system.Result.AddNewFile(new FileId("21345/Vileiko.txt"));
            system.Result.RemoveFile(new FileId("21345/Vileiko.txt"));
            
            Assert.True(system.Result.GetSize() == 4);
            
            system.Dispose();
        }
        
        [Test]
        public void Read()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile("System1.txt");
            system.Wait();
            
            Assert.True("1234" == system.Result.GetFileText(new FileId("21345/Vinichenko.txt")).Result);
            system.Result.RemoveFile(new FileId("21345/Vinichenko.txt"));
            Assert.ThrowsAsync<ArgumentException>( () =>  system.Result.GetFileText(new FileId("21345/Vinichenko.txt")));
            
            Assert.True(system.Result.GetSize() == 3);
        }
        
        [Test]
        [Parallelizable]
        public void GetMetaData()
        {
            var system = FileStorageCreator.CreateFileStorageFromSourceFile("System1.txt");
            system.Wait();

            Assert.True(system.Result.GetMetaDataForFile(new FileId("21345/Vinichenko.txt")) != null);
            Assert.True(!system.Result.GetMetaDataForFile(new FileId("2fgfgfg")).Result);
        }
    }
}