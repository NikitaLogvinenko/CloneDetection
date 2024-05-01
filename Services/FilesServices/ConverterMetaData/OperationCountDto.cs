using Exceptions;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeMetaDataConverter
{
    public sealed class OperationCountDto
    {
        [JsonProperty]
        public string OperationId { get; set; }
        [JsonProperty]
        public string CountNum { get; set; }
        [JsonProperty]
        public string Operand { get; set; }

        public OperationCountDto(string operationId, string countNum, string operand)
        {
            ExceptionsChecker.IsNullOrEmptyString(operationId);
            ExceptionsChecker.IsNullOrEmptyString(countNum);
            ExceptionsChecker.IsNull(operand);

            OperationId = operationId;
            CountNum = countNum;
            Operand = operand;
        }
    }
}
