Asset {#Asset}
================

## アセット参照方式
* ```Assets/Map/Root```というようにパス指定で参照する
* アセットを移動した場合はリダイレクタを作成する

```cpp
Optional<String> ReadAll(StringView path) {
    while (true) {
        File file(path);
        if (file) {
            u32 magic = file.readU32();
            if (magic==MAGIC("REDI")) {
                path = file.readString();
            } else {
                file.seek(0);
                return file.readAll();
            }    
        } else {
            break;
        }
    }
    return std::nullopt;
}
```
### なぜUUIDを使用しないか
* UUIDだと一度すべてのアセットを読み込んでMapを作らないと参照できない
* アセットは1つのリポジトリで管理されるためすでにファイルパス=ユニークIDである

### パスを使用する問題点
* UUIDと比べてメモリを消費する(16バイト→可変長)
* アセット名のスワップが面倒

## アセットのホットリロード
* エディタ上からのアセット編集は必ずメインスレッドから行う
* Ref<Texture> であれば、メンバを直接編集する
* setTexture()は呼ばない


## GameアセットとSystemアセット

## FilePath

## アーカイブ
* 絶対パス：ローカルファイルを読み込む
* 相対パス：アーカイブファイルがあればアーカイブから、無ければローカルから読み込む

DxLibと同様に特定の拡張子のアーカイブファイルをディレクトリとして扱う。以下のような構造になっていた場合Asset/Image/Sample.pngを指定するとImage.bin内部のSample.pngが読み込まれる。

* Asset
	* Image.bin
		* Sample.png
	* Image
		* Sample.png

## アセット識別子
| 方式   | 例                                    | 採用事例     |
|------|--------------------------------------|--------------|
| UUID | 86403732-bcac-af88-13ab-d4b8d8a2f91e | Unity　       |
| Path | Assets/Map/Root                      | UnrealEngine |

### UUID
* メリット
  * フォルダ移動にかかわらず固有のID
* デメリット
  * すべてのアセットを読み込むまでUUIDが有効かがわからない

### Path
* メリット
  * 人間も把握しやすい
* デメリット
  * フォルダ移動時にリダイレクタが必要
  * パス文字列がメモリ使用量を食う(Nameを使用する)