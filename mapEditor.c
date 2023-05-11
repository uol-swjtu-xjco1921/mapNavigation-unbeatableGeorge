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
// �ж��ǲ��� double  �ǲ���int  �Ƿ񳬹���max��min

int ask_modify_map(AdjacencyMatrix* adj_matrix)
{
    int modify = 0;
    char line[100];
    printf("\n��ǰ��ͼ�й��� %d ����� %d ���ߣ����Ѿ��Ե�ͼ���б༭ %d �Ρ�\n",
        adj_matrix->num_points, adj_matrix->num_edges, adj_matrix->num_edit);

    while (1)
    { /* ��ʾ�û��Ƿ���Ҫ�޸ĵ�ͼ���� */
        printf("\n���Ƿ���Ҫ�޸ĵ�ͼ���ԣ�\n");
        printf("1. ��\n");
        printf("2. ��\n");
        printf("�������ѡ��: ");

        // ��һ�е�����
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("��ȡʧ��\n");
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
            printf("��û��������ȷ��ѡ��(1 or 2)��\n");
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

    // �������ͣ�1��ʾ��ӵ㣬2��ʾ��ӱߣ�3��ʾ�༭��
    int operate_type;
    printf("\n��ѡ��������ͣ�\n");
    printf("1. ��ӵ�\n");
    printf("2. ��ӱ�\n");
    printf("3. �༭��\n");
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
        flag = 0;
        while (!flag) // ѭ����ȡ����
        {
            printf("\n������������id latitude longitude: \n");
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
            sscanf(buf, "%lld", &point_id);


            // ����point_id �����ж�point_id�Ƿ���ȷ
            printf("������latitude: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf))
            {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lf", &lat);


            // ����point_id �����ж�point_id�Ƿ���ȷ
            printf("������longitude: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf)) {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lf", &lon);

            flag = 1;
        }

        // �鿴���Ƿ����
        int result = is_point_exist(adj_matrix, point_id, lat, lon, 1);

        if (result == EXIST)
        { /* ���ظ��� */
            printf("The point ID %lld has exited in \n", point_id);
            return ERROR_EDIT_FAILED;
        } /* ���ظ��� */

        // ��ӵ㵽�ڽӱ� ���滹��Ҫ�ж��Ƿ���ӳɹ� ��Ȼ��Ҫ�жϱ߽�
        add_point_to_adjacency_matrix(adj_matrix, point_id, lat, lon);
        printf("�ѳɹ�����µ� id = %lld, latitude = %lf, longitude = %lf\n\n", point_id, lat, lon);
        break;

    case 2:
        // ��ӱ�
        flag = 0;
        while (!flag) // ѭ����ȡ����
        {
            printf("�����������id ����š����յ��źͱ߳�\n");
            // ����link_id �����ж�link_id�Ƿ���ȷ
            printf("����ߵ�id: ");
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
            sscanf(buf, "%lld", &link_id);


            // ������ʼ����
            printf("����ߵ���ʼ��: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_from);

            // �����յ���
            printf("����ߵ��յ�: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_to);

            // ����߳�
            printf("����ߵĳ���: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf)) {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lf", &length);

            // �ж��������Ƿ����
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

            // �ж�һ��ʼ�Ƿ���Ѿ����������link��id or link��
            if (is_edge_exist(adj_matrix, link_id, node_id_from, node_id_to, 1) == EXIST)
            {
                continue;
            }

            // �ж��Ի���
            if (node_id_from == node_id_to)
            {
                printf("\nYou are entering the self-loop edge!\n");
                continue;
            }

            flag = 1;
        }


        /* ��ʼ��ӱ� */
        add_edge_to_adjacency_matrix(adj_matrix, link_id, node_id_from, node_id_to, length);
        printf("�ѳɹ�����±� id = %lld, node_1 <=> node_2 = %lld <=> %lld, length = %lf\n", link_id, node_id_from, node_id_to, length);

        break;

    case 3:
        // �༭��
        flag = 0;
        while (!flag) // ѭ����ȡ����
        {
            // ����link_id �����ж�link_id�Ƿ���ȷ
            printf("����ߵ�id: ");
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
            sscanf(buf, "%lld", &link_id);

            // �жϱ� link_id �Ƿ��Ѿ�����
            if (is_edge_id_exist(adj_matrix, link_id) == NOT_EXIST)
            {
                printf("The link_id %lld is not existed! retype!\n", link_id);
                continue;
            }
            flag = 1;
        }

        flag = 0;
        while (!flag) // ѭ����ȡ����
        {
            // ����ߵ� from to
            printf("����ߵ���ʼλ�úͳ��� node_id_1��node_id_2��length\n");

            // ������ʼ����
            printf("����ߵ���ʼ��: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_from);

            // �����յ���
            printf("����ߵ��յ�: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n')
            {
                buf[len - 1] = '\0';
            }
            if (!check_lld(buf))
            {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lld", &node_id_to);

            // ����߳�
            printf("����ߵĳ���: ");
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                printf("������󣡣���\n");
                continue;
            }
            len = strlen(buf);
            if (buf[len - 1] == '\n') {
                buf[len - 1] = '\0';
            }
            if (!check_double(buf)) {
                printf("���������������!\n");
                continue;
            }
            sscanf(buf, "%lf", &length);


            // �ж��������Ƿ����
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

            // �ж� link �Ƿ��Ѿ����� (�������ı�ȡ����)
            if (is_edge_nodes_exist(adj_matrix, link_id, node_id_from, node_id_to) == EXIST)
            {
                printf("The link with node_1 <=> node_2 = %lld <=> %lld is existed in this map!!\n", node_id_from, node_id_to);
                continue;
            }

            // �ж��Ի���
            if (node_id_from == node_id_to)
            {
                printf("\nYou are entering the self-loop edge!\n");
                continue;
            }

            flag = 1;
        }

        // �༭��
        edit_edge(adj_matrix, link_id, node_id_from, node_id_to, length);
        printf("�ѳɹ�����±� id = %lld, node_1 <=> node_2 = %lld <=> %lld, length = %lf\n", link_id, node_id_from, node_id_to, length);

        break;

    default:
        printf("��֧�ֵĲ������͡�\n");
        return ERROR_EDIT_FAILED;
    }

    return EDIT_SUCCESSFULLY;
}

// �жϵ��Ƿ���� 1.node_id  2.ͬһ��lat��lon
// �༭���ʱ��  1.��ӡ node_id   2.��ӡ lat lon
// �жϱ��Ƿ���� 1.link_id  2.ͬһ��from to
// �༭�ߵ�ʱ��  1.��ӡ link_id    2.��ӡfrom to   3.��ӡ length

int edit_edge(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, double length)
{
    /* ���ұ����ڽӱ������λ�� */
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        /* ����ָ�� */
        if (edge == NULL)
        { /* free memory and exit with NULL_POINTER*/
            free_adjacency_matrix(adj_matrix);
            exit(NULL_POINTER);
        }

        /* �ı�ߵ����� */
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

    /* �жϵ��Ƿ��Ѿ��������ڽӱ��� �ж�point����*/
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        Point* point = &adj_matrix->points[i];
        /* ����ָ�� */
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
                printf("�õ��Ѿ��������ڽӱ��С���ͼ���Ѿ����ڵĵ���ϢΪ��id = %lld, latitude = %lf, longitude = %lf�� \n", point->id, point->lat, point->lon);
            }
            return EXIST;
        }
    }

    return NOT_EXIST;
}


int is_edge_exist(AdjacencyMatrix* adj_matrix, long long link_id, long long node_id_from, long long node_id_to, int flag)
{

    /* �жϱ��Ƿ��Ѿ��������ڽӱ��� */
    for (int i = 0; i < adj_matrix->num_edges; i++)
    {
        Edge* edge = &adj_matrix->edges[i];
        /* ����ָ�� */
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
                printf("�ñ��Ѿ��������ڽӱ��С���ͼ�еıߵ���ϢΪ��id = %lld, node_id_1 = %lld, node_id_2 = %lld��\n", edge->id, edge->from, edge->to);
            }
            return EXIST;
        }
    }

    return NOT_EXIST;
}

int is_edge_id_exist(AdjacencyMatrix* adj_matrix, long long link_id)
{

    /* �жϱ��Ƿ��Ѿ��������ڽӱ��� */
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

    /* �жϱ��Ƿ��Ѿ��������ڽӱ��� */
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
    /* �жϵ��Ƿ��Ѿ��������ڽӱ��� �߲���*/
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        Point* point = &adj_matrix->points[i];
        /* ����ָ�� */
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


// ���һ�����ǲ���long long
int check_lld(char* s)
{
    int max_len = MAX_LONG_LONG;
    // ����Ƿ�Ϊ��
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

// ���һ�����ǲ���double
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