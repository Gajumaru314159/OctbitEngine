# Scene

## 構成
```
World
	Level
		Entity(WorldObject)
			Component
		Level[]
```

## World
* 通常1つだけ存在します。エディタから実行される場合は複数生成されます。
* 異なるWorld同士で情報をやり取りすることはできません。
* Worldごとのシステムを持ちます
    * VFX
	* GI

## Level
* Entityを配置することのできるオブジェクトです。
* 階層的にサブレベルを配置することができます。
* Levelのタイムスケールは子レベルに継承されます。

## Entity
* Levelの構成要素です。
* Entity自体は機能を持たせずComponentを追加していきます。

## Compoent
