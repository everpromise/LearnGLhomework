#pragma once
#ifndef _STRUCTS_
#define _STRUCTS_

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 顶点集
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // 右上角
	0.5f, -0.5f, 0.0f,  // 右下角
	-0.5f, -0.5f, 0.0f, // 左下角
	-0.5f, 0.5f, 0.0f   // 左上角
};

// EBO 索引缓冲对象 - Element Buffer Object
unsigned int indices[] = { // 索引从0开始
	0, 1, 3, // 第一个三角形
	1, 2, 3 // 第二个三角形
};

#endif // !_STRUCTS_

