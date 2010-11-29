#include "v2i.h"

class BOUND : public v2i { };
class START : public v2i { };
class END : public v2i { };
class CURSOR : public v2i {public: int grasp, pivot_id;};
class PIVOT : public v2i {public: int dir;};
class LINE {
	public: 
		v2i pt1;
		v2i pt2;
		int pivot;
		int direction(void)
		{
			if(pt1.x == pt2.x)
				{return 1;}
			else if(pt1.y == pt2.y)
				{return 0;}
		}
};

struct pv_lvl 
{
	char lvl_author[25];
	v2i bound;
	v2i start;
	v2i end;
	CURSOR cursor;
	int pivot_num;
	PIVOT pivot[100];
	int line_num;
	LINE line[100];
};
