#include "v2i.h"

class BOUND : public v2i { };
class START : public v2i { };
class END : public v2i { };
class CURSOR : public v2i {public: int grasp, pivot_id;};
class PIVOT : public v2i {public: int dir, grasped;};
class LINE {
	public: 
		v2i pt1;
		v2i pt2;
		bool valid(void)
			{
				if(pt1.x == pt2.x || pt1.y == pt2.y)
					{return true;}
				else
					{return false;}
			}
};
