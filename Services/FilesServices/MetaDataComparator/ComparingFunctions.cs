using System;
using MetaDataComparator.Exceptions;
using MetaDataCreator;

namespace MetaDataComparator
{
    public static class ComparerMetaData
    {
        public static float CompareOperationCounting(OperationCounter first, OperationCounter second)
        {
            ComparatorExceptionChecker.CheckForNull(first);
            ComparatorExceptionChecker.CheckForNull(second);

            if (first.Operation.GetOperationId().GetId() != second.Operation.GetOperationId().GetId())
            {
                return 0;
            }

            if (first.Operation.GetSecondOperand() == null && second.Operation.GetSecondOperand() != null ||
                first.Operation.GetSecondOperand() != null && second.Operation.GetSecondOperand() == null)
            {
                return 0;
            }
            
            return Convert.ToSingle(Math.Min(first.UsageNum, second.UsageNum)) / Math.Max(first.UsageNum, second.UsageNum);
        }
        
        public static float CompareUsages(VariableUsage first, VariableUsage second)
        {
            ComparatorExceptionChecker.CheckForNull(first);
            ComparatorExceptionChecker.CheckForNull(second);
            
            float equationParam = 0;
            
            if (first.GetSize() == 0 || second.GetSize() == 0)
            {
                return 0;
            }

            var firstCounting = (VariableUsage)first.Clone();
            var secondCounting = (VariableUsage)second.Clone();
            
            foreach (var operationCountingFirst in firstCounting)
            {
                float equationParamMax = 0;

                OperationCounter toDeleteSecond = null;
                
                foreach (var operationCountingSecond in secondCounting)
                {
                    var compareOperationResult = CompareOperationCounting(operationCountingFirst, operationCountingSecond);
                    if (compareOperationResult > equationParamMax)
                    {
                        toDeleteSecond = operationCountingSecond;
                        equationParamMax = compareOperationResult;
                    }
                }
                secondCounting.TryRemove(toDeleteSecond);
                
                equationParam += equationParamMax;
            }
            
            return equationParam / Math.Max(first.GetSize(), second.GetSize());
        }
    
        public static float CompareMetaData(MetaData first, MetaData second)
        {
            ComparatorExceptionChecker.CheckForNull(first);
            ComparatorExceptionChecker.CheckForNull(second);
            
            ComparatorExceptionChecker.CheckForZero(first.GetSize());
            ComparatorExceptionChecker.CheckForZero(second.GetSize());
            
            float equationReal = 0;
    
            foreach (var firstIt in first)
            {
                if (second.HasSuchUsages(firstIt.Key))
                {
                    equationReal++;
                }
                else
                {
                    float totalUsagesEquation = 0;
    
                    foreach (var secondIt in second)
                    {
                        float comparingUsages = CompareUsages(firstIt.Key, secondIt.Key);
                        const float equationUsagesParam = (float)0.5;
                        if (comparingUsages > totalUsagesEquation)
                        {
                            totalUsagesEquation = comparingUsages > equationUsagesParam ? comparingUsages : 0;
                        }
                        equationReal += totalUsagesEquation;
                    }
                }
            }
            
            return equationReal/first.GetSize();
        }
        
        public static bool CheckEquationOfMetaData(MetaData first, MetaData second, float equationParam)
        {
            ComparatorExceptionChecker.CheckForRange(equationParam);
            
            return CompareMetaData(first, second) >= equationParam;
        }
        
        public static bool CheckEquationOfFile(string firstFilePath, string secondFilePath, float equationParam)
        {
            ComparatorExceptionChecker.CheckForRange(equationParam);
    
            MetaData firstMetaData = FileHandler.CreateMetaDataFromSourceFile(firstFilePath);
            MetaData secondMetaData = FileHandler.CreateMetaDataFromSourceFile(secondFilePath);
            
            return CheckEquationOfMetaData(firstMetaData, secondMetaData, equationParam);
        }
    }
}