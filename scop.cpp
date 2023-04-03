#define GL_SILENCE_DEPRECATION

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "scop.hpp"

t_vertex	*vertices[2] = {NULL, NULL};
t_face		*faces = NULL;
float		q = M_PI;
float		axis[7] = {0, 0, 0, 0, 0, 0, 0};
int			n = 0;
bool		tex_apply = false;
int			width, height;
std::string	data;
GLuint texture;

void	display()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	t_face	*tmp = faces;
	int		f = 0;
	while (tmp)
	{
		glBegin(GL_POLYGON);
			for (int i = 0; i < tmp->len; i++)
			{
				if (tex_apply)
					glTexCoord2f(-vertices[1][tmp->points[i] - 1].x, -vertices[1][tmp->points[i] - 1].y);
				else
					glColor3f((f % 3) == 0 ? 1 : 0, (f % 3) == 1 ? 1 : 0, (f % 3) == 2 ? 1 : 0);
				glVertex3f(((vertices[0][tmp->points[i] - 1].x - (axis[0] + axis[1]) / 2) * cos(q) - (vertices[0][tmp->points[i] - 1].z - (axis[4] + axis[5]) / 2) * sin(q) + (axis[0] + axis[1]) / 2) / (axis[6] * 1.2), (vertices[0][tmp->points[i] - 1].y) / (axis[6] * 1.2), ((vertices[0][tmp->points[i] - 1].z - (axis[4] + axis[5]) / 2) * cos(q) + (vertices[0][tmp->points[i] - 1].x - (axis[0] + axis[1]) / 2) * sin(q) + (axis[4] + axis[5]) / 2) / (axis[6] * 1.2));
			}
		glEnd();
		tmp = tmp->next;
		f++;
	}
	q -= M_PI / 200;
	if (q <= -M_PI)
		q = M_PI;
	glFlush();
}

void	move_object(unsigned char key)
{
	if (key == 49)
	{
		for (int i = 0; i < n; i++)
			vertices[0][i].x -= axis[6] / 100;
		axis[0] -= axis[6] / 100;
		axis[1] -= axis[6] / 100;
	}
	else if (key == 55)
	{
		for (int i = 0; i < n; i++)
			vertices[0][i].x += axis[6] / 100;
		axis[0] += axis[6] / 100;
		axis[1] += axis[6] / 100;
	}
	else if (key == 50)
	{
		for (int i = 0; i < n; i++)
			vertices[0][i].y -= axis[6] / 100;
		axis[2] -= axis[6] / 100;
		axis[3] -= axis[6] / 100;
	}
	else if (key == 56)
	{
		for (int i = 0; i < n; i++)
			vertices[0][i].y += axis[6] / 100;
		axis[2] += axis[6] / 100;
		axis[3] += axis[6] / 100;
	}
	else if (key == 51)
	{
		for (int i = 0; i < n; i++)
		{
			vertices[0][i].z -= axis[6] / 100;
			vertices[0][i].x *= 1.01;
			vertices[0][i].y *= 1.01;
			vertices[0][i].z *= 1.01;
		}
		axis[4] -= axis[6] / 100;
		axis[5] -= axis[6] / 100;
		axis[0] *= 1.01;
		axis[1] *= 1.01;
		axis[2] *= 1.01;
		axis[3] *= 1.01;
		axis[4] *= 1.01;
		axis[5] *= 1.01;
	}
	else if (key == 57)
	{
		for (int i = 0; i < n; i++)
		{
			vertices[0][i].z += axis[6] / 100;
			vertices[0][i].x /= 1.01;
			vertices[0][i].y /= 1.01;
			vertices[0][i].z /= 1.01;
		}
		axis[4] += axis[6] / 100;
		axis[5] += axis[6] / 100;
		axis[0] /= 1.01;
		axis[1] /= 1.01;
		axis[2] /= 1.01;
		axis[3] /= 1.01;
		axis[4] /= 1.01;
		axis[5] /= 1.01;
	}
}

void	key_presses(unsigned char key, int x, int y)
{
	(void)x;
	(void)y;
	if (key == 49 || key == 55 || key == 50 || key == 56 || key == 51 || key == 57)
		move_object(key);
	if (key == 116)
	{
		tex_apply = !tex_apply;
		if (tex_apply)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
	}
	if (key == 27)
		exit (0);
}

void	idle()
{
	glutPostRedisplay();
}

