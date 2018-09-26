
struct menu_page{
    uint8_t size;
    char* name;
    struct menu_page* parent;
    char *options[6];
    //struct* menu_page* child;

};

struct menu_page page_init(const char* name, const int* parent, const uint8_t size, const char* options[]);
void update_menu_page(struct menu_page page, enum joystick_direction dir,int position);

int cursor_counter(struct menu_page page, enum joystick_direction dir, int arrowPos);