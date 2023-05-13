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
    // ��ʼ����������
    long long input_start = 0;
    long long input_end = 0;
    long long start = 0;
    long long end = 0;
    // ���ñ�־ָʾ�����Ƿ���Ч
    int valid_input = 0;
    printf("\n");
    while (!valid_input)
    {
        printf("���������Ľڵ�ID: ");
        char input_buffer[1024];
        if (fgets(input_buffer, 1024, stdin) == NULL)
        {
            printf("������󣡣���\n");
            continue;
        }
        int len = strlen(input_buffer);
        if (input_buffer[len - 1] == '\n')
        {
            input_buffer[len - 1] = '\0';
        }
        if (!check_lld(input_buffer))
        {
            printf("���������������!\n");
            continue;
        }
        sscanf(input_buffer, "%lld", &input_start);
        start = hashtable_lookup(adj_matrix->point_hash, input_start);
        if (start == -1)
        {
            printf("��Ч�����ڵ� ID %lld�����������롣\n", input_start);
            continue; // ���ִ������¿�ʼѭ��
        }
        valid_input = 1;
    }
    valid_input = 0;
    while (!valid_input)
    {
        printf("�������յ�Ľڵ�ID: ");
        char input_buffer[1024];
        if (fgets(input_buffer, 1024, stdin) == NULL)
        {
            printf("������󣡣���\n");
            continue;
        }
        int len = strlen(input_buffer);
        if (input_buffer[len - 1] == '\n')
        {
            input_buffer[len - 1] = '\0';
        }
        if (!check_lld(input_buffer))
        {
            printf("���������������!\n");
            continue;
        }
        sscanf(input_buffer, "%lld", &input_end);
        end = hashtable_lookup(adj_matrix->point_hash, input_end);
        if (end == -1)
        {
            printf("��Ч���յ�ڵ� ID %lld�����������롣\n", input_end);
            continue; // ���ִ������¿�ʼѭ��
        }
        valid_input = 1; // ����������Ϸ������ñ�־���˳�ѭ��
    }
    // ѯ���Ƿ���Ҫ;����
    long long location= 0;
    int POI = 0;
    int result = ask_location_POI(adj_matrix, &location, &POI);
    // ����Ҫ;��ĳ��
    if (result == NO_LOCATION_NEEDED)
        dijkstra(adj_matrix, start, end, path);
    // ��Ҫ;��ĳ��nodes
    if (result == NODE_NEEDED)
    {
        location = hashtable_lookup(adj_matrix->point_hash, location);
        dijkstra_via(adj_matrix, start, end, location, path);
    }
    // ��Ҫ;��ĳ���ط�
    if (result == POINT_OF_INTEREST)
    {
        // ��¼path�ĳ���
        double length_of_path = INF;
        for (int n = 0; n < adj_matrix->num_edges; n++)
        {
            // ���û��poi ��ֱ������
            if (adj_matrix->edges[n].num_pois == 0)
            {
                continue;
            }

            // �����poi �Ϳ�poiֵ�ǲ�����ȷ��
            for (int j = 0; j < adj_matrix->edges[n].num_pois; j++)
            {
                if (adj_matrix->edges[n].pois[j] == POI)
                { // �ҵ�һ��POI��·��
                    int* path1 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // �����ڴ�ռ�
                    memset(path1, -1, sizeof(int) * adj_matrix->num_points);
                    int* path2 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // �����ڴ�ռ�
                    memset(path2, -1, sizeof(int) * adj_matrix->num_points);
                    int* path3 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // �����ڴ�ռ�
                    memset(path3, -1, sizeof(int) * adj_matrix->num_points);
                    int* path4 = (int*)malloc(sizeof(int) * (adj_matrix->num_points)); // �����ڴ�ռ�
                    memset(path4, -1, sizeof(int) * adj_matrix->num_points);

                    long long edge_node1 = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[n].from);
                    long long edge_node2 = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[n].to);
                    
                    // ����·���ֱ��route
                    double route1 = dijkstra(adj_matrix, start, edge_node1, path1) + dijkstra(adj_matrix, edge_node2, end, path2) + adj_matrix->adj[edge_node1][edge_node2];
                    double route2 = dijkstra(adj_matrix, start, edge_node2, path3) + dijkstra(adj_matrix, edge_node1, end, path4) + adj_matrix->adj[edge_node1][edge_node2];
                    
                    // ��ȥ�滻֮ǰ�ĸ�С
                    if (length_of_path < route1 && length_of_path < route2)
                    {
                        continue;
                    }
                    
                    // �Ƚ�ѡȡ�̵�route
                    if (route1 >= route2)
                    {
                        // ���� path
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
                        // ���� path
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
    printf("\n���Ƿ���Ҫ;��ĳ���ط���\n");
    printf("1. ����Ҫ\n");
    printf("2. ��Ҫ;����ͼ�ϵĽڵ�\n");
    printf("3. ��Ҫ;����ͼ�ϵĵص�\n");
    printf("�������ѡ������: ");

    while (1)
    {
        // ��һ�е�����
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("��ȡʧ��,�������룺\n");
            continue;
        }

        // ȥ����ĩ�Ļ��з�
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        // �Ƚ�֮�� �ó�ѡ���ֵ
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
            printf("��û��������ȷ��ѡ��(1 or 2 or 3)��\n");
            printf("���������룺");
        }
    }

    int flag = 0; // ��־λ����ʾ�Ƿ�ɹ�������������
    char buf[100]; // ����һ��������
    int len = 0;
    switch (operate_type)
    {
        case 1:
            return NO_LOCATION_NEEDED;
        case 2:
            flag = 0;
            while (!flag) // ѭ����ȡ����
            {
                printf("\n��������Ҫ�����ĸ��ڵ�: \n");
                // ����point_id �����ж�point_id�Ƿ���ȷ
                printf("������id: ");
                if (fgets(buf, sizeof(buf), stdin) == NULL)
                {
                    printf("������󣡣���\n");
                    continue;
                }
                int len = strlen(buf);
                if (buf[len - 1] == '\n')
                {
                    buf[len - 1] = '\0';
                }
                if (!check_lld(buf))
                {
                    printf("���������������!\n");
                    continue;
                }
                sscanf(buf, "%lld", location);
                // �鿴���Ƿ����(��������һ���Ϸ�������)
                int result = is_point_exist(adj_matrix, *location, 53.81, -1.55, 0);

                if (result != EXIST)
                { /* �㲻���� */
                    printf("The point ID %lld is not exited! retype!\n", *location);
                    continue;
                }
                flag = 1;
            }
            return NODE_NEEDED;
        case 3:
            flag = 0;
            while (!flag) // ѭ����ȡ����
            {
                printf("\n��������Ҫ�����ĸ��ط�: \n");
                // ����point_id �����ж�point_id�Ƿ���ȷ
                printf("����point of interest: ");
                if (fgets(buf, sizeof(buf), stdin) == NULL)
                {
                    printf("������󣡣���\n");
                    continue;
                }
                int len = strlen(buf);
                if (buf[len - 1] == '\n')
                {
                    buf[len - 1] = '\0';
                }
                if (!check_lld(buf))
                {
                    printf("���������������!\n");
                    continue;
                }
                sscanf(buf, "%ld", POI);
                // �鿴���Ƿ����(��������һ���Ϸ�������)
                //int result = is_POI_exist(adj_matrix, *location, *POI);

                //if (result != EXIST)
                //{ /* �㲻���� */
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
    st = (int*)malloc(sizeof(int) * adj_matrix->num_points); // �����ڴ�ռ�
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        st[i] = 0;
    }

    int* pre;
    pre = (int*)malloc(sizeof(int) * adj_matrix->num_points); // �����ڴ�ռ�
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
        // ������ path �е�Ԫ�ط�ת���õ�����·��
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

    // �������㵽;��������·�����ܾ���
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

    // ����޷�����㵽��;���㣬ֱ�ӷ���
    if (dist_start[via] == INF) {
        printf("There is no available path from node %d to node %d via node %d.\n", start, end, via);
        free(dist_start);
        free(dist_tmp);
        free(st);
        free(pre_start);
        free(pre_tmp);
        return;
    }

    // ���� st ����
    for (int i = 0; i < adj_matrix->num_points; i++) {
        st[i] = 0;
    }

    // �����;���㵽�յ�����·�����ܾ���
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

    // ����޷���;���㵽���յ㣬ֱ�ӷ���
    if (dist_tmp[end] == INF) {
        printf("There is no available path from node %d to node %d via node %d.\n", start, end, via);
        free(dist_start);
        free(dist_tmp);
        free(st);
        free(pre_start);
        free(pre_tmp);
        return;
    }

    // �ϲ�����·�����õ����յ����·��
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

    // ������·��
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