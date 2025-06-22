#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//structure for friend node(each friend of a user)
typedef struct friend{
    char name[10];
    struct friend *next;
}FRIEND;

//structure for user node
typedef struct node
 {
    char name[10];
    struct friend *friends;
 }NODE;

//function declarations
void print_network(NODE *person[], int n);
void list_friends(NODE *person[], int n,char *name);
void mutual_friends(NODE *person[],char *name1,char *name2, int n);
void recommend_friends(NODE *person[],char*name, int n,int degree);
void most_popular(NODE *person[],int n);
void shortest_path(NODE *person[],char *name1,char*name2, int n);
int find_index(NODE *person[],char* name,int n);
void add_friend(NODE *person[], int personIndex, int friendIndex);


int main()
{
    struct node *person[10]; //array for storing user names
    int n = 7; 
    
    char *names[] = {"Alice", "Bob", "Charlie", "Dave", "Eve", "Karen", "John"};

    //initializing user names and assigning friends to NULL
    for (int i=0; i<n; i++)
    {
        person[i] = (NODE *)malloc(sizeof(NODE)); 
        strcpy(person[i] -> name, names[i]);
        person[i] -> friends = NULL;
    }

    int no_fs = 7; //number of friendships

    char *friendships[][2] = 
    {
        {"Alice", "Bob"}, //friendships[0][0]="Alice" friendships[0][1]="Bob"
        {"Alice", "Eve"},
        {"Alice", "Dave"},
        {"Alice", "Charlie"},
        {"Bob", "Charlie"},
        {"Bob", "John"},
        {"Karen", "Dave"}
    };


    //adding friendship
    for (int i=0; i<no_fs; i++){
        int index1 = find_index(person, friendships[i][0], n);
        int index2 = find_index(person, friendships[i][1], n);
        //checks if both names are found
        if (index1 != -1 && index2 != -1) {
            add_friend(person, index1, index2);
            add_friend(person, index2, index1);
        }
    }

    int choice;
    char name1,name2;
    while(1)
    {
        printf("1.Print the network\n2.List friends of user\n");
        printf("3.Finding mutual friends\n4.Find recommendations\n5.Identifying popular users\n");
        printf("6.Finding shortest path\n7.Exit\n");
        printf("Enter your choice:  ");
        scanf("%d",&choice);
        
        switch(choice)
        {
            case 1: {
                        printf("The network :\n");
                        print_network(person,n);
                    }break;

            case 2: {
                        char name[10];
                        printf("Enter the name: ");
                        scanf("%s",&name);
                        list_friends(person,n,name);
                    }break;

            case 3:{
                        char name1[10],name2[10];
                        printf("Enter the names to find mutual friends : ");
                        scanf("%s %s",&name1,&name2);
                        mutual_friends(person,name1,name2,n);
                   }break;

            case 4:{
                        char name[10];
                        int degree;
                        printf("Enter the name to find recommended friends and the degree: ");
                        scanf("%s %d",&name,&degree);
                        recommend_friends(person,name,n,degree);
                   }break; 

            case 5:{
                        most_popular(person,n);
                   }break; 

            case 6:{
                        char name1[10],name2[10];
                        printf("Enter the names to find shortest path: ");
                        scanf("%s %s",&name1,&name2);
                        shortest_path(person,name1,name2,n);
                   }break;               


            default:{
                        exit(0);
                        break;

                   }                             

        }
    }
}

//finding index of a username
int find_index(NODE *person[],char* name,int n) {
    for (int i = 0; i < n; i++) {
        if (strcmp(person[i]->name, name) == 0) {
            return i;
        }
    }
    return -1;
}

//adding friendship
void add_friend(NODE *person[], int index1, int index2){
    FRIEND *newFriend = (FRIEND *)malloc(sizeof(FRIEND));
    strcpy(newFriend->name, person[index2]->name);
    newFriend->next = person[index1]->friends;
    person[index1]->friends = newFriend;
}

//printing network
void print_network(NODE *person[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%s: ", person[i]->name); //print name of user
        FRIEND *temp = person[i]->friends; //temporary pointer to FRIEND
        while (temp != NULL) {
            printf("%s ", temp->name);
            temp = temp->next;
        }
        printf("\n");
    }
}

//printing list of friends for a given username
void list_friends(NODE *person[], int n, char *name) 
{
    int index1 = find_index(person, name, n);
    if (index1 == -1) {
        printf("User not found.\n");
        return;
    }

    FRIEND *temp = person[index1]->friends;  //temporary pointer to FRIEND
    if (temp == NULL) {
        printf("%s has no friends.\n", name);
    } else {
        printf("Friends of %s:\n", name);
        while (temp != NULL) {
            printf("- %s\n", temp->name);
            temp = temp->next;
        }
    }
}

