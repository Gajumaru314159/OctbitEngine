using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OctbitEngine.Asset
{
    public interface IAssetManager
    {
        /// <summary>
        /// ファイル名に使用できない文字のリスト
        /// </summary>
        public static char[] InvalidChars = Path.GetInvalidFileNameChars();

        /// <summary>
        /// 名前に使用できる文字列か
        /// </summary>
        public static bool IsValidName(string name)
        {
            return name.IndexOfAny(InvalidChars) == -1;
        }

        /// <summary>
        /// フォルダを作成します。
        /// </summary>
        /// <exception cref="ArgumentException">
        /// * パスがルートフォルダ名から始まらない
        /// * 使用できない文字が含まれている
        /// * 既に存在する
        /// </exception>
        public IAssetFolder CreateFolder(string path);

        /// <summary>
        /// ファイルを作成します。
        /// </summary>
        /// <param name="path">ファイルのフルパス</param>
        /// <param name="type">ファイルタイプ名</param>
        /// <example>
        /// <code>
        /// AssetManager.CreateFile("Assets/Textures/Texture.png", "Texture");
        /// </code>
        /// </example>
        /// <exception cref="ArgumentException">
        /// * パスがルートフォルダ名から始まらない
        /// * 使用できない文字が含まれている
        /// * 既に存在する
        /// * アセットタイプが登録されていない
        /// </exception>
        public IAssetFile CreateFile(string path, string type);

        /// <summary>
        /// パスからIAssetForlderを探す
        /// </summary>
        public IAssetFolder? FindFolder(string path);

        /// <summary>
        /// パスからIAssetFileを探す
        /// </summary>
        public IAssetFile? FindFile(string path);

        /// <summary>
        /// ルートフォルダ
        /// </summary>
        public IAssetFolder RootFolder { get; }

        public event EventHandler<IAsset>? AssetCreated;
    }
}
