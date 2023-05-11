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


void dijkstra(AdjacencyMatrix* adj_matrix, int start, int end)
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
    }
    else
    {
        // 输出最短路径
        printf("The shortest distance from node %d to node %d is %lf.\n", start, end, dist[end]);

        int* path;
        path = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // 分配内存空间
        int len = 0;
        int temp = end;
        while (temp != start)
        {
            path[len++] = temp;
            temp = pre[temp];
        }
        path[len++] = start;

        // 正向打印路径
        printf("The path is: %d", path[len - 1]);
        for (int i = len - 2; i >= 0; i--)
            printf("->%d", path[i]);
        printf("\n");
        free(path); // 释放动态分配的内存空间
    }
    free(dist);
    free(st);
    free(pre);
}


int ask_find_path(AdjacencyMatrix* adj_matrix)
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
    dijkstra(adj_matrix, start, end);
    return FUNCTION_SUCCESS;
}