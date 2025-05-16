# イントロダクション {#Introduction}

## ビルド
### CMake
以下のinstallが必要です。
* Windows SDK
* Ninja
* CMake

#### ビルドステップ
1. ```x64 Native Tools Command Prompt for VS 2022```を起動
2. カレントディレクトリをルートディレクトリ(EngineRootMarkがあるディレクトリ)にする
3. ```cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="/EHsc" -DCMAKE_CXX_FLAGS_DEBUG=" " -DCMAKE_CXX_FLAGS_RELEASE=" " -B Build/CommandLine .```を実行して生成
4. ```ninja -C Build/CommandLine```を実行してビルド

#### 説明
* 環境変数を設定するため```x64 Native Tools Command Prompt for VS 2022```でビルドする必要があります
* cmakeでビルドする場合一部の外部ライブラリの不具合を回避するため