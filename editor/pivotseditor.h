#include "../common/game_obj.h"
#define EDIT_MODE 0
#define GAME_MODE 1
#define COMMAND_MODE 2

class pv_editor
{
public:
	pv_editor(void);
	void normal_keys(unsigned char key, int xx, int yy);
	void special_keys(int key, int xx, int yy);
	void mouse_button(int bbutton, int sstate, int xx, int yy);
	void mouse_motion(int xx, int yy);
	void render(int now);
	
protected:
	pv_lvl start;
	pv_lvl workspace;
	pv_lvl backup;	
	int mode;
	int cbuf_num;
	char command_buf[255];
	char command_his[500][255];
	int cmd_his_num;
	int cmd_his_cur;
	char message_buf[255];
	void command_key(unsigned char key);
	void command_specialkeys(int key);
	void sort_commands(void);
	v2i mouse;
};
