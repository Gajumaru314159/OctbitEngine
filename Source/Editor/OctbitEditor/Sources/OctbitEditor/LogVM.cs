using Livet;
using Reactive.Bindings;
using Reactive.Bindings.Helpers;
using System.Reactive.Linq;
using System.Windows.Controls;
using System.Windows.Media;
using Brush = System.Windows.Media.Brush;

namespace OctbitEditor
{
    public enum LogType
    {
        Error,
        Warning,
        Info
    }

    public class LogItem : ViewModel
    {
        private static Brush ErrorBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 209, G = 4, B = 4, A = 255 });
        private static Brush WarningBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 229, G = 175, B = 12, A = 255 });
        private static Brush InfoBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 233, G = 233, B = 233, A = 255 });
        public LogItem()
        {
            Message = "System.Windows.Data Error: 40 : BindingExpression path error: 'Value' property not found on 'object' ''SolidColorBrush' (HashCode=41314614)'. BindingExpression:Path=Brush.Value; DataItem='LogItem' (HashCode=33650236); target element is 'TextBlock' (Name=''); target property is 'Foreground' (type 'Brush')";
            CallStack = "New Log";
            
            LogType = Random.Shared.Next(0, 3) switch
            {
                0 => LogType.Error,
                1 => LogType.Warning,
                _ => LogType.Info
            };

            Brush =
                LogType switch
                {
                    LogType.Error => ErrorBrush,
                    LogType.Warning => WarningBrush,
                    _ => InfoBrush
                };
        }

        public LogType LogType { get; init; }
        public Brush Brush { get; init; }
        public string Message { get; init; }
        public string CallStack { get; init; }
    }

    public class LogVM
    {
        public LogVM()
        {

            LogItems.CollectionChanged += (sender, e) =>
            {
                InfoLogCount.Value = LogItems.Count(item => item.LogType == LogType.Info);
                WarningLogCount.Value = LogItems.Count(item => item.LogType == LogType.Warning);
                ErrorLogCount.Value = LogItems.Count(item => item.LogType == LogType.Error);
            };

            var filter = (LogItem item) =>
                item.Message.Contains(Filter.Value) &&
                item.LogType switch
                {
                    LogType.Info => IsInfoLogFiltered.Value,
                    LogType.Warning => IsWarningLogFiltered.Value,
                    LogType.Error => IsErrorLogFiltered.Value,
                    _ => false
                };
            FilteredItems = LogItems.ToFilteredReadOnlyObservableCollection(filter);
            Filter.Subscribe(_ => FilteredItems.Refresh(filter));

            Observable.Merge(IsInfoLogFiltered,IsWarningLogFiltered,IsErrorLogFiltered)
                .Subscribe(_ => FilteredItems.Refresh(filter));

            LogItems.Add(new LogItem());
            LogItems.Add(new LogItem());
            LogItems.Add(new LogItem());
            LogItems.Add(new LogItem());
            LogItems.Add(new LogItem());
            LogItems.Add(new LogItem());
            LogItems.Add(new LogItem());

            SelectedLogItem.Subscribe(item => SelectedLogMessage.Value = item?.Message??string.Empty);
            IsWarp.Subscribe(value => WarpVisibility.Value = value ? ScrollBarVisibility.Disabled : ScrollBarVisibility.Visible);
        }


        // リスト
        public ReactiveCollection<LogItem> LogItems { get; } = new();
        public IFilteredReadOnlyObservableCollection<LogItem> FilteredItems { get; }

        // 選択要素
        public ReactivePropertySlim<LogItem> SelectedLogItem { get; } = new();
        public ReactivePropertySlim<string> SelectedLogMessage { get; } = new(string.Empty);
        
        // テキストによるフィルタ
        public ReactiveProperty<string> Filter { get; } = new(string.Empty);

        // ログタイプによるフィルタ
        public ReactivePropertySlim<bool> IsInfoLogFiltered { get; } = new(true);
        public ReactivePropertySlim<bool> IsWarningLogFiltered { get; } = new(true);
        public ReactivePropertySlim<bool> IsErrorLogFiltered { get; } = new(true);
        // ログ数
        public ReactivePropertySlim<int> InfoLogCount { get; } = new(0);
        public ReactivePropertySlim<int> WarningLogCount { get; } = new(0);
        public ReactivePropertySlim<int> ErrorLogCount { get; } = new(0);

        // レイアウト
        public ReactivePropertySlim<bool> IsWarp { get; } = new(true);
        // TODO Converterに変更
        public ReactivePropertySlim<ScrollBarVisibility> WarpVisibility { get; } = new(ScrollBarVisibility.Disabled);
    }
}
