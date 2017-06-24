#include "chain_list.h"

struct chain_list* chain_create_list() {
  struct chain_list* list = malloc(sizeof(chain_list));
  list->size = 0;
  list->header = NULL;

  if(pthread_mutex_init(list->lock, NULL) != 0) {
        printf("\n Mutex init failed\n");
        free(list);
        return NULL;
    }
  return list;
}

struct chain_node* chain_create_client_node(char* userId) {
  struct chain_node* node = malloc(sizeof(chain_node));
  Client_Info* client = malloc(sizeof(Client_Info));

  if(pthread_mutex_init(&client->loginMutex, NULL) != 0) {
        printf("\nCreate Client Node Mutex init failed\n");
        free(client);
        free(node);
        return NULL;
  }

  strcpy(client->userId, userId);
  node->client = client;

  node->file = NULL;
  node->next = NULL;
  return node;
}

struct chain_node* chain_create_file_node(char* name, char* extension, char* last_modified, int size) {
  struct chain_node* node = malloc(sizeof(chain_node));
  UserFile* file = malloc(sizeof(UserFile));

  if(pthread_mutex_init(&file->fileMutex, NULL) != 0) {
        printf("\nCreate Client Node Mutex init failed\n");
        free(file);
        free(node);
        return NULL;
  }

  strcpy(file->name, name);
  strcpy(file->extension, extension);
  strcpy(file->last_modified, last_modified);
  file->size = size;

  node->file = file;

  node->client = NULL;
  node->next = NULL;
  return node;
}

int chain_add(chain_list* list, chain_node* node) {
  if(list != NULL) {
    if(list->header != NULL) {
      if(node->client && list->header->file != NULL) {
          printf("ADD ERROR: Incompatible list, node client trying to add on file list\n");
          return -1;

      } else if(node->file && list->header->client != NULL) {
          printf("ADD ERROR: Incompatible list, node file trying to add on client list\n");
          return -1;
      }
    }
    printf("STARTING ADD\n");
    pthread_mutex_lock(list->lock);
    node->next = list->header;
    list->header = node;
    list->size++;
    pthread_mutex_unlock(list->lock);
    return 1;
  }
    printf("ADD ERROR: List not initalized\n");
    return -1;
}

int chain_remove(chain_list* list, char* chainname) {
  if(list == NULL) {
    printf("REMOVE ERROR: List not initialized\n");
    return -1;
  }
  pthread_mutex_lock(list->lock);

  chain_node* previous = NULL;
  chain_node* node = list->header;

  if (node == NULL) {
    pthread_mutex_unlock(list->lock);
    return 1;
  }

  if(node->client) {
    while (node!=NULL && strcmp(node->client->userId,chainname)!=0) {
      previous = node;
      node = node->next;
    }
  } else if (node->file) {
    while (node!=NULL && strcmp(node->file->name,chainname)!=0) {
      previous = node;
      node = node->next;
    }
 }

 if (node == NULL) {
   pthread_mutex_unlock(list->lock);
   return 1;
 }

 list->size--;

 if (previous == NULL) { //Retira o primeiro elemento
    list->header = node->next;
 } else { //Retira do meio
    previous->next = node->next;
 }

  if(node->client != NULL) free(node->client);
  if(node->file != NULL) free(node->file);
  free(node);
  pthread_mutex_unlock(list->lock);
  return 1;
}

struct chain_node* chain_find(chain_list* list, char *chainname) {
  pthread_mutex_lock(list->lock);
  chain_node* node = list->header;

  if(node->client) {
    while (node!=NULL && strcmp(node->client->userId,chainname)!=0) {
        node = node->next;
    }
  } else if(node->file) {
    while (node!=NULL && strcmp(node->file->name,chainname)!=0) {
        node = node->next;
    }
  }

  pthread_mutex_unlock(list->lock);
  return node;
}

int chain_clear(chain_list* list) {
    if(list == NULL) {
      printf("CLEAR ERROR: List not initalized\n");
      return -1;
    }

    pthread_mutex_lock(list->lock);
    chain_node* node = list->header;

    while (node != NULL) {
      chain_node* next = node->next;

      free(node);
      node = next;
    }

    list->size = 0;
    list->header = NULL;
    pthread_mutex_unlock(list->lock);
    return 1;
}

int chain_print(chain_list* list) {
  if(list == NULL) {
    printf("PRINT ERROR: List not initalized\n");
    return -1;
  }

  pthread_mutex_lock(list->lock);
  if(list->header == NULL) printf("Empty List\n");

  chain_node* node = list->header;

  while(node != NULL) {
    printf("Node %s\n", node->client->userId);
    node = node->next;
  }
  pthread_mutex_unlock(list->lock);
  return 1;
}
