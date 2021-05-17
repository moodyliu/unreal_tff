# BlogForFun

> `BlogForFun` 则由许多小项目组成，主要是用于测试别人写的博客里面的功能，解决一些版本问题等（~~俗称抄袭作业~~）。

## YivanLee 巨佬博客篇

>  路径为 `Plugins/RayLine`

### Ray Line

* 出自：[虚幻 4 渲染编程（图元汇编篇）【第二卷：使用自己程序化生成的模型】](https://zhuanlan.zhihu.com/p/38462130)
* 说明：修复了教程上的版本问题以应用于 `4.26` 版本，此教程用于入门程序化生成模型。
* 操作说明：
  * 核心组件为 `RayBaseComponent` ，将其挂接到任何的Actor上即可。
  * 修改该组件 `Debug Sec` 参数可以调节各个方块间的距离。
  * 如果您的材质附上去仍为黑色，请尝试将您材质的Blend Mode调整为Translucent。

### VCube

* 出自：[虚幻 4 渲染编程（图元汇编篇）【第四卷：自定义模型资源管线】](https://zhuanlan.zhihu.com/p/39452117)
* 说明：修复了教程上的版本问题以应用于 `4.26` 版本，此教程用于自定义资源管线。
* 操作说明：
  * 核心组件为 `VCubeComponent` ，将其挂接到蓝图上即可。
  * 在蓝图编辑模式下可以设置组件的模型与材质。

### Verlet

![VerletComp](https://raw.githubusercontent.com/moodyliu/unreal_tff/main/Doc/Pics/VerletComp.png)

* 出自：[虚幻 4 渲染编程（图元汇编篇）【第五卷：游戏中的动力学模拟】](https://zhuanlan.zhihu.com/p/39703910)
* 说明：按照博客的说明完成组件，并做了一些可视化的功能。此教程用于入门布料模拟。
* 操作说明：
  * 核心组件为 `UVerletComponent` ，`AVerletDraw` 是挂接 UVerletComponent 组件的Actor类，在场景中放置此Actor类实例并允许即可看到布料模拟的效果。