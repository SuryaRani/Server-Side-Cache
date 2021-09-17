#include <string.h>

#define __USE_XOPEN
#include <time.h>

#include <stdint.h>


#include <err.h>

#include <stdlib.h>

#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <sys/stat.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>



//these are our global variables that the user input could determine and change
//i just wanted to make them globabl so i didnt have to deal with passing them in always
int cacheSize =3;
int fileSize =  65536;
 int lru = -1; 










//this is the begining of the linked list code i got from online
// http://www.martinbroadhurst.com/singly-linked-list-in-c.html
struct node {
   uint8_t* data;
   char* key;
   char* response;
   long dataLen;
   char* tme;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   struct node *ptr = head;
   printf("\n[ ");
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%s) ",ptr->key);
      ptr = ptr->next;
   }
	
   printf(" ]");
}

//insert link at the first location
void insertFirst(char* key, uint8_t* data,char* response,long dataLen,char* tme) {
   //create a linkm
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->key = key;
   link->data = data;
   link->response = response;
   link->dataLen = dataLen;
   link->tme = tme;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//delete first item
struct node* deleteFirst() {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
}



//is list empty
bool isEmpty() {
   return head == NULL;
}

int length() {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//find a link with given key
struct node* find(char* key) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(strcmp(current->key,key)!=0) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}

//delete a link with given key
void delete(char* key) {

   //start from the first link
   struct node* current = head;
   struct node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return;
   }

   //navigate through list
  while(strcmp(current->key,key)!=0) {

      //if it is last node
      if(current->next == NULL) {
         return;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      
      previous->next = current->next;
   }    
  
   free(current);
   current = NULL;
	
}


void deleteLast(){
  struct node * traverse = head;
  if(traverse ==NULL){
    return;
  }
  while(traverse->next!=NULL){
    traverse = traverse->next;
  }
  if(traverse->next == NULL){
    delete(traverse->key);
    traverse = NULL;
  }
}

//end of linked list code


























/**
   Creates a socket for connecting to a server running on the same
   computer, listening on the specified port number.  Returns the
   socket file descriptor on succes.  On failure, returns -1 and sets
   errno appropriately.
 */
int create_client_socket(uint16_t port) {
  int clientfd = socket(AF_INET, SOCK_STREAM, 0);
  if (clientfd < 0) {
    return -1;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (connect(clientfd, (struct sockaddr*) &addr, sizeof addr)) {
    return -1;
  }
  return clientfd;
}

/**
   Converts a string to an 16 bits unsigned integer.
   Returns 0 if the string is malformed or out of the range.
 */
uint16_t strtouint16(char number[]) {
  char *last;
  long num = strtol(number, &last, 10);
  if (num <= 0 || num > UINT16_MAX || *last != '\0') {
    return 0;
  }
  return num;
}

/**
   Creates a socket for listening for connections.
   Closes the program and prints an error message on error.
 */
int create_listen_socket(uint16_t port) {
  struct sockaddr_in addr;
  int listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
    err(EXIT_FAILURE, "socket error");
  }

  memset(&addr, 0, sizeof addr);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(INADDR_ANY);
  addr.sin_port = htons(port);
  if (bind(listenfd, (struct sockaddr*)&addr, sizeof addr) < 0) {
    err(EXIT_FAILURE, "bind error");
  }

  if (listen(listenfd, 500) < 0) {
    err(EXIT_FAILURE, "listen error");
  }

  return listenfd;
}


void storeFile(char* fileName, uint8_t* file,char* response,int dataLen,char* tme){
  //this should work for both fifo and lru

        insertFirst(fileName,file,response,dataLen,tme);
        printf("length: %d\n",length());
        if(length()-1 >=cacheSize){
          printf("i should be deleting\n");
          deleteLast();
        }

      
      


    
   
}

// this is our function to deal with when we recieve a put request from the client

void put(long length, int clientSock,int serverSock) //, char *buffer, char *maybeData)
{
   
        //create a buffer to store the incoming data with the content length as the length of the buffer
        uint8_t fileRecieved[1000];
        
       
        long var = 0;
        //get the data and place into a buffer
        ssize_t r = recv(clientSock, fileRecieved, 1000, 0);
       
    
        var += r;
        //write the data recieved from the client to the file we created on our server
        
        send(serverSock, fileRecieved, r,0);
      
        //make sure to do this until we have gotten all the way to the content length
        while (var != length)
        {
            r = recv(clientSock, fileRecieved, 1000, 0);
            var += r;
            send(serverSock, fileRecieved, r,0);
        }


  }

//this is the function that will deal with when we recieve a get request from the client
void get(char* fileName, int clientSock, int serverSock){
    
    //create buffer that will hold the initial ok with content length from the server
      char buff[4096];
       
      //we want to recieve the data from the server 
      ssize_t bytesRecieved = recv(serverSock,buff,4096,0);
       

      //we will now send however many bytes we recieved right back to the client
      send(clientSock,buff,bytesRecieved,0);
       
        //this is just incase we dont recieve anything so we can return an error
       if(bytesRecieved==0){
         return;
       }
       //we want to tokenize the ok response so we can get the content length and delimit by \r\n
       char* token = strtok(buff, "\r\n");
         
        token = strtok(NULL, "\r\n");
        long length = 0;
      if (sscanf(token, "Content-Length: %ld", &length) < 0)
            {
              return;
            }

    //create the buffer to recieve the file
      uint8_t fileRecieved[1000];
      uint8_t *sendingFile = malloc(fileSize);

      
        
       
        long var = 0;
        //get the data and place into a buffer
       
        ssize_t r = recv(serverSock, fileRecieved, 1000, 0);
    


        //this may need to be i<=r
        int j =0;
        for(int i = var;i<r;i++){
          sendingFile[i] = fileRecieved[j];
          j++;
        }
    
        var += r;
        //write the data recieved from the client to the file we created on our server
        send(clientSock, fileRecieved, r,0);
        
      
        //make sure to do this until we have gotten all the way to the content length
        while (var != length)
        {
            //might need to do length -var here might fail for bigger files
            r = recv(serverSock, fileRecieved, 1000, 0);

               j =0;
              for(int i = var;i<r;i++){
                sendingFile[i] = fileRecieved[j];
                j++;
              }

            var += r;
            send(clientSock, fileRecieved, r,0);
            
        }

        

        
        //create the ok message so we can store and send it to the client when getting it from cache
        char *okMesg = malloc(50);
        sprintf(okMesg, "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n", length);

          //this is so we get the last modified header line and store it in our cache as well
          token = strtok(NULL, "\r\n");

          char* tokMalc = malloc(100);
          for(int i =0; i<strlen(token);i++){
            tokMalc[i] = token[i];
          }
         
        //only add to cache if fileSize is less than the max specified fileSize
        if(length<=fileSize){
           storeFile(fileName, sendingFile,okMesg,length,tokMalc);
        }

     


  }


//this function will send a head request to the server to be able to get the last modified header line
  char* getLastModified(char* fileName,int serverSock){

        //create buffer for the head message that we need to send
          char headMsg[1000];
          sprintf(headMsg, "HEAD %s HTTP/1.1\r\nHost: localhost:4000\r\n\r\n",fileName);
          
          //send it to the client
          ssize_t headLen = strlen(headMsg);
          send(serverSock,headMsg,headLen,0);
         
        //i am mallocing because for some reason when i didnt the time was weird and off by 1 hour the first time i called diff 
          char* recievedTime = malloc(1000);
           
           //get the last modified header 
          recv(serverSock,recievedTime,1000,0);
           


          return recievedTime;


  }


  //this function takes in the pre saved cached last modified header and compares it to the freshly recieved last modified 
  // header and checks the difference between the two
  // returns false if server updated file and false if the cached file is up to date
  bool checkTime(char* fileName,int serverSock,char* tme){
    
    
    //these structs parse through the time and change that to time in seconds since the epoch
    struct tm originalTime;
    time_t ogTime;
    struct tm newTime;
    time_t nTime;

    //this is a function to parse through a string and place time values into a time object
    strptime(tme,"Last-Modified: %a, %d %b %Y %T GMT",&originalTime);
    //gives time object value of time in seconds since epoch 
    ogTime = mktime(&originalTime);

    
       char* recievedTime = getLastModified(fileName,serverSock);

        char* token = strtok(recievedTime, "\r\n");
         
        token = strtok(NULL, "\r\n");
        token = strtok(NULL, "\r\n");

        /*
        char t[1000];
        sscanf(token,"Last-Modified: %s",t);
        */


      strptime(token,"Last-Modified: %a, %d %b %Y %T GMT",&newTime);
      nTime = mktime(&newTime);

      double diff = difftime(ogTime,nTime);


      //could do quick fix if necessary and if it is -3600.00 then its still true

      printf("%s\n",tme);
      printf("%s\n",token);

      if(diff<0.0){
        return false;
      }

      



  return true;

}


int cache(char* fileName,int clientSock,int serverSock){
  //will only be here if request is get request
  // check if item is in cache if not we return something to signify that we didnt use cache file
  //if item is in cache then we check the time of our file and the file on the server
  // if it is older on our proxy then we reutrn something to signify we didnt use the cache file
  // if it is newer or same time then we send the cached file
  //return something to signify that it did work
  
struct node *foundLink = find(fileName);


  // if we have found the file in cache then we will need to check if we can send it straight from cache if not then go as normal
  if(foundLink==NULL){
    return -1;
  }
  else{
    //if we have the lru option on then we want to remove the file from the linked list and insert it back into the front
   if(lru==1){
        uint8_t* dataCopy = foundLink->data;
        char* responseCopy = foundLink->response;
        int dataLenCopy = foundLink->dataLen;
        char* timeCopy = foundLink->tme;
        delete(fileName);
        printList();
        insertFirst(fileName,dataCopy,responseCopy,dataLenCopy,timeCopy);
        printList();
       
      }

    
    //printf("%s\n",fileName);
    if(checkTime(fileName,serverSock,foundLink->tme)){
      printf("using cache\n");

        int len = foundLink->dataLen;
      send(clientSock,foundLink->response,strlen(foundLink->response),0);
      send(clientSock,foundLink->data,len,0);


    
     
    foundLink = NULL;
    }
    else{
      //we go into here if our checkTime returns false meaning the file was updated
      //we will delete the file and return -1 so we can run through the entire get funciton like normal
      printf("not going to use cache since it was updated\n");
       
      delete(fileName);
      foundLink = NULL;
      return -1;
    }
    
    

  }

  return 1;
}



// we go into here after our main funciton to accept and parse through client requests
void handle_connection(int clientSock,int serverSock) {
  
  //we need to parse request first
  char buff[4096];
   char func[5];
   
  
    char *token;
    char* fileName = malloc(100);
    char serverVersion[20];
 
  ssize_t bytes = recv(clientSock, buff, 4096, 0);
  if(bytes == 0 || bytes <0){
    close(clientSock);
    return;
  }
  //recieve the request and parse throught it like we did in httpserver
   token = strtok(buff, "\r\n");
   
    sscanf(token, "%s %s %s", func, fileName, serverVersion);
    
    

    token = strtok(NULL, "\r\n");

    long conLen = 0;
    
    while (token != NULL)
        {
            // this is to check if it is giving content length in request
            if (sscanf(token, "Content-Length: %ld", &conLen) > 0)
            {
            }
            token = strtok(NULL, "\r\n");
        }
       
    //this is where we do our three functions GET, PUT and everything else including HEAD and errors
    if (strcmp(func, "GET") == 0){
      //call cache
     
      if(cache(fileName,clientSock,serverSock) == -1){
        //we want to send the server the intiial request first and then we will recieve the data in our function
        send(serverSock,buff,bytes,0); 
        
        get(fileName,clientSock, serverSock);
        
      }

    }
    else if(strcmp(func,"PUT") == 0){
     
      send(serverSock,buff,bytes,0);
       
      put(conLen,clientSock,serverSock);
       
    //make sure that we recieve and send the response from the server to the client
      ssize_t recievedBytesFromServer = recv(serverSock,buff,4096,0);
        
      send(clientSock,buff,recievedBytesFromServer,0);
       
    }
    else{
      
      //this will deal with HEAD requests and error requests
      send(serverSock,buff,bytes,0);
     
       ssize_t recievedBytesFromServer = recv(serverSock,buff,4096,0);
      
      send(clientSock,buff,recievedBytesFromServer,0);
     
    }



  //if it is a get we call cache
  // if cache worked we do nothing
  // if cache did not work then we send the request like normal


  // when done, close socket
  
}



  

int main(int argc, char *argv[]) {
  int listenfd;
  uint16_t port =0;
  // You will have to modify this and add your own argument parsing
  if (argc < 3) {
    errx(EXIT_FAILURE, "wrong arguments: %s port_num server_port", argv[0]);
  }

 

   uint16_t serverPort;
   
  
  //parsing through program options
  for(int i = 1; i<argc; i++){
    switch(argv[i][1]){
        case 'c':
          cacheSize = atoi(argv[i+1]);
          break;
        case 'u':
          lru = 1;
          break;  
        case 'm':
          fileSize = atoi(argv[i+1]); 
          break;
        default:
          if(atoi(argv[i])> 1024){
            if(port==0){
              port = strtouint16(argv[i]);
            }
            else{
              serverPort = strtouint16(argv[i]);
            }
            
          }
          break;


    }

  }

//make sure that our port is valid and create both the listening and sending sockets
   if (port <=1024) {
    errx(EXIT_FAILURE, "invalid port number: %s", argv[1]);
  }
  printf("cache size: %d\n",cacheSize);
  
  listenfd = create_listen_socket(port);
 
  int serverSock = create_client_socket(serverPort);
  
  printf("Port: %d\n",port);
  printf("serverPort: %d\n",serverPort);
  printf("lru :%d\n",lru);

  
//keep going in loop to keep accepting client requests 
  while(1) {
  
    int connfd = accept(listenfd, NULL, NULL);
   
    if (connfd < 0) {
      warn("accept error");
      continue;
    }
    
    handle_connection(connfd,serverSock);
  }
  return EXIT_SUCCESS;
}
