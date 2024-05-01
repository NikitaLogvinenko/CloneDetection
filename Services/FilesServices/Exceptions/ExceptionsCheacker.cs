using Microsoft.VisualBasic;
using System;

namespace Exceptions
{
    public static class ExceptionsChecker
    {
        public static void IsNull<TType>(TType obj)
        {
            if (obj == null)
            {
                throw new Argument(nameof(obj) + " is null");
            }
        }

        public static void IsNullOrEmptyString(String str)
        {
            if (String.IsNullOrEmpty(str))
            {
                throw new Argument("Empty input string");
            }
        }

        public static void IsEmptyCollection(Collection collection)
        {
            if (collection.Count == 0)
            {
                throw new Argument("Empty collection");
            }
        }
    }
}
