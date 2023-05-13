#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "mapAdjMatrix.h"
#include "mapError.h"

/* Initialize adjacency and hash tables */
void init_adjacency_matrix(AdjacencyMatrix* adj_matrix)
{
    // init bounding
    adj_matrix->bounding.maxLat = 0;
    adj_matrix->bounding.maxLon = 0;
    adj_matrix->bounding.minLat = 0;
    adj_matrix->bounding.minLon = 0;
    // init num of points, egdes, edits, geoms 
    adj_matrix->num_points = 0;
    adj_matrix->num_edges = 0;
    adj_matrix->num_edit = 0;
    adj_matrix->num_geoms = 0;
    // init points, edges, adj, geom
    adj_matrix->points = NULL;
    adj_matrix->edges = NULL;
    adj_matrix->adj = NULL;
    adj_matrix->geoms = NULL;
    // init hash table
    adj_matrix->point_hash = (HashTable*)calloc(1, sizeof(HashTable));
    if (adj_matrix->point_hash == NULL)
        exit(ERROR_MALLOC_FAILED);
    init_hashtable(adj_matrix->point_hash);   // Initialize the hash table
}


/* Free adjacency matrix memory   */
void free_adjacency_matrix(AdjacencyMatrix* adj_matrix)
{
    if (adj_matrix->points != NULL)
        free(adj_matrix->points);

    if (adj_matrix->adj != NULL) {
        for (int i = 0; i < adj_matrix->num_points; i++) {
            if (adj_matrix->adj[i] != NULL) {
                free(adj_matrix->adj[i]);
            }
        }
        free(adj_matrix->adj);
    }

    if (adj_matrix->point_hash != NULL)
        free_hashtable(adj_matrix->point_hash);
}


