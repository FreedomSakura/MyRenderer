// ************ ������ **************
#include <windows.h>

#include "geometry.h"

#pragma once


// **����֡��**
// ��ȡ��ǰCPUʱ�䣨CPU���д��� ���� CPUƵ�ʣ�
double get_cpu_time(void);

// ����������
//Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P);