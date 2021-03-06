#include "pivots_game.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//TODO: what if a pivot is on two end points of different lines, what diffines which line pivots

bool pivots_game::load_file(char *location) 
{
	int i,j;
	FILE *loadFile;
	
	loadFile = fopen(location, "r");
	
	if(loadFile != NULL){
		fscanf(loadFile, "%s", lvl_author);
		fscanf(loadFile, "%d %d", &bound.x, &bound.y);
		fscanf(loadFile, "%d %d", &start.x, &start.y);
		fscanf(loadFile, "%d %d", &end.x, &end.y);
		cursor.x = start.x;
		cursor.y = start.y;
		cursor.grasp=0;
		
		fscanf(loadFile, "%d", &pivot_num);
		for(i=0; i<pivot_num; i++)
		{
			fscanf(loadFile,"%d %d %d", &pivot[i].x, &pivot[i].y, &pivot[i].dir);
		}
		
		fscanf(loadFile, "%d", &line_num);
		for(i=0; i<line_num; i++)
		{
			fscanf(loadFile,"%d %d", &line[i].pt1.x, &line[i].pt1.y);
			fscanf(loadFile,"%d %d", &line[i].pt2.x, &line[i].pt2.y);
			line[i].pivot = 0;
		}
		return true;
	}
	else
		{printf("File %s unable to load.\n", location); return false;}
	fclose(loadFile);
}

int pivots_game::request_move_cursor(int _x, int _y)
{
	int i;
	v2i move (_x,_y);
	
	//check to see if you are moving a pivot
	if(cursor.grasp==1)
	{
		for(i=0; i<line_num; i++)
		{
			//if you are moving a pivot check to see if the direction of the pivot and line match up and
			//check to see what line(s) you are on
			//check to see if the direction you want to move is consistant with a line you are on
			if(point_on_line(line[i], cursor.Get()) && 
					point_on_line(line[i], cursor.Get() + move) && 
					line[i].direction() == pivot[cursor.pivot_id].dir)
			{
				//move the cursor and the pivot
				cursor += move;
				pivot[cursor.pivot_id].x = cursor.x;
				pivot[cursor.pivot_id].y = cursor.y;
				i = line_num;
			}
		}
	}
	else
	{
		//check to see what line(s) you are on
		//check to see if the direction you want to move is consistant with a line you are on
		for(i=0; i < line_num; i++)
		{
			if(point_on_line(line[i], cursor.Get()) && point_on_line(line[i], cursor.Get() + move))
				{cursor += move; i = line_num;}
		}
	}
}

