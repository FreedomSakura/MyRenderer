#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "geometry.h"
#include "tgaimage.h"

#pragma once

using namespace std;

class Mesh
{
private:
	vector<Vec3f>verts_;
	vector<Vec2f>uvs_;
	vector<Vec3f>norms_;
	vector<int>faces_vert_;
	vector<int>faces_uv_;
	vector<int>faces_norm_;
	char* texPath;
	TGAImage diffuseMap_;


public:
	// 读取文件
	Mesh(const string filePath, const char* tex_path);
	int nums_verts();						// 计算该模型的顶点个数
	int nums_faces();						// 计算该模型的面数
	Vec3f get_vert_(int n);					// 获取第n的顶点位置
	Vec3f get_vert_(int iface, int n);		// 获取第iface个面的第n点
	Vec2f get_uv_(int n);					// 获取第n的顶点纹理坐标
	Vec2f get_uv_(int iface, int n);		// 获取第iface个面的第n顶点的纹理坐标
	Vec3f get_norm_(int n);					// 获取第n的顶点法线
	Vec3f get_norm_(int iface, int n);		// 获取第iface个面的第n顶点的法线
	int get_faces_vert_(int n);				// 获取第n个面的顶点索引

	// 读取纹理文件
	void load_diffuse_tex(const char* texPath, TGAImage& image);
	// 读取diffuse纹理
	Color diffuse(Vec2i tex_coord);
};




