#include <windows.h>
#include <stdio.h>


#include "geometry.h"
#include "tool.h"
#include "mesh.h"
#include "public.h"
#include "tgaimage.h"

#pragma once 

// ************************************************************************************
// һЩshader�ṹ
// ************************************************************************************
typedef struct a2v {
	Vec4f vertex_native;
	Vec3f normal_native;
	Vec2f texcoord_native;
} a2v;

// vs --> fs �Ľṹ
typedef struct v2f {
	Vec3f vertex;
	Vec3f normal;
	Vec2f texcoord;
	Color color_tex;
	//Vec3f ndc_coord;
	float intensity;
} v2f;

// ************************************************************************************
// ��ɫ�ṹCOLORREF���
// ************************************************************************************

// װ����ɫ�ṹCOLORREF
void load_COLORREF(COLORREF& color, BYTE R, BYTE G, BYTE B, BYTE A);
// ��ɫ*float
COLORREF multiply_COLORREF(COLORREF color, float alpha);

// ************************************************************************************
// �任�������洢�������ڶ���任�ľ���
// ************************************************************************************
class Transformer {
private:
	Matrix model_mat;
	Matrix lookAt_mat;
	Matrix proj_mat;
	Matrix viewPort_mat;
	Matrix MVP_mat;
	int width, height;	// ��Ļӳ�����

public:
	int v_x, v_y;		// v_x��v_y���ӿ�ƫ����

public:
	Transformer(Matrix& model, Matrix& lookAt, Matrix& proj, Matrix& viewPort_mat);
	// ������о���
	Transformer(int t_world_width, int t_world_height, int t_world_depth, float x, float y, float z,
		Vec3f camera_pos, Vec3f center, Vec3f up, 
		float FOV, float aspect, int n, int f, 
		int width, int height, int depth, int v_x, int v_y);
	// �����Model����
	Transformer(Vec3f camera_pos, Vec3f center, Vec3f up,
		float FOV, float aspect, int n, int f,
		int width, int height, int depth, int v_x, int v_y);
	Matrix& update_MVP();		// ���MVP������
	Matrix& update_MVP_without_model();			// ������model����
	Matrix& get_viewPort();	// ������Ļӳ������Ҫ�ľ���

	// ģ�ͱ任
	Matrix ModelTrans(int world_width, int world_height, int world_depth,
		float x, float y, float z);
	// �۲�任
	Matrix LookAt(Vec3f camera_pos, Vec3f center, Vec3f up);
	// ͶӰ -> ��Ϊͬ��ԳƵ�������
	// ���� orthogonal
	Matrix orthoProjection(int w, int h, int n, int f);
	// ͸�� Perspective
	//��1��ֱ��ȷ��������ʽ
	Matrix perspProjection(int w, int h, int n, int f);
	//��2��FOVʽ
	Matrix perspProjection(float FOV, float aspect, int n, int f);
	// ��Ļӳ��
	Matrix ViewPort(int width, int height, int depth, int x, int y);

};

