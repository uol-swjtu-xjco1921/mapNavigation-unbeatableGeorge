#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "pathFinder.h"
#include "mapEditor.h"
#include "mapError.h"

#define NO_PATH -1
#define NO_LOCATION_NEEDED 1
#define NODE_NEEDED 2
#define POINT_OF_INTEREST 3

int ask_find_path(AdjacencyMatrix* adj_matrix, int* path)
{
    // 初始化各个变量
    long long input_start = 0;
    long long input_end = 0;
    long long start = 0;
    long long end = 0;
    // 设置标志指示输入是否有效
    int valid_input = 0;
    printf("\n");
    while (!valid_input)
    {
        printf("请输入起点的节点ID: ");
        char input_buffer[1024];
        if (fgets(input_buffer, 1024, stdin) == NULL)
        {
            printf("输入错误！！！\n");
            continue;
        }
        int len = strlen(input_buffer);
        if (input_buffer[len - 1] == '\n')
        {
            input_buffer[len - 1] = '\0';
        }
        if (!check_lld(input_buffer))
        {
            printf("输入错误！重新输入!\n");
            continue;
        }
        sscanf(input_buffer, "%lld", &input_start);
        start = hashtable_lookup(adj_matrix->point_hash, input_start);
        if (start == -1)
        {
            printf("无效的起点节点 ID %lld，请重新输入。\n", input_start);
            continue; // 出现错误，重新开始循环
        }
        valid_input = 1;
    }
    valid_input = 0;
    while (!valid_input)
    {
        printf("请输入终点的节点ID: ");
        char input_buffer[1024];
        if (fgets(input_buffer, 1024, stdin) == NULL)
        {
            printf("输入错误！！！\n");
            continue;
        }
        int len = strlen(input_buffer);
        if (input_buffer[len - 1] == '\n')
        {
            input_buffer[len - 1] = '\0';
        }
        if (!check_lld(input_buffer))
        {
            printf("输入错误！重新输入!\n");
            continue;
        }
        sscanf(input_buffer, "%lld", &input_end);
        end = hashtable_lookup(adj_matrix->point_hash, input_end);
        if (end == -1)
        {
            printf("无效的终点节点 ID %lld，请重新输入。\n", input_end);
            continue; // 出现错误，重新开始循环
        }
        valid_input = 1; // 所有输入均合法，设置标志以退出循环
    }
    // 询问是否需要途径点
    long long location= 0;
    int POI = 0;
    int result = ask_location_POI(adj_matrix, &location, &POI);
    // 不需要途径某点
    if (result == NO_LOCATION_NEEDED)
        dijkstra(adj_matrix, start, end, path);
    // 需要途径某个nodes
    if (result == NODE_NEEDED)
    {
        location = hashtable_lookup(adj_matrix->point_hash, location);
        dijkstra_via(adj_matrix, start, end, location, path);
    }
    // 需要途径某个地方
    if (result == POINT_OF_INTEREST)
    {
        // 记录path的长度
        double length_of_path = INF;
        for (int n = 0; n < adj_matrix->num_edges; n++)
        {
            // 如果没有poi 就直接跳过
            if (adj_matrix->edges[n].num_pois == 0)
            {
                continue;
            }

            // 如果有poi 就看poi值是不是正确的
            for (int j = 0; j < adj_matrix->edges[n].num_pois; j++)
            {
                if (adj_matrix->edges[n].pois[j] == POI)
                { // 找到一个POI的路径
                    int* path1 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // 分配内存空间
                    memset(path1, -1, sizeof(int) * adj_matrix->num_points);
                    int* path2 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // 分配内存空间
                    memset(path2, -1, sizeof(int) * adj_matrix->num_points);
                    int* path3 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // 分配内存空间
                    memset(path3, -1, sizeof(int) * adj_matrix->num_points);
                    int* path4 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // 分配内存空间
                    memset(path4, -1, sizeof(int) * adj_matrix->num_points);

                    long long edge_node1 = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[n].from);
                    long long edge_node2 = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[n].to);
                    
                    // 两种路径分别的route
                    double route1 = dijkstra(adj_matrix, start, edge_node1, path1) + dijkstra(adj_matrix, edge_node2, end, path2) + adj_matrix->adj[edge_node1][edge_node2];
                    double route2 = dijkstra(adj_matrix, start, edge_node2, path3) + dijkstra(adj_matrix, edge_node1, end, path4) + adj_matrix->adj[edge_node1][edge_node2];
                    
                    // 不去替换之前的更小
                    if (length_of_path < route1 && length_of_path < route2)
                    {
                        continue;
                    }
                    
                    // 比较选取短的route
                    if (route1 >= route2)
                    {
                        // 重置 path
                        for (int i = 0; i < adj_matrix->num_points; i++)
                        {
                            path[i] = -1;
                        }
                        length_of_path = route2;
                        int len = 0;
                        for (int i = 0; i < adj_matrix->num_points; i++)
                        {
                            if (path3[i] < 0 || path3[i] > adj_matrix->num_points - 1)
                            {
                                break;
                            }
                            path[i] = path3[i];
                            len++;
                        }
                        for (int i = len; i < adj_matrix->num_points; i++)
                        {
                            if (path4[i - len] < 0 || path4[i - len] > adj_matrix->num_points - 1)
                            {
                                break;
                            }
                            path[i] = path4[i - len];
                        }
                    }
                    else
                    {
                        // 重置 path
                        for (int i = 0; i < adj_matrix->num_points; i++)
                        {
                            path[i] = -1;
                        }
                        length_of_path = route1;
                        int len = 0;
                        for (int i = 0; i < adj_matrix->num_points; i++)
                        {
                            if (path1[i] < 0 || path1[i] > adj_matrix->num_points - 1)
                            {
                                break;
                            }
                            path[i] = path1[i];
                            len++;
                        }
                        for (int i = len; i < adj_matrix->num_points; i++)
                        {
                            if (path2[i - len] < 0 || path2[i - len] > adj_matrix->num_points - 1)
                            {
                                break;
                            }
                            path[i] = path2[i - len];
                        }
                    }
                    free(path1);
                    free(path2);
                    free(path3);
                    free(path4);
                }
            }
        }
        printf("%lf", length_of_path);
    }
    return FUNCTION_SUCCESS;
}


