#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjList.h"


/* Initialize adjacency and hash tables */
void init_adjacency_list(AdjacencyList* adj_list)
{
    adj_list->num_points = 0;
    adj_list->num_edges = 0;
    adj_list->num_edit = 0;
    adj_list->points = NULL;
    adj_list->adj = NULL;
    adj_list->adj_len = NULL;
    adj_list->point_hash = (HashTable*)calloc(1, sizeof(HashTable));
    init_hashtable(adj_list->point_hash);   // Initialize the hash table
}


/* Free adjacency list memory   */
void free_adjacency_list(AdjacencyList* adj_list)
{
    if (adj_list->points != NULL)
        free(adj_list->points);
    if (adj_list->adj != NULL)
        free(adj_list->adj);
    if (adj_list->adj_len != NULL)
        free(adj_list->adj_len);
    if (adj_list->point_hash != NULL)
        free_hashtable(adj_list->point_hash);
}


/* Add a point to the adjacency list    */
void add_point_to_adjacency_list(AdjacencyList* adj_list, long long id, double lat, double lon)
{
    // Hash table
    if (hashtable_lookup(adj_list->point_hash, id) != -1)
    {
        printf("Duplicated point ID %lld\n", id);
        free_adjacency_list(adj_list);
        exit(1);
    }

    hashtable_add(adj_list->point_hash, id, adj_list->num_points);

    if (adj_list->num_points == 0)
    { // If it's the first point, allocate memory dynamically
        adj_list->points = (Point*)malloc(sizeof(Point));
        adj_list->adj = (int**)malloc(sizeof(int*));
        adj_list->adj_len = (int*)malloc(sizeof(int));
        if (adj_list->points == NULL || adj_list->adj == NULL || adj_list->adj_len == NULL)
        {
            printf("malloc failed");
            free_adjacency_list(adj_list);
            exit(1);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_list->points = (Point*)realloc(adj_list->points, (unsigned long long)(adj_list->num_points + 1) * sizeof(Point));
        adj_list->adj = (int**)realloc(adj_list->adj, (unsigned long long)(adj_list->num_points + 1) * sizeof(int*));
        adj_list->adj_len = (int*)realloc(adj_list->adj_len, (unsigned long long)(adj_list->num_points + 1) * sizeof(int));
        if (adj_list->points == NULL || adj_list->adj == NULL || adj_list->adj_len == NULL)
        {
            printf("malloc failed");
            free_adjacency_list(adj_list);
            exit(1);
        }
    }

    adj_list->points[adj_list->num_points].id = id;
    adj_list->points[adj_list->num_points].lat = lat;
    adj_list->points[adj_list->num_points].lon = lon;
    adj_list->adj[adj_list->num_points] = NULL;
    adj_list->adj_len[adj_list->num_points] = 0;
    adj_list->num_points++;
}


/* Add an edge to adjacency list */
void add_edge_to_adjacency_list(AdjacencyList* adj_list, long long id, long long from, long long to, double length)
{
    if (hashtable_lookup(adj_list->point_hash, from) == -1 || hashtable_lookup(adj_list->point_hash, to) == -1)
    {
        printf("Error: Invalid edge (%lld, %lld)\n", from, to);
        free_adjacency_list(adj_list);
        exit(1);
    }

    if (adj_list->num_edges == 0)
    { // If it's the first edge, allocate dynamically
        adj_list->edges = (Edge*)malloc(sizeof(Edge));
        if (adj_list->edges == NULL)
        {
            printf("malloc failed");
            free_adjacency_list(adj_list);
            exit(1);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_list->edges = (Edge*)realloc(adj_list->edges, (unsigned long long)(adj_list->num_edges + 1) * sizeof(Edge));
        if (adj_list->edges == NULL)
        {
            printf("malloc failed");
            free_adjacency_list(adj_list);
            exit(1);
        }
    }

    /* 存入边的信息到edges中 */
    adj_list->edges[adj_list->num_edges].id = id;
    adj_list->edges[adj_list->num_edges].from = from;
    adj_list->edges[adj_list->num_edges].to = to;
    adj_list->edges[adj_list->num_edges].length = length;

    /* Find information about the points from and to from the hash table    */
    int from_index = hashtable_lookup(adj_list->point_hash, from);
    int to_index = hashtable_lookup(adj_list->point_hash, to);

    /* Maintain adjacency list array    */
    adj_list->adj[from_index] = (int*)realloc(adj_list->adj[from_index], (unsigned long long)(adj_list->adj_len[from_index] + 1) * sizeof(int));
    if (adj_list->adj[from_index] == NULL)
    {
        printf("malloc failed");
        free_adjacency_list(adj_list);
        exit(1);
    }

    adj_list->adj[from_index][adj_list->adj_len[from_index]] = adj_list->num_edges;
    adj_list->adj_len[from_index]++;

    adj_list->adj[to_index] = (int*)realloc(adj_list->adj[to_index], (unsigned long long)(adj_list->adj_len[to_index] + 1) * sizeof(int));
    if (adj_list->adj[to_index] == NULL)
    {
        printf("malloc failed");
        free_adjacency_list(adj_list);
        exit(1);
    }

    adj_list->adj[to_index][adj_list->adj_len[to_index]] = adj_list->num_edges;
    adj_list->adj_len[to_index]++;

    adj_list->num_edges++;
}


void print_adjacency_list(AdjacencyList* adj_list)
{
    int i, j;

    for (i = 0; i < adj_list->num_points; i++)
    {
        printf("Point %lld (%.6lf, %.6lf):", adj_list->points[i].id, adj_list->points[i].lat, adj_list->points[i].lon);

        printf("   ");
        for (j = 0; j < adj_list->adj_len[i]; j++)
        {
            // Find the index of each element-edge
            int edge_index = adj_list->adj[i][j];
            // get edges
            Edge edge = adj_list->edges[edge_index];

            long long point_id = 0;
            if (edge.from == adj_list->points[i].id)
                point_id = edge.to;
            else
                point_id = edge.from;

            printf("(%lld) ", point_id);
        }
        printf("\n");
    }
}
