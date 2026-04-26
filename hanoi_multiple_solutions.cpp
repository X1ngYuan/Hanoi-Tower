/* XingYuan */

#include <iostream>
#include <iomanip>
#include <windows.h>
#include <conio.h>
#include "hanoi.h"
#include "cmd_console_tools.h"
#include "cmd_hdc_tools.h"

using namespace std;

/* ----------------------------------------------------------------------------------

     本文件功能：
    1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
    1、不允许定义外部全局变量（const及#define不在限制范围内）
    2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
    3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
    4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

   /* ========== 静态全局变量定义（仅允许这5个）========== */
static int total_move_count = 0;          // 总移动步数
static int disk_count[3] = { 0, 0, 0 };     // 三个柱子上的盘子数量
static int disk_array[3][MAX_LAYER] = { 0 }; // 三个柱子上的盘子编号
static int move_delay = 0;                // 延时毫秒数

/* ========== 辅助函数：等待按键 ========== */
void wait_for_key()
{
    cout << "\n按回车键继续..." << endl;
    _getch();
}

/* ========== 全局数据访问函数实现 ========== */
int get_total_move_count() { 
    return total_move_count; 
}
void set_total_move_count(int value) { 
    total_move_count = value; 
}
void increment_total_move_count() { 
    total_move_count++; 
}

int get_disk_count(int col) {
    if (col >= 0 && col < 3) 
        return disk_count[col];
    return 0;
}

void set_disk_count(int col, int value) {
    if (col >= 0 && col < 3) 
        disk_count[col] = value;
}

void increment_disk_count(int col) {
    if (col >= 0 && col < 3 && disk_count[col] < MAX_LAYER)
        disk_count[col]++;
}

void decrement_disk_count(int col) {
    if (col >= 0 && col < 3 && disk_count[col] > 0)
        disk_count[col]--;
}

int get_disk_array(int col, int row) {
    if (col >= 0 && col < 3 && row >= 0 && row < MAX_LAYER)
        return disk_array[col][row];
    return 0;
}

void set_disk_array(int col, int row, int value) {
    if (col >= 0 && col < 3 && row >= 0 && row < MAX_LAYER)
        disk_array[col][row] = value;
}

int get_move_delay() { 
    return move_delay; 
}
void set_move_delay(int value) { 
    move_delay = value; 
}

/* ========== 辅助函数实现 ========== */
void init_towers(int n, char start_col)
{
    set_total_move_count(0);
    for (int i = 0; i < 3; i++) {
        set_disk_count(i, 0);
        for (int j = 0; j < MAX_LAYER; j++) {
            set_disk_array(i, j, 0);
        }
    }

    int col_index;
    switch (start_col) {
        case 'A': col_index = 0; 
            break;
        case 'B': col_index = 1; 
            break;
        case 'C': col_index = 2; 
            break;
        default: col_index = 0; 
            break;
    }

    for (int i = n; i >= 1; i--) {
        set_disk_array(col_index, get_disk_count(col_index), i);
        increment_disk_count(col_index);
    }
}


bool is_valid_move(char from, char to)
{
    // 检查柱子编号是否有效
    if (from < 'A' || from > 'C' || to < 'A' || to > 'C') {
        return false;
    }

    // 源柱和目标柱不能相同
    if (from == to) {
        return false;
    }

    int from_index = get_column_index(from);
    int to_index = get_column_index(to);

    // 检查源柱是否为空
    if (get_disk_count(from_index) == 0) {
        return false;
    }

    // 获取源柱最上面的盘子
    int from_count = get_disk_count(from_index);
    int top_disk_from = get_disk_array(from_index, from_count - 1);

    // 获取目标柱最上面的盘子（如果目标柱不为空）
    if (get_disk_count(to_index) > 0) {
        int to_count = get_disk_count(to_index);
        int top_disk_to = get_disk_array(to_index, to_count - 1);

        // 检查是否大盘压小盘
        if (top_disk_from > top_disk_to) {
            return false;
        }
    }

    return true;
}


/* ========== 菜单项函数实现（框架）========== */
char calculate_middle_column(char src, char dst)
{
    if ((src == 'A' && dst == 'B') || (src == 'B' && dst == 'A')) {
        return 'C';
    }
    else if ((src == 'A' && dst == 'C') || (src == 'C' && dst == 'A')) {
        return 'B';
    }
    else {
        return 'A';
    }
}

void hanoi_basic_solution()
{
    char start_col, target_col;
    int disk_num;

    input_parameters(1, &start_col, &target_col, &disk_num, NULL);

    char middle_col = calculate_middle_column(start_col, target_col);


    set_total_move_count(0);
    hanoi_recursive(disk_num, start_col, target_col, middle_col, 1);

    wait_for_key();
}

void hanoi_basic_with_count()
{
    char start_col, target_col;
    int disk_num;

    input_parameters(2, &start_col, &target_col, &disk_num, NULL);

    char middle_col = calculate_middle_column(start_col, target_col);

    set_total_move_count(0);
    hanoi_recursive(disk_num, start_col, target_col, middle_col, 2);
    wait_for_key();
}

void hanoi_array_horizontal()
{
    char start_col, target_col;
    int disk_num;



    input_parameters(3, &start_col, &target_col, &disk_num, NULL);

    init_towers(disk_num, start_col);

    char middle_col = calculate_middle_column(start_col, target_col);

    print_horizontal_array(0, 0, ' ', ' ', 0, 0);

    set_total_move_count(0);



    hanoi_recursive(disk_num, start_col, target_col, middle_col, 3);
    wait_for_key();
}



void hanoi_array_vertical_horizontal()
{
    char start_col, target_col;
    int disk_num, delay_time;


    // 获取输入参数（模式4）
    input_parameters(4, &start_col, &target_col, &disk_num, &delay_time);
    set_move_delay(delay_time);

    // 初始化柱子
    init_towers(disk_num, start_col);

    // 计算中间柱子
    char middle_col = calculate_middle_column(start_col, target_col);

    // 绘制初始界面（只清屏一次）
    draw_initial_screen(disk_num, start_col, target_col, MenuItem4_Start_X, MenuItem4_Start_Y);

    // 绘制所有初始盘子
    draw_all_disks(disk_num, start_col, MenuItem4_Start_X, MenuItem4_Start_Y);

    // 重置步数计数器
    set_total_move_count(0);

 
    if (get_move_delay() == 0) {
        cct_gotoxy(0, MenuItem4_Start_Y + disk_num + 10);
        cout << "按任意键开始单步演示...";
        _getch();
    }

    // 执行递归（模式4：纵向+横向显示）
    hanoi_recursive(disk_num, start_col, target_col, middle_col, 4);

    // 显示完成信息
    cct_gotoxy(0, MenuItem4_Start_Y + disk_num + 12);
    cout << "\n移动完成！总步数：" << get_total_move_count() << endl;

    wait_for_key();
}