int ask_location_POI(AdjacencyMatrix* adj_matrix, long long* location, int* POI)
{
    char line[100];
    int operate_type;
    printf("\n你是否需要途径某个地方：\n");
    printf("1. 不需要\n");
    printf("2. 需要途径地图上的节点\n");
    printf("3. 需要途径地图上的地点\n");
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
            return NO_LOCATION_NEEDED;
        case 2:
            flag = 0;
            while (!flag) // 循环读取输入
            {
                printf("\n请输入你要经过哪个节点: \n");
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
                sscanf(buf, "%lld", location);
                // 查看点是否存在(这里用了一个合法的坐标)
                int result = is_point_exist(adj_matrix, *location, 53.81, -1.55, 0);

                if (result != EXIST)
                { /* 点不存在 */
                    printf("The point ID %lld is not exited! retype!\n", *location);
                    continue;
                }
                flag = 1;
            }
            return NODE_NEEDED;
        case 3:
            flag = 0;
            while (!flag) // 循环读取输入
            {
                printf("\n请输入你要经过哪个地方: \n");
                // 输入point_id 并且判断point_id是否正确
                printf("输入point of interest: ");
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
                sscanf(buf, "%ld", POI);
                // 查看点是否存在(这里用了一个合法的坐标)
                //int result = is_POI_exist(adj_matrix, *location, *POI);

                //if (result != EXIST)
                //{ /* 点不存在 */
                //    printf("The POI %lld is not exited! retype!\n", *location);
                //    continue;
                //}
                flag = 1;
            }
            return POINT_OF_INTEREST;
    }
}


double dijkstra(AdjacencyMatrix* adj_matrix, int start, int end, int* path)
{
    double* dist;
    dist = (double*)malloc(sizeof(double) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        dist[i] = INF;
    }

    int* st;
    st = (int*)malloc(sizeof(int) * adj_matrix->num_points); // 分配内存空间
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        st[i] = 0;
    }

    int* pre;
    pre = (int*)malloc(sizeof(int) * adj_matrix->num_points); // 分配内存空间
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        pre[i] = -1;
    }

    dist[start] = 0;

    for (int i = 0; i < adj_matrix->num_points; i++) {
        int k = -1;
        for (int j = 0; j < adj_matrix->num_points; j++) {
            if (!st[j] && (k == -1 || dist[k] > dist[j])) {
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < adj_matrix->num_points; j++) {
            if (!st[j] && dist[j] > dist[k] + adj_matrix->adj[k][j]) {
                dist[j] = dist[k] + adj_matrix->adj[k][j];
                pre[j] = k;
            }
        }
    }

    if (dist[end] == INF)
    {
        printf("There is no available path from node %d to node %d.\n", start, end);
        free(dist);
        free(st);
        free(pre);
        return NO_PATH;
    }
    else
    {
        //printf("The shortest distance from node %d to node %d is %lf.\n", start, end, dist[end]);
        int len = 0;
        int temp = end;
        while (temp != start)
        {
            path[len++] = temp;
            temp = pre[temp];
        }
        path[len++] = start;
        // 将数组 path 中的元素翻转，得到逆序路径
        for (int i = 0; i < len / 2; i++)
        {
            int tmp = path[i];
            path[i] = path[len - i - 1];
            path[len - i - 1] = tmp;
        }
    }
    double temp = dist[end];
    free(dist);
    free(st);
    free(pre);
    return temp;
}


