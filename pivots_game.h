#include "game_obj.h"


class pivots_game
{
public:
	bool load_file(char *location);
	
	int request_move_cursor(int x, int y);
	int request_pivot(int dir);
	int request_pick_drop_pivot(void);
	
	v2i get_bound(void);
	v2i pos_start(void);
	v2i pos_end(void);
	v2i pos_cursor(void);
	int grasp_cursor(void);
	int num_pivot(void);
	v2i pos_pivot(int i);
	int dir_pivot(int i);
	int num_line(void);
	v2i pos_line_pt1(int i);
	v2i pos_line_pt2(int i);
	int declare_win(void);
	void get_author(char *s);
	
	//for undo and redo keep a list of the command executed and succeeded
	//then upon undo execute the pervious item in the list
	//would only be for pivots and moving pivots
	void undo_move(void);  
	void redo_move(void);
	
protected:
	char lvl_author[25];
	BOUND bound;
	START start;
	END end;
	CURSOR cursor;
	int pivot_num;
	PIVOT pivot[100];
	int line_num;
	LINE line[100];
	bool point_on_line(LINE _line, v2i _p);
	bool point_on_middle_line(LINE _line, v2i _p);
	bool line_in_line(LINE line_1, LINE line_2);
	int fsign(int i);
	bool check_bound(v2i pt);
};