int pivots_game::request_pivot(int dir)
{	
	/*there needs to be a series of tests to see if the line can pivot.
			1. Is the pivot direction the different than the line direction? if so don't pivot
			2. Is there more than one pivot on the line?  if so don't pivot
			3. Is the line currently interseting another line?  ditto
			4. In the processes of pivoting would the line "hit" another line? ditto
			5. Will the line pivot out of bounds? ditto
			6. Will line share more than one point with another line? ditto
			7. Will the line be interseting another line (other than an end point)? ditto
		After these tests, cursor.pivot_id will be set to the pivot that the line(s) will pivot
		around and line.pivot will be set to true.
	*/
	int i, k, j;
	bool turn_test = true;
	bool turn_pivot = true;
	if (!cursor.grasp)
	{
		//check to see which pivot you are on
		for (i = 0; i < pivot_num; i++)
		{
			if (cursor.Get()==pivot[i].Get())
			{
				//check to see which line the pivot is on
				for (k = 0; k < line_num; k++)
				{
					if (point_on_line(line[k], pivot[i].Get()))
					{
						//Set the cursors pivot_id to the pivot it is on
						cursor.pivot_id = i;
						
						//1. Is the pivot direction the different than the line direction? if so don't pivot
						if(line[k].direction() != pivot[i].dir)
							{turn_test = false;}
						
						//2. Is there more than one pivot on the line?  if so don't pivot
						for (j=0; j<pivot_num; j++)
						{
							if(point_on_line(line[k], pivot[j].Get()) && i != j)
								{turn_test = false;}
						}
						
						//3. Is the line currently interseting another line?  ditto
						for (j=0; j<line_num; j++)
						{
							if(line_in_line(line[k], line[j]) && k != j)
								{turn_test = false;}
						}
						
						//get the points of the rotated line for further tests
						v2i pt1 = turn(pivot[i].Get(), line[k].pt1, dir);
						v2i pt2 = turn(pivot[i].Get(), line[k].pt2, dir);
						LINE temp;
						temp.pt1 = pt1;
						temp.pt2 = pt2;
						
						//4. In the processes of pivoting would the line "hit" another line? ditto
						for (j=0; j<line_num; j++)
						{							
							if ((in_arc_test(pivot[i].Get(), line[k].pt1, temp.pt1, line[j].pt1, dir) && j!=k) ||
									(in_arc_test(pivot[i].Get(), line[k].pt2, temp.pt2, line[j].pt1, dir) && j!=k) ||
									(in_arc_test(pivot[i].Get(), line[k].pt1, temp.pt1, line[j].pt2, dir) && j!=k) ||
									(in_arc_test(pivot[i].Get(), line[k].pt2, temp.pt2, line[j].pt2, dir) && j!=k))
									{
										turn_test = false;
									}						
						}
						
						//5. Will the line pivot out of bounds? ditto
						if(!check_bound(pt1) || !check_bound(pt2))
						{
							{turn_test = false;}
						}
						
						//6. Will line share more than one point with another line? ditto
						for (j=0; j<line_num; j++)
						{
							if(((point_on_middle_line(line[j], temp.pt1) && point_on_middle_line(temp, line[j].pt1)) ||
								 (point_on_middle_line(line[j], temp.pt1) && point_on_middle_line(temp, line[j].pt2)) ||
								 (point_on_middle_line(line[j], temp.pt2) && point_on_middle_line(temp, line[j].pt1)) || 
								 (point_on_middle_line(line[j], temp.pt2) && point_on_middle_line(temp, line[j].pt2))) && 
								 j != k)
								{turn_test = false;}
						}
						
						
						//7. Will the line be interseting another line (other than an end point)? ditto
						for (j=0; j<line_num; j++)
						{
							if(line_in_line(temp, line[j]) && k != j)
								{turn_test = false;}
						}
						
												
						//if all tests past then set pivot to 1 in the line.
						//at the most we'll only be pivoting one line
						if(turn_test)
							{line[k].pivot=true;}
					}
				}
				
				//was thinking here would actually turn the lines and pivot
				for (k=0; k<line_num; k++)
				{
					if(line[k].pivot)
					{
						if(turn_pivot)
						{
							(pivot[cursor.pivot_id].dir == 0) ? (pivot[cursor.pivot_id].dir = 1) : (pivot[cursor.pivot_id].dir = 0);
							turn_pivot = false;
						}
						line[k].pivot = false;
						line[k].pt1 = turn(pivot[cursor.pivot_id].Get(), line[k].pt1, dir);
						line[k].pt2 = turn(pivot[cursor.pivot_id].Get(), line[k].pt2, dir);
					}
				}
			}
		}
	}
}

int pivots_game::request_pick_drop_pivot(void)
{
	//check to see if you are on a pivot
	//if you are on a pivot note which pivot it is
	if (cursor.grasp==1)
	{
		int i;
		cursor.grasp=0;
		
		//make sure you are not dropping the pivot on another pivot 
		for (i = 0; i < pivot_num; i++)
		{
			if(cursor.Get()==pivot[i].Get() && i != cursor.pivot_id)
			{
				cursor.grasp=1;
			}
		}	
	}
	else if (cursor.grasp==0)
	{
		//cycle through to see if cursor is on a pivot
		//if he is on a pivot, set grasp equal to 1, and pivot_id to the correct pivot
		int i;
		for (i = 0; i < pivot_num; i++)
		{
			if(cursor.Get()==pivot[i].Get())
			{
				cursor.grasp = 1;
				cursor.pivot_id = i;
			}
		}
	}
}

