# BP Function For Fun

![BPFunctionForFunPath](/Pics/BPFunctionForFunPath.png)

## Set variables in anim blueprint with Cpp Sample

在 `ThirdPerson_AnimBP` 动画蓝图中，有一个变量叫 `JumpStartSequence` ，其是角色进行起跳时会播放的动作序列（在实际开发中，为了通用性往往会有大量的这种变量）。

在示例中使用 `SetAnimWithName` 方法实现了在Cpp端对其进行修改为Walk动画序列。

![SetBpVarByCpp](/Pics/SetBpVarByCpp.png)

