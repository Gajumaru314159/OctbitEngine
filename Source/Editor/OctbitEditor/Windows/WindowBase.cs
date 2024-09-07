using Livet;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEditor
{
    public class TabBase : ViewModel
    {
        public string ContentId { get; }
        public string Title { get; protected set; }

        protected TabBase(string contentId)
        {
            ContentId = contentId;
            Title = contentId;
        }
    }
}
