using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEditor.Controls.AssetBrowser
{
    public partial class AssetBrowserVM
    {
        private void GenerateMenuItems()
        {
            using (var group = MenuItems.AddGroup("_Create"))
            {
                group.AddCommand("Folder", CreateFolder);
                group.AddSeparator();
                group.AddCommand("Scene", () => CreateAsset("Scene"));
                group.AddCommand("Material", () => CreateAsset("Material"));
            }
            MenuItems.AddCommand("Show in Explorer", ShowInExplorer);

            MenuItems.AddCommand("Open", OpenAsset);
            MenuItems.AddCommand("Delete", DeleteAssets);
            MenuItems.AddCommand("Rename", "F2", Rename);
            MenuItems.AddCommand("Copy Path",CopyPath);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Reimport",Reimport);
            MenuItems.AddSeparator();
            MenuItems.AddCommand("Show Dependencies",ShowDependencies);
        }
        private void CreateFolder()
        {
        }

        private void CreateAsset(string type)
        {

        }

        private void ShowInExplorer()
        {
            // AssetBrowserItem? item = SelectedFolder.Value;
            // if (item == null) return;
            // while (item is not AssetBrowserFolderItem) item = item?.Parent;
            // var actualPath = Path.GetFullPath(Path.Combine(WorkSpace.RootPath, ((AssetBrowserFolderItem)item).Folder.PhysicalPath));
            // System.Diagnostics.Process.Start("explorer.exe", actualPath);
        }
        private void OpenAsset()
        {
        }
        private void DeleteAssets()
        {
        }

        private void Rename()
        {
        }

        private void CopyPath()
        {
        }
        private void Reimport()
        {
        }

        private void ShowDependencies()
        {

        }
    }
}
