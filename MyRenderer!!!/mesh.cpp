#include "mesh.h"

// ��ȡobj�ļ�
Mesh::Mesh(const string filePath, const char* tex_path) {
	// ������ȡ�ļ���
	ifstream in;
	in.open(filePath, ifstream::in);
	if (!in.is_open()) {
		cout << "��obj�ļ�ʧ�ܣ�" << endl;
	}

	//cout << "123" << endl;

	// ���ڴ洢ÿ�е�����
	string line;

	int c = 0;
	while (!in.eof()) {
		getline(in, line);
		istringstream iss(line.c_str());
		char trash;
		if (!line.compare(0, 2, "v ")) {
			iss >> trash;	// ��"v"����char���͵�trash��ͬʱǰ��һ���ַ���λ��
			Vec3f v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;

			verts_.push_back(v);
		}
		else if (!line.compare(0, 3, "vt ")) {
			iss >> trash >> trash;
			Vec2f v;
			iss >> v.x;
			iss >> v.y;

			uvs_.push_back(v);
		}
		else if (!line.compare(0, 3, "vn ")) {
			iss >> trash >> trash;
			Vec3f v;
			iss >> v.x;
			iss >> v.y;
			iss >> v.z;

			norms_.push_back(v);
		}
		else if (!line.compare(0, 2, "f ")) {
			iss >> trash;

			int v, u, n;

			int cnt = 0;
			while (iss >> v >> trash >> u >> trash >> n) {
				faces_vert_.push_back(--v);
				faces_uv_.push_back(--u);
				faces_norm_.push_back(--n);

				cnt++;
			}

			if (cnt != 3) {
				cerr << "��ģ�Ͳ�֧�������������" << endl;
				in.close();
				return;
			}
		}
	}
	// �ر��ļ���
	in.close();
	// ��ʾģ����Ϣ
	cout << "** v: " << nums_verts() << " f: " << nums_faces() << " vt: " << uvs_.size() << " vn: " << norms_.size() << " **" << endl;

	load_diffuse_tex(tex_path, diffuseMap_);
}

// �����ģ�͵Ķ������
int Mesh::nums_verts() {
	return verts_.size();
}

// �����ģ�͵�����
int Mesh::nums_faces() {
	return faces_vert_.size() / 3;
}

// ��ȡ��n�Ķ���λ��
Vec3f Mesh::get_vert_(int n) {
	return verts_[n];
}

// ��ȡ��iface����ĵ�n��
Vec3f Mesh::get_vert_(int iface, int n) {
	return verts_[faces_vert_[iface * 3 + n]];
}

// ��ȡ��n�Ķ�����������
Vec2f Mesh::get_uv_(int n) {
	return uvs_[n];
}
// ��ȡ��iface����ĵ�n�������������
//Vec2f Mesh::get_uv_(int iface, int n) {
//	return uvs_[faces_uv_[iface * 3 + n]];
//}
Vec2f Mesh::get_uv_(int iface, int n) {
	Vec2f uv = uvs_[faces_uv_[iface * 3 + n]];
	return Vec2f(uv.x * diffuseMap_.get_width(), uv.y * diffuseMap_.get_height());
}

// ��ȡ��n�Ķ��㷨��
Vec3f Mesh::get_norm_(int n) {
	return norms_[n];
}
// ��ȡ��iface����ĵ�n����ķ���
Vec3f Mesh::get_norm_(int iface, int n) {
	return norms_[faces_norm_[iface * 3 + n]];
}

// ��ȡ��n����Ķ�������
int Mesh::get_faces_vert_(int n) {
	return faces_vert_[n];
}

// ��ȡ����
void Mesh::load_diffuse_tex(const char* texPath, TGAImage& image) {
	// ��ȡ����
	image.read_tga_file(texPath);
}

// ��ȡ����
//Color Mesh::diffuse(Vec2i tex_coord, TGAImage& image) {
//	return image.get(tex_coord.x, tex_coord.y);
//}
Color Mesh::diffuse(Vec2i tex_coord) {
	return diffuseMap_.get(tex_coord.x, tex_coord.y);
}



