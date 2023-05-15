// This program includes header files for standard input/output streams, 
// commonly used utility libraries, and custom implementation files.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hashTable.h" // Header file for hash table implementation
#include "mapAdjMatrix.h" // Header file for adjacency matrix graph representation
#include "pathFinder.h" // Header file for path finding algorithms
#include "mapEditor.h" // Header file for editing map data
#include "mapError.h" // Header file for error handling in map operations

#define NO_PATH -1 // Constant value representing no path found between two points
#define NO_LOCATION_NEEDED 1 // Constant value representing that a location does not need to be found
#define NODE_NEEDED 2 // Constant value representing that a node is needed in the map
#define POINT_OF_INTEREST 3 // Constant value representing a point of interest in the map

int ask_find_path(AdjacencyMatrix* adj_matrix, int* path)
{
    // Initialize variables
    long long input_start = 0;
    long long input_end = 0;
    long long start = 0;
    long long end = 0;
    int valid_input = 0;
    printf("\n");

    // Get the starting point node ID from the user
    while (!valid_input)
    {
        printf("Please enter the ID of the starting point: ");
        // Check data type long long (id)
        char input_buffer[100];
        if (fgets(input_buffer, 100, stdin) == NULL)
        {
            printf("Input error! Please try again!\n");
            continue;
        }
        int len = strlen(input_buffer);
        if (input_buffer[len - 1] == '\n')
        {
            input_buffer[len - 1] = '\0';
        }
        if (!check_lld(input_buffer))
        {
            printf("Input error! Please try again!\n");
            continue;
        }
        if (sscanf(input_buffer, "%lld", &input_start) != 1)
            continue;


        start = hashtable_lookup(adj_matrix->point_hash, input_start);
        if (start == -1)
        {   
            // An error occurs. Restart the loop
            printf("Invalid starting point node ID %lld. Please try again.\n", input_start);
            continue;
        }
        valid_input = 1;
    }

    valid_input = 0;

    // Get the ending point node ID from the user
    while (!valid_input)
    {
        printf("Please enter the ID of the ending point: ");
        // Check data type long long (id)
        char input_buffer[100];
        if (fgets(input_buffer, 100, stdin) == NULL)
        {
            printf("Input error! Please try again!\n");
            continue;
        }
        int len = strlen(input_buffer);
        if (input_buffer[len - 1] == '\n')
        {
            input_buffer[len - 1] = '\0';
        }
        if (!check_lld(input_buffer))
        {
            printf("Input error! Please try again!\n");
            continue;
        }
        if (sscanf(input_buffer, "%lld", &input_end) != 1)
            continue;

        end = hashtable_lookup(adj_matrix->point_hash, input_end);
        if (end == -1)
        {
            // An error occurs. Restart the loop
            printf("Invalid ending point node ID %lld. Please try again.\n", input_end);
            continue;
        }
        valid_input = 1;
    }

    // Ask whether there is any location needs to be passed through
    long long location= 0;
    int POI = 0;
    int result = ask_location_POI(adj_matrix, &location, &POI);

    // If there is no location to be passed through
    if (result == NO_LOCATION_NEEDED)
    {
        char line[100];
        printf("\nDo you want the shortest route or the fastest route?\n");
        printf("1. Shortest route\n");
        printf("2. Fastest route\n");
        printf("Enter your choice:");

        while (1)
        {
            // Read the input line
            if (fgets(line, sizeof(line), stdin) == NULL)
            {
                printf("Reading failed, please try again:\n");
                continue;
            }
            // Remove the newline character at the end of the line
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }

            // Compare the input with available options
            if (strcmp(line, "1") == 0)
            {
                double len_path = dijkstra(adj_matrix, start, end, path);
                // printf the path
                if (len_path == INF)
                {
                    printf("No path can be found!");
                    break;
                }
                printf("The path: ");
                for (int i = 0; i < adj_matrix->num_points; i++)
                {
                    if (path[i] < 0 || path[i] > adj_matrix->num_points - 1)
                    {
                        break;
                    }
                    // print the point id
                    if (i == 0)
                        printf("%lld", adj_matrix->points[path[i]].id);
                    else
                        printf("->%lld", adj_matrix->points[path[i]].id);
                }
                printf("\n Total length: %lf\n", len_path);
                break;
            }
            else if (strcmp(line, "2") == 0)
            {
                // change the every matrix value from length to time
                for (int i = 0; i < adj_matrix->num_edges; i++)
                {
                    int index_form = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[i].from);
                    int index_to = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[i].to);
                    double time = adj_matrix->edges[i].length / adj_matrix->edges[i].speedLimit;
                    adj_matrix->adj[index_form][index_to] = time;
                    adj_matrix->adj[index_to][index_form] = time;
                }
                double len_time = dijkstra(adj_matrix, start, end, path);
                // printf the path
                if (len_time == INF)
                {
                    printf("No path can be found!");
                    break;
                }
                printf("The path: ");
                for (int i = 0; i < adj_matrix->num_points; i++)
                {
                    if (path[i] < 0 || path[i] > adj_matrix->num_points - 1)
                    {
                        break;
                    }
                    // print the point id
                    if (i == 0)
                        printf("%lld", adj_matrix->points[path[i]].id);
                    else
                        printf("->%lld", adj_matrix->points[path[i]].id);
                }
                printf("\n Total time: %lf\n", len_time);
                break;
            }
            else
            {
                printf("You did not enter a valid option (1 or 2).\n");
                printf("Please try again:");
            }
        }
    }

    // If there is a node to be passed through
    if (result == NODE_NEEDED)
    {
        location = hashtable_lookup(adj_matrix->point_hash, location);
        dijkstra_via(adj_matrix, start, end, location, path);
    }

    // If there is a point of interest to be visited
    if (result == POINT_OF_INTEREST)
    {
        // Record the length of the path
        double length_of_path = INF;
        for (int n = 0; n < adj_matrix->num_edges; n++)
        {
            // Skip edges with no POI
            if (adj_matrix->edges[n].num_pois == 0)
            {
                continue;
            }

            // Check if the POI value is correct
            for (int j = 0; j < adj_matrix->edges[n].num_pois; j++)
            {
                if (adj_matrix->edges[n].pois[j] == POI)
                {
                    // meet POI, the progress follows
                    int* path1 = (int*)malloc(sizeof(int) * (adj_matrix->num_points));
                    memset(path1, -1, sizeof(int) * adj_matrix->num_points);
                    int* path2 = (int*)malloc(sizeof(int) * (adj_matrix->num_points));
                    memset(path2, -1, sizeof(int) * adj_matrix->num_points);
                    int* path3 = (int*)malloc(sizeof(int) * (adj_matrix->num_points));
                    memset(path3, -1, sizeof(int) * adj_matrix->num_points);
                    int* path4 = (int*)malloc(sizeof(int) * (adj_matrix->num_points));
                    memset(path4, -1, sizeof(int) * adj_matrix->num_points);

                    long long edge_node1 = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[n].from);
                    long long edge_node2 = hashtable_lookup(adj_matrix->point_hash, adj_matrix->edges[n].to);
                    
                    // Calculate two different routes
                    double route1 = dijkstra(adj_matrix, start, edge_node1, path1) + dijkstra(adj_matrix, edge_node2, end, path2) + adj_matrix->adj[edge_node1][edge_node2];
                    double route2 = dijkstra(adj_matrix, start, edge_node2, path3) + dijkstra(adj_matrix, edge_node1, end, path4) + adj_matrix->adj[edge_node1][edge_node2];
                    
                    // If the old path was shorter, don't replace it
                    if (length_of_path < route1 && length_of_path < route2)
                    {
                        continue;
                    }
                    
                    // Compare the two routes and choose the shorter one
                    if (route1 >= route2)
                    {
                        // reset path
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
                        // reset path
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
                    // free memory
                    free(path1);
                    free(path2);
                    free(path3);
                    free(path4);
                }
            }
        }
        // printf the path
        if (length_of_path == INF)
        {
            printf("No path can be found!");
            return FUNCTION_SUCCESS;
        }
        printf("The path: ");
        for (int i = 0; i < adj_matrix->num_points; i++)
        {
            if (path[i] < 0 || path[i] > adj_matrix->num_points - 1)
            {
                break;
            }
            // print points id
            if (i == 0)
                printf("%lld", adj_matrix->points[path[i]].id);
            else
                printf("->%lld", adj_matrix->points[path[i]].id);
        }
        printf("Total length: %lf\n", length_of_path);
    }
    return FUNCTION_SUCCESS;
}


