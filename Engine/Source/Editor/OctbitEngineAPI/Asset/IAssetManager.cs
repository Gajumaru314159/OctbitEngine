namespace OctbitEngine.Asset
{
    public interface IAssetManager
    {

        /// <summary>
        /// ファイル名に使用できない文字のリスト
        /// </summary>
        public static char[] InvalidChars = System.IO.Path.GetInvalidFileNameChars();

        /// <summary>
        /// 名前に使用できる文字列か
        /// </summary>
        public bool IsValidName(string name)
        {
            return name.IndexOfAny(InvalidChars) == -1;
        }

        #region FileSystem Operation

        /// <summary>
        /// ルートフォルダ
        /// </summary>
        public IAssetFolder RootFolder { get; }

        /// <summary>
        /// 全てのAssetFileにアクセスする
        /// </summary>
        public IEnumerable<IAssetFile> AllAssetFile { get; }

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
        ///     <list type="bullet">
        ///         <item>パスがルートフォルダ名から始まらない</item>
        ///         <item>使用できない文字が含まれている</item>
        ///         <item>既に存在する</item>
        ///         <item>アセットタイプが登録されていない</item>
        ///     </list>
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

        #endregion

        #region Import

        /// <summary>
        /// 指定したパスのファイルをアセットとしてインポートできるか
        /// </summary>
        public bool CanImport(string path);

        /// <summary>
        /// ファイルをインポートします。
        /// </summary>
        public bool Import(string path,IAssetFolder folder);

        #endregion

        #region Access



        #endregion

        /// <summary>
        /// アセットのパスからGUIDを取得します。
        /// </summary>
        //Guid AssetPathToGuid(string path);

        //string? GuidToAssetPath(Guid guid);
        //string? GuidAssetPath(object instance);

        //bool Contains(object instance);




        //Test
        public Type? FindEditorType(Type? type);

    }
}
