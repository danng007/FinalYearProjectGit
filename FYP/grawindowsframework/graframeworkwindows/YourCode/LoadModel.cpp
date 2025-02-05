﻿#include "LoadModel.h"

#define YU 1//800//2
LoadModel::LoadModel()
{
	
}

LoadModel::~LoadModel()
{

}

void LoadModel::LoadOBJ(const char * path)
{

		ifstream ifs(path);//cube bunny Eight
		string s;
		Face *f;
		POINT3 *v;
		Normals *vn;
		Texture    *vt;
		Material *mp;
		while (getline(ifs, s))
		{
			if (s.length()<2)continue;
			if (s[0] == 'v')
			{
				if (s[1] == 't')
				{//vt 
					istringstream in(s);
					vt = new Texture();
					string head; //ignore first word
					in >> head >> vt->TU >> vt->TV;
					m_pic.VT.push_back(*vt);
				}
				else if (s[1] == 'n')
				{//vn
					istringstream in(s);
					vn = new Normals();
					string head;
					in >> head >> vn->NX >> vn->NY >> vn->NZ;
					m_pic.VN.push_back(*vn);
				}
				else
				{//v 
					istringstream in(s);
					v = new POINT3();
					string head;
					in >> head >> v->X >> v->Y >> v->Z;
					m_pic.V.push_back(*v);
				}
			}
			if (s[0] == 'u') //usematerial filepath
			{
				istringstream in(s);
				mp = new Material();
				string head;
				in >> head >> mp->filePath;
				m_pic.M.push_back(*mp);
				counter++; //count number of faces
				faceCounter[counter] = 0; //initialise
			}
			else if (s[0] == 'f'){//f 
				faceCounter[counter] ++;
				for (int k = s.size() - 1; k >= 0; k--){
					if (s[k] == '/')s[k] = ' '; //divide string by '/'
				}
				istringstream in(s);
				f = new Face();
				string head;
				in >> head;
				int i = 0;
				while (i<3)
				{
					if (m_pic.V.size() != 0)
					{
						in >> f->V[i];
						f->V[i] -= 1;
					}
					if (m_pic.VT.size() != 0)
					{
						in >> f->T[i];
						f->T[i] -= 1;
					}
					if (m_pic.VN.size() != 0)
					{
						in >> f->N[i];
						f->N[i] -= 1;
					}
					i++;
				}
				m_pic.F.push_back(*f);
			}
		}
		
		for (int i = 0; i < m_pic.M.size(); i++)
		{
			filePath = "./Resources/models/" + m_pic.M[i].filePath + ".bmp"; // combine into full file address
			filePaths[i] = filePath;
			textIDs[i] = Scene::GetTexture(filePaths[i]);
		}
		
}
void LoadModel::RenderModel()
{
	outCounter = 0;
	counter = 0;
	if (m_pic.M.size() > 0)
	{
		glBindTexture(GL_TEXTURE_2D, textIDs[counter]);
	
	}
		for (int i = 0; i<m_pic.F.size(); i++)
		{
			if (outCounter == faceCounter[counter] && counter + 1 < m_pic.M.size())
			{
				counter++;
				outCounter = 0;
				glBindTexture(GL_TEXTURE_2D, textIDs[counter]);
			}
			glBegin(GL_TRIANGLES);                           
			if (m_pic.VT.size() != 0)glTexCoord2f(m_pic.VT[m_pic.F[i].T[0]].TU, m_pic.VT[m_pic.F[i].T[0]].TV);  //textures   
			if (m_pic.VN.size() != 0)glNormal3f(m_pic.VN[m_pic.F[i].N[0]].NX, m_pic.VN[m_pic.F[i].N[0]].NY, m_pic.VN[m_pic.F[i].N[0]].NZ);//f vectore
			glVertex3f(m_pic.V[m_pic.F[i].V[0]].X / YU, m_pic.V[m_pic.F[i].V[0]].Y / YU, m_pic.V[m_pic.F[i].V[0]].Z / YU);        // vertices 

			if (m_pic.VT.size() != 0)glTexCoord2f(m_pic.VT[m_pic.F[i].T[1]].TU, m_pic.VT[m_pic.F[i].T[1]].TV);  
			if (m_pic.VN.size() != 0)glNormal3f(m_pic.VN[m_pic.F[i].N[1]].NX, m_pic.VN[m_pic.F[i].N[1]].NY, m_pic.VN[m_pic.F[i].N[1]].NZ);
			glVertex3f(m_pic.V[m_pic.F[i].V[1]].X / YU, m_pic.V[m_pic.F[i].V[1]].Y / YU, m_pic.V[m_pic.F[i].V[1]].Z / YU);     

			if (m_pic.VT.size() != 0)glTexCoord2f(m_pic.VT[m_pic.F[i].T[2]].TU, m_pic.VT[m_pic.F[i].T[2]].TV); 
			if (m_pic.VN.size() != 0)glNormal3f(m_pic.VN[m_pic.F[i].N[2]].NX, m_pic.VN[m_pic.F[i].N[2]].NY, m_pic.VN[m_pic.F[i].N[2]].NZ);
			glVertex3f(m_pic.V[m_pic.F[i].V[2]].X / YU, m_pic.V[m_pic.F[i].V[2]].Y / YU, m_pic.V[m_pic.F[i].V[2]].Z / YU);  
			glEnd();
			outCounter++;
		}
		
	glBindTexture(GL_TEXTURE_2D, 0);
}