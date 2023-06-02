#include <windows.h>
#include <stdio.h>
//#include <stdlib.h>
#include <iostream>
#include <thread>

#include "public.h"
#include "tool.h"
#include "geometry.h"
#include "mesh.h"
#include "graphic.h"
#include "tgaimage.h"


using namespace std;

// �ؼ�����
static HWND hwnd;			// ����id
static HDC screen_dc;		//                                                                              
// BITMAP
static HBITMAP screen_hb;	// �µ�bitmap
static HBITMAP screen_ob;	// �ϵ�bitmap
unsigned int* screen_fb;	// ������

// *******��Ϣ��Ӧ���*********
// ����������
int keys_buffer[512];
// ���Իص������е���Ϣϵͳ�ض���
short zDelta = 0;
// ��Ӧ��־
// ����Ӧ��һЩ��ı����������Ϣ����Ϣ��flag��Ϊtrue�����Ǹ���MVP����
bool flag = false;

// ****************************
// �ƹ�λ��
Vec3f light_pos(1.f, 1.f, 1.5f);
// LookAt�������
// ���λ��
Vec3f camera_pos(0.f, 0.f, 5.f);
// Camera����/��׼�ĵط�
Vec3f center(0, 0, 0);
// ���ڶ�λx���õ���ʱy��(y cross z = x)
// ������λ������x����������� �����۵�����
Vec3f up(0, -1, 0);		// ͼ��Ϊʲô���ǵ��������أ�������֪�����Ķ��ģ���ʱ�������


// ��ɫ
COLORREF white_color = 255 | (255) << 8 | (255) << 16 | (255) << 24;
COLORREF black_color = 0 | (0) << 8 | (0) << 16 | (255) << 24;
COLORREF back_color	 = 128 | (128) << 8 | (128) << 16 | (255) << 24;

// Z-buffer
float* zbuffer;

// ��ȡ��ģ���ļ�·��
//string objPath = "./obj/Charmander.obj";
//string objPath = "./obj/african_head.obj";
const string objPath = "./obj/diablo3_pose/diablo3_pose.obj";
//const string objPath = "./obj/SD_unitychan_humanoid.obj";
//const string objPath = "./obj/mary/Marry.obj";

// ����·��
//const char* texPath = "./obj/african_head_diffuse.tga";
const char* texPath = "./obj/diablo3_pose/diablo3_pose_diffuse.tga";
//const char* texPath = "./obj/utc_all2_light.tga";
//const char* texPath = "./obj/mary/Marry_diffuse.tga";

// ģ��
Mesh* mesh;
// TGAͼ��
TGAImage* image;
int byteDepth = 24;		// �������

// ��
void Draw(HWND hwnd);

// ���֡����
void clear_frameBuffer(unsigned int* frameBuffer, int width, int height, COLORREF color);
// �����Ȼ���
void clear_zbuffer(float* zbuffer, int width, int height);
// Shader�ṹ�Ż�&����
void Draw_2(HWND hwnd, Renderer renderer);
// Gouruad��ɫ
void Draw_2_Gouruad(HWND hwnd, Renderer renderer, Vec3f lightPos);	
// ��vs��ps���
void Draw_2_vs_and_ps(HWND hwnd, Renderer renderer);
// ���Կ��ٻ���Ⱦ
void Draw_3_mutil(HWND hwnd, Renderer renderer);
// ���Կ��ٴ洢...������һ��processShader�õ���a2v�洢��������������Ϳ���ʡȥprocessShader��
void Draw_4_fastStorage(HWND hwnd, Renderer renderer, vector<a2v>* a2v_s_);
// ��Ⱦ����������
void Draw_5_single(HWND hwnd, Renderer renderer);
// �����Ż�Shader�͹�դ������Ķ���
void Draw_6_test_Shader(HWND hwnd, Renderer renderer);

// ��ʼ�������ಢע��
void init_Register_wndclass(WNDCLASS& wndclass, HINSTANCE hInstance, TCHAR szAppName[]);
// ����bitmapinfo
void set_bitmap(BITMAPINFO& bi, HDC& screen_dc, HBITMAP& screen_hb, HBITMAP& screen_ob,
	unsigned int*& screen_fb, LPVOID ptr);

