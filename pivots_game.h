#include "game_obj.h"


class pivots_game
{
public:
	int load_file(char *location);
	
	int request_move_cursor(int x, int y);
	int request_pivot(int dir);
	int request_pick_drop_pivot(void);
	v2i get_bound(void);
	v2i pos_start(void);
	v2i pos_end(void);
	v2i pos_cursor(void);
	int num_pivot(void);
	v2i pos_pivot(int i);
	int num_line(void);
	v2i pos_line_pt1(int i);
	v2i pos_line_pt2(int i);
	int declare_win(void);
	//for undo and redo keep a list of the command executed and succeeded
	//then upon undo execute the pervious item in the list
	void undo_move(void);  
	
	void redo_move(void);
	char lvl_author[25];
	
protected:
	BOUND bound;
	START start;
	END end;
	CURSOR cursor;
	int pivot_num;
	PIVOT pivot[100];
	int line_num;
	LINE line[100];
};