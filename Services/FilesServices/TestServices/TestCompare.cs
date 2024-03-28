using System;
using System.Collections.Generic;
using NUnit.Framework;
using MetaDataComparator;
using MetaDataCreator;

namespace TestComparingFunction
{
    [TestFixture]
    public class TestCompareOperationCounting
    {
        [Test]
        public void Similar()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting1 = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            
            Assert.True(Math.Abs(ComparerMetaData.CompareOperationCounting(counter, counting1) - 1) < 0.001);
        }

        [Test]
        public void ZeroUsage()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 0);
            
            Assert.True(ComparerMetaData.CompareOperationCounting(counter, counting2) == 0);
        }
        
        [Test]
        public void NotZeroUsage()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);
            
            Assert.True(Math.Abs(ComparerMetaData.CompareOperationCounting(counter, counting2) - 0.6) < 0.001);
        }
    }

    [TestFixture]
    public class TestCompareUsages
    {
        [Test]
        public void UsagesNullInput()
        {
            Assert.Throws<ComparatorInputArgumentException>(()=> ComparerMetaData.CompareUsages(null, null));
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
            VariableUsage variableUsage1 = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();
            
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

    [TestFixture]
    public class TestCompareMetaData
    {
        [Test]
        public void NullInput()
        {
            Assert.Throws<ComparatorInputArgumentException>(()=> ComparerMetaData.CompareMetaData(null, null));
        }
    
        [Test]
        public void NotZero()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);

            VariableUsage variableUsage = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();
            
            variableUsage.Add(counter);
            variableUsage2.Add(counting2);

            MetaData metaData = new MetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            MetaData metaData2 = new MetaData();
            metaData2.AddVariable(variableUsage2, new Variable("b"));
            
            Assert.True(Math.Abs(ComparerMetaData.CompareMetaData(metaData, metaData2) - 0.6) < 0.001);
        }
    
        [Test]
        public void FewElements()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);
            OperationCounter counter3 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);
            OperationCounter counting4 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);

            VariableUsage variableUsage = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();
            
            variableUsage.Add(counter);
            variableUsage2.Add(counting2);
            variableUsage.Add(counter3);
            variableUsage2.Add(counting4);

            MetaData metaData = new MetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            MetaData metaData2 = new MetaData();
            metaData2.AddVariable(variableUsage2, new Variable("b"));
            
            Assert.True(Math.Abs(ComparerMetaData.CompareMetaData(metaData, metaData2) - 0.8) < 0.001);
        }
    }

    [TestFixture]
    public class TestEquationMetaData
    {
        [Test]
        public void FewElements()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);
            OperationCounter counter3 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);
            OperationCounter counting4 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);

            VariableUsage variableUsage = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();
            
            variableUsage.Add(counter);
            variableUsage2.Add(counting2);
            variableUsage.Add(counter3);
            variableUsage2.Add(counting4);

            MetaData metaData = new MetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            MetaData metaData2 = new MetaData();
            metaData2.AddVariable(variableUsage2, new Variable("b"));
            
            Assert.True(ComparerMetaData.CheckEquationOfMetaData(metaData, metaData2, (float)0.8));
        }
        
        [Test]
        public void FullElements()
        {
            OperationCounter counter = new OperationCounter(new UnaryOperation(new OperationId("++")), 5);
            OperationCounter counting2 = new OperationCounter(new UnaryOperation(new OperationId("++")), 3);
            OperationCounter counter3 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);
            OperationCounter counting4 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);
            OperationCounter counter5 = new OperationCounter(new BinaryOperation(new OperationId("--"), new Variable("a")), 2);
            OperationCounter counting6 = new OperationCounter(new UnaryOperation(new OperationId("--")), 2);

            VariableUsage variableUsage = new VariableUsage();
            VariableUsage variableUsage2 = new VariableUsage();
            
            variableUsage.Add(counter);
            variableUsage2.Add(counting2);
            variableUsage.Add(counter3);
            variableUsage2.Add(counting4);
            variableUsage.Add(counter5);
            variableUsage2.Add(counting6);

            MetaData metaData = new MetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            MetaData metaData2 = new MetaData();
            metaData2.AddVariable(variableUsage2, new Variable("b"));
            
            Assert.True(ComparerMetaData.CheckEquationOfMetaData(metaData, metaData2, (float)0.4));
            Assert.True(!ComparerMetaData.CheckEquationOfMetaData(metaData, metaData2, (float)0.6));
        }
    }

}