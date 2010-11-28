#include "../common/game_obj.h"
#define EDIT_MODE 0
#define GAME_MODE 1
#define COMMAND_MODE 2

class pv_editor
{
public:
	void normal_keys(unsigned char key, int xx, int yy);
	void special_keys(int key, int xx, int yy);
	void mouse_button(int bbutton, int sstate, int xx, int yy);
	void mouseMotion(int xx, int yy);
	void print_workspace();
	
protected:
	pv_lvl start;
	pv_lvl workspace;
	pv_lvl backup;	
	int mode;
	char command_buf[255];
	char message_buf[255];
	v2i mouse;
};
