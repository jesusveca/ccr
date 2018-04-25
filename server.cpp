#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

struct sockaddr_in stSockAddr;
int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
char buffer[7];
int n;
vector<int> sockets;

void aceptar(int ConnectFD)
{
  do {
     n = read(ConnectFD,buffer,1);
     char user[1];
     sprintf(user,"%d",ConnectFD);

     if (n < 0) perror("ERROR reading from socket");
     cout<<"Client "<<ConnectFD<< " replay: "<<buffer<<endl;

     char tipo;
     tipo= buffer[0];
     cout<<"FUERA DEL IF"<<buffer[0]<<endl;

     if(buffer[0]=='M' || buffer[0]=='S')
     {
        n = read(ConnectFD,buffer,5);
        cout<<"DENTRO DEL IF"<<buffer[0]<<endl;

        char id_usuario[7];
        char aux[7];

        cout<<"id_usuario: "<<id_usuario<<endl;
        cout<<"tipo :"<<tipo<<endl;
        id_usuario[0]=user[0];
        id_usuario[1]=tipo;
        //strcat(id_usuario[0],tipo[0]);
        id_usuario[2] = buffer[0];
        id_usuario[3] = buffer[1];
        id_usuario[4] = buffer[2];
        id_usuario[5] = buffer[3];
        id_usuario[6] = buffer[4];

        for(int i=0;i<7;i++)
          buffer[i] = id_usuario[i];

        cout<<"Nuevo buffer: "<<buffer<<endl;
        for(int i=0;i<sockets.size();i++)
         {
             cout<<"Enviando a Cliente: "<<sockets[i]<<endl;
             //char user = '0' + ConnectFD;
             //buffer[0] = user;
             n = write(sockets  [i],buffer,sizeof(buffer));
             if (n < 0) perror("ERROR writing to socket");
         }
      }
    // else if(buffer[0]=='H')
    //   {
    //     n = read(ConnectFD,buffer,1);
    //     char herido [3];
    //     herido[0] = user[0];
    //     herido[1] = 'H';
    //     herido[2] = buffer[0];
    //     char buffer[3];
    //     for(int i=0;i<3;i++)
    //       buffer[i] = herido[i];
    //
    //     cout<<"Buffer Herido: "<<buffer<<endl;
    //     for(int i=0;i<sockets.size();i++)
    //      {
    //          cout<<"Enviando a Cliente: "<<sockets[i]<<endl;
    //          //char user = '0' + ConnectFD;
    //          //buffer[0] = user;
    //          n = write(sockets[i],buffer,sizeof(buffer));
    //          if (n < 0) perror("ERROR writing to socket");
    //
    //      }
      //   cin>>n;

      //}
    }while(buffer != "END");

    shutdown(ConnectFD, SHUT_RDWR);
    close(ConnectFD);
}

int main(void)
{
  if(-1 == SocketFD)
  {
    perror("can not create socket");
    exit(EXIT_FAILURE);
  }

  memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

  stSockAddr.sin_family = AF_INET;
  stSockAddr.sin_port = htons(1100);
  stSockAddr.sin_addr.s_addr = INADDR_ANY;

  if(-1 == ::bind(SocketFD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
  {
    perror("error bind failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  if(-1 == listen(SocketFD, 10))
  {
    perror("error listen failed");
    close(SocketFD);
    exit(EXIT_FAILURE);
  }

  while(1)
  {
    int ConnectFD = accept(SocketFD, NULL, NULL);

    if(0 > ConnectFD)
    {
      perror("error aceptar failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    } else {
      sockets.push_back(ConnectFD);
      std::thread (aceptar, ConnectFD).detach();
    }
  }

  close(SocketFD);
  return 0;
}