/* ========== 菜单项5：图形解-预备-画三个圆柱 ========== */
void hanoi_draw_three_columns()
{

    // 初始化图形模式
    init_graphics_mode();
    cct_gotoxy(Status_Line_X, Status_Line_Y);

    // 绘制三个圆柱（按顺序，每个步骤都有延时）
    draw_three_columns();

    // 释放图形资源
    hdc_release();

    wait_for_key();

}

/* ========== 菜单项6：图形解-预备-在起始柱上画n个盘子 ========== */
void hanoi_draw_initial_disks() {
    char start_col, target_col;
    int disk_num;
    int dummy_delay;

    input_parameters(6, &start_col, &target_col, &disk_num, &dummy_delay);

    init_graphics_mode();

    // 状态栏输出
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    for (int i = 0; i < 120; i++) cout << " ";
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << start_col << " 移动到 " << target_col
        << "，共 " << disk_num << " 层";

    draw_three_columns();

    // 使用draw_disks_on_column函数
    draw_disks_on_column(disk_num, start_col);

    hdc_release();
    wait_for_key();
}
/* ========== 菜单项7：图形解-预备-第一次移动 ========== */
void hanoi_first_move() {
    char start_col, target_col;
    int disk_num;
    int delay_time;

    // 获取输入参数（模式7）
    input_parameters(7, &start_col, &target_col, &disk_num, &delay_time);
    set_move_delay(delay_time);

    // 初始化图形模式
    init_graphics_mode();

    // 在状态栏输出信息
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    for (int i = 0; i < 120; i++) cout << " ";
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << start_col << " 移动到 " << target_col
        << "，共 " << disk_num << " 层，延时设置为 " << get_move_delay() << "ms";

    // 如果是单步演示，显示提示
    if (get_move_delay() == 0) {
        cct_gotoxy(Status_Line_X, Status_Line_Y + 1);
        cout << "单步演示模式：按任意键继续每一步...";
    }

    // 绘制三个圆柱
    draw_three_columns();

    // 初始化柱子，在起始柱上放置盘子
    init_towers(disk_num, start_col);

    // 绘制所有盘子
    for (int i = 0; i < disk_num; i++) {
        int current_disk = disk_num - i;
        draw_disk_graphic(current_disk, start_col, i, false);
        Sleep(HDC_Init_Delay);
    }

    // 计算第一次移动的目标柱
    char first_move_dest;
    if (disk_num == 1) {
        first_move_dest = target_col;
    }
    else {
        first_move_dest = calculate_middle_column(start_col, target_col);
    }

    // 更新状态栏显示移动信息
    cct_gotoxy(Status_Line_X, Status_Line_Y + 2);
    for (int i = 0; i < 120; i++) cout << " ";
    cct_gotoxy(Status_Line_X, Status_Line_Y + 2);


    // 使用共用的move_disk函数执行移动（模式7）
    move_disk(1, start_col, first_move_dest, 7);

    // 更新状态栏显示完成
    cct_gotoxy(Status_Line_X, Status_Line_Y + 3);
    for (int i = 0; i < 120; i++) cout << " ";
    cct_gotoxy(Status_Line_X, Status_Line_Y + 3);


    // 释放图形资源
    hdc_release();

    wait_for_key();
}

void hanoi_auto_move_demo() {
    char start_col, target_col;
    int disk_num, delay_time;

    // 获取输入参数（模式8）
    input_parameters(8, &start_col, &target_col, &disk_num, &delay_time);
    set_move_delay(delay_time);

    // 初始化图形模式（先初始化，但先不画图形）
    init_graphics_mode();

    // 在状态栏输出初始信息
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    for (int i = 0; i < 120; i++) cout << " ";
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << start_col << " 移动到 " << target_col
        << "，共 " << disk_num << " 层，延时设置为 " << get_move_delay() << "ms（前7步，后面自动变为0ms）";

    // ========== 第一步：先绘制字符显示部分 ==========
    // 使用MenuItem8_Start_X, MenuItem8_Start_Y常量
    int menu8_start_x = MenuItem8_Start_X;
    int menu8_start_y = MenuItem8_Start_Y;

    // 初始化柱子数据
    init_towers(disk_num, start_col);

    // 绘制字符版的初始界面
    draw_initial_screen(disk_num, start_col, target_col, menu8_start_x, menu8_start_y);

    // 绘制所有初始盘子（字符版）
    draw_all_disks(disk_num, start_col, menu8_start_x, menu8_start_y);

    // ========== 第二步：再绘制图形部分 ==========
    // 绘制三个圆柱
    draw_three_columns();

    // 绘制所有初始盘子（图形版）
    for (int i = 0; i < disk_num; i++) {
        int current_disk = disk_num - i;
        draw_disk_graphic(current_disk, start_col, i, false);
        Sleep(HDC_Init_Delay);
    }

    // 计算中间柱子
    char middle_col = calculate_middle_column(start_col, target_col);

    // 重置步数计数器
    set_total_move_count(0);

    // 执行递归（模式8：图形动画 + 字符显示）
    hanoi_recursive(disk_num, start_col, target_col, middle_col, 8);

    // 显示完成信息
    cct_gotoxy(menu8_start_x, menu8_start_y + disk_num + 12);

    // 释放图形资源
    hdc_release();

    wait_for_key();
}