// This function asks the user if they need to pass through a specific location or POI on the map.
int ask_location_POI(AdjacencyMatrix* adj_matrix, long long* location, int* POI)
{
    char line[100];
    int operate_type;

    // Display options for the user to choose from
    printf("\nDo you need to pass through a specific node or POI(point of interest) on the map?\n");
    printf("1. No need\n");
    printf("2. Need to pass through a node on the map\n");
    printf("3. Need to pass through a point of interest on the map\n");
    printf("Please enter your selection: ");

    while (1)
    {
        // Read user input
        if (fgets(line, sizeof(line), stdin) == NULL)
        {
            printf("Reading failed, please try again:\n");
            continue;
        }
        // Remove newline character from user input
        if (line[strlen(line) - 1] == '\n')
        {
            line[strlen(line) - 1] = '\0';
        }

        // Check user's selection and set operate_type accordingly
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
            printf("You did not enter a correct option (1 or 2 or 3).\n");
            printf("Please try again: ");
        }
    }

    // Flag to indicate whether user input is valid
    int flag = 0;
    // Define a buffer
    char buf[100];

    switch (operate_type)
    {
        case 1:
            return NO_LOCATION_NEEDED;
        
        case 2:
            flag = 0;
            // Loop to repeatedly prompt user to input node ID until valid input is received
            while (!flag)
            {
                printf("\nPlease enter the node ID you want to pass through: \n");
                // Input node ID and check if it is valid
                printf("Enter node ID: ");
                if (fgets(buf, sizeof(buf), stdin) == NULL)
                {
                    printf("Invalid input! Please try again!\n");
                    continue;
                }
                int len = strlen(buf);
                if (buf[len - 1] == '\n')
                {
                    buf[len - 1] = '\0';
                }
                if (!check_lld(buf))
                {
                    printf("Invalid input! Please try again!\n");
                    continue;
                }
                if (sscanf(buf, "%lld", location) != 1)
                    continue;

                // Check if the node exists on the map based on its coordinates (here we only check if it exists in the table)
                int result = is_point_exist(adj_matrix, *location, 53.81, -1.55, 0);

                if (result != EXIST)
                {
                    // Node does not exist
                    printf("The node ID %lld does not exist! Please try again!\n", *location);
                    continue;
                }
                flag = 1;
            }
            return NODE_NEEDED;
        
        case 3:
            flag = 0;
            // Loop to repeatedly prompt user to input POI ID until valid input is received
            while (!flag)
            {
                printf("\nPlease enter the POI ID you want to pass through: \n");
                // Input POI ID and check if it is valid
                printf("Enter point of interest ID: ");
                if (fgets(buf, sizeof(buf), stdin) == NULL)
                {
                    printf("Invalid input! Please try again!\n");
                    continue;
                }
                int len = strlen(buf);
                if (buf[len - 1] == '\n')
                {
                    buf[len - 1] = '\0';
                }
                if (!check_lld(buf))
                {
                    printf("Invalid input! Please try again!\n");
                    continue;
                }
                if (sscanf(buf, "%d", POI) != 1)
                    continue;

                // Check if the POI exists on the map
                int result = is_POI_exist(adj_matrix, *POI);
                if (result != EXIST)
                {
                    // POI does not exist
                    printf("The POI ID %d does not exist! Please try again!\n", *POI);
                    continue;
                }
                flag = 1;
            }
            return POINT_OF_INTEREST;
    }
    return FUNCTION_SUCCESS;
}


