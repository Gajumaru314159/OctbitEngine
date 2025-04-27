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


# サブページ
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