using MetaDataComparator;
using MetaDataCreator;
using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CLR;

namespace TestProject2
{
    [TestFixture]
    public class TestClr
    {
        [Test]
        public void Similar()
        {
            AlreadyCSharp class1 = new AlreadyCSharp(10);

            Console.WriteLine(class1.GetA());
            Console.WriteLine(class1.GetName());
        }
    }
}
