using CodeMetaData;
using CodeMetaData.Operations;
using CodeMetaData.VariableClasses;
using CodeMetaDataComparator.Exceptions;
using Exceptions;
using System.Collections;

namespace CodeMetaDataComparator
{
    public static class ComparerMetaData
    {
        private static float _compareUsageParam = (float)0.5;

        private class ComparingUsage : IComparingType
        {
            private VariableUsage _variableUsage;

            public ComparingUsage(VariableUsage variableUsage) 
            {
                _variableUsage = variableUsage;
            }

            public object Clone()
            {
                return new ComparingUsage((VariableUsage)_variableUsage.Clone());
            }

            public IEnumerator GetEnumerator()
            {
                return _variableUsage.GetEnumerator();
            }

            public int GetSize()
            {
                return _variableUsage.GetSize();
            }

            public void Remove<Type>(Type obj)
            {
                if(obj is OperationCounter tempObj)
                {
                    _variableUsage.Remove(tempObj);
                }
            }
        }

        private class ComparingFunctionMetaData : IComparingType
        {
            private List<VariableUsage> _funcMetaData;

            public ComparingFunctionMetaData(List<VariableUsage> funcCodeMetaData)
            {
                _funcMetaData = funcCodeMetaData;
            }

            public object Clone()
            {
                return new ComparingFunctionMetaData(new List<VariableUsage>(_funcMetaData));
            }

            public IEnumerator GetEnumerator()
            {
                return _funcMetaData.GetEnumerator();
            }

            public int GetSize()
            {
                return _funcMetaData.Count;
            }

            public void Remove<Type>(Type obj)
            {
                if (obj is VariableUsage tempObj)
                {
                    _funcMetaData.Remove(tempObj);
                }
            }
        }

        private static float Compare<Type>(IComparingType first, IComparingType second, Func<Type, Type, float> comparer)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);

            if (first.GetSize() == 0 || second.GetSize() == 0)
            {
                return 0;
            }

            float equationParam = 0;
            var firstCounting = (IComparingType)first.Clone();
            var secondCounting = (IComparingType)second.Clone();

            foreach (Type operationCountingFirst in firstCounting)
            {
                float equationOperationCounterParamMax = 0;

                Type toDeleteSecond = default(Type);

                foreach (Type operationCountingSecond in secondCounting)
                {
                    var compareOperationResult = comparer(operationCountingFirst, operationCountingSecond);

                    if (compareOperationResult > equationOperationCounterParamMax)
                    {
                        toDeleteSecond = operationCountingSecond;
                        equationOperationCounterParamMax = compareOperationResult > _compareUsageParam ? compareOperationResult : 0;
                    }
                }

                if (toDeleteSecond != null)
                {
                    secondCounting.Remove(toDeleteSecond);
                }

                equationParam += equationOperationCounterParamMax;
            }

            return equationParam / Math.Max(first.GetSize(), second.GetSize());
        }

        public static float CompareOperationCounters(OperationCounter first, OperationCounter second)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);

            if (first.Operation.GetOperationId().GetId() != second.Operation.GetOperationId().GetId())
            {
                return 0;
            }

            if (first.UsageNum == second.UsageNum)
            {
                if (first.UsageNum == 0)
                {
                    return 0;
                }

                return 1;
            }

            return Convert.ToSingle(Math.Min(first.UsageNum, second.UsageNum)) / Math.Max(first.UsageNum, second.UsageNum);
        }

        public static float CompareUsage(VariableUsage first, VariableUsage second)
        {
            return Compare<OperationCounter>(new ComparingUsage(first), new ComparingUsage(second), CompareOperationCounters);
        }

        public static float CompareFunctionMetaData(CodeMetaData.FunctionCodeMetaData first, CodeMetaData.FunctionCodeMetaData second)
        {
            return Compare<VariableUsage>(new ComparingFunctionMetaData(first.Keys), new ComparingFunctionMetaData(second.Keys), CompareUsage);
        }

        public static float CompareFileMetaData(CodeMetaData.FileMetaData first, CodeMetaData.FileMetaData second)
        {
            ExceptionsChecker.IsNull(first);
            ExceptionsChecker.IsNull(second);

            if(first.Count == 0 || second.Count == 0)
            {
                return 0;
            }

            float equationReal = 0;

            foreach (var firstIt in first.MetaData)
            {
                float equationFuncCompareMax = 0;

                foreach (var secondIt in second.MetaData)
                {
                    float comparingFunctions = CompareFunctionMetaData(firstIt.MetaData, secondIt.MetaData);

                    if (comparingFunctions > equationFuncCompareMax)
                    {
                        equationFuncCompareMax = comparingFunctions;
                    }
                }

                equationReal += equationFuncCompareMax;
            }

            return equationReal / Math.Max(first.Count, second.Count);
        }

        public static bool CheckEquationOfFileMetaData(CodeMetaData.FileMetaData first, CodeMetaData.FileMetaData second, float equationParam)
        {
            ComparatorExceptionChecker.IsParametrInAvailableRange(equationParam);

            return CompareFileMetaData(first, second) >= equationParam;
        }
    }
}