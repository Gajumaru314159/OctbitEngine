# Framework {#Framework}
Frameworkはゲームエンジンを構成する基本のモジュール群です。

* ✔ 作業完了
* ✏️ 実装中
* ⏳ 未着手

|モジュール名|namespace|機能|説明|実装状況|
|:--|---|---|---|---|
|Core|ob::core|数学/DI/パーサ/JobManager|ユーティリティ群|✔|
|Platform|ob::platform|ウィンドウ/ファイルIO|プラットフォーム機能の共通化|✔|
|Input|ob::input|マウス/キーボード/ゲームパッド|DirectInputやXInputの共通化|✔|
|RHI|ob::rhi|Texture/Buffer|描画APIの抽象化レイヤ|✔|
|Graphics|ob::graphics|Material/RenderJob|RHIをベースとした描画システム|✏️|
|Model|ob::model|StaticMesh/SkeltalMesh|3Dモデル描画|✏️|
|Sequence|ob::squence|タイムライン制御|アニメーションやサウンドの再生|⏳|
|Animation|ob::animation|パラメータアニメ―ション|カーブに従った値変化|⏳|
|Physics|ob::physics|物理演算/衝突|剛体処理やクロスシミュレーション|⏳|
|GUI|ob::gui|Widget/Layout|ユーザインターフェイス|⏳|
|Sound|ob::sound|Audio/Effector|音関係|⏳|
|VFX|ob::vfx|パーティクル|ShurikenやNiagaraのようなエフェクトシステム|⏳|
|Asset|ob::asset|Asset|アセット読み込み/アセット間依存|⏳|
|Engine|ob::engine|Engine|各種モジュール起動|⏳|
|Editor|ob::editor|OctbitEditorとの通信|エディタ機能はこのモジュールに集約させています|⏳|

Coreモジュール
--------------
Coreモジュールでは全てのモジュールで使用されるユーティリティを実装します。
* アサート
* メモリ割り当て
* 数学ライブラリ
* 文字列
* JSON
* XML
* ModuleManager
* JobManager
* Image

### メモリアロケートについて
メモリ割り当てはプロットフォームによって実装が異なるため、
Foundationモジュールではmallocによる標準のアロケートの仕組みのみを実装します。
外部ライブラリ(Mimalloc)や専用のアルゴリズム(TLSF)によるメモリ割り当てを行う場合は、
上位のモジュールで実装したものをプログラムのエントリ直後に設定してください。

Platformモジュール
--------------
* Window
* Dialog
* Network
* DLL

Inputモジュール
-----------------
Inputモジュールでは、マウスやキーボード、ゲームパッドなどの入力デバイスを共通化して扱う仕組みを提供します。
デバイスは上位モジュールからも追加できるようになっています。
入力は```Axis``` ```Button```に抽象化されています。
* Mouse
* Keyboard
* GamePad
* MIDIController
* Pentablet
* Touch

RHIモジュール
-----------------
Rendering Hardware Interfaceです。
RHIモジュールではDirectXやVulkanなどの異なる描画APIの共通化を行います。
```c++
Ref<Texture> texture = Texture::Load(path);
Ref<Shader> shader = Shader::Load(path);

CommandListDesc desc;
desc.name = TC("MainCommandList");
desc.type = CommandListType::Graphic;
Ref<CommandList> cmdList = CommandList::Create(desc);
```
* CommandList
* GPUResource

Graphicsモジュール
-----------------
GraphicsモジュールではRHIモジュールより上の層で共通化できる描画機能の実装を行います。
* Material
* HotReload
* ZSort
* Font
  
Modelモジュール
-----------------
Modelモジュールではスタティックメッシュやスケルタルメッシュを実装します。

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

Physicsモジュール
-----------------
PhysicsモジュールではPhysXやBulletなどの物理演算ライブラリの共通化をします。
* Collider
* Softbody
* Cloth

Assetモジュール
--------------
Assetモジュールではゲームアセット単位でのファイル管理をします。アセット間の依存解決や、アンロード処理を行います。

Engineモジュール
--------------
EntityやComponentの仕組みを実装します。ModelやPhysicsをComponentとして使用する場合はこのモジュールにComponentを実装します。

※ModelやPhysicsからはEntityやComponentへアクセスできません。

# サブページ
* \subpage CodingRule
* \subpage Core
* \subpage Platform
* \subpage Input
* \subpage RHI
* \subpage Graphics
* \subpage Model
* \subpage Engine
* \subpage Core
* \subpage Asset
* \subpage Debug
* \subpage Editor
* \subpage Runtiime