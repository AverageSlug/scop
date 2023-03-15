#define GL_SILENCE_DEPRECATION

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "scop.hpp"

t_vertex	*vertices = NULL;
t_face		*faces = NULL;
float		q = M_PI;
float		axis[7] = {0, 0, 0, 0, 0, 0, 0};
int			n = 0;
bool		tex_apply = false;
int			width, height;
const char	*data;
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
					glTexCoord2f(((vertices[tmp->points[i] - 1].x - (axis[0] + axis[1]) / 2) * cos(q) - (vertices[tmp->points[i] - 1].z - (axis[4] + axis[5]) / 2) * sin(q) + (axis[0] + axis[1]) / 2) / (axis[6] * 1.2), (vertices[tmp->points[i] - 1].y) / (axis[6] * 1.2));
				else
					glColor3f((f % 3) == 0 ? 1 : 0, (f % 3) == 1 ? 1 : 0, (f % 3) == 2 ? 1 : 0);
				glVertex3f(((vertices[tmp->points[i] - 1].x - (axis[0] + axis[1]) / 2) * cos(q) - (vertices[tmp->points[i] - 1].z - (axis[4] + axis[5]) / 2) * sin(q) + (axis[0] + axis[1]) / 2) / (axis[6] * 1.2), (vertices[tmp->points[i] - 1].y) / (axis[6] * 1.2), -(((vertices[tmp->points[i] - 1].z - (axis[4] + axis[5]) / 2) * cos(q) + (vertices[tmp->points[i] - 1].x - (axis[0] + axis[1]) / 2) * sin(q) + (axis[4] + axis[5]) / 2) / (axis[6] * 1.2)));
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
			vertices[i].x -= axis[6] / 100;
		axis[0] -= axis[6] / 100;
		axis[1] -= axis[6] / 100;
	}
	else if (key == 55)
	{
		for (int i = 0; i < n; i++)
			vertices[i].x += axis[6] / 100;
		axis[0] += axis[6] / 100;
		axis[1] += axis[6] / 100;
	}
	else if (key == 50)
	{
		for (int i = 0; i < n; i++)
			vertices[i].y -= axis[6] / 100;
		axis[2] -= axis[6] / 100;
		axis[3] -= axis[6] / 100;
	}
	else if (key == 56)
	{
		for (int i = 0; i < n; i++)
			vertices[i].y += axis[6] / 100;
		axis[2] += axis[6] / 100;
		axis[3] += axis[6] / 100;
	}
	else if (key == 51)
	{
		for (int i = 0; i < n; i++)
		{
			vertices[i].x *= 1.01;
			vertices[i].y *= 1.01;
			vertices[i].z *= 1.01;
			// vertices[i].z -= axis[6] / 100;
		}
		axis[0] *= 1.01;
		axis[1] *= 1.01;
		axis[2] *= 1.01;
		axis[3] *= 1.01;
		axis[4] *= 1.01;
		axis[5] *= 1.01;
		// axis[4] -= axis[6] / 100;
		// axis[5] -= axis[6] / 100;
	}
	else if (key == 57)
	{
		for (int i = 0; i < n; i++)
		{
			vertices[i].x /= 1.01;
			vertices[i].y /= 1.01;
			vertices[i].z /= 1.01;
			// vertices[i].z += axis[6] / 100;
		}
		axis[0] /= 1.01;
		axis[1] /= 1.01;
		axis[2] /= 1.01;
		axis[3] /= 1.01;
		axis[4] /= 1.01;
		axis[5] /= 1.01;
		// axis[4] += axis[6] / 100;
		// axis[5] += axis[6] / 100;
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
			if (!vertices)
				vertices = new t_vertex[n + 1];
			else
			{
				t_vertex	*tmp = new t_vertex[n + 1];
				for (int i = 0; i < n; i++)
				{
					tmp[i].x = vertices[i].x;
					tmp[i].y = vertices[i].y;
					tmp[i].z = vertices[i].z;
				}
				delete [] vertices;
				vertices = tmp;
			}
			int		pos = 0, pos2 = 0;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[n].x = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			if (axis[0] == 0 && axis[1] == 0)
			{
				axis[0] = vertices[n].x;
				axis[1] = vertices[n].x;
			}
			if (vertices[n].x > axis[0])
				axis[0] = vertices[n].x;
			else if (vertices[n].x < axis[1])
				axis[1] = vertices[n].x;
			pos = pos2;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[n].y = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			if (axis[2] == 0 && axis[3] == 0)
			{
				axis[2] = vertices[n].y;
				axis[3] = vertices[n].y;
			}
			if (vertices[n].y > axis[2])
				axis[2] = vertices[n].y;
			else if (vertices[n].y < axis[3])
				axis[3] = vertices[n].y;
			pos = pos2;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[n].z = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			if (axis[4] == 0 && axis[4] == 0)
			{
				axis[4] = vertices[n].z;
				axis[5] = vertices[n].z;
			}
			if (vertices[n].z > axis[4])
				axis[4] = vertices[n].z;
			else if (vertices[n].z < axis[5])
				axis[5] = vertices[n].z;
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
	std::string		tmp_data;
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
				tmp_data.append(str);
		}
		std::getline(file, str);
	}
	file.close();
	std::string		tmp_data69;
	for (int j = height - 1; j >= 0; j--)
		for (int i = 0; i < width * 3; i += 3)
		{
			tmp_data69.push_back(tmp_data[i + (j * width * 3)]);
			tmp_data69.push_back(tmp_data[i + (j * width * 3) + 1]);
			tmp_data69.push_back(tmp_data[i + (j * width * 3) + 2]);
		}
	data = tmp_data69.c_str();
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, texture);
	glutDisplayFunc(display);
	glutKeyboardFunc(key_presses);
	glMatrixMode(GL_PROJECTION);
	glEnable(GL_DEPTH_TEST);
	glLoadIdentity();
	glutIdleFunc(idle);
	glutMainLoop();
}
