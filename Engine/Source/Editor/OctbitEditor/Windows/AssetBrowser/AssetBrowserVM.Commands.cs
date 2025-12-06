using Common.Linq;
using Common.Tree;
using OctbitEngine.Config;
using System.Diagnostics.CodeAnalysis;
using System.IO;
using System.Reactive.Linq;
using System.Windows;
using System.Windows.Input;

namespace OctbitEditor
{
    public partial class AssetBrowserVM
    {
        public ICommand MoveUpCommand { get; }
        public ICommand MoveBackCommand { get; }
        public ICommand MoveForwardCommand { get; }

        public ICommand CreateFolderCommand { get; }
        public ICommand DeleteCommand { get; }
        public ICommand OpenCommand { get; }

        private void GenerateMenuItems()
        {

            {
                var group = MenuItems.AddGroup("_Create");
                group.AddCommand("_Folder", "Ctrl+Shift+N", CreateFolder);
                group.AddSeparator();

                // Import専用アセット以外を生成
                group.AddEmptyCommand("_Scene");
                group.AddEmptyCommand("_Material");

            }
            MenuItems.AddCommand("Show in _Explorer", ShowInExplorer);
            MenuItems.AddCommand("_Open", OpenAsset, CanOpenAsset);
            MenuItems.AddCommand("_Delete","Delete", DeleteAssets);
            MenuItems.AddEmptyCommand("_Rename", "F2");
            MenuItems.AddEmptyCommand("_Copy Path","Ctrl+C");
            MenuItems.AddSeparator();
            MenuItems.AddEmptyCommand("_Reimport","Ctrl+R");
            MenuItems.AddSeparator();
            MenuItems.AddEmptyCommand("_Show Dependencies","Shift+D");
        }

        private void MoveUp()
        {
            if (SelectedFolder.Value.Parent is AssetBrowserFolderItem parent)
            {
                SelectedFolder.Value = parent;
            }
        }
        private void MoveBack()
        {
            if(0 < m_undoHistory.Count)
            {
                m_executingHistory = true;
                m_redoHistory.Push(SelectedFolder.Value);
                SelectedFolder.Value = m_undoHistory.Pop();
            }
        }
        private void MoveForward()
        {
            if (0 < m_redoHistory.Count)
            {
                m_executingHistory = true;
                m_undoHistory.Push(SelectedFolder.Value);
                SelectedFolder.Value = m_redoHistory.Pop();
            }
        }


        private void ShowInExplorer()
        {
            AssetBrowserFolderItem folder = SelectedFolder.Value;
            string? filename = null;

            switch (SelectedItemsInList.FirstOrNull())
            {
                case AssetBrowserFolderItem folder_:
                    folder = folder_;
                    break;
                case AssetBrowserFileItem file:
                    folder = (AssetBrowserFolderItem)file.Parent!;
                    filename = file.Name;
                    break;
                case AssetBrowserAssetItem asset:
                    folder = (AssetBrowserFolderItem)asset.Parent!.Parent!;
                    break;
            }

            var actualPath = Path.GetFullPath(Path.Combine(WorkSpace.RootPath, folder.Folder.PhysicalPath));

            string args = actualPath;

            System.Diagnostics.Process.Start("explorer.exe", args);
        }

        private void CreateFolder()
        {
            var basePath = $"{SelectedFolder.Value.Folder.Path}/NewFolder";
            var createPath = basePath;
            int index = 1;

            if (AssetManager.FindFolder(createPath) != null)
            {
                while (true)
                {
                    createPath = string.Format("{0}_{1:000}", basePath, index++);
                    if (AssetManager.FindFolder(createPath)==null)
                    {
                        break;
                    }
                }
            }

            var newFolder = AssetManager.CreateFolder(createPath);
        }
        private void DeleteAssets()
        {
            var roots = SelectedItemsInList.SelectRoot(i => i.Parent);
            foreach (var item in roots)
            {
                switch (item)
                {
                    case AssetBrowserFolderItem folder:
                        folder.Folder.Delete();
                        break;
                    case AssetBrowserFileItem file:
                        file.File.Delete();
                        break;
                }
            }
        }

        private void OpenAsset()
        {
            var items = SelectedItemsInList.ToArray();

            if (items.Length==1)
            {
                if (items[0] is AssetBrowserFolderItem folder)
                {
                    SelectedFolder.Value = folder;
                }
                if (items[0] is AssetBrowserFileItem file)
                {
                    var editorType = AssetManager.FindEditorType(file.File.Asset?.GetType());
                    if (editorType != null)
                    {
                        var window = Activator.CreateInstance(editorType, [file.File]) as Window;
                        window?.Show();
                    }

                }
            }


        }


    }
}
