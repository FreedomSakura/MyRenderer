// ************ 工具类 **************
#include <windows.h>

#include "geometry.h"

#pragma once


// **计算帧数**
// 获取当前CPU时间（CPU运行次数 除以 CPU频率）
double get_cpu_time(void);

// 求重心坐标
//Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
Vec3f barycentric(Vec2f A, Vec2f B, Vec2f C, Vec2f P);