void hanoi_game_version() {
    char start_col, target_col;
    int disk_num, delay_time;

    // 获取输入参数（模式9）- 现在会正确获取起始柱、目标柱、盘子数和延时
    input_parameters(9, &start_col, &target_col, &disk_num, &delay_time);
    set_move_delay(delay_time);

    // 初始化图形模式
    init_graphics_mode();

    // 在状态栏输出初始信息
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    for (int i = 0; i < 120; i++) cout << " ";
    cct_gotoxy(Status_Line_X, Status_Line_Y);
    cout << "从 " << start_col << " 移动到 " << target_col
        << "，共 " << disk_num << " 层，延时设置为 " << get_move_delay() << "ms";

    // 如果是单步演示，显示提示
    if (get_move_delay() == 0) {
        cct_gotoxy(Status_Line_X, Status_Line_Y + 1);
        cout << "单步演示模式：按任意键继续每一步...";
    }

    // ========== 第一步：先绘制字符显示部分 ==========
    // 使用MenuItem9_Start_X, MenuItem9_Start_Y常量
    int menu9_start_x = MenuItem9_Start_X;
    int menu9_start_y = MenuItem9_Start_Y;

    // 初始化柱子数据
    init_towers(disk_num, start_col);

    // 绘制字符版的初始界面
    draw_initial_screen(disk_num, start_col, target_col, menu9_start_x, menu9_start_y);

    // 绘制所有初始盘子（字符版）
    draw_all_disks(disk_num, start_col, menu9_start_x, menu9_start_y);

    // ========== 第二步：再绘制图形部分 ==========
    // 绘制三个圆柱
    draw_three_columns();

    // 绘制所有初始盘子（图形版）
    for (int i = 0; i < disk_num; i++) {
        int current_disk = disk_num - i;
        draw_disk_graphic(current_disk, start_col, i, false);
        Sleep(HDC_Init_Delay);
    }

    // 重置步数计数器
    set_total_move_count(0);

    // 游戏主循环
    bool game_running = true;
    bool game_won = false;

    // 输入提示行位置（放在初始横向数组的上面一行）
    int prompt_x = menu9_start_x;
    int prompt_y = menu9_start_y - 1;  // 横向数组的上面一行

    // 用于存储用户输入的字符数组
    const int MAX_INPUT = 10;
    char user_input[MAX_INPUT];

    while (game_running && !game_won) {
        // 显示输入提示
        cct_gotoxy(prompt_x, prompt_y);
        cout << "请输入移动的柱号(命令形式：AC=A顶端的盘子移动到C，Q=退出): ";

        // 获取用户输入（使用字符数组，不用string）
        cin.getline(user_input, MAX_INPUT);

        // 检查输入流状态，清除可能的错误
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
        }

        // 清除输入提示行（清空用户输入的内容）
        cct_gotoxy(prompt_x, prompt_y);
        for (int i = 0; i < 120; i++) cout << " ";

        // 处理用户输入
        int input_length = 0;
        while (user_input[input_length] != '\0' && input_length < MAX_INPUT) {
            input_length++;
        }

        if (input_length == 1) {
            // 单字符命令
            char cmd = user_input[0];
            if (cmd == 'Q' || cmd == 'q') {
                // 退出游戏
                game_running = false;
                cct_gotoxy(prompt_x, prompt_y);
                cout << "游戏退出！";
                continue;
            }
            else {
                // 无效命令
                cct_gotoxy(prompt_x, prompt_y);
                cout << "无效命令！请输入两个字母或Q退出。";
                continue;
            }
        }
        else if (input_length == 2) {
            // 两个字符命令
            char from_col = user_input[0];
            char to_col = user_input[1];

            // 转换为大写
            if (from_col >= 'a' && from_col <= 'z') {
                from_col = from_col - 'a' + 'A';
            }
            if (to_col >= 'a' && to_col <= 'z') {
                to_col = to_col - 'a' + 'A';
            }

            // 检查命令格式
            if (from_col < 'A' || from_col > 'C' || to_col < 'A' || to_col > 'C') {
                cct_gotoxy(prompt_x, prompt_y);
                cout << "无效柱号！请输入A-C之间的字母。";
                continue;
            }

            // 检查移动是否合法
            if (!is_valid_move(from_col, to_col)) {
                cct_gotoxy(prompt_x, prompt_y);
                cout << "非法移动！请检查规则：1)源柱不能为空 2)不能大盘压小盘";
                continue;
            }

            // 获取要移动的盘子编号（源柱最上面的盘子）
            int from_index = get_column_index(from_col);
            int from_count = get_disk_count(from_index);
            int disk_to_move = get_disk_array(from_index, from_count - 1);

            // 执行移动
            // 1. 更新字符显示（先清除源位置）
            int total_disks = 0;
            for (int i = 0; i < 3; i++) {
                total_disks += get_disk_count(i);
            }
            int from_row = get_disk_row_position(from_index, from_count - 1, menu9_start_y, total_disks);
            clear_disk_at_position(from_index, from_row, menu9_start_x);

            // 2. 执行图形动画
            move_disk(disk_to_move, from_col, to_col, 9);

            // 3. 更新数据位置
            update_disk_position(disk_to_move, from_col, to_col);
            increment_total_move_count();

            // 4. 更新字符显示（绘制目标位置）
            int to_index = get_column_index(to_col);
            int to_count = get_disk_count(to_index);
            int to_row = get_disk_row_position(to_index, to_count - 1, menu9_start_y, total_disks);
            draw_disk_at_position(to_index, to_row, disk_to_move, menu9_start_x);

            // 5. 更新状态显示行
            update_status_line(get_total_move_count(), disk_to_move, from_col, to_col,
                menu9_start_x, menu9_start_y, total_disks);

            // 检查是否赢得游戏（所有盘子都在目标柱上）
            int target_index = get_column_index(target_col);
            if (get_disk_count(target_index) == disk_num) {
                // 检查是否按顺序排列（1号在最上面，disk_num号在最下面）
                bool correct_order = true;
                for (int i = 0; i < disk_num; i++) {
                    if (get_disk_array(target_index, i) != (disk_num - i)) {
                        correct_order = false;
                        break;
                    }
                }

                if (correct_order) {
                    game_won = true;
                    // 显示游戏胜利信息
                    cct_gotoxy(prompt_x, prompt_y);
                    cout << "恭喜！游戏胜利！总步数：" << get_total_move_count() << "步";

                    // 在完成信息下面再显示一行
                    cct_gotoxy(prompt_x, prompt_y + 1);
                    cout << "按任意键返回主菜单...";
                    _getch();
                }
            }
        }
        else {
            // 无效输入长度
            cct_gotoxy(prompt_x, prompt_y);
            cout << "无效输入！请输入两个字母或Q退出。";
        }
    }

    // 释放图形资源
    hdc_release();

    wait_for_key();
}

/* ========== 共用函数实现（框架）========== */
void hanoi_recursive(int n, char from, char to, char via, int mode)
{
    if (n == 1) {
        execute_move(from, to, mode, 1);  // 执行单步移动
    }
    else {
        hanoi_recursive(n - 1, from, via, to, mode);
        execute_move(from, to, mode, n);  // 移动第n个盘子
        hanoi_recursive(n - 1, via, to, from, mode);
    }
}