int		parse(char *filename)
{
	std::fstream	file;
	std::string		str;

	file.open(filename, std::ios::in);
	if (!file)
	{
		std::cout << "Error opening file" << std::endl;
		return (1);
	}
	std::getline(file, str);
	while (file)
	{
		if (str.size() && str[0] == 'v')
		{
			if (!vertices[0])
			{
				vertices[0] = new t_vertex[n + 1];
				vertices[1] = new t_vertex[n + 1];
			}
			else
			{
				t_vertex	*tmp = new t_vertex[n + 1];
				t_vertex	*tmp2 = new t_vertex[n + 1];
				for (int i = 0; i < n; i++)
				{
					tmp[i].x = vertices[0][i].x;
					tmp[i].y = vertices[0][i].y;
					tmp[i].z = vertices[0][i].z;
					tmp2[i].x = vertices[1][i].x;
					tmp2[i].y = vertices[1][i].y;
					tmp2[i].z = vertices[1][i].z;
				}
				delete [] vertices[0];
				delete [] vertices[1];
				vertices[0] = tmp;
				vertices[1] = tmp2;
			}
			int		pos = 0, pos2 = 0;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[0][n].x = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			vertices[1][n].x = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			if (axis[0] == 0 && axis[1] == 0)
			{
				axis[0] = vertices[0][n].x;
				axis[1] = vertices[0][n].x;
			}
			if (vertices[0][n].x > axis[0])
				axis[0] = vertices[0][n].x;
			else if (vertices[0][n].x < axis[1])
				axis[1] = vertices[0][n].x;
			pos = pos2;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[0][n].y = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			vertices[1][n].y = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			if (axis[2] == 0 && axis[3] == 0)
			{
				axis[2] = vertices[0][n].y;
				axis[3] = vertices[0][n].y;
			}
			if (vertices[0][n].y > axis[2])
				axis[2] = vertices[0][n].y;
			else if (vertices[0][n].y < axis[3])
				axis[3] = vertices[0][n].y;
			pos = pos2;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[0][n].z = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			vertices[1][n].z = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			if (axis[4] == 0 && axis[4] == 0)
			{
				axis[4] = vertices[0][n].z;
				axis[5] = vertices[0][n].z;
			}
			if (vertices[0][n].z > axis[4])
				axis[4] = vertices[0][n].z;
			else if (vertices[0][n].z < axis[5])
				axis[5] = vertices[0][n].z;
			n++;
		}
		if (str.size() && str[0] == 'f')
		{
			t_face	*tmp = faces;
			if (!faces)
			{
				faces = new t_face;
				tmp = faces;
			}
			else
			{
				while (tmp->next)
					tmp = tmp->next;
				tmp->next = new t_face;
				tmp = tmp->next;
			}
			tmp->next = NULL;
			size_t	len = 0, pos = 0, pos2 = 0;
			while ((pos = str.find(" ", pos)) && pos != str.npos)
			{
				len++;
				pos++;
			}
			tmp->len = len;
			tmp->points = new int[len];
			for (size_t i = 0; i < len; i++)
			{
				pos = pos2;
				pos = str.find(" ", pos);
				pos2 = str.find(" ", pos + 1);
				tmp->points[i] = std::stoi(str.substr(pos + 1, pos2 - pos - 1));
			}
		}
		std::getline(file, str);
	}
	file.close();
	return (0);
}

int		parse_texture(char *filename)
{
	std::fstream	file;
	std::string		str;
	int				i = 0;

	file.open(filename, std::ios::in);
	if (!file)
	{
		std::cout << "Error opening file" << std::endl;
		return (1);
	}
	std::getline(file, str);
	while (file)
	{
		if (!str.size() || str[0] != '#')
		{
			if (i < 3)
			{
				if (i == 1)
				{
					int		pos = 0, pos2 = 0;
					pos = str.find(" ", pos);
					pos2 = str.find(" ", pos + 1);
					width = stoi(str.substr(0, pos));
					height = stoi(str.substr(pos + 1, pos2));
				}
				i++;
				std::getline(file, str);
				continue ;
			}
			else
				data.append(str);
		}
		std::getline(file, str);
	}
	file.close();
	return (0);
}

int		main(int argc, char** argv)
{
	if (argc != 3 || !argv[1] || !argv[2])
	{
		std::cerr << "Wrong number of arguments" << std::endl;
		return (0);
	}
	if (parse(argv[1]))
	{
		std::cerr << "Invalid file" << std::endl;
		return (0);
	}
	if (parse_texture(argv[2]))
	{
		std::cerr << "Invalid file" << std::endl;
		return (0);
	}
	for (int i = 0; i < 6; i++)
		if (abs(axis[i]) > axis[6])
			axis[6] = abs(axis[i]);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(1600, 80);
	glutInitWindowSize(400, 400);
	glutCreateWindow("42");
	glDisable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.c_str());
	glutDisplayFunc(display);
	glutKeyboardFunc(key_presses);
	glEnable(GL_DEPTH_TEST);
	glutIdleFunc(idle);
	glutMainLoop();
}
