OctbitEngine
============
OctbitEngineはマルチプラットフォームのゲームエンジンを目指して開発を進めています。

対応予定プラットフォーム
* PC
  * Windows
  * Linux
* Android

対応予定グラフィックAPI
* DirectX12
* Vulkan

Build Status
============
未対応




モジュール構成
==============
各モジュールはドキュメント作成のため別の名前空間に定義されますが、運用上はusing namespaceによりすべてob空間で使用できます。そのため各クラスは名前の衝突が起こらないように気を付ける必要があります。
ランタイムで切り替えをする場合は別モジュールで、コンパイル時に確定する場合は同一モジュールで実装します。

|モジュール名|namespace|機能|説明|
|:--|---|---|---|
|Project|任意|Component/Logic|ゲームロジック|
|Engine|ob::engine|各種モジュール起動||
|Sequence|ob::squence|タイムライン制御|アニメーションやサウンドの再生|
|Model|ob::model|StaticMesh/SkeltalMesh|3Dモデル描画|
|Animation|ob::animation|パラメータアニメ―ション|カーブに従った値変化|
|Physics|ob::physics|物理演算/衝突|剛体処理やクロスシミュレーション|
|GUI|ob::gui|Widget/Layout|ユーザインターフェイス|
|Input|ob::input|マウス/キーボード/ゲームパッド||
|Sound|ob::sound|Audio/Effector|音関係|
|VFX|ob::vfx|パーティクル||
|Rendering|ob::rendering|Material/RenderJob|Graphicをベースとした描画システム|
|Graphic|ob::graphic|Texture/GPUResource|描画APIのラッパー|
|Asset|ob::asset|アセット読み込み/アセット間依存||
|File|ob::file|暗号化ファイルIO/非同期ロード|アーカイブファイルの読み込みや非同期ロード|
|Core|ob::core|数学/モジュール管理/パーサ/JobManager|ユーティリティ群|
|Platform|ob::platform|ウィンドウ/ファイルIO|プラットフォーム機能の共通化|
|Foundation|ob|プリミティブ型/メモリ|システム|

Soundモジュール
-----------------
Soundモジュールではオーディオファイルの読み込みや再生をします。
* Sound
* SoundEffect
  
Sequenceモジュール
-----------------
Sequenceモジュールでは、タイムラインを使用してアニメーションやサウンドの再生、イベントの通知などの仕組みを提供します。

Animationモジュール
-----------------
Animationモジュールでは、スカラー値やBool値をアニメーション変化させる仕組みを提供します。
  
Modelモジュール
-----------------
Modelモジュールではスタティックメッシュやスケルタルメッシュを実装します。ゲーム制作で一般的に使われるFBXファイルにはアニメーション情報も含まれているため、ボーンアニメーションなどもこのモジュールで実装しています。

Physicsモジュール
-----------------
PhysicsモジュールではPhysXやBulletなどの物理演算ライブラリの共通化をします。
* Collider
* Softbody
* Cloth

Inputモジュール
-----------------
Inputモジュールでは、マウスやキーボード、ゲームパッドなどの入力デバイスを共通化して扱う仕組みを提供します。
* Mouse
* Keyboard
* GamePad
* MIDIController
* Pentablet
* Touch

Renderingモジュール
-----------------
RenderingモジュールではGraphicモジュールより上の層で共通化できる描画機能の実装を行います。
* Material
* HotReload
* ZSort
* Font

Renderingモジュール
-----------------
RenderingモジュールではGraphicモジュールより上の層で共通化できる描画機能の実装を行います。
* Material
* HotReload
* ZSort
* PostProcess

Graphicモジュール
-----------------
GraphicモジュールではDirectXやVulkanなどの異なる描画APIの共通化を行います。
プラットフォーム(OS)ごとに起動方法が異なるため、識別マクロを使用して個別の起動処理を実装する必要があります。
```c++
auto graphic=createGraphicAPI(ERHI::Vulkan);
#if defined(OS_WINDOWS)
auto window=platform->getMainWindow();
graphic->startup(window->getHWND());
#elif defined(OS_LINUX)
graphic->startup();
#endif
```
* CommandList
* GPUResource

Assetモジュール
--------------
Assetモジュールではゲームアセット単位でのファイル管理をします。アセット間の依存解決や、アンロード処理を行います。

Fileモジュール
--------------
Fileモジュールでは暗号化/アーカイブ化されたファイルの読み込みや非同期読み込み機能を提供します。Platform層でのファイル読み込みは単純なバイナリ読み込みのみをサポートしますが、Fileモジュールでは開発時の平文読み込みとリリース時の複合読み込みを隠ぺいします。

Coreモジュール
--------------
Coreモジュールでは全てのモジュールで使われるユーティリティを実装します。内部で標準ライブラリを使用することもできますが、プラットフォームごとに挙動が異なる可能性があるため非推奨です。
* アサート
* メモリ割り当て
* 数学ライブラリ
* 文字列
* JSON
* XML
* ModuleManager
* JobManager
* Image

Platformモジュール
--------------
Platformモジュールはウィンドウ表示やファイルIOなど、プラットフォーム毎の機能を共通化します。プラットフォーム毎に呼び出すモジュールを切り替える場合を除き、プラットフォーム毎にクラスを特殊化する必要がある場合はこのモジュールで実装します。これより上の層でプラットフォーム間の違いを意識する必要がないように設計してください。
* Window
* Dialog
* Network
* Time
* StringEncoder
* ModuleLoader

Foundationモジュール
--------------
Foundationモジュールではプリミティブ型やアサート、メモリアロケータなど、全てのモジュールで定義が必要な機能を実装します。重要な点は**必要**な機能であることで、よく使われる機能はCoreモジュールに実装してください。また、必要な機能であっても上位のモジュールで拡張して利用することを前提に設計し、機能の追加により他モジュールに影響しないようにしえください。
### メモリアロケートについて
メモリ割り当てはプロットフォームによって実装が異なるため、Foundationモジュールではmallocによる標準のアロケートの仕組みのみを実装します。
外部ライブラリ(Mimalloc)や専用のアルゴリズム(TLSF)によるメモリ割り当てを行う場合は、上位のモジュールで実装したものをプログラムのエントリ直後に設定してください。そのためプログラムのエントリ以前にアロケートが発生する場合(static変数のコンストラクタ)mallocによってアロケートされることに注意してください。

Open-Source Libraries
=====================
OctbitEngineは以下のオープンソースライブラリを使用しています。
* DirectXTex
* EASTL
* fmt
* gsl
* IUTest