void execute_move(char from, char to, int mode, int disk_num)
{
    switch (mode) {
        case 1: // 基本解
            format_move_output(1, disk_num, from, to);
            break;

        case 2: // 步数记录
            increment_total_move_count();
            format_move_output(2, disk_num, from, to);
            break;

        case 3: // 横向数组显示
            update_disk_position(disk_num, from, to);
            increment_total_move_count();
            print_horizontal_array(get_total_move_count(), disk_num, from, to, 0, 0);
            break;

        case 4: // 纵向+横向显示（字符图形界面）
        {
            // 获取源柱和目标柱索引
            int from_index = get_column_index(from);
            int to_index = get_column_index(to);

            // 获取总盘子数
            int total_disks = 0;
            for (int i = 0; i < 3; i++) {
                total_disks += get_disk_count(i);
            }

            // 1. 找到要移动的盘子在源柱中的位置（从0开始，0是最底下）
            int from_count = get_disk_count(from_index);
            int disk_position = -1;
            for (int i = 0; i < from_count; i++) {
                if (get_disk_array(from_index, i) == disk_num) {
                    disk_position = i;  // i=0是最底下，i=from_count-1是最上面
                    break;
                }
            }
            if (disk_position == -1)
                return;

            // 2. 计算源位置的行坐标
            int from_row = get_disk_row_position(from_index, disk_position, MenuItem4_Start_Y, total_disks);

            // 3. 延时（动画效果）
            if (get_move_delay() > 0) {
                Sleep(get_move_delay() / 2);
            }
            else if (get_move_delay() == 0) {
                // 按回车单步演示
                _getch();
            }

            // 4. 清除源位置的盘子（动画第一步）
            clear_disk_at_position(from_index, from_row, MenuItem4_Start_X);

            // 5. 更新数据位置（内存中的数组）
            update_disk_position(disk_num, from, to);
            increment_total_move_count();

            // 6. 计算目标位置的行坐标
            int to_count = get_disk_count(to_index);
            // 新盘子放在目标柱的最上面，所以索引是to_count-1
            int to_row = get_disk_row_position(to_index, to_count - 1, MenuItem4_Start_Y, total_disks);

            // 7. 绘制目标位置的盘子（动画第二步）
            draw_disk_at_position(to_index, to_row, disk_num, MenuItem4_Start_X);

            // 8. 更新状态显示行（横向数组）
            update_status_line(get_total_move_count(), disk_num, from, to,
                MenuItem4_Start_X, MenuItem4_Start_Y, total_disks);

            // 9. 延时
            if (get_move_delay() > 0) {
                Sleep(get_move_delay() / 2);
            }
            break;
        }

        case 8: // 图形动画 + 字符显示（菜单项8）
        {
            // 先执行图形动画（有延时），再更新字符显示（无延时但等待动画结束）

            // 获取源柱和目标柱索引
            int from_index = get_column_index(from);
            int to_index = get_column_index(to);

            // 获取总盘子数
            int total_disks = 0;
            for (int i = 0; i < 3; i++) {
                total_disks += get_disk_count(i);
            }

            // 1. 找到要移动的盘子在源柱中的位置
            int from_count = get_disk_count(from_index);
            int disk_position = -1;
            for (int i = 0; i < from_count; i++) {
                if (get_disk_array(from_index, i) == disk_num) {
                    disk_position = i;
                    break;
                }
            }
            if (disk_position == -1)
                return;

            // 2. 计算源位置的行坐标（用于后面字符显示）
            int from_row = get_disk_row_position(from_index, disk_position, MenuItem8_Start_Y, total_disks);

            // 3. 执行图形动画移动（这里有延时控制）
            // 检查当前步数：前7步使用用户设置的延时，后面自动变为0
            int current_step = get_total_move_count() + 1; // 将要执行的步数
            int original_delay = get_move_delay();
            int actual_delay = (current_step <= 7 && original_delay > 0) ? original_delay : 0;

            // 临时设置实际延时
            int temp_delay = get_move_delay();
            set_move_delay(actual_delay);

            // 执行图形动画
            move_disk(disk_num, from, to, 8);

            // 恢复原来的延时设置
            set_move_delay(temp_delay);


            // 4. 图形动画完成后，立即更新数据位置
            update_disk_position(disk_num, from, to);
            increment_total_move_count();

            // 5. 计算目标位置的行坐标
            int to_count = get_disk_count(to_index);
            int to_row = get_disk_row_position(to_index, to_count - 1, MenuItem8_Start_Y, total_disks);

            // 6. 清除源位置的盘子（字符版）- 动画结束后立即更新，无延时
            clear_disk_at_position(from_index, from_row, MenuItem8_Start_X);

            // 7. 绘制目标位置的盘子（字符版）- 动画结束后立即更新，无延时
            draw_disk_at_position(to_index, to_row, disk_num, MenuItem8_Start_X);

            // 8. 更新状态显示行（横向数组）- 动画结束后立即更新，无延时
            update_status_line(get_total_move_count(), disk_num, from, to,
                MenuItem8_Start_X, MenuItem8_Start_Y, total_disks);

            break;
        }
    }
}

void format_move_output(int mode, int disk_num, char from, char to)
{
    if (mode == 1) {
        // 模式1: 1# A---->C
        cout << disk_num << "# " << from << "---->" << to << endl;
    }
    else if (mode == 2) {
        // 模式2: 前面有步数编号
        cout << setw(4) << get_total_move_count() << ": "
            << "(" << disk_num << "# " << from << "-->" << to << ")" << endl;
    }
}

