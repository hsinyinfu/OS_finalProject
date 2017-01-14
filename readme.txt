linux編譯指令:
g++ OS_main.cpp -O2 -std=c++11 -lGL -lGLU -lglut -pthread -RGBpixmap.h RGBpixmap.cpp

Mac編譯指令:
g++ OS_main.cpp -O2 -framework OpenGL -framework GLUT -framework Cocoa RGBpixmap.h RGBpixmap.cpp

bug/issue:
必須要持續移動游標畫面才會更新,不知道為什麼

#press button Q to quit program.
