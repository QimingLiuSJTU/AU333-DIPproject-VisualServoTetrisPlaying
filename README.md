# AU333-DIPproject-VisualServoTetrisPlaying
 利用摄像头观察iPad上的游戏界面，通过电脑决策控制机械臂点击屏幕，实现自动玩俄罗斯方块的功能。

## Description
  本项目是上海交通大学自动化系AU333数字图像处理课程的大作业。项目是通过摄像头捕捉iPad上的俄罗斯方块游戏界面，根据界面提取游戏信息并进行决策，最后控制机械臂点击屏幕，控制游戏不断进行。实际测试的最好成绩是消除214行。<br>
  
  本工程是依赖C++11标准编写的CyberDIP在Windows环境下的配套软件，详见[qtCyberDIP](https://github.com/LostXine/qtCyberDIP)。我们仅限于修改工程中的**usrGameController.h**和**usrGameController.cpp**两个文件，所以在code文件夹中仅列出这两个文件，实际使用时替换掉工程中的默认同名文件即可。我们使用了C++调用python的方法完成游戏决策，python文件也在code中列出，详细调用方法参见**Python.h**库。<br>
  
  程序逻辑和实现方法参见**report.pdf**。
  
  游戏效果展示请观看**DemoVideo.mp4**。

## Project participant
Qiming Liu, Sizhe Chen, Peidong Zhang in Automation Department, SEIEE, SJTU.
