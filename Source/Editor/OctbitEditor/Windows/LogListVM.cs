using Common.Log;
using CommonView.Menu;
using Livet;
using Reactive.Bindings;
using Reactive.Bindings.Helpers;
using System.Reactive.Linq;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using Brush = System.Windows.Media.Brush;

namespace OctbitEditor
{
    public class LogItem : ViewModel
    {
        private static Brush ErrorBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 209, G = 4, B = 4, A = 255 });
        private static Brush WarningBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 229, G = 175, B = 12, A = 255 });
        private static Brush InfoBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 233, G = 233, B = 233, A = 255 });
        private static Brush TraceBrush = new SolidColorBrush(new System.Windows.Media.Color { R = 200, G = 200, B = 200, A = 255 });
        public LogItem(LogObject log)
        {
            Message = log.Message;
            CallStack = log.StackTrace;
            LogLevel = log.Level;

            Brush =
                LogLevel switch
                {
                    LogLevel.Error => ErrorBrush,
                    LogLevel.Warning => WarningBrush,
                    _ => InfoBrush
                };
        }

        public LogLevel LogLevel { get; init; }
        public Brush Brush { get; init; }
        public string Message { get; init; }
        public string CallStack { get; init; }
    }

    public class LogListVM
    {
        public LogListVM()
        {

            LogItems.CollectionChanged += (sender, e) =>
            {
                InfoLogCount.Value = LogItems.Count(item => item.LogLevel == LogLevel.Info);
                WarningLogCount.Value = LogItems.Count(item => item.LogLevel == LogLevel.Warning);
                ErrorLogCount.Value = LogItems.Count(item => item.LogLevel == LogLevel.Error);
            };

            var filter = (LogItem item) =>
                item.Message.Contains(Filter.Value) &&
                item.LogLevel switch
                {
                    LogLevel.Info => IsInfoLogFiltered.Value,
                    LogLevel.Warning => IsWarningLogFiltered.Value,
                    LogLevel.Error => IsErrorLogFiltered.Value,
                    _ => false
                };
            FilteredItems = LogItems.ToFilteredReadOnlyObservableCollection(filter);
            Filter.Subscribe(_ => 
            FilteredItems.Refresh(filter)
            );

            Observable.Merge(IsInfoLogFiltered,IsWarningLogFiltered,IsErrorLogFiltered)
                .Subscribe(_ => FilteredItems.Refresh(filter));

            SelectedLogItem.Subscribe(item => SelectedLogMessage.Value = item?.Message??string.Empty);
            IsWarp.Subscribe(value => WarpVisibility.Value = value ? ScrollBarVisibility.Disabled : ScrollBarVisibility.Visible);


            ClearCommand = new DelegateCommand(() => { LogItems.Clear(); });

            Log.Logged += OnLogged;
            Log.Trace("CoreSystem initialized");
            Log.Info("CoreSystem initialized");
            Log.Warning("CoreSystem initialized");
            Log.Error("CoreSystem initialized");
        }
        private void OnLogged(LogObject log)
        {

            lock (LogItems)
            {
                LogItems.Add(new(log));
            }
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

        public ICommand ClearCommand { get; }

        // レイアウト
        public ReactivePropertySlim<bool> IsWarp { get; } = new(false);
        // TODO Converterに変更
        public ReactivePropertySlim<ScrollBarVisibility> WarpVisibility { get; } = new(ScrollBarVisibility.Disabled);
    }
}
