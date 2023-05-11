#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <ctype.h>
//#include <unistd.h> //sleep()

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapEditor.h"
#include "mapError.h"

#define EDIT_SUCCESSFULLY 0
#define EXIST 20
#define NOT_EXIST 21

#define MAX_LONG_LONG 12
#define MAX_DOUBLE 13
// 判断是不是 double  是不是int  是否超过了max和min

int ask_modify_map(AdjacencyMatrix* adj_matrix)
{
    int modify = 0;
    char line[100];
    printf("\n当前地图中共有 %d 个点和 %d 条边，您已经对地图进行编辑 %d 次。\n",
        adj_matrix->num_points, adj_matrix->num_edges, adj_matrix->num_edit);

    while (1)
    { /* 提示用户是否需要修改地图属性 */
        printf("\n您是否需要修改地图属性？\n");
        printf("1. 是\n");
        printf("2. 否\n");
        printf("输入你的选择: ");

        // 读一行的数据
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("读取失败\n");
            continue;
        }
        // 去除行末的换行符
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        // 比较之后 得出选择的值
        if (strcmp(line, "1") == 0)
        {
            if (edit_map(adj_matrix) == EDIT_SUCCESSFULLY)
            {
                modify = 1;
                adj_matrix->num_edit++;
            }
        }
        else if (strcmp(line, "2") == 0)
        {
            break;
        }
        else
        {
            printf("你没有输入正确的选项(1 or 2)。\n");
        }
    }

    return modify;
}