// �ص�
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, PSTR szCmdLine, int iCmdShow)
//��1������ID
//��2��0
//��3��ִ�г����������
//��4��ָ�����ڳ�ʼΪ��С��or���
{
	//************************************************************************************************
	// ׼������
	//************************************************************************************************
	//1�������ؼ�����
	static TCHAR szAppName[] = TEXT("bitmap!");			// ��������
	MSG msg;											// ��Ϣ�ṹ
	WNDCLASS wndclass;									// ������
	BITMAPINFO bi;										// �洢bitmap������Ϣ�Ľṹ

	// ���仺����
	LPVOID ptr = new LPVOID[width * height];

	// Console
	AllocConsole();								//��������̨
	FILE* stream;
	freopen_s(&stream, "CON", "r", stdin);		// �ض���������
	freopen_s(&stream, "CON", "w", stdout);		// �ض��������

	//2�����ô����������
	//3��ע�ᴰ����
	init_Register_wndclass(wndclass, hInstance, szAppName);
	
	//4����������ʵ��
	hwnd = CreateWindow(
		szAppName,				// ��������
		TEXT("bitmap_Test"),	// ��������
		WS_OVERLAPPEDWINDOW,	// ���ڷ��
		CW_USEDEFAULT,			// xƫ��
		CW_USEDEFAULT,			// yƫ��
		width + 15,				// ���
		height + 40,			// �߶�
		NULL,					// ������
		NULL,					// �˵�
		hInstance,				// ����ID
		NULL					// ��������
	);

	// *****����bitmap*****
	set_bitmap(bi, screen_dc, screen_hb, screen_ob, screen_fb, ptr);

	//5����ʾ����
	ShowWindow(hwnd, iCmdShow);
	//6�����´���
	UpdateWindow(hwnd);

	//************************************************************************************************
	//// **6������λͼ��������������ɫ**��fragment shaderӦ�����⣩

	// ***** new ******
	// ����ģ��������
	mesh = new Mesh(objPath, texPath);
	// ������Ȼ�����
	float* zbuffer = new float[width * height];
	for (int i = 0; i < width * height; i++) zbuffer[i] = -(std::numeric_limits<float>::max)();
	// TGAImage
	image = new TGAImage(width, height, byteDepth);

	//************************************************************************************************
	// ģ�ͱ任
	float FOV = 45.f;
	float aspect = 4.f / 3.f;
	int n = 10;
	int f = 1000;
	//// ������о���
	//Transformer transformer(world_width, world_height, world_depth, 0, 0, 0,
	//	camera_pos, center, up, 
	//	FOV, aspect, n, f,
	//	width, height, depth, v_x, v_y);
	// �����Model����
	Transformer transformer(camera_pos, center, up,
		FOV, aspect, n, f,
		width, height, depth, v_x, v_y);

	int render_model = 1;
	// Renderer
	Renderer renderer(width, height, screen_fb, zbuffer, mesh, render_model, black_color, image, transformer);

	// ��䱳����ɫ
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) 
			screen_fb[j * width + i] = back_color;
	}

	//// �����ģ��
	//PointModel(mesh, color, screen_fb, width, height);

	//// �����ģ��
	//LineModel(mesh, color, screen_fb, width, height);

	// *************************** ����ģ�� ****************************
	//vector<a2v> a2v_s_;

	//for (int i = 0; i < mesh->nums_faces(); i++) {
	//	a2v a_s[3];
	//	v2f v_s[3];
	//	for (int j = 0; j < 3; j++) {
	//		a_s[j] = renderer.processShader(i, j);
	//		a2v_s_.push_back(a_s[j]);
	//		v_s[j] = renderer.vertexShader(a_s[j]);
	//	}
	//}

	// *************************** ����ʱ�䣡***************************
	//// ����֡��
	//double test_time_1 = 0;
	//double test_time_2 = 0;

	//// ��ʼ��Ⱦ��ʱ��
	//test_time_1 = get_cpu_time();


	//test_time_2 = get_cpu_time();
	//
	////cout << "time_1: " << test_time_1 << endl;
	////cout << "time_2: " << test_time_2 << endl;
	//cout << "��֡��Ⱦ����ʱ��Ϊ��" << test_time_2 - test_time_1 << endl;

	//// **************************************************************
	//// ���ԣ�����������
	//Vec3f screen_coords[3];
	//screen_coords[0] = Vec3f(-0.1, -0.3, 0.3);
	//screen_coords[1] = Vec3f(0.4, 0.2, 0.2);
	//screen_coords[2] = Vec3f(0.7, 0.3, 0.1);
	//for (int k = 0; k < 3; k++) {
	//	screen_coords[k].x = (screen_coords[k].x + 1) * 0.5 * width;
	//	screen_coords[k].y = (screen_coords[k].y + 1) * 0.5 * (height);
	//	screen_coords[k].z = (screen_coords[k].z + 1) * 0.5;
	//	//screen_coords[k].z = screen_coords[k].z + 1;
	//	//cout << "screen_coords[" << k << "].z: " << screen_coords[k].z << endl;
	//}
	//DrawTriangle_barycentric_zbuffer(screen_coords, color, screen_fb, zbuffer, width, height);


	// **************************************************************
	//HDC hDC = GetDC(hwnd);
	//// **7������ɫ������ԴDCת����Ŀ��DC��Ҳ����CreateCompatibleDC()�������ڴ��豸�����ģ�**
	//BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	//ReleaseDC(hwnd, hDC);


	//************************************************************************************************
	// ��Ϣ/��Ⱦѭ��
	//************************************************************************************************

	// ����֡��
	int num_frames = 0;				// ֡��
	float print_time = 0;			// 

	// ��ʼ��Ⱦ��ʱ��
	print_time = get_cpu_time();

	unsigned int count = 0;

	//7����Ϣѭ��
	while (1) {
		// ��ȡ��Ⱦ���ʱ�䣨��һ�β��㣩
		float curr_time = 0;
		curr_time = get_cpu_time();

		// ��ջ�����
		clear_frameBuffer(screen_fb, width, height, back_color);
		clear_zbuffer(zbuffer, width, height);
		
		if (keys_buffer[0x41]) camera_pos.x -= 0.2;
		if (keys_buffer[0x44]) camera_pos.x += 0.2;
		if (keys_buffer[0x57]) camera_pos.y += 0.2;
		if (keys_buffer[0x53]) camera_pos.y -= 0.2;
		if (keys_buffer[0x46]) camera_pos.z = -camera_pos.z;

		if (zDelta > 0) camera_pos.z -= 0.2;
		if (zDelta < 0) camera_pos.z += 0.2;

		zDelta = 0;

		// ���¾���
		renderer.get_transformer().LookAt(camera_pos, center, up);
		//renderer.get_transformer().update_MVP();
		renderer.get_transformer().update_MVP_without_model();
		
		//PointModel(hwnd, renderer, white_color, screen_dc);
		//Draw_2(hwnd, renderer);
		//Draw_2_vs_and_ps(hwnd, renderer);
		Draw_2_Gouruad(hwnd, renderer, light_pos);
		//Draw_4_fastStorage(hwnd, renderer, &a2v_s_);
		//Draw_4_fastStorage(hwnd, renderer);

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			
			msg.lParam = (DWORD)&renderer;
			//��1��������Ϣ
			TranslateMessage(&msg);
			//��2��������Ϣ
			DispatchMessage(&msg);
		}
		else {
			// û��ӦҲҪ����
			//clear_frameBuffer(screen_fb, width, height);
			//Draw_2(hwnd, renderer);
			//Draw(hwnd);
		}

		// ����֡��
		num_frames += 1;
		if (curr_time - print_time >= 1) {
			int sum_millis = (int)((curr_time - print_time) * 1000);
			float avg_millis = (float)sum_millis / num_frames;

			printf("fps: %3d , avg: %f ms\n", num_frames, avg_millis);

			num_frames = 0;
			print_time = curr_time;

		}
	}

	//************************************************************************************************
	// �ͷ��ڴ�
	//************************************************************************************************

	// �ͷſ���̨
	FreeConsole();

	// �ͷ��ڴ�
	delete[](ptr);
	ptr = NULL;

	delete(mesh);
	mesh = NULL;

	delete[](zbuffer);
	zbuffer = NULL;

	delete image;
	image = NULL;


	//************************************************************************************************

	return msg.wParam;
}
 

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	//1�������ؼ�����
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	//2��������Ϣ
	switch (message) {
	case WM_SIZE:
	{
		//light_pos.z += 1.f;
		//cout << "**** lightPos.z: " << light_pos.z << endl;
		Draw(hwnd);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		light_pos.z += 1.f;
		//flag = true;

		break;
	}

	case WM_RBUTTONDOWN:
	{
		light_pos.z -= 1.f;
		//flag = true;

		break;
	}

	case WM_MOUSEWHEEL:
	{
		//flag = true;
		zDelta = wParam >> 16;	
		break;
	}

	case WM_KEYDOWN:
	{
		//flag = true;
		keys_buffer[wParam & 511] = 1;
		break;
	}
	case WM_KEYUP:
	{
		keys_buffer[wParam & 511] = 0;
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

// ��
void Draw(HWND hwnd)
{
	HDC hDC = GetDC(hwnd);
	// **7������ɫ������ԴDCת����Ŀ��DC��Ҳ����CreateCompatibleDC()�������ڴ��豸�����ģ�**
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hDC);
}

void clear_frameBuffer(unsigned int* frameBuffer, int width, int height, COLORREF color) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			frameBuffer[j * width + i] = color;
		}
	}
}
// �����Ȼ���
void clear_zbuffer(float* zbuffer, int width, int height) {
	int zero = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			zbuffer[j * width + i] = zero;
		}
	}
}

