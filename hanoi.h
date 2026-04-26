/* XingYuan */
#pragma once

/* 将所有const常量定义的头文件包含进来 */
#include "hanoi_const_value.h"
#include <iostream>
using namespace std;

/* ------------------------------------------------------------------------------------------------------

	 本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */

   /* ========== 全局变量声明（仅允许的5个）========== */
extern int total_move_count;                      // 总移动步数
extern int disk_count[3];                         // 每个柱子上的盘子数量
extern int disk_array[3][MAX_LAYER];              // 每个柱子上的盘子编号
extern int move_delay;                            // 延时毫秒数

/* ========== 根据作业要求的共用函数声明 ========== */

// 1. 一个递归函数 - 菜单项 1/2/3/4/8 共用（必须≤15行）
// mode参数：1-基本解，2-步数记录，3-横向显示，4-纵向+横向，8-图形动画
void hanoi_recursive(int n, char from, char to, char via, int mode);

// 2. 一个输入函数 - 菜单项 1/2/3/4/6/7/8 共用
void input_parameters(int mode, char* start_col = NULL, char* target_col = NULL,
	int* disk_num = NULL, int* delay_time = NULL);

// 3. 一个横向输出函数 - 菜单项 3/4/8 共用
void print_horizontal_array(int step, int disk_num, char from, char to, int start_x, int start_y);

// 4. 一个纵向输出函数 - 菜单项 4/8 共用
void print_vertical_array(int start_x, int start_y);

// 5. 一个画柱子函数 - 菜单项 5/6/7/8/9 共用
void draw_three_columns();

// 6. 一个移动函数 - 菜单项 7/8/9 共用
void move_disk(int disk_num, char from, char to, int mode = 8);

/* ========== 菜单项函数声明 ========== */
void hanoi_basic_solution();                      // 菜单项1
void hanoi_basic_with_count();                    // 菜单项2  
void hanoi_array_horizontal();                    // 菜单项3
void hanoi_array_vertical_horizontal();           // 菜单项4
void hanoi_draw_three_columns();                  // 菜单项5
void hanoi_draw_initial_disks();                  // 菜单项6
void hanoi_first_move();                          // 菜单项7
void hanoi_auto_move_demo();                      // 菜单项8
void hanoi_game_version();                        // 菜单项9


/* ========== 辅助函数声明 ========== */
void init_towers(int n, char start_col);          // 初始化柱子
void update_disk_position(int disk_num, char from, char to); // 更新盘子位置
void init_graphics_mode();                        // 初始化图形模式
bool is_valid_move(char from, char to);           // 检查移动合法性
char calculate_middle_column(char src, char dst); // 计算中间柱
void execute_move(char from, char to, int mode, int disk_num);
void format_move_output(int mode, int disk_num, char from, char to);
void clear_disk_at(int col, int row, int start_x);
void draw_disk_at(int col, int row, int disk_num, int start_x);
int get_column_index(char col);
void print_tower_content(int col_index);
void draw_initial_screen(int n, char from, char to, int start_x, int start_y);
void draw_all_disks(int n, char start_col, int start_x, int start_y);
void clear_disk_at_position(int col, int row, int start_x);
void draw_disk_at_position(int col, int row, int disk_num, int start_x);
void update_status_line(int step, int disk_num, char from, char to, int start_x, int start_y, int total_disks);
int get_disk_row_position(int col_index, int disk_index, int start_y, int total_disks);
int get_column_x_pos(char col);
int get_disk_width(int disk_num);
int get_disk_y_pos(int row);
void draw_disk_graphic(int disk_num, char col, int row, bool clear = false);
void draw_disks_on_column(int n, char col);  // 在指定柱子上绘制n个盘子
void move_disk_up(int disk_num, char col, int current_row, int target_top_y);
void move_disk_down(int disk_num, char col, int start_y, int target_row);
void animate_disk_move(int disk_num, char from_col, char to_col);
int get_column_top_y();
int get_column_bottom_y();
void move_disk_horizontal(int disk_num, int from_center, int to_center, int y_pos);
void update_status_bar_for_menu8(int disk_num, char from, char to);
void update_menu8_display(int step, int disk_num, char from, char to, int total_disks);
void clear_disk_at_menu8(int col, int row);
void draw_disk_at_menu8(int col, int row, int disk_num);
bool is_valid_move(char from, char to);
