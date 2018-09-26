
typedef struct Menu_page{
    uint8_t size;
    char* name;
    menu_page* parent;
    menu_page options[6];
} menu_page;

menu_page page_init(const char* name, const menu_page* parent, const uint8_t size, const menu_page* options);
void update_menu_page(menu_page page, enum joystick_direction dir,int position);

int cursor_counter(menu_page page, enum joystick_direction dir, int arrowPos);

menu_page menu_initialize();