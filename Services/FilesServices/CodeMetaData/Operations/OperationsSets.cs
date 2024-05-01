using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CodeMetaData.Operations
{
    internal static class OperationsSets
    {
        public static HashSet<string> UnaryOperationsSet = new HashSet<string>()
        {
            "++", "--", "~", "!", "-", "+", "-", "&", "*", "(type)", "sizeof"
        };
        public static HashSet<string> BinaryOperationsSet = new HashSet<string>()
        {
            "+", "-", "*", "/", "%", "<<", ">>", "<", ">", "<=", ">=", "==", "!=", "&", "^", "|", "&&", "||", "=", "*=", "/=", "+=", "-=", "%=", "<<=", ">>=", "&=", "|=", "^="
        };
    }
}
