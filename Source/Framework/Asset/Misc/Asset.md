Asset
================

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

## アセットバイナリの構成
アセットのバイナリはUUID.binという形で保存する。同一フォルダに全てのアセットを保存するとファイルシステムが重くなってしまうので先頭2文字でフォルダを分ける。(Minectaftの.minecraft/assets/objects以下参考)
* 00
	* 000c82756fd54e40cb236199f2b479629d0aca2f.bin
	* 00aa12fe2aab46f5252a795ae944cae92caba77b.bin
* 01
	* 01c5ba2d1645698cc2f6e462982bd4513a3e0d93.bin
	* 01db5c538d2de459f02047208bebf00e6a085ef9.bin