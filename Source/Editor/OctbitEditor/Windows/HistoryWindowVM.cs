using CommonView.History;
using Livet;

namespace OctbitEditor
{
    public class HistoryWindowVM : ViewModel
    {
        public HistoryWindowVM()
        {
            History.Executed+=OnHistoryExecuted;
        }

        private void OnHistoryExecuted()
        {
            RaisePropertyChanged(nameof(UndoRecords));
            RaisePropertyChanged(nameof(HistoryInfo));
        }

        public IEnumerable<History.HistoryRecord> UndoRecords => History.UndoRecords.ToList();

        public string HistoryInfo => $"Undo:{History.UndoCount} Redo:{History.RedoCount}";

    }
}
