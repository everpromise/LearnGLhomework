#pragma once
#ifndef _STRUCTS_
#define _STRUCTS_

#include <iostream>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// ���㼯
float vertices[] = {
	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	0.5f, -0.5f, 0.0f,  // ���½�
	-0.5f, -0.5f, 0.0f, // ���½�
	-0.5f, 0.5f, 0.0f   // ���Ͻ�
};

// EBO ����������� - Element Buffer Object
unsigned int indices[] = { // ������0��ʼ
	0, 1, 3, // ��һ��������
	1, 2, 3 // �ڶ���������
};

#endif // !_STRUCTS_