void input_parameters(int mode, char* start_col, char* target_col,
    int* disk_num, int* delay_time)
{
    // 所有模式都需要盘子数（除了模式5）
    if (disk_num && mode != 5) {
        while (true) {
            cout << "请输入汉诺塔的层数(1-" << MAX_LAYER << "): " << endl;
            cin >> *disk_num;
            if (cin.fail() || *disk_num < 1 || *disk_num > MAX_LAYER) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "输入错误！" << endl;
            }
            else {
                break;
            }
        }
    }

    // 模式1-4,6-9需要起始柱 
    if (start_col && (mode <= 4 || (mode >= 6 && mode <= 9))) {
        while (true) {
            cout << "请输入起始柱(A-C): " << endl;
            cin >> *start_col;
            if (*start_col >= 'a' && *start_col <= 'z') {
                *start_col = *start_col - 'a' + 'A';
            }
            if (*start_col >= 'A' && *start_col <= 'C') {
                break;
            }
            cout << "输入错误！" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    // 模式1,2,3,4,6,7,8,9需要目标柱
    if (target_col && (mode == 1 || mode == 2 || mode == 3 || mode == 4 || mode == 6
        || mode == 7 || mode == 8 || mode == 9)) {
        while (true) {
            cout << "请输入目标柱(A-C): " << endl;
            cin >> *target_col;
            if (*target_col >= 'a' && *target_col <= 'z') {
                *target_col = *target_col - 'a' + 'A';
            }
            if (*target_col >= 'A' && *target_col <= 'C') {
                // 检查起始柱和目标柱是否相同（模式1,2,6,7,9需要检查）
                if ((mode == 1 || mode == 2 || mode == 6 || mode == 7 || mode == 9) &&
                    *start_col == *target_col) {
                    cout << "目标柱不能与起始柱相同！" << endl;
                    continue;
                }
                break;
            }
            cout << "输入错误！" << endl;
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }

    // 模式3,4,7,8,9需要延时，模式6不需要延时
    if (delay_time && (mode == 3 || mode == 4 || mode == 7 || mode == 8 || mode == 9)) {
        while (true) {
            // 模式7,8,9使用0-20的范围
            if (mode == 7 || mode == 8 || mode == 9) {
                cout << "请输入移动速度(0-20：0-按回车单步演示 1-20:延时1-20ms)" << endl;
            }
            else {
                cout << "请输入移动速度(0-200：0-按回车单步演示 1-200:延时1-200ms)" << endl;
            }

            cin >> *delay_time;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "输入错误！请输入整数。" << endl;
                continue;
            }

            // 根据模式检查不同的范围
            if (mode == 7 || mode == 8 || mode == 9) {
                if (*delay_time < 0 || *delay_time > 20) {
                    cout << "输入错误！请输入0-20之间的整数。" << endl;
                    cin.ignore(10000, '\n');
                    continue;
                }
            }
            else {
                if (*delay_time < 0 || *delay_time > 200) {
                    cout << "输入错误！请输入0-200之间的整数。" << endl;
                    cin.ignore(10000, '\n');
                    continue;
                }
            }

            set_move_delay(*delay_time);
            break;
        }
    }
}

void print_horizontal_array(int step, int disk_num, char from, char to, int start_x, int start_y)
{


    if (step == 0) {
        cout << "初始:                ";
    }
    else {
        cout << "第" << setw(4) << step << " 步(" << setw(2) << disk_num << "): "
            << from << "-->" << to << " ";
    }

    // 打印A柱
    cout << "A:";
    print_tower_content(0);

    // 打印B柱
    cout << "B:";
    print_tower_content(1);

    // 打印C柱
    cout << "C:";
    print_tower_content(2);

    cout << endl;
}

/* ========== 清除指定位置的盘子（纵向显示用）========== */
void clear_disk_at(int col, int row, int start_x)
{
    int x_pos;
    switch (col) {
        case 0: 
            x_pos = start_x + Underpan_A_X_OFFSET; 
            break;
        case 1: 
            x_pos = start_x + Underpan_A_X_OFFSET + Underpan_Distance; 
            break;
        case 2: 
            x_pos = start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance;
            break;
        default:
            return;
    }

    cct_gotoxy(x_pos, row);
    cout << "    ";  // 用4个空格清除
}

/* ========== 绘制指定位置的盘子（纵向显示用）========== */
void draw_disk_at(int col, int row, int disk_num, int start_x)
{
    int x_pos;
    switch (col) {
        case 0:
            x_pos = start_x + Underpan_A_X_OFFSET;
            break;
        case 1: 
            x_pos = start_x + Underpan_A_X_OFFSET + Underpan_Distance; 
            break;
        case 2: 
            x_pos = start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance;
            break;
        default:
            return;
    }

    cct_gotoxy(x_pos-3, row);
    cout << setw(4) << disk_num; // 4字符宽度显示
}

/* ========== 纵向输出函数（菜单项4/8共用）========== */
void print_vertical_array(int start_x, int start_y)
{
    int base_row = start_y + Underpan_A_Y_OFFSET - 1;

    // 绘制底座
    cct_gotoxy(start_x + Underpan_A_X_OFFSET - 2, base_row);
    for (int i=0;i<  2 * Underpan_Distance + 5; i++)
        cout << "=";

    // 绘制柱子标签 A B C
    cct_gotoxy(start_x + Underpan_A_X_OFFSET , base_row + 1);
    cout << "A";
    cct_gotoxy(start_x + Underpan_A_X_OFFSET + Underpan_Distance , base_row + 1);
    cout << "B";
    cct_gotoxy(start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance , base_row + 1); 
    cout << "C";

    // 绘制所有盘子（从下往上）
    for (int col = 0; col < 3; col++) {
        int count = get_disk_count(col);
        for (int i = 0; i < count; i++) {
            int disk_num = get_disk_array(col, i);
            int row = base_row - 1 - i; // 从底座往上数

            // 根据列确定X坐标
            int x_pos;
            switch (col) {
                case 0: 
                    x_pos = start_x + Underpan_A_X_OFFSET; 
                    break;
                case 1:
                    x_pos = start_x + Underpan_A_X_OFFSET + Underpan_Distance; 
                    break;
                case 2:
                    x_pos = start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance; 
                    break;
            }

            cct_gotoxy(x_pos-3, row);
            cout << setw(4) << disk_num; // 4字符宽度显示盘子编号
        }
    }
}

/* ========== 辅助函数：获取柱子索引 ========== */
int get_column_index(char col)
{
    switch (col) {
        case 'A':
            return 0;
        case 'B': 
            return 1;
        case 'C': 
            return 2;
        default: 
            return 0;
    }
}

int calculate_tower_width(int col_index)
{
    int width = 0;
    int count = get_disk_count(col_index);

    for (int i = 0; i < count; i++) {
        int disk_num = get_disk_array(col_index, i);
        if (disk_num >= 10) {
            width += 3; // 两位数 + 空格
        }
        else {
            width += 2; // 一位数 + 空格
        }
    }

    if (count > 0) {
        width--; // 最后一个数字后面不需要空格
    }

    return width;
}

void print_tower_content(int col_index)
{
    int count = get_disk_count(col_index);

    if (count == 0) {
        // 空柱子：填充21个空格
        for (int i = 0; i < 21; i++) {
            cout << " ";
        }
        return;
    }
    int current_width = calculate_tower_width(col_index);
    // 从栈底到栈顶打印（即从大到小）
    for (int i = 0; i < count; i++) {
        int disk_num = get_disk_array(col_index, i);

        if (i == 0) {
            // 第一个数字：如果是两位数就不加空格，一位数就加空格
            if (disk_num >= 10) {
                cout << disk_num;
            }
            else {
                cout << " " << disk_num;
                current_width++; // 个位数前面多了一个空格，所以实际宽度+1
            }
        }
        else {
            // 后面的数字都加空格
            cout << " " << disk_num;
        }
    }

    // 填充空格使每个柱子占用固定宽度（21个字符）

    for (int i = current_width; i < 21; i++) {
        cout << " ";
    }
}

void update_disk_position(int disk_num, char from, char to)
{
    int from_index = get_column_index(from);
    int to_index = get_column_index(to);

    int from_count = get_disk_count(from_index);

    // 找到要移动的盘子（从数组末尾开始找，因为栈顶在末尾）
    for (int i = from_count - 1; i >= 0; i--) {
        if (get_disk_array(from_index, i) == disk_num) {
            // 从源柱子移除
            for (int j = i; j < from_count - 1; j++) {
                set_disk_array(from_index, j, get_disk_array(from_index, j + 1));
            }
            set_disk_array(from_index, from_count - 1, 0);
            decrement_disk_count(from_index);

            // 添加到目标柱子顶部
            int to_count = get_disk_count(to_index);
            set_disk_array(to_index, to_count, disk_num);
            increment_disk_count(to_index);
            break;
        }
    }
}

/* ========== 绘制汉诺塔初始界面 ========== */
void draw_initial_screen(int n, char from, char to, int start_x, int start_y)
{
    cct_cls();  // 只初始时清屏一次

    // 显示标题信息
    cct_gotoxy(start_x, start_y);
    cout << "从 " << from << " 移动到 " << to << "，共 " << n << " 层，";
    cout << "延时设置为 " << get_move_delay() << "ms";

    // 绘制空白的汉诺塔框架
    // 圆盘开始的行



    // 绘制底座
    int base_row = start_y + Underpan_A_Y_OFFSET - 1;
    int disk_row_start = base_row - n;

    cct_gotoxy(start_x + Underpan_A_X_OFFSET - 2, base_row);
    for (int i = 0; i < 2 * Underpan_Distance + 5; i++)
        cout << "=";

    // 绘制柱子标签
    cct_gotoxy(start_x + Underpan_A_X_OFFSET , base_row + 1);
    cout << "A";
    cct_gotoxy(start_x + Underpan_A_X_OFFSET + Underpan_Distance , base_row + 1);
    cout << "B";
    cct_gotoxy(start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance , base_row + 1); 
    cout << "C";

    // 显示初始状态行（横向数组）
    cct_gotoxy(start_x, base_row + 3);
    cout << "初始:                ";

    // 显示内部数组值（初始状态）- 使用你的print_tower_content
    cout << "A:";
    print_tower_content(0);
    cout << "B:";
    print_tower_content(1);
    cout << "C:";
    print_tower_content(2);
}

/* ========== 绘制所有盘子（纵向）========== */
void draw_all_disks(int n, char start_col, int start_x, int start_y)
{

    int base_row = start_y + Underpan_A_Y_OFFSET - 1;
    int disk_row_start = base_row - n;

    // 根据起始柱绘制所有盘子
    int col_index;
    switch (start_col) {
        case 'A':
            col_index = 0; 
            break;
        case 'B': 
            col_index = 1; 
            break;
        case 'C': 
            col_index = 2; 
            break;
        default:
            col_index = 0;
    }

    // 绘制所有圆盘（从下往上）
    int count = get_disk_count(col_index);
    for (int i = 0; i < count; i++) {
        int disk_num = get_disk_array(col_index, i);
        int current_row = base_row - 1 - i;  // 从底座往上数

        // 根据起始柱位置确定X坐标
        int x_pos;
        switch (col_index) {
            case 0:
                x_pos = start_x + Underpan_A_X_OFFSET;
                break;
            case 1: 
                x_pos = start_x + Underpan_A_X_OFFSET + Underpan_Distance; 
                break;
            case 2:
                x_pos = start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance;
                break;
        }

        cct_gotoxy(x_pos-3, current_row);
        cout  << setw(4) << disk_num;
    }
}

/* ========== 清除指定位置的盘子 ========== */
void clear_disk_at_position(int col, int row, int start_x)
{
    int x_pos;
    switch (col) {
        case 0:
            x_pos = start_x + Underpan_A_X_OFFSET; 
            break;
        case 1:
            x_pos = start_x + Underpan_A_X_OFFSET + Underpan_Distance;
            break;
        case 2:
            x_pos = start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance;
            break;
    }

    cct_gotoxy(x_pos, row);
    cout << "    ";  // 用4个空格清除
}

/* ========== 绘制指定位置的盘子 ========== */
void draw_disk_at_position(int col, int row, int disk_num, int start_x)
{
    int x_pos;
    switch (col) {
        case 0: 
            x_pos = start_x + Underpan_A_X_OFFSET;
            break;
        case 1: 
            x_pos = start_x + Underpan_A_X_OFFSET + Underpan_Distance; 
            break;
        case 2: 
            x_pos = start_x + Underpan_A_X_OFFSET + 2 * Underpan_Distance; 
            break;
    }

    cct_gotoxy(x_pos-3, row);
    cout  << setw(4) << disk_num;
}

/* ========== 更新状态显示行 ========== */
void update_status_line(int step, int disk_num, char from, char to, int start_x, int start_y, int total_disks)
{
   
    int base_row = start_y + Underpan_A_Y_OFFSET - 1;
   
    cct_gotoxy(start_x, base_row + 3);
    if (step == 0) {
        cout << "初始:                ";
    }
    else {
        cout << "第" << setw(3) << step << "步("
            << disk_num << "#: " << from << "-->" << to << ")  ";
    }

    // 显示内部数组值（使用你的空格逻辑）
    cout << "A:";
    print_tower_content(0);
    cout << "B:";
    print_tower_content(1);
    cout << "C:";
    print_tower_content(2);
}

/* ========== 获取盘子行坐标 ========== */
int get_disk_row_position(int col_index, int disk_index, int start_y, int total_disks)
{
    // disk_index: 0表示最底下的盘子（最大的），count-1表示最上面的盘子（最小的）
    int base_row = start_y + Underpan_A_Y_OFFSET - 1;
    return base_row - 1 - disk_index;
}


/* ========== 初始化图形模式 ========== */
void init_graphics_mode()
{
    // 先清屏字符模式
    cct_cls();

    // 使用常量文件中的背景色初始化图形窗口
    hdc_init(HDC_COLOR[0], HDC_COLOR[0], 120 * 8, 40 * 16);
    hdc_cls();
}

/* ========== 画三个圆柱（菜单项5/6/7/8/9共用）========== */
void draw_three_columns()
{
    // 获取常量参数
    int start_x = HDC_Start_X;                  // A柱左上角X
    int start_y = HDC_Start_Y;                  // A柱左上角Y
    int base_width = HDC_Base_Width;            // w - 立柱宽度
    int base_high = HDC_Base_High;              // h - 盘/底座高度
    int distance = HDC_Underpan_Distance;       // 底盘之间的间隔
    int base_color = HDC_COLOR[MAX_LAYER + 1];  // 基座颜色

    // 底盘宽度和高度
    int underpan_width = 23 * base_width;       // 底盘宽度 = 23w
    int underpan_height = base_high;            // 底盘高度 = h

    // 立柱宽度和高度
    int column_width = base_width;              // 立柱宽度 = w
    int column_height = 12 * base_high;         // 立柱高度 = 12h

    // 立柱在底盘中间：(底盘宽度-立柱宽度)/2
    int column_offset = (underpan_width - column_width) / 2;

    // 底盘顶部Y坐标（底座的上边缘）
    int underpan_top_y = start_y;

    Sleep(HDC_Init_Delay);
    // 1. 画A柱底盘
    hdc_rectangle(start_x, underpan_top_y, underpan_width, underpan_height, base_color);
    Sleep(HDC_Init_Delay);

    // 2. 画B柱底盘（A底盘右边缘 + 间隔）
    int b_underpan_x = start_x + underpan_width + distance;
    hdc_rectangle(b_underpan_x, underpan_top_y, underpan_width, underpan_height, base_color);
    Sleep(HDC_Init_Delay);

    // 3. 画C柱底盘（B底盘右边缘 + 间隔）
    int c_underpan_x = b_underpan_x + underpan_width + distance;
    hdc_rectangle(c_underpan_x, underpan_top_y, underpan_width, underpan_height, base_color);
    Sleep(HDC_Init_Delay);

    // 4. 画A柱立柱（从底座顶部开始向上画）
    // 立柱底部Y = 底座顶部Y
    // 立柱顶部Y = 底座顶部Y - 立柱高度
    hdc_rectangle(start_x + column_offset, underpan_top_y - column_height,
        column_width, column_height, base_color);
    Sleep(HDC_Init_Delay);

    // 5. 画B柱立柱
    hdc_rectangle(b_underpan_x + column_offset, underpan_top_y - column_height,
        column_width, column_height, base_color);
    Sleep(HDC_Init_Delay);

    // 6. 画C柱立柱
    hdc_rectangle(c_underpan_x + column_offset, underpan_top_y - column_height,
        column_width, column_height, base_color);
    Sleep(HDC_Init_Delay);
}

void move_disk(int disk_num, char from, char to, int mode)
{
    if (mode == 7 || mode == 8 || mode == 9) {
        // 图形模式下的移动（带动画）
        animate_disk_move(disk_num, from, to);

        // 注意：对于模式8和9，数据位置在execute_move中更新
        // 对于模式7，需要在这里更新数据位置和步数
        if (mode == 7) {
            update_disk_position(disk_num, from, to);
            increment_total_move_count();
        }
        // 模式9的更新在游戏逻辑中处理
    }
    // 其他模式已经在execute_move中处理
}

/* ========== 获取柱子X坐标 ========== */
int get_column_x_pos(char col)
{
    int start_x = HDC_Start_X;
    int base_width = HDC_Base_Width;
    int distance = HDC_Underpan_Distance;

    int underpan_width = 23 * base_width;
    int column_offset = (underpan_width - base_width) / 2 + HDC_Base_Width / 2;

    switch (col) {
        case 'A': 
            return start_x + column_offset;
        case 'B':
            return start_x + underpan_width + distance + column_offset;
        case 'C': 
            return start_x + 2 * (underpan_width + distance) + column_offset;
        default: 
            return start_x + column_offset;
    }
}

/* ========== 获取盘子宽度 ========== */
int get_disk_width(int disk_num)
{
    return (3 + (disk_num - 1) * 2) * HDC_Base_Width;
}

/* ========== 获取盘子Y坐标 ========== */
int get_disk_y_pos(int row)
{
    // row: 0表示最底下的盘子（在底座上），row越大位置越高
    int start_y = HDC_Start_Y;          // 底座顶部Y坐标
    int base_high = HDC_Base_High;      // 盘子高度

    // 盘子底部Y坐标 = 底座顶部 - (row + 1) × 盘子高度
    // row=0: 第一个盘子在底座上，Y = start_y - base_high
    // row=1: 第二个盘子在第一个盘子上，Y = start_y - 2×base_high
    return start_y - (row + 1) * base_high;
}

/* ========== 绘制单个盘子 ========== */
void draw_disk_graphic(int disk_num, char col, int row, bool clear)
{
    if (disk_num < 1 || disk_num > MAX_LAYER) return;

    int disk_width = get_disk_width(disk_num);    // (disk_num+2) × w
    int disk_height = HDC_Base_High;              // h
    int disk_color = HDC_COLOR[disk_num];         // 盘子颜色

    // 获取柱子中心X坐标
    int column_center_x = get_column_x_pos(col);  // 柱子中心X
    int base_width = HDC_Base_Width;              // w

    // 盘子左边缘X坐标 = 柱子中心 - 盘子宽度/2
    int disk_x = column_center_x - disk_width / 2;

    // 盘子底部Y坐标
    int disk_y = get_disk_y_pos(row);

    if (clear) {
        // 清除：用背景色重画
        hdc_rectangle(disk_x, disk_y, disk_width, disk_height, HDC_COLOR[0]);
        // 恢复被盘子挡住的柱子部分
        hdc_rectangle(column_center_x - base_width / 2, disk_y,
            base_width, disk_height, HDC_COLOR[MAX_LAYER + 1]);
    }
    else {
        // 绘制盘子
        hdc_rectangle(disk_x, disk_y, disk_width, disk_height, disk_color);
    }
}

void draw_disks_on_column(int n, char col) {
    if (n < 1 || n > MAX_LAYER) return;

    // 获取柱子索引
    int col_index = get_column_index(col);

    // 清空当前柱子的盘子数据
    set_disk_count(col_index, 0);

    // 绘制盘子（从下往上，从大到小）
    for (int i = 0; i < n; i++) {
        int disk_num = n - i;  // 从下往上：10, 9, 8, ... 1
        int row = i;           // row=0最底下，row=n-1最上面

        // 记录盘子数据
        set_disk_array(col_index, i, disk_num);
        increment_disk_count(col_index);

        // 绘制盘子图形
        draw_disk_graphic(disk_num, col, row, false);

        // 添加延时，让绘制过程可见
        Sleep(HDC_Init_Delay);
    }
}


/* ========== 动画移动函数：上移盘子 ========== */
void move_disk_up(int disk_num, char col, int current_row, int target_top_y)
{
    int disk_width = get_disk_width(disk_num);
    int disk_height = HDC_Base_High;
    int disk_color = HDC_COLOR[disk_num];
    int column_center_x = get_column_x_pos(col);
    int base_width = HDC_Base_Width;

    // 柱子边界
    int column_left = column_center_x - base_width / 2;
    int column_right = column_left + base_width;
    int column_top_y = get_column_top_y();    // 柱子顶部
    int column_bottom_y = get_column_bottom_y(); // 柱子底部

    // 盘子边界
    int disk_left = column_center_x - disk_width / 2;
    int disk_right = disk_left + disk_width;

    int current_y = get_disk_y_pos(current_row);

    while (current_y > target_top_y) {
        int move_y = HDC_Step_Y;
        if (current_y - move_y < target_top_y) {
            move_y = current_y - target_top_y;
        }

        // 清除下部move_y行的盘子部分
        int clear_y = current_y + disk_height - move_y;
        hdc_rectangle(
            disk_left,
            clear_y,
            disk_width,
            move_y,
            HDC_COLOR[0]
        );

        // 恢复被清除的柱子部分（只在柱子高度范围内）
        // 计算清除区域与柱子重叠的部分
        int clear_bottom = clear_y + move_y;

        // 只处理在柱子高度范围内的部分
        if (clear_y < column_bottom_y) {
            int restore_top = max(clear_y, column_top_y);
            int restore_bottom = min(clear_bottom, column_bottom_y);

            if (restore_bottom > restore_top) {
                int restore_height = restore_bottom - restore_top;
                int restore_y = restore_top;

                // 计算盘子与柱子的水平重叠
                if (disk_left < column_right && disk_right > column_left) {
                    int overlap_left = max(disk_left, column_left);
                    int overlap_right = min(disk_right, column_right);
                    int overlap_width = overlap_right - overlap_left;

                    if (overlap_width > 0) {
                        hdc_rectangle(
                            overlap_left,
                            restore_y,
                            overlap_width,
                            restore_height,
                            HDC_COLOR[MAX_LAYER + 1]
                        );
                    }
                }
            }
        }

        // 绘制上部move_y行的盘子部分
        int draw_y = current_y - move_y;
        hdc_rectangle(
            disk_left,
            draw_y,
            disk_width,
            move_y,
            disk_color
        );

        // 更新当前Y坐标
        current_y -= move_y;

        int current_step = get_total_move_count() + 1; // 将要执行的步数

        if (current_step <= 7) {
            // 延时或单步演示
            if (get_move_delay() > 0) {
                Sleep(get_move_delay());
            }
            else if (get_move_delay() == 0) {
                _getch();
            }
        }

    }
}

void move_disk_horizontal(int disk_num, int from_center, int to_center, int y_pos)
{
    int disk_width = get_disk_width(disk_num);
    int disk_height = HDC_Base_High;
    int disk_color = HDC_COLOR[disk_num];

    // 将柱子中心坐标转换为盘子左边缘坐标
    int from_left = from_center - disk_width / 2;
    int to_left = to_center - disk_width / 2;

    int current_left = from_left;

    while (current_left != to_left) {
        int move_step;

        if (to_left > current_left) {
            // 向右移动
            move_step = HDC_Step_X;
            if (current_left + move_step > to_left) {
                move_step = to_left - current_left;
            }

            // 清除左侧move_step列
            hdc_rectangle(
                current_left,
                y_pos,
                move_step,
                disk_height,
                HDC_COLOR[0]
            );

            // 绘制右侧move_step列
            hdc_rectangle(
                current_left + disk_width,
                y_pos,
                move_step,
                disk_height,
                disk_color
            );

            current_left += move_step;
        }
        else {
            // 向左移动
            move_step = -HDC_Step_X;
            if (current_left + move_step < to_left) {
                move_step = to_left - current_left;
            }

            // 清除右侧|move_step|列
            hdc_rectangle(
                current_left + disk_width + move_step,
                y_pos,
                -move_step,
                disk_height,
                HDC_COLOR[0]
            );

            // 绘制左侧|move_step|列
            hdc_rectangle(
                current_left + move_step,
                y_pos,
                -move_step,
                disk_height,
                disk_color
            );

            current_left += move_step;
        }


        int current_step = get_total_move_count() + 1; // 将要执行的步数

        if (current_step <= 7) {
            // 延时或单步演示
            if (get_move_delay() > 0) {
                Sleep(get_move_delay());
            }
            else if (get_move_delay() == 0) {
                _getch();
            }
        }
    }
}

/* ========== 动画移动函数：下移盘子 ========== */
void move_disk_down(int disk_num, char col, int start_y, int target_row)
{
    int disk_width = get_disk_width(disk_num);
    int disk_height = HDC_Base_High;
    int disk_color = HDC_COLOR[disk_num];
    int column_center_x = get_column_x_pos(col);
    int base_width = HDC_Base_Width;

    // 柱子边界
    int column_left = column_center_x - base_width / 2;
    int column_right = column_left + base_width;
    int column_top_y = get_column_top_y();
    int column_bottom_y = get_column_bottom_y();

    // 盘子边界
    int disk_left = column_center_x - disk_width / 2;
    int disk_right = disk_left + disk_width;

    int target_y = get_disk_y_pos(target_row);
    int current_y = start_y;

    while (current_y < target_y) {
        int move_y = HDC_Step_Y;
        if (current_y + move_y > target_y) {
            move_y = target_y - current_y;
        }

        // 1. 清除上部move_y行的盘子部分
        int clear_top = current_y;
        int clear_bottom = clear_top + move_y;

        hdc_rectangle(
            disk_left,
            clear_top,
            disk_width,
            move_y,
            HDC_COLOR[0]
        );

        // 2. 恢复被清除的柱子部分（只在柱子高度范围内）
        if (clear_top < column_bottom_y) {
            int restore_top = max(clear_top, column_top_y);
            int restore_bottom = min(clear_bottom, column_bottom_y);

            if (restore_bottom > restore_top) {
                int restore_height = restore_bottom - restore_top;
                int restore_y = restore_top;

                // 计算盘子与柱子的水平重叠
                if (disk_left < column_right && disk_right > column_left) {
                    int overlap_left = max(disk_left, column_left);
                    int overlap_right = min(disk_right, column_right);
                    int overlap_width = overlap_right - overlap_left;

                    if (overlap_width > 0) {
                        hdc_rectangle(
                            overlap_left,
                            restore_y,
                            overlap_width,
                            restore_height,
                            HDC_COLOR[MAX_LAYER + 1]
                        );
                    }
                }
            }
        }

        // 3. 绘制下部move_y行的盘子部分
        hdc_rectangle(
            disk_left,
            current_y + disk_height,
            disk_width,
            move_y,
            disk_color
        );

        // 4. 更新当前Y坐标
        current_y += move_y;


        int current_step = get_total_move_count() + 1; // 将要执行的步数

        if (current_step <= 7) {
            // 延时或单步演示
            if (get_move_delay() > 0) {
                Sleep(get_move_delay());
            }
            else if (get_move_delay() == 0) {
                _getch();
            }
        }
    }
}

/* ========== 动画移动一个盘子（完整过程） ========== */
void animate_disk_move(int disk_num, char from_col, char to_col)
{
    // 获取源柱和目标柱的柱子中心X坐标
    int from_center_x = get_column_x_pos(from_col);
    int to_center_x = get_column_x_pos(to_col);

    // 盘子宽度
    int disk_width = get_disk_width(disk_num);

    // 传递柱子中心X坐标
    int from_center = from_center_x;
    int to_center = to_center_x;

    // 获取当前盘子在源柱上的位置
    int from_index = get_column_index(from_col);
    int from_count = get_disk_count(from_index);
    int from_row = -1;

    // 找到要移动的盘子（最上面的盘子）
    for (int i = from_count - 1; i >= 0; i--) {
        if (get_disk_array(from_index, i) == disk_num) {
            from_row = i;
            break;
        }
    }
    if (from_row == -1)
        return;

    // 步骤1：上移到顶部（HDC_Top_Y）
    move_disk_up(disk_num, from_col, from_row, HDC_Top_Y);

    // 步骤2：水平移动到目标柱上方
    int top_y = HDC_Top_Y;
    move_disk_horizontal(disk_num, from_center, to_center, top_y);

    // 步骤3：下移到目标柱的顶部位置
    int to_index = get_column_index(to_col);
    int to_count = get_disk_count(to_index);
    // 目标位置：如果目标柱有盘子，就放在最上面；否则放在底座上（row=0）
    int target_row = (to_count > 0) ? to_count : 0;
    move_disk_down(disk_num, to_col, top_y, target_row);

    // 注意：数据位置更新在调用者（move_disk）中处理，这里不重复更新
}

/* ========== 获取柱子顶部Y坐标 ========== */
int get_column_top_y()
{
    // 柱子顶部Y = 底座顶部Y - 柱子高度
    // 柱子高度 = 12 * base_high
    return HDC_Start_Y - 12 * HDC_Base_High;
}

/* ========== 获取柱子底部Y坐标 ========== */
int get_column_bottom_y()
{
    // 柱子底部Y = 底座顶部Y
    return HDC_Start_Y;
}

void update_status_bar_for_menu8(int disk_num, char from, char to)
{
    // 在状态栏下方显示移动信息
    int status_y = Status_Line_Y + 3;

    // 清除之前的显示
    cct_gotoxy(0, status_y);
    for (int i = 0; i < 120; i++) cout << " ";

    // 显示当前移动信息
    cct_gotoxy(0, status_y);
    cout << "第 " << get_total_move_count() << " 步: 移动盘子 "
        << disk_num << "# 从 " << from << " 到 " << to;
}

/* ========== 更新菜单项8的字符显示 ========== */
void update_menu8_display(int step, int disk_num, char from, char to, int total_disks)
{
    // 使用MenuItem8_Start_X, MenuItem8_Start_Y
    update_status_line(step, disk_num, from, to,
        MenuItem8_Start_X, MenuItem8_Start_Y, total_disks);
}

/* ========== 清除菜单项8位置的盘子（字符显示用）========== */
void clear_disk_at_menu8(int col, int row)
{
    clear_disk_at_position(col, row, MenuItem8_Start_X);
}

/* ========== 绘制菜单项8位置的盘子（字符显示用）========= */
void draw_disk_at_menu8(int col, int row, int disk_num)
{
    draw_disk_at_position(col, row, disk_num, MenuItem8_Start_X);
}
