#include "chain_list.h"

int nodeCount = 0;
struct chain_list* list;
pthread_t tid[2];

void* createNode(void *arg) {
    printf("\n Job createNode started\n");
    char name4[40] = "UserName ";
    while(nodeCount < 10) {
      char nodeName[50];
      bzero(nodeName,50);
      sprintf(nodeName,"%s%d",name4,nodeCount);
      struct chain_node* node = chain_create_client_node(nodeName);
      if(chain_add(list,node)) printf("Node %s created\n",nodeName);

      nodeCount++;
      sleep(1);
    }
    printf("\n Job createNode finished\n");

    return NULL;
}

void* removeNode(void *arg) {
    printf("\n Job removeNode started\n");
    while(nodeCount < 10 || list->size != 0) {
      while(list->size == 0);
      char nNode[50];
      bzero(nNode, 50);
      strcpy(nNode,list->header->client->userId);

      if(chain_remove(list, nNode)) printf("Node %s removed\n",nNode);
      sleep(1);
    }
    printf("\n Job removeNode finished\n");

    return NULL;
}

int main() {
    char name[50] = "clientName1";
    char name2[50] = "clientName2";
    char name3[50] = "clientName3";

    printf("\nCREATING LIST\n ");
    list = chain_create_list();
    printf("\nCREATING NODES\n ");
    struct chain_node* node = chain_create_client_node(name);
    struct chain_node* node2 = chain_create_client_node(name2);
    struct chain_node* node3 = chain_create_client_node(name3);
    struct chain_node* nodeFind;

    printf("\n ADDING NODES\n ");
    if(chain_add(list, node)) {
      printf("ADDING NODE\n");
      printf("Nodename: %s\n", list->header->client->userId);
      printf("Size: %d\n", list->size);
    } else {
      printf("ADDING FAIL\n");
    }

    if(chain_add(list, node2)) {
      printf("ADDING NODE\n");
      printf("Nodename: %s\n", list->header->client->userId);
      printf("Size: %d\n", list->size);
    }

    if(chain_add(list, node3)) {
      printf("ADDING NODE\n");
      printf("Nodename: %s\n", list->header->client->userId);
      printf("Size: %d\n", list->size);
    }

    if(chain_remove(list, name)) {
      printf("REMOVING NODE\n");
      printf("Size: %d\n", list->size);
      nodeFind = chain_find(list, name);
      if(nodeFind == NULL) printf("Node not found\n");
    }

    nodeFind = chain_find(list, name2);

    if(nodeFind) {
      printf("FIND NODE\n");
      printf("Node %s\n", nodeFind->client->userId);
    }

    printf("PRINT LIST\n");
    chain_print(list);

    chain_clear(list);

    printf("CLEAR LIST\n");
    chain_print(list);

    chain_clear(list);

    int err = pthread_create(&(tid[0]), NULL, &createNode, NULL);
    if (err != 0)
           printf("\ncan't create thread :[%s]", strerror(err));

    err = pthread_create(&(tid[1]), NULL, &removeNode, NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(list->lock);

    return 0;
}
