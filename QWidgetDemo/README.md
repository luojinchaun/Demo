﻿# QSS Skin Builder
 
# 此项目的作者为“刘典武”，我只是从他手上购买了此项目的源码，除了写了几个文档外没有对此项目进行任何改动。我将此项目开源出来和大家一起观摩学习，但请大家不要骚扰原作者刘典武！原作者刘典武拥有此项目代码的所有权利！

## 开发计划
1. 所有其他窗体都是其布居中的widget。
2. 左上角图标、标题、标题居中、右上角最小化最大化关闭都可设置，包括设置样式+图标+图形字体（默认图形字体）。
3. 左上角图标及右上角三个按钮可视化控制。同时提供外部访问权限。
4. 无边框窗体可拉伸控制。
5. 提供换肤接口，内置8套样式选择，也可自定义样式路径。
6. 做成设计师插件，可以直接拖曳使用，所见即所得。
7. 后期增加内置信息框、颜色框等弹出窗体的支持。
8. 重新设计QSS样式，去掉单选框图片、滚动条图片，增加主菜单样式。

### 样式表格式
1. 第一行为特殊自定义部分，可以通过读取文本文件识别到特殊的颜色值。用于特殊处理。
2. 第二行为全局样式设置，例如无虚线，全局字体大小，文字颜色，禁用控件颜色。
3. 其他部分
4. 标签控件
5. 按钮控件

## 介绍：
1. 极简设计，傻瓜式操作步骤:，只需简单几步即可设计出漂亮的皮肤。
2. 所见即所得，想要什么好的皮肤，分分钟搞定。
3. 自动生成样式中所需要的对应颜色的图片资源文件，比如单选框、复选框指示器图片。
4. 集成自定义无边框标题栏样式、左边导航切换样式、顶部导航切换样式、设备面板样式。

## 样式表

### 银色风格
* 字体颜色：#000000
* 面板背景：#F5F5F5
* 边框颜色：#B2B6B9
* 普通渐变：#E1E4E6 #CCD3D9
* 加深渐变：#F2F3F4 #E7E9EB
* 高亮颜色：#00BB9E

### 蓝色风格
* 字体颜色：#324C6C
* 面板背景：#CFDDEE
* 边框颜色：#7F9AB8
* 普通渐变：#C0D3EB #BCCFE7
* 加深渐变：#D2E3F5 #CADDF3
* 高亮颜色：#00BB9E

### 淡蓝色风格
* 字体颜色：#386487
* 面板背景：#EAF7FF
* 边框颜色：#C0DCF2
* 普通渐变：#DEF0FE #C0DEF6
* 加深渐变：#F2F9FF #DAEFFF
* 高亮颜色：#00BB9E

### 深蓝色风格
* 字体颜色：#7AAFE3
* 面板背景：#0E1A32
* 边框颜色：#132743
* 普通渐变：#133050 #133050
* 加深渐变：#033967 #033967
* 高亮颜色：#00BB9E

### 灰色风格
* 字体颜色：#000000
* 面板背景：#F0F0F0
* 边框颜色：#A9A9A9
* 普通渐变：#E4E4E4 #A2A2A2
* 加深渐变：#DBDBDB #C1C1C1
* 高亮颜色：#00BB9E

### 浅灰色风格：
* 字体颜色：#6F6F6F
* 面板背景：#F0F0F0
* 边框颜色：#D4D0C8
* 普通渐变：#EEEEEE #E5E5E5
* 加深渐变：#FCFCFC #F7F7F7
* 高亮颜色：#00BB9E

### 深灰色风格
* 字体颜色：#5D5C6C
* 面板背景：#EBECF0
* 边框颜色：#A9ACB5
* 普通渐变：#D8D9DE #C8C8D0
* 加深渐变：#EFF0F4 #DDE0E7
* 高亮颜色：#00BB9E

### 黑色风格
* 字体颜色：#F0F0F0
* 面板背景：#464646
* 边框颜色：#353535
* 普通渐变：#4D4D4D #292929
* 加深渐变：#636363 #575757
* 高亮颜色：#00BB9E

### 浅黑色风格
* 字体颜色：#E7ECF0
* 面板背景：#616F76
* 边框颜色：#738393
* 普通渐变：#667481 #566373
* 加深渐变：#778899 #708090
* 高亮颜色：#00BB9E

### 深黑色风格
* 字体颜色：#D7E2E9
* 面板背景：#1F2026
* 边框颜色：#111214
* 普通渐变：#242629 #141518
* 加深渐变：#007DC4 #0074BF
* 高亮颜色：#00BB9E

### PS黑色风格
* 字体颜色：#DCDCDC
* 面板背景：#444444
* 边框颜色：#242424
* 普通渐变：#484848 #383838
* 加深渐变：#646464 #525252
* 高亮颜色：#00BB9E

### 黑色扁平
* 字体颜色：#BEC0C2
* 面板背景：#2E2F30
* 边框颜色：#67696B
* 普通渐变：#404244 #404244
* 加深渐变：#262829 #262829
* 高亮颜色：#00BB9E

### 白色扁平
* 字体颜色：#57595B
* 面板背景：#FFFFFF
* 边框颜色：#B6B6B6
* 普通渐变：#E4E4E4 #E4E4E4
* 加深渐变：#F6F6F6 #F6F6F6
* 高亮颜色：#00BB9E

## 截图预览
* ![image](Screenshot/1.png)
* ![image](Screenshot/2.png)
* ![image](Screenshot/3.png)
* ![image](Screenshot/4.png)
* ![image](Screenshot/5.png)
* ![image](Screenshot/6.png)
* ![image](Screenshot/7.png)
* ![image](Screenshot/8.png)