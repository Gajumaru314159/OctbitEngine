using System;
using System.Collections.Generic;
using System.DirectoryServices;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;

namespace CommonView.Controls.Inspector
{
    public class InspectableReflectionObject
    {
        public static IList<IInspectableProperty> Create(object obj)
        {
            var result = new List<IInspectableProperty>();

            var type = obj.GetType();
            var properties = type.GetProperties(BindingFlags.Instance | BindingFlags.Public);
            foreach (PropertyInfo p in properties)
            {
                result.Add(new InspectableReflectionProperty(obj, p));
            }

            return result;
        }
    }
}