void Draw_2(HWND hwnd, Renderer renderer) {
	for (int i = 0; i < renderer.get_nums_faces(); i++) {
		v2f v_s[3];
		for (int j = 0; j < 3; j++) {
			v_s[j] = renderer.vertexShader(renderer.processShader(i, j));
		}

		// �����µ�Shader�ṹ
		DrawTriangle_barycentric_Shader(v_s, light_pos, renderer);
	}

	HDC hDC = GetDC(hwnd);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hDC);
}
void Draw_2_Gouruad(HWND hwnd, Renderer renderer, Vec3f lightPos) {
	//std::shared_ptr<a2v> a_s = std::make_shared<a2v>(new a2v[3], std::default_delete<a2v[]>());
	//std::shared_ptr<v2f> v_s = std::make_shared<v2f>(new v2f[3], std::default_delete<v2f[]>());
	std::shared_ptr<a2v> a_s(new a2v[3], std::default_delete<a2v[]>());
	std::shared_ptr<v2f> v_s(new v2f[3], std::default_delete<v2f[]>());
	for (int i = 0; i < renderer.get_nums_faces(); i++) {
		
		for (int j = 0; j < 3; j++) {
			//a_s[j] = renderer.processShader(i, j);
			renderer.processShader(i, j, &a_s.get()[j]);

			// 80->140 = 60֡ 
			//v_s[j] = renderer.vertexShader_Gouruad(a_s[j], lightPos);
			renderer.vertexShader_Gouruad(&a_s.get()[j], &v_s.get()[j], lightPos);
		}

		// �����µ�Shader�ṹ
		// 80->150 = 70֡
		DrawTriangle_barycentric_Shader_Gouruad(v_s.get(), renderer);
	}

	HDC hDC = GetDC(hwnd);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hDC);
}

