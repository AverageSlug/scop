#define GL_SILENCE_DEPRECATION

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "scop.h"
#include <stdio.h>

void	display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_POLYGON);
		glColor3f(0.3, 0, 0); glVertex3f(1, 1, 0);
		glColor3f(0.3, 0, 0); glVertex3f(1, -1, 0);
		glColor3f(0.3, 0, 0); glVertex3f(-1, -1, 0);
		glColor3f(0.3, 0, 0); glVertex3f(-1, 1, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(1, 1, 0); glVertex3f(0.5, 0.5, -1);
		glColor3f(1, 1, 0); glVertex3f(0.5, -0.5, -1);
		glColor3f(1, 1, 0); glVertex3f(-0.5, -0.5, -1);
		glColor3f(1, 1, 0); glVertex3f(-0.5, 0.5, -1);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0, 0.5, 0); glVertex3f(0.75, 0.75, 0);
		glColor3f(0, 1, 0); glVertex3f(0.5, 0.5, -1);
		glColor3f(0, 1, 0); glVertex3f(0.5, -0.5, -1);
		glColor3f(0, 0.5, 0); glVertex3f(0.75, -0.75, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0, 0.5, 0.5); glVertex3f(0.75, -0.75, 0);
		glColor3f(0, 1, 1); glVertex3f(0.5, -0.5, -1);
		glColor3f(0, 1, 1); glVertex3f(-0.5, -0.5, -1);
		glColor3f(0, 0.5, 0.5); glVertex3f(-0.75, -0.75, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0, 0, 0.5); glVertex3f(0.75, -0.75, 0);
		glColor3f(0, 0, 1); glVertex3f(0.5, -0.5, -1);
		glColor3f(0, 0, 1); glVertex3f(-0.5, -0.5, -1);
		glColor3f(0, 0, 0.5); glVertex3f(-0.75, -0.75, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.5, 0, 0.5); glVertex3f(-0.75, -0.75, 0);
		glColor3f(1, 0, 1); glVertex3f(-0.5, -0.5, -1);
		glColor3f(1, 0, 1); glVertex3f(-0.5, 0.5, -1);
		glColor3f(0.5, 0, 0.5); glVertex3f(-0.75, 0.75, 0);
	glEnd();

	glBegin(GL_POLYGON);
		glColor3f(0.5, 0.5, 0.5); glVertex3f(-0.75, 0.75, 0);
		glColor3f(1, 1, 1); glVertex3f(-0.5, 0.5, -1);
		glColor3f(1, 1, 1); glVertex3f(0.5, 0.5, -1);
		glColor3f(0.5, 0.5, 0.5); glVertex3f(0.75, 0.75, 0);
	glEnd();

	glFlush();
}

int		parse420(char *filename)
{
	return (0);
}

int		main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("Wrong number of arguments\n");
		return (0);
	}

	t_vertex	*lmao;
	if (parse420(argv[1]))
	{
		printf("Invalid file\n");
		return (0);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowPosition(80, 80);
	glutInitWindowSize(400, 400);
	glutCreateWindow("42");
	glutDisplayFunc(display);

	glutMainLoop();
}