CommandRecorder
---------------


RenderContext
-------------
* drawRenderer

JobContextが設定されている場合は内部的にDispatch処理がされる。


```cpp

class RenderContext{
public:
	void drawScene();

	auto createCommandRecorder()->CommandRecorder*;
private:
	Array<CommandRecorder> m_recorder; 
};

```


* CommandRecorderはスレッド毎にアロケート
* スワップできる
* 