// ��vs��ps���
void Draw_2_vs_and_ps(HWND hwnd, Renderer renderer) {
	for (int i = 0; i < renderer.get_nums_faces(); i++) {
		v2f v_s[3];
		for (int j = 0; j < 3; j++) {
			v_s[j] = renderer.vertexShader_and_proecc(i, j);
		}

		// �����µ�Shader�ṹ
		DrawTriangle_barycentric_Shader(v_s, light_pos, renderer);
		//DrawTriangle_barycentric_zbuffer(v_s, renderer);
	}

	HDC hDC = GetDC(hwnd);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hDC);
}

// ���̰߳汾
void Draw_3_mutil(HWND hwnd, Renderer renderer) {
	for (int i = 0; i < renderer.get_nums_faces(); i++) {
		a2v a_s[3];
		v2f v_s[3];
		for (int j = 0; j < 3; j++) {
			a_s[j] = renderer.processShader(i, j);
			v_s[j] = renderer.vertexShader(a_s[j]);
		}

		//thread t(DrawTriangle_barycentric_Shader, v_s, light_pos, renderer);
		//t.detach();
	}

	HDC hDC = GetDC(hwnd);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hDC);
}

// ���Կ��ٶ�ȡ......?��΢����һ��� 1.656 -> 1.625
void Draw_4_fastStorage(HWND hwnd, Renderer renderer, vector<a2v>* a2v_s_) {
	for (int i = 0; i < renderer.get_nums_faces(); i++) {
		v2f v_s[3];
		for (int j = 0; j < 3; j++) {
			v_s[j] = renderer.vertexShader((*a2v_s_)[i*3 + j]);
		}

		DrawTriangle_barycentric_Shader(v_s, light_pos, renderer);
	}

	HDC hDC = GetDC(hwnd);
	BitBlt(hDC, 0, 0, width, height, screen_dc, 0, 0, SRCCOPY);
	ReleaseDC(hwnd, hDC);
}