void dijkstra_via(AdjacencyMatrix* adj_matrix, int start, int end, int via, int* path) 
{
    double* dist_start;
    dist_start = (double*)malloc(sizeof(double) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++) {
        dist_start[i] = INF;
    }

    double* dist_tmp;
    dist_tmp = (double*)malloc(sizeof(double) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++) {
        dist_tmp[i] = INF;
    }

    int* st;
    st = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++) {
        st[i] = 0;
    }

    int* pre_start;
    pre_start = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++) {
        pre_start[i] = -1;
    }

    int* pre_tmp;
    pre_tmp = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++) {
        pre_tmp[i] = -1;
    }

    dist_start[start] = 0;
    dist_tmp[via] = 0;

    // 计算从起点到途经点的最短路径和总距离
    for (int i = 0; i < adj_matrix->num_points; i++) {
        int k = -1;
        for (int j = 0; j < adj_matrix->num_points; j++) {
            if (!st[j] && (k == -1 || dist_start[k] > dist_start[j])) {
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < adj_matrix->num_points; j++) {
            if (!st[j] && dist_start[j] > dist_start[k] + adj_matrix->adj[k][j]) {
                dist_start[j] = dist_start[k] + adj_matrix->adj[k][j];
                pre_start[j] = k;
            }
        }
        if (k == via) break;
    }

    // 如果无法从起点到达途经点，直接返回
    if (dist_start[via] == INF) {
        printf("There is no available path from node %d to node %d via node %d.\n", start, end, via);
        free(dist_start);
        free(dist_tmp);
        free(st);
        free(pre_start);
        free(pre_tmp);
        return;
    }

    // 重置 st 数组
    for (int i = 0; i < adj_matrix->num_points; i++) {
        st[i] = 0;
    }

    // 计算从途经点到终点的最短路径和总距离
    for (int i = 0; i < adj_matrix->num_points; i++) {
        int k = -1;
        for (int j = 0; j < adj_matrix->num_points; j++) {
            if (!st[j] && (k == -1 || dist_tmp[k] > dist_tmp[j])) {
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < adj_matrix->num_points; j++) {
            if (!st[j] && dist_tmp[j] > dist_tmp[k] + adj_matrix->adj[k][j]) {
                dist_tmp[j] = dist_tmp[k] + adj_matrix->adj[k][j];
                pre_tmp[j] = k;
            }
        }
        if (k == end) break;
    }

    // 如果无法从途经点到达终点，直接返回
    if (dist_tmp[end] == INF) {
        printf("There is no available path from node %d to node %d via node %d.\n", start, end, via);
        free(dist_start);
        free(dist_tmp);
        free(st);
        free(pre_start);
        free(pre_tmp);
        return;
    }

    // 合并两段路径，得到最终的最短路径
    int* tmp_path1;
    tmp_path1 = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    int len1 = 0;
    int tmp = via;
    while (tmp != start) {
        tmp_path1[len1++] = tmp;
        tmp = pre_start[tmp];
    }
    tmp_path1[len1++] = start;

    int* tmp_path2;
    tmp_path2 = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    int len2 = 0;
    tmp = end;
    while (tmp != via) {
        tmp_path2[len2++] = tmp;
        tmp = pre_tmp[tmp];
    }
    tmp_path2[len2++] = via;

    int len = len1 + len2 - 1;
    for (int i = 0; i < len1; i++) {
        path[i] = tmp_path1[len1 - 1 - i];
    }
    for (int i = 1; i < len2; i++) {
        path[len1 + i - 1] = tmp_path2[len2 - 1 - i];
    }

    // 输出最短路径
    printf("The shortest distance from node %d to node %d via node %d is %lf.\n", start, end, via, dist_start[via] + dist_tmp[end]);
    printf("The path is: %d", path[0]);
    for (int i = 1; i < len; i++) {
        printf("->%d", path[i]);
    }
    printf("\n");

    free(tmp_path1);
    free(tmp_path2);
    free(dist_start);
    free(dist_tmp);
    free(st);
    free(pre_start);
    free(pre_tmp);
}