#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct team_member {
    int age;
    char *name;
    struct team_member *next;
};

int main(){
    
    char *name[]={"Andy","Mike","Paul",NULL};
    int age[3]={26,27,28};
    
    // Create head
    struct team_member *head = (struct team_member*)malloc(sizeof(struct team_member));
    
    printf("=== Linked List demo === \n");
    
    //Add data at head
    head->name=name[0];
    head->age=age[0];
    head->next = NULL;
    
    //Now, let current point to Head
    struct team_member *current = head;
    
    for(int i=1;i<3;i++)
    {
        //let current->next point to new node
        struct team_member *tmp_t = (struct team_member*)malloc(sizeof(struct team_member));
        current->next = tmp_t;
        
        //let current point to new node and add data
        current = current->next;
        current->age=age[i];
        current->name=name[i];
        current->next = NULL;
    }
    
    // let current point to head again
    current = head;
    
    //dump linked list data
    while(current)
    {
        printf("Age:%d\tName:%s\n",current->age,current->name);
        current = current->next;
    }
    // start delete Mike
    printf("=== Linked List delete demo === \n");
    char * dname="Mike";
    printf("Delete Mike ...\n");
    struct team_member *temp_t;
    temp_t = head;
    
    if(!strcmp(temp_t->name,dname))
    {
        head = head->next;
        free(temp_t);
    }
    else
    {
        temp_t = head;
        while(temp_t && strcmp(temp_t->name,dname))
        {
            current = temp_t;
            temp_t = temp_t->next;
        }
        if(!temp_t)
        {
            printf("Not found!\n");
        }
        else
        {
            current->next = temp_t->next;
            free(temp_t);
        }
    }
    // let current point to head again
    current = head;
    
    //dump linked list data
    while(current)
    {
        printf("Age:%d\tName:%s\n",current->age,current->name);
        current = current->next;
    }
        
    // start insert Rex
    printf("=== Linked List insert demo === \n");
    char * i_name = "Rex";
    int i_age = 30;
    int locate = 0;
    int target = 1;
    
    struct team_member *insert_t = (struct team_member*)malloc(sizeof(struct team_member));
    insert_t->name = i_name;
    insert_t->age = i_age;
    
    current = head;
    if(target == 0)
    {
        insert_t->next = current;
        head = insert_t;
    }
    else
    {
        while(current && locate < target - 1)
        {
            locate++;
            current = current->next;
        }
        insert_t->next = current->next;
        current->next = insert_t;
    }
    
    // let current point to head again
    current = head;
    //dump linked list data
    while(current)
    {
        printf("Age:%d\tName:%s\n",current->age,current->name);
        current = current->next;
    }
    
    return 0;
}