//finding mutual friends btw 2 given usernames
void mutual_friends(NODE *person[],char *name1,char *name2, int n)
{
   int index1 = find_index(person, name1, n);
   int index2 = find_index(person, name2, n);
    
    if (index1 == -1 || index2 == -1) {
        printf("One or both users not found.\n");         //Check if they are present or not 
        return;
    }
    printf("Mutual friends between %s and %s:\n", name1, name2);
    FRIEND* temp1 = person[index1]->friends;                           //This temp1 contains the list of all friends of name1
    while (temp1 != NULL) 
    {
        FRIEND* temp2 = person[index2]->friends;                       //This temp2 contains the list of all friends of name2
        while (temp2 != NULL) 
        {
            if (strcmp(temp1->name, temp2->name) == 0)                    //Checks the friends in both the list
            {
                printf("- %s\n", temp1->name);
            }
            temp2 = temp2->next;
        }
        temp1 = temp1->next;
    }
}


void recommend_friends(NODE *person[], char *name, int n, int degree) {
    int index1 = find_index(person, name, n); // Find the index of the user

    if (index1 == -1) {
        printf("User not found.\n");
        return;
    }

    int visited[10] = {0};  // To track visited nodes
    int level[10] = {0};    // To track degree level of connections
    int queue[10];          // Queue for BFS to store friend indices
    int f = 0, r = 0;

    queue[r++] = index1;  // Start BFS from the specified user
    visited[index1] = 1;

    printf("Friend recommendations for %s within %d levels:\n", name, degree);

    while (f < r) {
        int current = queue[f++];
        FRIEND *curfriends = person[current]->friends;

        while (curfriends != NULL)
         {
            int findex = find_index(person, curfriends->name, n);

            if (findex != -1 && !visited[findex]) 
            {
                level[findex] = level[current] + 1;

                // Only recommend if within degree and not a direct friend (level > 1)
                if (level[findex] <= degree && level[findex] > 1) 
                {
                    printf("- %s\n", curfriends->name);
                }

                // Continue BFS if within degree
                if (level[findex] < degree) 
                {
                    queue[r++] = findex;  // Add to queue to continue BFS
                }

                visited[findex] = 1;  // Mark as visited
            }
            curfriends = curfriends->next;
        }
    }
}


void most_popular(NODE *person[], int n) {
    int fcounts[n]; //array to store friend counts of each person
    for (int i = 0; i < n; i++)       //count friends of each person
    {
        int fcount = 0;
        FRIEND *temp = person[i]->friends;
        while (temp != NULL) 
        {
            fcount++;
            temp = temp->next;
        }
        fcounts[i] = fcount;
    }

    for (int i = 0; i < n - 1; i++) 
    {
        for (int j = i + 1; j < n; j++) 
        {
            if (fcounts[i] < fcounts[j]) //bubble sort of friends array by popularity
            { 
                NODE *ptemp = person[i];
                person[i] = person[j];
                person[j] = ptemp;

                int tempc = fcounts[i];
                fcounts[i] = fcounts[j];
                fcounts[j] = tempc;
            }
        }
    }

    printf("Users ranked by popularity: \n");
    for (int i = 0; i < n; i++) {
        printf("%s: %d friends\n", person[i]->name, fcounts[i]);
    }
}


void shortest_path(NODE *person[], char *name1, char *name2, int n) {
    int start = find_index(person, name1, n);  
    int end = find_index(person, name2, n);    
    if (start == -1 || end == -1) 
    {
        printf("One or both users not found.\n");
        return;
    }
    int vis[n];  
    int prev[n];     
    for (int i = 0; i < n; i++) 
    {
        vis[i] = 0;  
        prev[i] = -1;    
    }
    int queue[n]; //BFS Queue
    int front = 0, rear = 0;
    queue[rear++] = start;  
    vis[start] = 1;      
    while (front < rear) //BFS
    {
        int current = queue[front++];  // Get the current user to process
        FRIEND *temp = person[current]->friends;  // Get the list of friends of the current user
        while (temp != NULL) 
        {
            int next = find_index(person, temp->name, n); 
            if (!vis[next]) 
            {
                vis[next] = 1;         
                prev[next] = current;     
                queue[rear++] = next;      
                if (next == end) 
                {
                    printf("Shortest path from %s to %s:\n", name1, name2);
                    int path[n];
                    int path_len = 0;

                    for (int i = next; i != -1; i = prev[i]) 
                    {
                        path[path_len++] = i;  
                    }

                    for (int i = path_len - 1; i >= 0; i--) 
                    {
                        printf("%s ", person[path[i]]->name);  
                        if (i > 0) printf("-> "); 
                    }
                    printf("\n");
                    return;  
                }
            }
            temp = temp->next;  
        }
    }
    printf("No path found between %s and %s.\n", name1, name2);
}
