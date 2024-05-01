using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Exceptions
{
    public sealed class Argument: ArgumentException
    {
        public Argument(string message) : base(message)
        {

        }
    }
}
