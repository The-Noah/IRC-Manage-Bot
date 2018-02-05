// Copyright (C) 2018 The Noah
// DOES NOT WORK!!!
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// internet stuff
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

// define stuff
#define PORT 6667
#define MAXDATASIZE 256 // max number of bytes we can get at once

const char
  *serv = "chat.freenode.net",
  *botName = "aRandomBot1jgw",
  *chann = "##zehra",
  *owner = "Zehra";

bool isPartOf(char* w1, char* w2){
  int i=0;
  int j=0;
  while(w1[i]!='\0'){
    if(w1[i] == w2[j]){
      int init = i;
      while (w1[i] == w2[j] && w2[j]!='\0'){
        j++;
        i++;
      }
      if(w2[j]=='\0')
        return true;
      j=0;
    }
    i++;
  }
  return false;
}

bool isPartOf2(char* w1, char* w2){
  int i=0;
  int j=0;
  for(i;i < strlen(w1); i++){
    if(w1[i] == w2[j])
      j++;
  }
  if(strlen(w2) == j)
    return true;
  else
    return false;
}

bool StartsWith(const char *a, const char *b){
  if(strncmp(a, b, strlen(b)) == 0) return true;
  return false;
}

bool charSearch(char *toSearch, char *searchFor){
  int len = strlen(toSearch);
  int forLen = strlen(searchFor); // The length of the searchfor field
  //Search through each char in toSearch
  for (int i = 0; i < len;i++){
    //If the active char is equil to the first search item then search toSearch
    if (searchFor[0] == toSearch[i]){
      bool found = true;
      //search the char array for search field
      for (int x = 1; x < forLen; x++){
        if (toSearch[i+x]!=searchFor[x])
          found = false;
      }
      //if found return true;
      if (found == true)
        return true;
    }
  }
  return false;
}

int main(int argc, char *argv[]){
  printf("Starting....\n");
  // display info
  printf("Server: %s\n", serv);
  printf("Bot Name: %s\n", botName);
  printf("Channel: %s\n", chann);
  printf("Owner: %s\n", owner);

  // init socket
  int client;
  bool isExit;
  int bufsize = 1024;
  char buffer[bufsize];
  struct sockaddr_in server_addr;

  client = socket(AF_INET,SOCK_STREAM,0);
  if(client < 0) return -1;
  printf("[+] Socket created\n");

  // get IP from hostname
  struct hostent *he;
  printf("[*] Trying to get hostname\n");
  he = gethostbyname(serv);
  if(he)
    printf("[+] IP found for host %s\n", he->h_name);
  else{
    printf("[!] Get get IP from hostname %s failed\n", serv);
    return -1;
  }

  memcpy(&server_addr.sin_addr,he->h_addr_list[0],he->h_length);
  //server_addr.sin_addr.s_addr = inet_ntoa(serv);
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  printf("[*] Trying to connect to %s:%d\n", serv, PORT);
  if(connect(client,(struct sockaddr*)&server_addr,sizeof(server_addr)) == 0)
    printf("[+] Connection okay\n");
  else{
    printf("[!] Connection not okay\n");
    return -1;
  }

  for(int i = 0; i < 3; i++)
    recv(client,buffer,bufsize,0);
  printf("[+] Connection confirmed\n");

  // create USER message and send it
  char userMsg[bufsize];
  strcpy(userMsg,"USER ");
  strcat(userMsg,botName);
  strcat(userMsg," ");
  strcat(userMsg,botName);
  strcat(userMsg," ");
  strcat(userMsg,botName);
  //strcat(userMsg," :IRC-Manage-Bot\n");
  strcat(userMsg," ");
  strcat(userMsg,botName);
  strcat(userMsg,"\n");
  printf("[*] Sending %s", userMsg);
  send(client, userMsg, sizeof(userMsg), 0);

  // create NICK message and send it
  char nickMsg[bufsize];
  strcpy(nickMsg,"NICK ");
  strcat(nickMsg,botName);
  strcat(nickMsg,"\n");
  printf("[*] Sending %s", nickMsg);
  send(client, nickMsg, sizeof(nickMsg), 0);

  //char pmMsg[bufsize] = "PRIVMSG nickserv :iNOOPE\r\n";
  //printf("[*] Sending %s", pmMsg);
  //send(client, pmMsg, sizeof(pmMsg), 0);

  //recv(client,buffer,bufsize,0);
  // create JOIN message and send it
  char joinMsg[bufsize];
  strcpy(joinMsg,"JOIN ");
  strcat(joinMsg,chann);
  strcat(joinMsg,"\n");
  printf("[*] Sending %s", joinMsg);
  send(client, joinMsg, sizeof(joinMsg), 0);

  char *pong = (char*)"PONG :pingis\r\n";
  //recv(client,buffer,bufsize,0);
  //printf("a\n");
  send(client,pong,sizeof(pong),0);

  while(true){
    recv(client,buffer,bufsize,0);
    if(isPartOf2(buffer,(char*)"PING") || isPartOf2((char*)"PING",buffer) || charSearch(buffer,(char*)"PING") || StartsWith(buffer,(char*)"PING") /*||
      ((char*)buffer[0] == (char*)"P" && (char*)buffer[1] == (char*)"I" &&
      (char*)buffer[2] == (char*)"N" && (char*)buffer[3] == (char*)"G")*/){
      printf("ping\n");
      //send(client,pong,sizeof(pong),0);
      const char *res;
      memcpy(res, buffer + 5, sizeof(buffer));
      send(client,res,sizeof(res),0);
    }
    if(isPartOf2(buffer,(char*)"ERROR") || isPartOf2((char*)"ERROR",buffer) || charSearch(buffer,(char*)"ERROR") || StartsWith(buffer,(char*)"ERROR")){
      printf("[!] ERROR\n");
      printf("[*] Recv: %s", buffer);
      break;
    }
    printf("[*] Recv: %s", buffer);
  }

  printf("Ending....\n");
  close(client);
  return 0;
}
