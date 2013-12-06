打砖块 (8051)
==============

# 项目简介

用 AT89C52 做的一个打砖块游戏机，使用 Proteus 进行的仿真，部分电路如果要实体化还需改进。

使用 AMPIRE12864 作为显示器，三个按键分别控制平台的“左右移动”和“开始游戏”。为了保证游戏中音乐与画面的流畅，使用另外一块 AT89C52 来控制 sounder 进行声音的播放。

现阶段 V2 已完成。

# V2 改进

1. 更改了碰撞后消失砖块的大小。
2. 在两个单片机之间加了两条同步线来使画面和声音更加同步。
3. 可以手动调整挡板移动方式，共两种移动方式：
   
   - 按键按下移动，不按不移动 (Not smooth)
   - 按键指定方向，挡板自动移动 (Smooth)

# V1 简介

## 游戏规则

玩家控制屏幕底端的平台左右移动来保证小球在屏幕中碰撞。当小球碰撞到上屏的砖块时，被碰撞的砖块会消失，当所有砖块被消掉后，此关胜利，总共 3 关，3 关全部胜利以后，游戏重置；当小球碰触到屏幕下边缘时，失去一条命，总共 5 条命，当 5 条命全部失去以后，游戏结束，整个游戏重置。

游戏过程中，有四种道具，随机出现在下屏的随机位置。每个道具的图案是一个字母，当小球碰触道具图案时，道具生效。以下是道具介绍：

1. 变长道具（道具图案是 “L” 字样 ![变长道具](https://lh5.googleusercontent.com/-ay36V6-TnVw/UoeDXl8zxmI/AAAAAAAABWo/aq7HFFR_784/s31/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.20.08.png)）：道具生效后，平台变为铺满整个屏幕的底边，一段时间后失效，复原为原来长度。
2. 清除道具（道具图案是 “C” 字样 ![清除道具](https://lh6.googleusercontent.com/-_PYJavumack/UoeDX7XP9TI/AAAAAAAABWs/Ign1Zn3GPeA/s33/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.20.24.png)）：生效后，随机清除一部分的砖块。
3. 胜利道具（道具图案是 “W” 字样 ![胜利道具](https://lh6.googleusercontent.com/-DG5aOe5ti0s/UoeDYLY2ViI/AAAAAAAABWk/OsG_balBJP8/s35/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.21.08.png)）：生效后，此关胜利。
4. 死亡道具（道具图案是 “D” 字样 ![死亡道具](https://lh3.googleusercontent.com/-btLEA34gYD4/UoeDXMaDysI/AAAAAAAABWU/QtzDuDzqYYM/s29/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.19.42.png)）：生效后，直接失去一条命。

每一关，上半屏砖块组成的图案都不同（下面是三关的砖块图案），下方平台的移动速度也不同，速度会随着关卡数增大而变快，而且平台的长度也会变短，所以难度也会随之增大。小球的纵向移动速度不变，横向移动速度和平台一致，每次碰撞平台的时候，平台的移动方向和速度就会传递给小球。

![第一关砖块图案](https://lh5.googleusercontent.com/-OciVp2e6XnE/UoeDXIK9vcI/AAAAAAAABWM/gzRk9e89D7I/s455/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.19.20.png)

![第二关砖块图案](https://lh6.googleusercontent.com/-d3jCz1ka0pw/UoeDaTrBJPI/AAAAAAAABXQ/fnqbIvgXodg/s452/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.24.24.png)

![第三关砖块图案](https://lh6.googleusercontent.com/-BRp240xR57w/UoeDasw4kMI/AAAAAAAABXY/KX8Rw5KPsBQ/s453/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.26.44.png)


## 文件说明

+ circuit 文件夹：Proteus 仿真电路
+ material 文件夹：部分使用到的素材
+ program 文件夹：C 8051 的程序

  - sounder_main.c：控制 sounder 的单片机使用的程序
  - main.c：主单片机（控制显示器和按键的单片机）的程序
  - lcd12864.h 和 lcd12864.c：为了实现对 AMPIRE12864 的控制编写的函数，包括：
  
    1. ```lcd_write_command ( command )```：向 AMPIRE12864 中写入一个命令 command。
    2. ```lcd_write_data ( data )```：向 AMPIRE12864 中写入一个8位的数据 data。
    3. ```lcd_read_data ( )```：从 AMPIRE12864 中读取一个8位的数据。
    4. ```lcd_init ( )```：初始化 AMPIRE12864。
    5. ```first_page ( )```：清空整个屏幕。
    6. ```choose_screen ( n )```：选择屏幕。（同时选择两个屏幕：n=0；左屏幕：n=1；右屏幕：n=2）
    7. ```set_dot ( x,y )```：点亮坐标为 (x,y) 的像素点。（左上角的像素点为 (0,0)，x 坐标向右延伸，y 坐标向下延伸）
    8. ```clear_dot ( x,y )```：清除坐标为 (x,y) 的像素点。（坐标系同上）
    9. ```read_dot ( x,y )```：读取坐标为 (x,y) 的像素点的状态。
  - game.h 和 game.c：用于刷新屏幕和计算游戏状态的函数。

## 电路图

![电路图](https://lh6.googleusercontent.com/-ldlHxz4m384/UocrUK_Sy4I/AAAAAAAABV0/RSKXVdsHxJc/s720/circuit.jpg)

## 运行画面

![开始画面](https://lh5.googleusercontent.com/-Zfwu4WWGgPA/UoeDXAfFipI/AAAAAAAABWQ/r451XxrQqQs/s543/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.18.47.png)

![游戏进行中的画面](https://lh4.googleusercontent.com/-kP_-IcYWnk0/UoeDY4d1ljI/AAAAAAAABXI/VZ0FNyGnSY4/s535/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.21.47.png)

![死亡后显示剩余生命的画面](https://lh5.googleusercontent.com/-wRC1xSJ4d4E/UoeDZG8JYvI/AAAAAAAABXA/GJjNwrrN1Fk/s544/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.22.17.png)

![5 条命全部耗费完毕之后的画面](https://lh6.googleusercontent.com/-NJ48URX34xc/UoeDZUi1y6I/AAAAAAAABW8/C0oJXVNGqoc/s528/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.22.48.png)

![一关获胜之后的画面](https://lh4.googleusercontent.com/-IM0DGKy4E1Y/UoeDaDz8iLI/AAAAAAAABXc/XecvfvZClZY/s530/%25E5%25B1%258F%25E5%25B9%2595%25E5%25BF%25AB%25E7%2585%25A7%25202013-11-16%2520%25E4%25B8%258B%25E5%258D%258810.24.09.png)
