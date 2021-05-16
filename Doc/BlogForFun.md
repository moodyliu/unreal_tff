# BlogForFun

> `BlogForFun` 则由许多小项目组成，主要是用于测试别人写的博客里面的功能，解决一些版本问题等（~~俗称抄袭作业~~）。

## Ray Line

>  路径为 `Plugins/RayLine`

* 出自：[虚幻 4 渲染编程（图元汇编篇）【第二卷：使用自己程序化生成的模型】](https://zhuanlan.zhihu.com/p/38462130)
* 说明：修复了教程上的版本问题以应用于 `4.26` 版本，此教程用于入门程序化生成模型。
* 操作说明：
  * 核心组件为 `RayBaseComponent` ，将其挂接到任何的Actor上即可。
  * 修改该组件 `Debug Sec` 参数可以调节各个方块间的距离。
  * 如果您的材质附上去仍为黑色，请尝试将您材质的Blend Mode调整为Translucent。