double dijkstra(AdjacencyMatrix* adj_matrix, int start, int end, int* path)
{
    // Allocate memory space for the distance array, visited array, and predecessor array.
    double* dist;
    dist = (double*)malloc(sizeof(double) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        dist[i] = INF;
    }

    int* st;
    st = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        st[i] = 0;
    }

    int* pre;
    pre = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    for (int i = 0; i < adj_matrix->num_points; i++)
    {
        // Initialize the predecessor array with -1, meaning that no node has been visited yet.
        pre[i] = -1;
    }

    dist[start] = 0;

    // Use the visited array to keep track of the nodes that have been visited.
    // Use the distance array to keep track of the shortest distance from the starting node to each node.
    // Use the predecessor array to keep track of the previous node in the shortest path.
    for (int i = 0; i < adj_matrix->num_points; i++) 
    {
        int k = -1;
        for (int j = 0; j < adj_matrix->num_points; j++) 
        {
            if (!st[j] && (k == -1 || dist[k] > dist[j])) 
            {
                // Choose the unvisited node with the smallest distance from the starting node.
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < adj_matrix->num_points; j++) 
        {
            if (!st[j] && dist[j] > dist[k] + adj_matrix->adj[k][j]) 
            {
                dist[j] = dist[k] + adj_matrix->adj[k][j];
                // Update the predecessor array to record the previous node in the shortest path.
                pre[j] = k;
            }
        }
    }

    if (dist[end] == INF)
    {
        printf("There is no available path from node %lld to node %lld.\n", adj_matrix->points[start].id, adj_matrix->points[end].id);
        free(dist);
        free(st);
        free(pre);
        return NO_PATH;
    }
    else
    {
        // If a path exists, construct the path array by tracing back from the ending node to the starting node.
        int len = 0;
        int temp = end;
        while (temp != start)
        {
            path[len++] = temp;
            temp = pre[temp];
        }
        path[len++] = start;
        // Reverse the order of the elements in the path array to obtain the actual path from the starting node to the ending node.
        for (int i = 0; i < len / 2; i++)
        {
            int tmp = path[i];
            path[i] = path[len - i - 1];
            path[len - i - 1] = tmp;
        }
    }
    double temp = dist[end];
    
    // free memory
    free(dist);
    free(st);
    free(pre);
    
    // return length of temp
    return temp;
}