v2i pivots_game::get_bound()
{
	return bound.Get();
}

v2i pivots_game::pos_start()
{
	return start.Get();
}

v2i pivots_game::pos_end()
{
	return end.Get();
}

v2i pivots_game::pos_cursor()
{
	return cursor.Get();
}

int pivots_game::grasp_cursor()
{
	return cursor.grasp;
}

int pivots_game::num_pivot()
{
	return pivot_num;
}

v2i pivots_game::pos_pivot(int i)
{
	return pivot[i].Get();
}

int pivots_game::dir_pivot(int i)
{
	return pivot[i].dir;
}

int pivots_game::num_line()
{
	return line_num;
}

v2i pivots_game::pos_line_pt1(int i)
{
	return line[i].pt1.Get();
}

v2i pivots_game::pos_line_pt2(int i)
{
	return line[i].pt2.Get();
}

bool pivots_game::declare_win(void)
{
	if(cursor == end)
		{return true;}
	else
		{return false;}
}

//function tests whether a given point is on a given line segment.
// you would think there is a more efficent way...
// I think I thought of a better way:
//make a function that will take to line points and turn the segment into a string.
//then you can use the numbers from the string to test the line.  the first number in the 
//string will have to be the length of the line, then after that the differents points in the line.
bool pivots_game::point_on_line(LINE _line, v2i _p)
{
	int i;
	bool test = false;
	if(_line.pt1.y == _line.pt2.y)
	{
		int sign = fsign(_line.pt2.x - _line.pt1.x);
		
		if(sign == 1)
		{
			for(i=_line.pt1.x; i<=_line.pt2.x; i += sign)
			{
				if(i == _p.x && _line.pt1.y == _p.y)
					{test = true;}
			}
		}
		else if (sign == -1)
		{
			for(i=_line.pt1.x; i>=_line.pt2.x; i += sign)
			{
				if(i == _p.x && _line.pt1.y == _p.y)
					{test = true;}
			}
		}
	}
	else if (_line.pt1.x == _line.pt2.x)
	{
		int sign = fsign(_line.pt2.y - _line.pt1.y);
		if(sign == 1)
		{
			for(i=_line.pt1.y; i<=_line.pt2.y; i += sign)
			{
				if(i == _p.y && _line.pt1.x == _p.x)
					{test = true;}
			}
			
		}
		else if (sign == -1)
		{
			for(i=_line.pt1.y; i>=_line.pt2.y; i += sign)
			{
				if(i == _p.y && _line.pt1.x == _p.x)
					{test = true;}
			}	
		}
	}
	return test;
}

//function tests whether a given point is on a given line segment.
// different than above because it doesn't include end points
bool pivots_game::point_on_middle_line(LINE _line, v2i _p)
{
	int i;
	bool test = false;
	if(_line.pt1.y == _line.pt2.y)
	{
		int sign = fsign(_line.pt2.x - _line.pt1.x);
		
		if(sign == 1)
		{
			for(i=_line.pt1.x+1; i<_line.pt2.x; i += sign)
			{
				if(i == _p.x && _line.pt1.y == _p.y)
					{test = true;}
			}
		}
		else if (sign == -1)
		{
			for(i=_line.pt1.x-1; i>_line.pt2.x; i += sign)
			{
				if(i == _p.x && _line.pt1.y == _p.y)
					{test = true;}
			}
		}
	}
	else if (_line.pt1.x == _line.pt2.x)
	{
		int sign = fsign(_line.pt2.y - _line.pt1.y);
		if(sign == 1)
		{
			for(i=_line.pt1.y+1; i<_line.pt2.y; i += sign)
			{
				if(i == _p.y && _line.pt1.x == _p.x)
					{test = true;}
			}
			
		}
		else if (sign == -1)
		{
			for(i=_line.pt1.y-1; i>_line.pt2.y; i += sign)
			{
				if(i == _p.y && _line.pt1.x == _p.x)
					{test = true;}
			}	
		}
	}
	return test;
}

