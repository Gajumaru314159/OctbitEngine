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
        public LogItem(LogObject log)
        {
            Message = log.Message;
            CallStack = log.StackTrace;
            LogLevel = log.Level;
        }

        public LogLevel LogLevel { get; init; }
        public string Message { get; init; }
        public string CallStack { get; init; }
    }

    public class LogListVM : TabBase
    {
        public LogListVM()
            : base("LogList")
        {

            LogItems.CollectionChanged += (sender, e) =>
            {
                InfoLogCount.Value = LogItems.Count(item => item.LogLevel == LogLevel.Info);
                WarningLogCount.Value = LogItems.Count(item => item.LogLevel == LogLevel.Warn);
                ErrorLogCount.Value = LogItems.Count(item => item.LogLevel == LogLevel.Error);
            };

            var filter = (LogItem item) =>
                item.Message.Contains(Filter.Value) &&
                item.LogLevel switch
                {
                    LogLevel.Info => IsInfoLogFiltered.Value,
                    LogLevel.Warn => IsWarningLogFiltered.Value,
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
        }
        private void OnLogged(LogObject log)
        {
            System.Windows.Application.Current.Dispatcher.Invoke(() =>
            {
                lock (LogItems)
                {
                    LogItems.Add(new(log));
                }
            });
        }

        // リスト
        public ReactiveCollection<LogItem> LogItems { get; } = new();
        public IFilteredReadOnlyObservableCollection<LogItem> FilteredItems { get; }

        // 選択要素
        public ReactivePropertySlim<LogItem> SelectedLogItem { get; } = new();
        public ReactivePropertySlim<string> SelectedLogMessage { get; } = new(string.Empty);
        
        // テキストによるフィルタ
        public ReactivePropertySlim<string> Filter { get; } = new(string.Empty);

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
