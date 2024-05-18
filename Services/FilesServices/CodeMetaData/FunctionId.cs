﻿using Exceptions;

namespace CodeMetaData
{
    public class FunctionId
    {
        private readonly string _id;

        public FunctionId(string functionId)
        {
            ExceptionsChecker.IsNullOrEmptyString(functionId);

            _id = functionId;
        }

        public string Id => _id;
    }
}
