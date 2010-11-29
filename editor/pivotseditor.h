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
	void render();
	
protected:
	pv_lvl start;
	pv_lvl workspace;
	pv_lvl backup;	
	int mode;
	int cbuf_num;
	char command_buf[255];
	char message_buf[255];
	v2i mouse;
};