/* Add a point to the adjacency matrix    */
void add_point_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long id, double lat, double lon)
{
    // Look up in Hash table to prevent the same point id
    if (hashtable_lookup(adj_matrix->point_hash, id) != -1)
    {
        printf("Duplicated point ID %lld!\n", id);
        free_adjacency_matrix(adj_matrix);
        exit(ERROR_HASH_LOOK_UP);
    }

    // Add point in Hash table
    if (hashtable_add(adj_matrix->point_hash, id, adj_matrix->num_points) == ERROR_HASH_ADD)
    {
        printf("Too much points! hash add failed!\n");
        free_adjacency_matrix(adj_matrix);
        exit(ERROR_HASH_ADD);
    }

    if (adj_matrix->num_points == 0)
    { // If it's the first point, allocate memory dynamically
        adj_matrix->points = (Point*)malloc(sizeof(Point));
        adj_matrix->adj = (double**)malloc(sizeof(double*));
        adj_matrix->adj[0] = (double*)malloc(sizeof(double));
        adj_matrix->adj[0][0] = 0;  // 初始化为 0
        if (adj_matrix->points == NULL || adj_matrix->adj == NULL)
        {
            printf("malloc failed in point add process!\n");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_matrix->points = (Point*)realloc(adj_matrix->points, (unsigned long long)(adj_matrix->num_points + 1) * sizeof(Point));
        adj_matrix->adj = (double**)realloc(adj_matrix->adj, (unsigned long long)(adj_matrix->num_points + 1) * sizeof(double*));
        if (adj_matrix->points == NULL || adj_matrix->adj == NULL)
        {
            printf("malloc failed in point add process!\n");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }

        // initial the new pointer to NULL
        adj_matrix->adj[adj_matrix->num_points] = NULL;

        for (int i = 0; i < adj_matrix->num_points + 1; i++) 
        {
            adj_matrix->adj[i] = (double*)realloc(adj_matrix->adj[i], (unsigned long long)(adj_matrix->num_points + 1) * sizeof(double));
            if (i == adj_matrix->num_points && adj_matrix->adj[i] == NULL) 
            {
                printf("malloc failed in point add process!\n");
                free_adjacency_matrix(adj_matrix);
                exit(ERROR_MALLOC_FAILED);
            }
        }

        // initial the new row and col to INF
        for (int i = 0; i < adj_matrix->num_points + 1; i++) 
        {
            adj_matrix->adj[i][adj_matrix->num_points] = INF;
            adj_matrix->adj[adj_matrix->num_points][i] = INF;
        }
        // The point on the diagonal is 0
        adj_matrix->adj[adj_matrix->num_points][adj_matrix->num_points] = 0;
    }

    adj_matrix->points[adj_matrix->num_points].id = id;
    adj_matrix->points[adj_matrix->num_points].lat = lat;
    adj_matrix->points[adj_matrix->num_points].lon = lon;
    adj_matrix->num_points++;
}


/* Add an edge to adjacency matrix */
void add_edge_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long id, long long from,
    long long to, double length, double veg, double arch, double land, double speedLimit, int POIs[], int len_pois)
{
    if (hashtable_lookup(adj_matrix->point_hash, from) == -1 || hashtable_lookup(adj_matrix->point_hash, to) == -1)
    {
        printf("Error: Invalid edge (%lld, %lld)\n", from, to);
        free_adjacency_matrix(adj_matrix);
        exit(ERROR_FIND_PATH_FAILED);
    }

    if (adj_matrix->num_edges == 0)
    { // If it's the first edge, allocate dynamically
        adj_matrix->edges = (Edge*)malloc(sizeof(Edge));
        if (adj_matrix->edges == NULL)
        {
            printf("malloc failed");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_matrix->edges = (Edge*)realloc(adj_matrix->edges, (unsigned long long)(adj_matrix->num_edges + 1) * sizeof(Edge));
        if (adj_matrix->edges == NULL)
        {
            printf("malloc failed");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    // init pois
    adj_matrix->edges[adj_matrix->num_edges].pois = NULL;

    /* 存入边的信息到edges中 */
    adj_matrix->edges[adj_matrix->num_edges].id = id;
    adj_matrix->edges[adj_matrix->num_edges].from = from;
    adj_matrix->edges[adj_matrix->num_edges].to = to;
    adj_matrix->edges[adj_matrix->num_edges].length = length;
    adj_matrix->edges[adj_matrix->num_edges].veg = veg;
    adj_matrix->edges[adj_matrix->num_edges].arch = arch;
    adj_matrix->edges[adj_matrix->num_edges].land = land;
    adj_matrix->edges[adj_matrix->num_edges].speedLimit = speedLimit;
    adj_matrix->edges[adj_matrix->num_edges].num_pois = len_pois;
    
    /* add POIs to edges */
    if (len_pois != 0)
    {
        adj_matrix->edges[adj_matrix->num_edges].pois = (int*)malloc(sizeof(int) * len_pois);
        for (int i = 0; i < len_pois; i++)
        {
            adj_matrix->edges[adj_matrix->num_edges].pois[i] = POIs[i];
            printf("%d ", adj_matrix->edges[adj_matrix->num_edges].pois[i]);
        }
        printf("\n");
    }

    /* Find information about the points from and to from the hash table    */
    int from_index = hashtable_lookup(adj_matrix->point_hash, from);
    int to_index = hashtable_lookup(adj_matrix->point_hash, to);
    adj_matrix->adj[from_index][to_index] = length;
    adj_matrix->adj[to_index][from_index] = length;
    adj_matrix->num_edges++;
}


void add_geoms_to_adjacency_matrix(AdjacencyMatrix* adj_matrix, long long geom_id, int len, char tokens[][20])
{
    if (adj_matrix->num_geoms == 0)
    { // If it's the first geom, allocate dynamically
        adj_matrix->geoms = (Geom*)malloc(sizeof(Geom));
        if (adj_matrix->geoms == NULL)
        {
            printf("malloc failed");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    else
    { // Otherwise, memory needs to be reallocated
        adj_matrix->geoms = (Geom*)realloc(adj_matrix->geoms, (unsigned long long)(adj_matrix->num_geoms + 1) * sizeof(Geom));
        if (adj_matrix->geoms == NULL)
        {
            printf("malloc failed");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_MALLOC_FAILED);
        }
    }
    adj_matrix->geoms[adj_matrix->num_geoms].nodes = NULL;
    adj_matrix->geoms[adj_matrix->num_geoms].id = geom_id;
    adj_matrix->geoms[adj_matrix->num_geoms].len_nodes = len - 3;
    adj_matrix->geoms[adj_matrix->num_geoms].nodes = (long long*)malloc(sizeof(long long) * (len-3));
    for (int i = 2; i < len - 1; i++)
    {
        long long node_id = 0;
        if (sscanf(tokens[i], "node=%lld", &node_id) != 1)
        {
            fprintf(stderr, "Error: Failed to parse geom data in %d\n");
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_DATA_READ_FAILED);
        }
        int result = 0;
        result = hashtable_lookup(adj_matrix->point_hash, node_id);
        if (result == -1)
        {
            printf("Error: Invalid node %lld in geom id %lld\n", node_id, geom_id);
            free_adjacency_matrix(adj_matrix);
            exit(ERROR_FIND_PATH_FAILED);
        }
        adj_matrix->geoms[adj_matrix->num_geoms].nodes[i - 2] = result;
    }
    adj_matrix->num_geoms++;
}