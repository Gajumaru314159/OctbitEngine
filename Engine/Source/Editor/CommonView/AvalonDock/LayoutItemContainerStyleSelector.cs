using AvalonDock.Layout;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace CommonView.AvalonDock
{
    /// <summary>
    /// LayoutItem のコンテナ (ウィンドウやタブ) のスタイルを選択するクラス。
    /// XAML 上で DataType プロパティに ViewModel の型を指定した DataTemplate を定義することができる。
    /// </summary>
    [ContentProperty("Items")]
    public class LayoutItemContainerStyleSelector : StyleSelector
    {
        /// <summary>
        /// Style のリスト。XAML でこのクラスの個要素として定義した Style はこのリストに追加される。
        /// </summary>
        public List<LayoutItemTypedStyle> Items { get; set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public LayoutItemContainerStyleSelector()
        {
            Items = new List<LayoutItemTypedStyle>();
        }

        /// <summary>
        /// スタイル選択時のコールバック
        /// </summary>
        /// <param name="item"></param>
        /// <param name="container"></param>
        /// <returns></returns>
        public override Style? SelectStyle(object? item, System.Windows.DependencyObject container)
        {
            // item には ViewModel が入っている。
            // ViewModel の型に対応するテンプレートを返す。
            var styleData = Items?.Find(s => s?.DataType != null && (item?.GetType().IsSubclassOf(s.DataType)??false));
            if (styleData != null) return styleData.Style;

            return base.SelectStyle(item, container);
        }
    }

    /// <summary>
    /// ViewModel の型と Style をペアにするためのデータクラス
    /// </summary>
    [ContentProperty("Style")]
    public class LayoutItemTypedStyle
    {
        /// <summary>
        /// ViewModel の型
        /// </summary>
        public Type? DataType
        {
            get;
            set;
        }

        /// <summary>
        /// 対応するスタイル
        /// </summary>
        public Style? Style
        {
            get;
            set;
        }
    }



    /// <summary>
    /// LayoutItem の内容となる DataTemplate を選択するクラス。
    /// XAML 上で DataType プロパティに ViewModel の型を指定した DataTemplate を定義することができる。
    /// </summary>
    [ContentProperty("Items")]
    public class LayoutItemTemplateSelector : DataTemplateSelector
    {
        /// <summary>
        /// テンプレートのリスト。XAML でこのクラスの個要素として定義した DataTemplate はこのリストに追加される。
        /// </summary>
        public List<DataTemplate> Items { get; set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public LayoutItemTemplateSelector()
        {
            Items = new List<DataTemplate>();
        }

        /// <summary>
        /// LayoutItem のコンテンツに適用する DataTemplate を選択する。
        /// </summary>
        public override DataTemplate? SelectTemplate(object? item, System.Windows.DependencyObject container)
        {
            // item には ViewModel が入っている。
            // ViewModel の型に対応するテンプレートを返す。
            var template = Items.Find(dt => item?.GetType().Equals(dt.DataType)??false);
            if (template != null) return template;

            return base.SelectTemplate(item, container);
        }
    }


    /// <summary>
    /// アプリを始めて起動したとき等、各ウィンドウの初期レイアウトを決めるためのクラス
    /// </summary>
    [ContentProperty("Items")]
    public class LayoutInitializer : ILayoutUpdateStrategy
    {
        /// <summary>
        /// LayoutInsertTarget のリスト。XAML でこのクラスの個要素として定義した LayoutInsertTarget はこのリストに追加される。
        /// </summary>
        public List<LayoutInsertTarget> Items { get; set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public LayoutInitializer()
        {
            Items = new List<LayoutInsertTarget>();
        }

        /// <summary>
        /// 
        /// </summary>
        public bool BeforeInsertAnchorable(LayoutRoot layout, LayoutAnchorable anchorableToShow, ILayoutContainer destinationContainer)
        {
            LayoutAnchorablePane? destPane = destinationContainer as LayoutAnchorablePane;
            if (destinationContainer != null &&
                destinationContainer.FindParent<LayoutFloatingWindow>() != null)
                return false;

            var viewModel = anchorableToShow.Content;
            if (viewModel == null) return false;

            var propInfo = viewModel.GetType().GetProperty("ContentId", BindingFlags.Public | BindingFlags.Instance);
            if (viewModel == null || propInfo == null) return false;

            var contentId = (string)propInfo.GetValue(viewModel)!;

            var target = Items.Find((t) => t.ContentId == contentId);
            if (target == null) return false;

            // 選択した名前の領域を取得し、そこにドッキングウィンドウを追加する
            var pane = layout.Descendents().OfType<LayoutAnchorablePane>().FirstOrDefault(d => d.Name == target.TargetLayoutName);
            if (pane != null)
            {
                pane.Children.Add(anchorableToShow);
                return true;
            }
            return false;
        }

        public void AfterInsertAnchorable(LayoutRoot layout, LayoutAnchorable anchorableShown)
        {
        }

        public bool BeforeInsertDocument(LayoutRoot layout, LayoutDocument anchorableToShow, ILayoutContainer destinationContainer)
        {
            return false;
        }

        public void AfterInsertDocument(LayoutRoot layout, LayoutDocument anchorableShown)
        {
        }
    }

    /// <summary>
    /// コンテンツID と インスタンス化された LayoutContent の配置先をペアにするためのデータクラス
    /// </summary>
    public class LayoutInsertTarget
    {
        /// <summary>
        /// コンテンツID
        /// </summary>
        public string ContentId
        {
            get;
            set;
        } = string.Empty;

        /// <summary>
        /// インスタンス化された LayoutContent の配置先の名前
        /// </summary>
        public string TargetLayoutName
        {
            get;
            set;
        } = string.Empty;
    }
}