// ************************************************************************************
// ��Ⱦ��
// ************************************************************************************
class Renderer {
private:
	int width;
	int height;
	unsigned int* screen_fb;
	float* zbuffer;
	Mesh* mesh;
	int render_model;			// ��Ⱦģʽ��1:��ͨģʽ�������Σ���2:�߿�ģʽ��3:��ģʽ
	COLORREF background;		// ������ɫ
	Transformer transformer;	// �任�ṹ
	TGAImage* image;			// ����/TGAImage��
	
public:
	Renderer(int r_width, int r_height, unsigned int*& r_screen_fb, float* r_zbuffer, Mesh*& r_mesh,
		int r_render_model, COLORREF r_background, TGAImage* r_image, Transformer r_Transformer) :
		width(r_width), height(r_height), screen_fb(r_screen_fb), zbuffer(r_zbuffer), mesh(r_mesh),
		render_model(r_render_model), background(r_background), image(r_image), transformer(r_Transformer)
	{}
	//Renderer(int width, int height, unsigned int*& screen_fb, float* zbuffer, Mesh*& mesh,
	//	int render_model, COLORREF background) :
	//	width(), height(), screen_fb(), zbuffer(), mesh(),
	//	render_model(), background()
	//{}
	// ���̴�����ɫ��������������iface��0-2������n�ҵ����㣬�������������ݴ洢��a2v�ṹ��
	virtual a2v processShader(int iface, int n);	// ������������Դ��ģ�
	// ������ɫ��������a2v������󷵻�v2f
	virtual v2f vertexShader(a2v a);
	// �ߵ�����ɫ
	virtual v2f vertexShader_Gouruad(a2v a, Vec3f lightPos);
	// ��proecssShader��vertexShader�����һ��
	virtual v2f vertexShader_and_proecc(int iface, int n);
	// ƬԪ��ɫ���������ƬԪ������������Դ��ɫ�����д���bool����ֵ��ʾ�Ƿ�����ƬԪ
	virtual bool fragmentShader(v2f v, Vec3f bar, COLORREF& color, Vec3f lightPos);
	// �ߵ�����ɫ
	bool fragmentShader_Gouruad(COLORREF& c, float intensity);
	// ��Ⱦzbuffer��
	virtual bool fragmentShader(Color& c, float z);

	Color diffuse(Vec2i tex_coord);		// ��ȡdiffuse����
	int get_nums_faces();				// ��ȡ����Ⱦ��������ģ���������
	unsigned int* get_screen_fb();			// ��ȡscreen_fb
	float* get_zbuffer();				// ��ȡzbuffer
	Transformer& get_transformer();			// ��ȡ�任�豸
	int get_width();
	int get_height();

};


// ************************************************************************************
// ����ͼԪ����
// ************************************************************************************
// ����
void DrawLine(Vec2i v1, Vec2i v2, COLORREF color, unsigned int* screen_fb, const int width, const int height);
// ��ģʽ
void PointModel(HWND hwnd, Renderer renderer, COLORREF color, HDC screen_dc);
// �߿�ģʽ
void LineModel(Mesh* mesh, COLORREF color, unsigned int* screen_fb, const int width, const int height);

// ��������
// ɨ���߻���
void DrawTriangle_scan_my(Vec2i v1, Vec2i v2, Vec2i v3, COLORREF color, unsigned int* screen_fb, int width, int height);

// �������귨
void DrawTriangle_barycentric(Vec3f* ptr, COLORREF color, unsigned int* screen_fb, float* zbuffer, const int width, const int height);
void DrawTriangle_b_test(Vec4f* ptr, COLORREF color, unsigned int* screen_fb, float* zbuffer, const int width, const int height);

// ���Լ���shader���½ṹ
void DrawTriangle_barycentric_Shader(v2f* v_s, Vec3f lightPos, Renderer renderer);
// �𶥵㡪���ߵ�����ɫ
void DrawTriangle_barycentric_Shader_Gouruad(v2f* v_s, Renderer renderer);
// ������
void DrawTriangle_barycentric_Shader_pixel(v2f* v_s, Vec3f lightPos, Renderer renderer);

void DrawTriangle_barycentric_Shader_origin(v2f* v_s, Vec3f lightPos, Renderer renderer);
// �½ṹ�������β���
void DrawTriangle_barycentric_Shader_single(v2f* v_s, Vec3f lightPos, Renderer renderer, COLORREF color);

//  ����������䣬������ͼ
void DrawTriangle_barycentric_zbuffer(v2f* v_s, Renderer renderer);

// �����޳�
bool is_back_facing(Vec3f a, Vec3f b, Vec3f c);
// ��׶�ü�
int transform_check_cvv(Vec4f v);

// ************************************************************************************
// Shading
// ************************************************************************************
// ��ɫƵ��
// ��������Blinn-Phong��ɫģ��-��-Gouruad shading
// Blinn-Phong��ɫģ��
float Blinn_Phong_shading(Vec3f vertPos, Vec3f normal, Vec3f lightPos);


void DrawTriangle_barycentric_Shader_test(v2f* v_s, Vec3f lightPos, Renderer renderer);


