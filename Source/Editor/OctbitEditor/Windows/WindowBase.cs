using Livet;

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
    public class DocumentBase : ViewModel
    {
        public string ContentId { get; }
        public string Title { get; protected set; }

        protected DocumentBase(string contentId)
        {
            ContentId = contentId;
            Title = contentId;
        }
    }
}