// This function calculates the shortest path from start to end via a given via point using Dijkstra's algorithm.
void dijkstra_via(AdjacencyMatrix* adj_matrix, int start, int end, int via, int* path) 
{
    // Initialize arrays for distance, visited nodes, and previous nodes
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

    // Set initial values for starting and via nodes
    dist_start[start] = 0;
    dist_tmp[via] = 0;

    // Visit nodes from start to via using Dijkstra's algorithm
    for (int i = 0; i < adj_matrix->num_points; i++) 
    {
        int k = -1;
        for (int j = 0; j < adj_matrix->num_points; j++) 
        {
            if (!st[j] && (k == -1 || dist_start[k] > dist_start[j])) 
            {
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < adj_matrix->num_points; j++) 
        {
            if (!st[j] && dist_start[j] > dist_start[k] + adj_matrix->adj[k][j]) 
            {
                dist_start[j] = dist_start[k] + adj_matrix->adj[k][j];
                pre_start[j] = k;
            }
        }
        if (k == via) break;
    }

    // If there is no available path from start to via, return NO_PATH
    if (dist_start[via] == INF) 
    {
        printf("There is no available path from node %lld to node %lld via node %lld.\n", adj_matrix->points[start].id, adj_matrix->points[end].id, adj_matrix->points[via].id);
        free(dist_start);
        free(dist_tmp);
        free(st);
        free(pre_start);
        free(pre_tmp);
        return;
    }

    // Reset visited node array
    for (int i = 0; i < adj_matrix->num_points; i++) 
    {
        st[i] = 0;
    }

    // Calculate shortest path and distance from via to end using Dijkstra's algorithm
    for (int i = 0; i < adj_matrix->num_points; i++) 
    {
        int k = -1;
        for (int j = 0; j < adj_matrix->num_points; j++) 
        {
            if (!st[j] && (k == -1 || dist_tmp[k] > dist_tmp[j])) 
            {
                k = j;
            }
        }
        st[k] = 1;

        for (int j = 0; j < adj_matrix->num_points; j++) 
        {
            if (!st[j] && dist_tmp[j] > dist_tmp[k] + adj_matrix->adj[k][j]) 
            {
                dist_tmp[j] = dist_tmp[k] + adj_matrix->adj[k][j];
                pre_tmp[j] = k;
            }
        }
        if (k == end) break;
    }

    // If there is no available path from via to end, return NO_PATH
    if (dist_tmp[end] == INF) {
        printf("There is no available path from node %lld to node %lld via node %lld.\n", adj_matrix->points[start].id, adj_matrix->points[end].id, adj_matrix->points[via].id);
        free(dist_start);
        free(dist_tmp);
        free(st);
        free(pre_start);
        free(pre_tmp);
        return;
    }

    // Combine the two paths into a final shortest path
    // first path
    int* tmp_path1;
    tmp_path1 = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    int len1 = 0;
    int tmp = via;
    while (tmp != start) 
    {
        tmp_path1[len1++] = tmp;
        tmp = pre_start[tmp];
    }
    tmp_path1[len1++] = start;

    // second path
    int* tmp_path2;
    tmp_path2 = (int*)malloc(sizeof(int) * adj_matrix->num_points);
    int len2 = 0;
    tmp = end;
    while (tmp != via) 
    {
        tmp_path2[len2++] = tmp;
        tmp = pre_tmp[tmp];
    }
    tmp_path2[len2++] = via;

    int len = len1 + len2 - 1;
    for (int i = 0; i < len1; i++) 
    {
        path[i] = tmp_path1[len1 - 1 - i];
    }
    for (int i = 1; i < len2; i++) 
    {
        path[len1 + i - 1] = tmp_path2[len2 - 1 - i];
    }

    // Output the shortest path and distance
    printf("The shortest distance from node %lld to node %lld via node %lld is %lf.\n", adj_matrix->points[start].id, adj_matrix->points[end].id, adj_matrix->points[via].id, dist_start[via] + dist_tmp[end]);
    printf("The path is: %lld", adj_matrix->points[path[0]].id);
    for (int i = 1; i < len; i++) 
    {
        printf("->%lld", adj_matrix->points[path[i]].id);
    }
    printf("\n");

    // Free memory
    free(tmp_path1);
    free(tmp_path2);
    free(dist_start);
    free(dist_tmp);
    free(st);
    free(pre_start);
    free(pre_tmp);
}
