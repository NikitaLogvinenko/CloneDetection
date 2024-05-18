using CodeMetaData;
using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaDataComparator.Exceptions;
using Exceptions;

namespace CodeMetaDataComparator
{
    public static class ComparerMetaData
    {
        private static float param = (float)0.5;
        public static float CompareOperationCounters(OperationCounter first, OperationCounter second)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);

            if (first.Operation.GetOperationId().GetId() != second.Operation.GetOperationId().GetId())
            {
                return 0;
            }

            if (first.Operation.GetType() != second.Operation.GetType())
            {
                return 0;
            }
            
            return Convert.ToSingle(Math.Min(first.UsageNum, second.UsageNum)) / Math.Max(first.UsageNum, second.UsageNum);
        }
        
        public static float CompareUsages(VariableUsage first, VariableUsage second)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);
            
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
                    var compareOperationResult = CompareOperationCounters(operationCountingFirst, operationCountingSecond);
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
    
        public static float CompareMetaData(CodeMetaData.FunctionCodeMetaData first, CodeMetaData.FunctionCodeMetaData second)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);
            
            ComparatorExceptionChecker.IsZeroSizeMetaData(first);
            ComparatorExceptionChecker.IsZeroSizeMetaData(second);
            
            float equationReal = 0;
    
            foreach (var firstIt in first)
            {
                if (second.HasUsages(firstIt.Key))
                {
                    equationReal++;
                }
                else
                {
                    float totalUsagesEquation = 0;
    
                    foreach (var secondIt in second)
                    {
                        float comparingUsages = CompareUsages(firstIt.Key, secondIt.Key);
                        if (comparingUsages > totalUsagesEquation)
                        {
                            totalUsagesEquation = comparingUsages > param ? comparingUsages : 0;
                        }
                        equationReal += totalUsagesEquation;
                    }
                }
            }
            
            return equationReal/ (first.GetSize() > second.GetSize() ? first.GetSize() : second.GetSize());
        }

        public static bool CheckEquationOfMetaData(CodeMetaData.FunctionCodeMetaData first, CodeMetaData.FunctionCodeMetaData second, float equationParam)
        {
            ComparatorExceptionChecker.IsParametrInAvailableRange(equationParam);
            
            return CompareMetaData(first, second) >= equationParam;
        }

        public static float CompareFileMetaData(CodeMetaData.FileMetaData first, CodeMetaData.FileMetaData second)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);

            float equationReal = 0;

            foreach (var firstIt in first.MetaData)
            {
                float totalUsagesEquation = 0;

                foreach (var secondIt in second.MetaData)
                {
                    float comparingUsages = CompareMetaData(firstIt.MetaData, secondIt.MetaData);
                    if (comparingUsages > totalUsagesEquation)
                    {
                        totalUsagesEquation = comparingUsages > param ? comparingUsages : 0;
                    }
                    equationReal += totalUsagesEquation;
                }
            }

            return equationReal / (first.Count > second.Count ? first.Count : second.Count);
        }

        public static bool CheckEquationOfFileMetaData(CodeMetaData.FileMetaData first, CodeMetaData.FileMetaData second, float equationParam)
        {
            ComparatorExceptionChecker.IsParametrInAvailableRange(equationParam);

            return CompareFileMetaData(first, second) >= equationParam;
        }
    }
}