#ifndef GRAPH
#define GRAPH

int** takeinput(int *V, int *N) {
    FILE *inputFile;
    char buffer[256];

    inputFile = fopen(INPUT, "r");
    if (inputFile == NULL) {
        perror("Error opening input file");
        exit(1);
    }

    fscanf(inputFile, "%d", V);
    fscanf(inputFile, "%d", N);

    int** adjmat = (int**)malloc(sizeof(int*) * (*V));
    if (adjmat == NULL) {
        perror("Error allocating memory for adjacency matrix");
        exit(1);
    }

    for (int i = 0; i < *V; i++) {
        adjmat[i] = (int*)malloc(sizeof(int) * (*V));
        if (adjmat[i] == NULL) {
            perror("Error allocating memory for adjacency matrix row");
            exit(1);
        }
        for (int j = 0; j < *V; j++) {
            adjmat[i][j] = 0;
        }
    }

    for (int i = 0; i < *N; i++) {
        int u, v;
        if (fscanf(inputFile, "%d %d", &u, &v) != 2) {
            perror("Error reading edge from input file");
            exit(1);
        }
        if (u >= 0 && u < *V && v >= 0 && v < *V) {
            adjmat[u][v] = 1;
            adjmat[v][u] = 1;
        }
    }

    fclose(inputFile);

    return adjmat;
}



// void bfs_nicer(int start, const vector<vector<int>>& graph) {
//     // Get the number of vertices in the graph
//     int V = graph.size();
    
//     // Initialize a visited array to keep track of visited nodes
//     vector<bool> visited(V, false);
    
//     // Initialize a queue (FIFO) for BFS traversal
//     queue<int> q;
    
//     // Start BFS from the starting node
//     q.push(start);
//     visited[start] = true;
    
//     // Traverse the graph using BFS
//     while (!q.empty()) {
//         // Dequeue a vertex from the queue
//         int node = q.front();
//         q.pop();
        
//         // Process the current node (print here, can be modified)
//         cout << node << " ";
        
//         // Visit all adjacent vertices of the dequeued node
//         for (int neighbor : graph[node]) {
//             if (!visited[neighbor]) {
//                 visited[neighbor] = true;
//                 q.push(neighbor);
//             }
//         }
//     }
// }


void traversal_bfs(LL** adjlist, int V, int vert, vector* visited, vector* answer) {
    // Allocating memory is necessary otherwise it will cause unexpected behaviour
    queue* q = (queue*)malloc(sizeof(queue));
    q_init(q);
    q_push_back(q, vert);
    vec_change(visited,vert, 1);

    while (!q_isempty(q)) {  
        int temp = q_get_front(q);
        q_pop_front(q);
        
        vec_push_back(answer, temp);
        // vec_print(answer);

        // Previously it was *(adjlist+temp)->head;  paranthesis has to be added beacause it is evaluated in order.
        node* tempnode = (*(adjlist+temp))->head;
        // printf("tempnodedata : %d\n",tempnode->data);
        // printf("visited size :%d\n",vec_at(visited, tempnode->data));

        while (tempnode != NULL) {
            if (vec_at(visited, tempnode->data) != 1) {
                q_push_back(q, tempnode->data);
                vec_change(visited, tempnode->data, 1);
            }
            tempnode = tempnode->next;
        }
    }

    // Free the queue after use
    q_free(q);
}

vector* BFS(LL** adjlist, int V) {
    vector* visited = (vector*)malloc(sizeof(vector));
    vector* answer = (vector*)malloc(sizeof(vector));

    vec_init(visited);
    vec_init(answer);

    for (int i = 0; i < V; i++) {
        vec_push_back(visited, 0);
    }

    for (int i = 0; i < V; i++) {
        if (vec_at(visited, i) != 1) {
            traversal_bfs(adjlist, V, i, visited, answer);
        }
    }

    // Free the visited vector after use
    vec_free(visited);
    free(visited);

    return answer;
}

void traversal_dfs(LL** adjlist,int V,int vert,vector* visited,vector* answer){
    vec_change(visited,vert,1);
    vec_push_back(answer,vert);
    node* tempnode = (*(adjlist+vert))->head;
    while(tempnode!=NULL){
        if(vec_at(visited,tempnode->data)!=1){
            traversal_dfs(adjlist,V,tempnode->data,visited,answer);
        }
        tempnode = tempnode->next;
    }
}


vector* DFS(LL** adjlist,int V){
    vector* visited = (vector*)malloc(sizeof(vector));
    vector* answer = (vector*)malloc(sizeof(vector));

    vec_init(visited);
    vec_init(answer);

    for(int i = 0;i<V;i++){
        vec_push_back(visited,0);
    }

    for(int i =0;i<V;i++){
        if(vec_at(visited,i) != 1){
            traversal_dfs(adjlist,V,i,visited,answer);
        }
    }
    vec_free(visited);
    free(visited);

    return answer;
}


LL** mattolist(int** adjmat,int V){
    LL** list = (LL**)malloc(sizeof(LL*)*V);

    for (int i = 0; i < V; i++) {
        list[i] = (LL*)malloc(sizeof(LL));
        ll_init(list[i]);
    }

    for(int i = 0;i<V;i++){
        for(int j = 0;j<V;j++){
            if(adjmat[i][j] == 1){
                ll_insertatfront(*(list+i),j);
                // ll_insertatfront(*(list+j),i);
            }
        }
    }

    return list;
}


#endif