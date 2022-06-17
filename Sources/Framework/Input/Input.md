Input
=========

## ネイティブデバイス
トリガー/スカラー/ベクトルなどのイベントを受け取る
* Keyboard
* Mouse
* Joypad

# インプットデバイス
ネイティブデバイスの入力を各種イベントにバインドすることで複数の入力方法をサポートする。

# コード例
Input
```c++

class TriggerInput{
protected:
    Input(const IInputDevice*,u32 code);
}
class AxisInput{
protected:
    Input(const IInputDevice*,u32 code);
}
class KeyboardInput:public Input{
}

class Keyboard{
public:
    static const KeyboardInput F;
}
class GamePad{
public:
    GamePad(s32 userIndex)
    {
        F = GamePadInput(GamePadManager::Get(userIndex),Key::F);
    }
    const GamePadInput F;
}
```
マウス
```c++
Mouse::SetPos(pos);
if(Mouse::Left.down());
if(Mouse::IsConnected());
if(0.0f<Mouse::Wheel.value().x);
```
ゲームパッド
```c++
GamePad().vibrate(1.5f,100.0f);
auto wave=[](f32 t){return Math::Frac(t)*100.0f};
GamePad().vibrate(1.5f,wave);
GamePad(0).isConnected();

```
バインディング
```c++
Player::Player(){
    // Player破棄時にバインドを解除するためハンドルが必要
    // InputComponentを経由する場合Behaviorにハンドルはいらないはず
    Keyboard::Space.bindDown(m_handle,this,Player::onJump);
    Gamepad(0)::F.bindAxis(m_handle,this,Player::onMove);
}
void Player::onJump(){
    LOG_INFO("Jump!!");
}
void Player::onMove(Vec2 value){
    LOG_INFO("Move[{},{}]",value.x,value.y);
}

```
if文で取得
```c++
if(Keyboard::F.down());
if((Keyboard::Ctrl+Keyboard::F).down());
if(Mouse::Left.up());
if(GamePad().L2.up());
if(GamePad().LStick.isZero());
if(GamePad(1).L2.up());
if(InputMapper(1).Jump.pressed());
```