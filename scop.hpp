#ifndef SCOP_H
# define SCOP_H

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

typedef struct	s_vertex
{
	float		x;
	float		y;
	float		z;
}				t_vertex;

typedef struct		s_face
{
	int				*points;
	int				len;
	struct s_face	*next;
}					t_face;

#endif