bool pivots_game::check_bound(v2i pt)
{
	if(pt.x >= 0 && pt.y >= 0 && pt.x <= bound.x && pt.y <= bound.y)
		{return true;}
	else
		{return false;}
	
}

int pivots_game::fsign(int i)
{
	if(i>0){return 1;}
	else if(i==0){return 0;}
	else if(i<0){return -1;}
}

//Tests to see if two lines are intersecting in the middle
bool pivots_game::line_in_line(LINE line_1, LINE line_2)
{
	int i;
	bool test = false;
	if(line_1.pt1.y == line_1.pt2.y)
	{
		int sign = fsign(line_1.pt2.x - line_1.pt1.x);
		
		if(sign == 1)
		{
			for(i=line_1.pt1.x+1; i<line_1.pt2.x; i += sign)
			{
				v2i _p (i, line_1.pt1.y);
				if(point_on_middle_line(line_2, _p))
					{test = true;}
			}
		}
		else if (sign == -1)
		{
			for(i=line_1.pt1.x-1; i>line_1.pt2.x; i += sign)
			{
				v2i _p (i, line_1.pt1.y);
				if(point_on_middle_line(line_2, _p))
					{test = true;}
			}
		}
	}
	else if (line_1.pt1.x == line_1.pt2.x)
	{
		int sign = fsign(line_1.pt2.y - line_1.pt1.y);
		if(sign == 1)
		{
			for(i=line_1.pt1.y+1; i<line_1.pt2.y; i += sign)
			{
				v2i _p (line_1.pt1.x, i);
				if(point_on_middle_line(line_2, _p))
					{test = true;}
			}
			
		}
		else if (sign == -1)
		{
			for(i=line_1.pt1.y-1; i>line_1.pt2.y; i += sign)
			{
				v2i _p (line_1.pt1.x, i);
				if(point_on_middle_line(line_2, _p))
					{test = true;}
			}	
		}
	}
	return test;
}

v2i pivots_game::turn(v2i ref, v2i pt, int dir)
{
	//set up the 90 degree rotation matrix
	v2i m1 (0 , -1*dir);
	v2i m2 (1*dir , 0);
	//do the matrix transformation:
	//move the point to the origin with resepct to the reference point then rotate it
	v2i rot (v2iDot((pt - ref),m1), v2iDot((pt - ref),m2));
	//then move the point back to where it was with recept to the reference point and return the value
	return ref + rot;
}

bool pivots_game::in_arc_test(v2i ref, v2i origin, v2i turn, v2i test, int dir)
{
	origin -= ref;
	turn -= ref;
	test -= ref;
	
	bool return_val = false;
	if(turn.LengthSquared() > test.LengthSquared())
	{
		double th_origin = atan2(origin.y, origin.x);
		double th_turn = atan2(turn.y, turn.x);
		double th_test = atan2(test.y, test.x);
		
		if (dir == -1 && th_turn + 0.1 > M_PI && th_turn - 0.1 < M_PI)
			{th_turn -= M_PI*2;}
		else if (dir == 1 && th_origin + 0.1 > M_PI && th_origin - 0.1 < M_PI)
			{th_origin -= M_PI*2;}
		
		if(dir == 1 && th_test < th_turn && th_test > th_origin)
			{return_val = true;}
		else if (dir == -1 && th_test > th_turn && th_test < th_origin)
			{return_val = true;}
		
	}
	return return_val;
}