// ��Ⱦ���������β����ٶ�
void Draw_5_single(HWND hwnd, Renderer renderer) {
	// ��һ��������
	a2v a_s_2[3];
	v2f v_s_2[3];
	a_s_2[0].vertex_native = Vec4f(-0.3, -0.3, -0.3, 1);
	a_s_2[1].vertex_native = Vec4f(-0.5, 0.1, -0.3, 1);
	a_s_2[2].vertex_native = Vec4f(0.8, 0.6, 0.6, 1);

	for (int i = 0; i < 3; i++) v_s_2[i] = renderer.vertexShader(a_s_2[i]);

	DrawTriangle_barycentric_Shader_single(v_s_2, light_pos, renderer, white_color);
}

// �����Ż�Shader�͹�դ������Ķ���
void Draw_6_test_Shader(HWND hwnd, Renderer renderer) {
	for (int i = 0; i < mesh->nums_faces(); i++) {
		a2v a_s[3];
		v2f v_s[3];
		for (int j = 0; j < 3; j++) {
			a_s[j] = renderer.processShader(i, j);
			v_s[j] = renderer.vertexShader(a_s[j]);
		}

		// �����µ�Shader�ṹ
		DrawTriangle_barycentric_Shader_test(v_s, light_pos, renderer);
		//DrawTriangle_barycentric_zbuffer(v_s, renderer);
	}
}

// ��ʼ�����ڲ�ע��
void init_Register_wndclass(WNDCLASS& wndclass, HINSTANCE hInstance, TCHAR szAppName[]) {
	//2�����ô����������
	wndclass.style = CS_HREDRAW | CS_VREDRAW;						// ���ڷ��
	wndclass.lpfnWndProc = WndProc;									//�ص�
	wndclass.cbClsExtra = 0;										// 0		
	wndclass.cbWndExtra = 0;										// 0
	wndclass.hInstance = hInstance;									// ����ID
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				// ͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);					// ���
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//����ɫ
	wndclass.lpszMenuName = NULL;									// �˵�
	wndclass.lpszClassName = szAppName;								//��������

	//3��ע�ᴰ����
	if (!RegisterClass(&wndclass)) {
		MessageBox(NULL, TEXT("This program can only run under Windows NT!"), szAppName, MB_ICONERROR);
		return;
	}
}

// ����bitmapinfo
void set_bitmap(BITMAPINFO& bi, HDC& screen_dc, HBITMAP& screen_hb, HBITMAP& screen_ob,
	unsigned int*& screen_fb, LPVOID ptr) {
	// **1������bitmap**
	bi.bmiHeader.biSize = sizeof(BITMAPINFO);		// ���ṹ��С
	bi.bmiHeader.biWidth = width;					// bitmap�Ŀ���->��
	bi.bmiHeader.biHeight = height;					// bitmap�ĸߣ���->��
	bi.bmiHeader.biPlanes = 1;						// ָ��ƽ̨����Ϊ1
	bi.bmiHeader.biBitCount = 32;					// ָ�������ɫ��Ϊ2^32
	bi.bmiHeader.biCompression = BI_RGB;			// ѹ��ģʽ
	bi.bmiHeader.biSizeImage = width * height * 4;	// bitmap�Ĵ�С
	bi.bmiHeader.biXPelsPerMeter = 0;				// λͼ�豸��ˮƽ�ֱ���
	bi.bmiHeader.biYPelsPerMeter = 0;				// λͼ�豸�Ĵ�ֱ�ֱ���
	bi.bmiHeader.biClrUsed = 0;						// ʵ���õ�����ɫ�����ɫ������
	bi.bmiHeader.biClrImportant = 0;				// ��ʾλͼҪ�õ�����ɫ������

	// bitmap
	// **2�������ڴ��豸�������**
	HDC hdc = GetDC(hwnd);
	screen_dc = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, hdc);

	// **3������DIBSection**
	screen_hb = CreateDIBSection(screen_dc, &bi, DIB_RGB_COLORS, &ptr, 0, 0);

	// **4����ob����hb����һ��״̬**
	screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);

	// **5����void*���͵�ָ��ǿ��ת��Ϊunsigned int*ָ���ֵ��fb
	screen_fb = (unsigned int*)ptr;
}



