// Currently the following is an open source code for reference only
// this is Dijkstra's algorithm

#include <stdio.h>

#define V 20                   //�����������
#define INFINITY 65535

typedef struct {
    int vexs[V];         //�洢ͼ�ж�������
    int arcs[V][V];      //��ά���飬��¼����֮��Ĺ�ϵ
    int vexnum, arcnum;  //��¼ͼ�Ķ������ͻ����ߣ���
}MGraph;


//���ݶ��㱾�����ݣ��жϳ������ڶ�ά�����е�λ��
int LocateVex(MGraph* G, int v)
{
    int i = 0;

    //����һά���飬�ҵ�����v
    for (; i < G->vexnum; i++)
    {
        if (G->vexs[i] == v)
        {
            break;
        }
    }

    //����Ҳ����������ʾ��䣬����-1
    if (i > G->vexnum)
    {
        printf("no such vertex.\n");
        return -1;
    }

    return i;
}


//����������Ȩͼ
void CreateDG(MGraph* G)
{
    printf("����ͼ�Ķ������ͱ�����");
    scanf("%d %d", &(G->vexnum), &(G->arcnum));

    printf("����������㣺");
    for (int i = 0; i < G->vexnum; i++)
    {
        scanf("%d", &(G->vexs[i]));
    }

    for (int i = 0; i < G->vexnum; i++)
    {
        for (int j = 0; j < G->vexnum; j++)
        {
            G->arcs[i][j] = INFINITY;
        }
    }

    printf("��������ߵ����ݣ�\n");
    for (int i = 0; i < G->arcnum; i++)
    {
        int v1, v2, w;
        scanf("%d %d %d", &v1, &v2, &w);
        int n = LocateVex(G, v1);
        int m = LocateVex(G, v2);
        if (m == -1 || n == -1)
        {
            return;
        }
        G->arcs[n][m] = w;
        G->arcs[m][n] = w;
    }
}


//�Ͻ�˹�����㷨��v0��ʾ����������ʼ�����������е��±�
void Dijkstra_minTree(MGraph G, int v0, int p[V], int D[V])
{
    int final[V];//Ϊ������������һ�����ֵ������ȷ�ϸö����Ƿ��Ѿ��ҵ����·��

    //�Ը�������г�ʼ��
    for (int v = 0; v < G.vexnum; v++)
    {
        final[v] = 0;
        D[v] = G.arcs[v0][v];       // D[V]�������arcs[0][v]
        p[v] = 0;                   // p[V]�������0
    }

    //������v0λ�±�Ķ���Ϊ��ʼ�㣬���Բ������ж�
    D[v0] = 0;
    final[v0] = 1;
    int k = 0;

    for (int i = 0; i < G.vexnum; i++)
    {
        int min = INFINITY;
        //ѡ�񵽸�����Ȩֵ��С�Ķ��㣬��Ϊ������ȷ�����·���Ķ���
        for (int w = 0; w < G.vexnum; w++)
        {
            if (!final[w])
            {
                if (D[w] < min)
                {
                    k = w;
                    min = D[w];
                }
            }
        }

        //���øö���ı�־λΪ1�������´��ظ��ж�
        final[k] = 1;

        //��v0���������Ȩֵ���и���
        for (int w = 0; w < G.vexnum; w++)
        {
            if (!final[w] && (min + G.arcs[k][w] < D[w]))
            {
                D[w] = min + G.arcs[k][w];
                p[w] = k;//��¼�������·���ϴ��ڵĶ���
            }
        }
    }
}


//int main() {
//    MGraph G;
//    CreateDG(&G);
//
//    int P[V] = { 0 };   // ��¼���� 0 �������������̵�·��
//    int D[V] = { 0 };   // ��¼���� 0 �������������Ȩֵ
//    Dijkstra_minTree(G, 0, P, D);
//
//    printf("���·��Ϊ��\n");
//
//    for (int i = 1; i < G.vexnum; i++)
//    {
//        printf("%d - %d�����·���еĶ����У�", i, 0);
//        printf(" %d-", i);
//        int j = i;
//
//        //����ÿһ�����·���϶���¼�ž����Ķ��㣬���Բ���Ƕ�׵ķ�ʽ������ɵõ��������·���ϵ����ж���
//        while (P[j] != 0)
//        {
//            printf("%d-", P[j]);
//            j = P[j];
//        }
//
//        printf("0\n");
//    }
//
//    printf("Դ�㵽����������·������Ϊ:\n");
//
//    for (int i = 1; i < G.vexnum; i++)
//    {
//        printf("%d - %d : %d \n", G.vexs[0], G.vexs[i], D[i]);
//    }
//    return 0;
//}
