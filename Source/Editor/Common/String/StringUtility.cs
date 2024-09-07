using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Common.String
{
    public static class StringUtility
    {
        public static bool MatchExtentions(this string? path, params string[] extentions)
        {
            var ext = Path.GetExtension(path);
            return extentions.Any(e => e == ext);
        }
    }
}