int edit_map(AdjacencyMatrix* adj_matrix)
{
    /* line to store message readed */
    char line[100];
    /* point message */
    long long point_id = -1;
    double lat = -1;
    double lon = -1;
    /* edge message */
    long long link_id = -1;
    long long node_id_from = -1;
    long long node_id_to = -1;
    double length = -1;

    // 操作类型，1表示添加点，2表示添加边，3表示编辑边
    int operate_type;
    printf("\n请选择操作类型：\n");
    printf("1. 添加点\n");
    printf("2. 添加边\n");
    printf("3. 编辑边\n");
    printf("输入你的选择类型: ");


    while (1)
    {
        // 读一行的数据
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("读取失败,重新输入：\n");
            continue;
        }

        // 去除行末的换行符
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        // 比较之后 得出选择的值
        if (strcmp(line, "1") == 0)
        {
            operate_type = 1;
            break;
        }
        else if (strcmp(line, "2") == 0)
        {
            operate_type = 2;
            break;
        }
        else if (strcmp(line, "3") == 0)
        {
            operate_type = 3;
            break;
        }
        else
        {
            printf("你没有输入正确的选项(1 or 2 or 3)。\n");
            printf("请重新输入：");
        }
    }



    int flag = 0; // 标志位，表示是否成功读入输入数据
    char buf[100]; // 定义一个缓冲区
    int len = 0;
    switch (operate_type)
    {
    case 1:
        flag = 0;
        while (!flag) // 循环读取输入
        {
            printf("\n请依次输入点的id latitude longitude: \n");
            // 输入point_id 并且判断point_id是否正确
            printf("输入点的id: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            int len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &point_id);


            // 输入point_id 并且判断point_id是否正确
            printf("输入点的latitude: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lf", &lat);


            // 输入point_id 并且判断point_id是否正确
            printf("输入点的longitude: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf)) {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lf", &lon);

            flag = 1;
        }

        // 查看点是否存在
        int result = is_point_exist(adj_matrix, point_id, lat, lon, 1);

        if (result == EXIST)
        { /* 点重复了 */
            printf("The point ID %lld has exited in \n", point_id);
            return ERROR_EDIT_FAILED;
        } /* 点重复了 */

        // 添加点到邻接表 后面还需要判断是否添加成功 当然需要判断边界
        add_point_to_adjacency_matrix(adj_matrix, point_id, lat, lon);
        printf("已成功添加新点 id = %lld, latitude = %lf, longitude = %lf\n\n", point_id, lat, lon);
        break;

    case 2:
        // 添加边
        flag = 0;
        while (!flag) // 循环读取输入
        {
            printf("请依次输入边id 起点编号、边终点编号和边长\n");
            // 输入link_id 并且判断link_id是否正确
            printf("输入边的id: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            int len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &link_id);


            // 输入起始点编号
            printf("输入边的起始点: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_from);

            // 输入终点编号
            printf("输入边的终点: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_to);

            // 输入边长
            printf("输入边的长度: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf)) {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lf", &length);

            // 判断两个点是否存在
            if (is_node_exist(adj_matrix, node_id_from) == NOT_EXIST)
            {
                printf("The node_id_1 is not exist! please retype!\n");
                continue;
            }
            if (is_node_exist(adj_matrix, node_id_to) == NOT_EXIST)
            {
                printf("The node_id_2 is not exist! please retype!\n");
                continue;
            }

            // 判断一开始是否就已经存在了这个link（id or link）
            if (is_edge_exist(adj_matrix, link_id, node_id_from, node_id_to, 1) == EXIST)
            {
                continue;
            }

            // 判断自环边
            if (node_id_from == node_id_to)
            {
                printf("\nYou are entering the self-loop edge!\n");
                continue;
            }

            flag = 1;
        }


        /* 开始添加边 */
        add_edge_to_adjacency_matrix(adj_matrix, link_id, node_id_from, node_id_to, length);
        printf("已成功添加新边 id = %lld, node_1 <=> node_2 = %lld <=> %lld, length = %lf\n", link_id, node_id_from, node_id_to, length);

        break;

    case 3:
        // 编辑边
        flag = 0;
        while (!flag) // 循环读取输入
        {
            // 输入link_id 并且判断link_id是否正确
            printf("输入边的id: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            int len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &link_id);

            // 判断边 link_id 是否已经存在
            if (is_edge_id_exist(adj_matrix, link_id) == NOT_EXIST)
            {
                printf("The link_id %lld is not existed! retype!\n", link_id);
                continue;
            }
            flag = 1;
        }

        flag = 0;
        while (!flag) // 循环读取输入
        {
            // 输入边的 from to
            printf("输入边的起始位置和长度 node_id_1，node_id_2，length\n");

            // 输入起始点编号
            printf("输入边的起始点: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_from);

            // 输入终点编号
            printf("输入边的终点: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_to);

            // 输入边长
            printf("输入边的长度: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("输入错误！！！\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf)) {
                printf("输入错误！重新输入!\n");
                continue;
            }
            sscanf(buf, "%lf", &length);


            // 判断两个点是否存在
            if (is_node_exist(adj_matrix, node_id_from) == NOT_EXIST)
            {
                printf("The node_id_1 is not exist! please retype!\n");
                continue;
            }
            if (is_node_exist(adj_matrix, node_id_to) == NOT_EXIST)
            {
                printf("The node_id_2 is not exist! please retype!\n");
                continue;
            }

            // 判断 link 是否已经存在 (被其他的边取代了)
            if (is_edge_nodes_exist(adj_matrix, link_id, node_id_from, node_id_to) == EXIST)
            {
                printf("The link with node_1 <=> node_2 = %lld <=> %lld is existed in this map!!\n", node_id_from, node_id_to);
                continue;
            }

            // 判断自环边
            if (node_id_from == node_id_to)
            {
                printf("\nYou are entering the self-loop edge!\n");
                continue;
            }

            flag = 1;
        }

        // 编辑边
        edit_edge(adj_matrix, link_id, node_id_from, node_id_to, length);
        printf("已成功添加新边 id = %lld, node_1 <=> node_2 = %lld <=> %lld, length = %lf\n", link_id, node_id_from, node_id_to, length);

        break;

    default:
        printf("不支持的操作类型。\n");
        return ERROR_EDIT_FAILED;
    }

    return EDIT_SUCCESSFULLY;
}

// 判断点是否存在 1.node_id  2.同一个lat和lon
// 编辑点的时候  1.打印 node_id   2.打印 lat lon
// 判断边是否存在 1.link_id  2.同一个from to
// 编辑边的时候  1.打印 link_id    2.打印from to   3.打印 length

int edit_edge(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, double length)
{
    /* 查找边在邻接表里面的位置 */
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        /* 检查空指针 */
        if (edge == NULL)
        { /* free memory and exit with NULL_POINTER*/
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        /* 改变边的内容 */
        if (edge->id == link_id)
        {
            edge->from = node_id_from;
            edge->to = node_id_to;
            edge->length = length;
            return EDIT_SUCCESSFULLY;
        }
    }

    return ERROR_EDIT_FAILED;
}



int is_point_exist(AdjacencyMatrix* adj_matrix, long long point_id, double lat, double lon, int flag)
{

    /* 判断点是否已经存在于邻接表中 判断point部分*/
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        Point* point = &adj_matrix->points[i];
        /* 检查空指针 */
        if (point == NULL)
        { /* free memory and exit with NULL_POINTER*/
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        /* */
        if (point->id == point_id || (point->lat == lat && point->lon == lon))
        {
            if (flag)
            {
                printf("该点已经存在于邻接表中。地图中已经存在的点信息为：id = %lld, latitude = %lf, longitude = %lf。 \n", point->id, point->lat, point->lon);
            }
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_edge_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, int flag)
{

    /* 判断边是否已经存在于邻接表中 */
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        /* 检查空指针 */
        if (edge == NULL)
        { /* free memory and exit with NULL_POINTER*/
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        /* */
        if (edge->id == link_id || (edge->from == node_id_from && edge->to == node_id_to))
        {
            if (flag)
            {
                printf("该边已经存在于邻接表中。地图中的边的信息为：id = %lld, node_id_1 = %lld, node_id_2 = %lld。\n", edge->id, edge->from, edge->to);
            }
            return EXIST;
        }
    }

    return NOT_EXIST;
}

int is_edge_id_exist(AdjacencyMatrix* adj_matrix, long long link_id)
{

    /* 判断边是否已经存在于邻接表中 */
    for (int i = 0; i < adj_matrix->num_edges - 1; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        if (edge->id == link_id)
        {
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_edge_nodes_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to)
{

    /* 判断边是否已经存在于邻接表中 */
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        if (edge->from == node_id_from && edge->to == node_id_to && link_id != edge->id)
        {
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_node_exist(AdjacencyMatrix* adj_matrix, long long node_id)
{
    /* 判断点是否已经存在于邻接表中 边部分*/
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        Point* point = &adj_matrix->points[i];
        /* 检查空指针 */
        if (point == NULL)
        { /* free memory and exit with NULL_POINTER*/
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        /* */
        if (point->id == node_id)
        {
            return EXIST;
        }
    }
    return NOT_EXIST;
}


// 检查一个数是不是long long
int check_lld(char* s)
{
    int max_len = MAX_LONG_LONG;
    // 检查是否为空
    if (s == NULL || *s == '\0')
    {
        return 0;
    }
    int n = 0;
    if (*s == '+' || *s == '-')
    {
        s++;
        n++;
    }
    while (*s != '\0')
    {
        if (!isdigit(*s))
        {
            return 0;
        }
        s++;
        n++;
        if (n > max_len)
        {
            return 0;
        }
    }
    return 1;
}

// 检查一个数是不是double
int check_double(const char* s)
{
    int max_len = MAX_DOUBLE;
    if (s == NULL || *s == '\0')
    {
        return 0;
    }
    int n = 0;
    if (*s == '+' || *s == '-')
    {
        s++;
        n++;
    }
    int dot_seen = 0;
    while (*s != '\0')
    {
        if (*s == '.')
        {
            if (dot_seen)
            {
                return 0;
            }
            else {
                dot_seen = 1;
            }
        }
        else if (!isdigit(*s))
        {
            return 0;
        }
        s++;
        n++;
        if (n > max_len)
        {
            return 0;
        }
    }
    return 1;
}