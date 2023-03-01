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

void	display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	t_face	*tmp = faces;
	while (tmp)
	{
		glBegin(GL_POLYGON);
			for (int i = 0; i < tmp->len; i++)
			{
				glColor3f(1, 1, 1); glVertex3f(vertices[tmp->points[i] - 1].x * cos(q) - vertices[tmp->points[i] - 1].z * sin(q), vertices[tmp->points[i] - 1].y, vertices[tmp->points[i] - 1].z * cos(q) + vertices[tmp->points[i] - 1].x * sin(q));
			}
		glEnd();
		std::cout << q << std::endl;
		q -= M_PI / 7200;
		if (q <= -M_PI)
			q = M_PI;
		tmp = tmp->next;
	}
	glFlush();
}

void	idle()
{
	glutPostRedisplay();
}

int		parse(char *filename)
{
	std::fstream	file;
	std::string		str;
	int				n = 0;

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
			pos = pos2;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[n].y = std::stof(str.substr(pos + 1, pos2 - pos - 1));
			pos = pos2;
			pos = str.find(" ", pos);
			pos2 = str.find(" ", pos + 1);
			vertices[n].z = std::stof(str.substr(pos + 1, pos2 - pos - 1));
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

int		main(int argc, char** argv)
{
	if (argc != 2 || !argv[1])
	{
		std::cout << "Wrong number of arguments" << std::endl;
		return (0);
	}
	if (parse(argv[1]))
	{
		std::cout << "Invalid file" << std::endl;
		return (0);
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(1600, 80);
	glutInitWindowSize(400, 400);
	glutCreateWindow("42");
	glutDisplayFunc(display);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glutIdleFunc(idle);
	glutMainLoop();
}