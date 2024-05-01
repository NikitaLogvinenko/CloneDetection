using System;
using CodeMetaData;
using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaDataComparator;
using CodeMetaDataComparator.Exceptions;
using NUnit.Framework;


namespace TestFilesServices
{

    [TestFixture]
    public class TestCompareMetaData
    {
        [Test]
        public void NullInput()
        {
            Assert.Throws<Exceptions.Argument>(()=> ComparerMetaData.CompareMetaData(null, null));
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

            SourceCodeMetaData metaData = new SourceCodeMetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            SourceCodeMetaData metaData2 = new SourceCodeMetaData();
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

            SourceCodeMetaData metaData = new SourceCodeMetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            SourceCodeMetaData metaData2 = new SourceCodeMetaData();
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

            SourceCodeMetaData metaData = new SourceCodeMetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            SourceCodeMetaData metaData2 = new SourceCodeMetaData();
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

            SourceCodeMetaData metaData = new SourceCodeMetaData();
            metaData.AddVariable(variableUsage, new Variable("a"));
            SourceCodeMetaData metaData2 = new SourceCodeMetaData();
            metaData2.AddVariable(variableUsage2, new Variable("b"));
            
            Assert.True(ComparerMetaData.CheckEquationOfMetaData(metaData, metaData2, (float)0.4));
            Assert.True(!ComparerMetaData.CheckEquationOfMetaData(metaData, metaData2, (float)0.6));
        }
    }

}