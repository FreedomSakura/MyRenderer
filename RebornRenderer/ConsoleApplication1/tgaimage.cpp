#include <iostream>

#include "tgaimage.h"


TGAImage::TGAImage() : data(NULL), width(0), height(0), bytespp(0) {
}

TGAImage::TGAImage(int w, int h, int bpp) : data(NULL), width(w), height(h), bytespp(bpp) {
	unsigned long nbytes = width * height * bytespp;
	data = new unsigned char[nbytes];
	memset(data, 0, nbytes);
}

TGAImage::~TGAImage() {
	if (data) delete[] data;
}

// ��ȡtga�ļ�
bool TGAImage::read_tga_file(const char* filename) {
	// �ͷ�data���ڴ棨��Ϊ�Ƕ�ȡ�ļ������е�data���ܲ�����
	if (data) delete[] data;	
	data = NULL;

	std::ifstream in;
	in.open(filename, std::ios::binary);
	if (!in.is_open()) {
		std::cerr << "can't open file" << filename << "\n";
		in.close();
		return false;
	}

	// ��ȡTGA File Header
	TGA_Header header;
	in.read((char*)&header, sizeof(header));
	if (!in.good()) {
		in.close();
		std::cerr << "an error occured while reading the header\n";
		return false;
	}
	// �޸�TGAImage������
	width = header.width;
	height = header.height;
	bytespp = header.bitsPerPixel >> 3;		// (>>3) == (/8) ����3λ�൱�ڳ���8
	if (width <= 0 || height <= 0 || (bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA)) {
		in.close();
		std::cerr << "bad bpp (or width/height) value\n";
		return false;
	}
	
	// ������data��
	unsigned long nbytes = bytespp * width * height;
	data = new unsigned char[nbytes];

	if (3 == header.dataTypeCode || 2 == header.dataTypeCode) {			// ��ѹ��
		in.read((char*)data, nbytes);
		if (!in.good()) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else if (10 == header.dataTypeCode || 11 == header.dataTypeCode) {
		if (!load_rle_data(in)) {
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else {
		in.close();
		std::cerr << "unknown file format " << (int)header.dataTypeCode << "\n";
		return false;
	}

	std::cout << "��ȡtga�ļ��ɹ�����������Ϊ:" << width << " x " << height << " / " << bytespp * 8 << std::endl;;

	in.close();
	return true;
}

bool TGAImage::load_rle_data(std::ifstream& in) {
	unsigned long pixelcount = width * height;
	unsigned long currentpixel = 0;		// ��ǰ�����أ�����1024*1024
	unsigned long currentbyte = 0;		// ��ǰ�ı��أ�����1024*1024*24

	Color colorbuffer;

	do {
		unsigned char chunkheader = 0;
		// ��ȡ���ݿ�ĵ�һλ
		chunkheader = in.get();
		if (!in.good()) {
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		// ���ݿ�����λ����0�������������ŵ��Ƿ��ظ����ݣ������ɵ�7λ���������127��
		if (chunkheader < 128) {
			chunkheader++;
			for (int i = 0; i < chunkheader; i++) {
				in.read((char*)colorbuffer.raw, bytespp); // ��ȡ��ɫ��
				if (!in.good()) {
					std::cerr << "an error occured while reading the header\n";
					return false;
				}

				for (int t = 0; t < bytespp; t++)
					data[currentbyte++] = colorbuffer.raw[t];

				currentpixel++;		// ��ǰ������λ��+1

				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
		// ���ݿ�����λ����1�� �����������ŵ����ظ����ݣ�����Ҳ�ɵ�7λ���������127��
		else {
			chunkheader -= 127;
			in.read((char*)colorbuffer.raw, bytespp);	// ��ȡ��ɫ��
			if (!in.good()) {
				std::cerr << "an error occured while reading the header\n";
				return false;
			}

			for (int i = 0; i < chunkheader; i++) {
				for (int t = 0; t < bytespp; t++)
					data[currentbyte++] = colorbuffer.raw[t];
				
				currentpixel++;		// ��ǰ������λ��+1

				if (currentpixel > pixelcount) {
					std::cerr << "Too many pixels read\n";
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);

	return true;
}

Color TGAImage::get(int x, int y) {
	// �߽��ж�
	//std::cout << "x: " << x << " y: " << y << std::endl;
	if (!data || x < 0 || y < 0 || x >= width || y >= height) {
		std::cout << "ͼ���������벻�Ϸ����޷���ȡ��ȷ����ɫ��\n" << std::endl;
		return Color();
	}

	Color c;
	unsigned char* p = data + (y * width + x) * bytespp;

	for (int i = 0; i < bytespp; i++)
		c.raw[i] = p[i];

	return c;
}

int TGAImage::get_width()
{
	return width;
}
int TGAImage::get_height()
{
	return height;
}





