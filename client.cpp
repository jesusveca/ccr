#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include "mio.cpp"
using namespace std;
//g++ client.cpp -std=c++11  -pthread -lncurses -o c

struct sockaddr_in stSockAddr;
int Res;
int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
int n;

void update_bala(char dir,int a, int b)
{
  switch (dir)
  {
      case '8'://Arriba

        for(int i=0; i<a;i++)///a es y
          {
             // imprimir();
              // sleep(1);
              matrix[i][b]=' ';
              //sleep(1);
              //imprimir();
          }
        break;
      case '2':///abajo
        for(int i=a+2; i<dimension ;i++)
          matrix[i][b]=' ';
        break;
      case '6': //derecha
        for(int i=b+2; i<dimension ;i++)
          matrix[a][i]=' ';
        break;
      case '4': ///izquierda
        for(int i=0; i<b ;i++)
          matrix[a][i]=' ';
         break;
      case '9':
        for(int i=b+2; i<dimension;i++)//derecha
          for(int j=a; j>=0; j--)//arriba
              if(i+j==a+b+1)
                  matrix[j][i]=' ';
        break;
      case '3':
        for(int i=b+2; i<dimension;i++)//derecha
          for(int j=a; j<dimension; j++)//abajo
              if(j-i==a-b)
                  matrix[j][i]=' ';
        break;
      case '1':
        for(int i=b-1; i>=0;i--)//izquierda
          for(int j=a; j<dimension; j++)//abajo
              if(i+j==a+b+1)
                  matrix[j][i]=' ';
        break;
      case '7':
        for(int i=b-1; i>=0;i--)//derecha
          for(int j=a; j>=0; j--)//abajo
              if(j-i==a-b)
                  matrix[j][i]=' ';
        break;
      default:
        break;
  }
 }

 void update(char dir, int x, int y, char dis)
 {
         if(dir=='6')//derecha
         {
           matrix[x][mod(y-1,dimension)]=' ';
           matrix[(x+1)][mod(y-1,dimension)]=' ';
           if(y-1<0)
           {
             matrix[x][mod(y-2,dimension)]=' ';
             matrix[(x+1)][mod(y-2,dimension)]=' ';
           }
         }
         if(dir=='4')//izquierda
         {
           matrix[x][mod(y+2,dimension)]=' ';matrix[(x+1)][mod(y+2,dimension)]=' ';
           if(y+2>dimension-1)
           {
             // cout<<"SUPER COUT!"<<mod(y+3,dimension)<<endl;
             matrix[x][mod(y+3,dimension)]=' ';matrix[(x+1)][mod(y+3,dimension)]=' ';
           }

         }

         if(dir=='8')  //esto es arriba
         {
           matrix[mod(x+2,dimension)][y]=' ';matrix[mod(x+2,dimension)][(y+1)]=' ';
           if(x+2>dimension-1)
           {
             matrix[mod(x+3,dimension)][y]=' ';matrix[mod(x+3,dimension)][(y+1)]=' ';
           }
         }
         if(dir=='5')  //8 es bajo
         {
           matrix[mod(x-1,dimension)][y]=' ';matrix[mod((x-1),(dimension))][(y+1)]=' ';
           if(x-1<0)
           {
             matrix[mod(x-2,dimension)][y]=' ';matrix[mod((x-2),(dimension))][(y+1)]=' ';
           }
         }


 }

 void writeS()
 {
     while(true)
     {
       //cout<<"HERE"<<endl;
       char buffer[7];
       //initscr ();
       teclas();
       //cbreak ();
       for (int i = 0; i < sizeof(buffer); i++)
       {
           buffer[i] = '\0';
       }
       main_no_main(buffer);

       n = write(SocketFD, buffer, sizeof(buffer));
     }

 }


 void readS()
 {
    while(true)
    {
      char buffer[7];
      //cout<<"BufferREadS: "<<buffer<<endl;
      n = read(SocketFD,buffer,sizeof(buffer));
      if (n < 0) perror("ERROR reading from socket");

      string ac ;
      string ab ;
      ac += buffer[2];
      ac += buffer[3];
      ab += buffer[4];
      ab += buffer[5];
      //cout<<"ac: "<<ac<<" ab: "<<ab<<endl;
      int a = stoi(ac);                  //De char a entero
      int b = stoi(ab);
      //cout<<"a: "<<a<<" b: "<<b<<endl;


      cuadrado(a,b,'A'+ buffer[0],colisiones[buffer[0]]);
      cout<<"VIDAS "<<vidas[buffer[0]]<<endl;
      update(buffer[6],a,b,buffer[1]);

      char buffer1[2];
      if(buffer[1]=='S')balas(a,b, buffer[6],buffer1);
      // if(buffer1[0]=='H')
      // {
      //   write(SocketFD, buffer1, sizeof(buffer1));
      // }

      imprimir();
      //sleep(1);
      update_bala(buffer[6],a,b);
      //sleep(1);

      //cout<<"Server replay: "<<buffer<<endl;

    }
 }

 int main(void)
 {

   initscr ();
   inicia(dimension);


   if (-1 == SocketFD)
   {
     perror("cannot create socket");
     exit(EXIT_FAILURE);
   }

   memset(&stSockAddr, 0, sizeof(struct sockaddr_in));

   stSockAddr.sin_family = AF_INET;
   stSockAddr.sin_port = htons(1100);
   Res = inet_pton(AF_INET, "192.168.1.2", &stSockAddr.sin_addr);

   if (0 > Res)
   {
     perror("error: first parameter is not a valid address family");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }
   else if (0 == Res)
   {
     perror("char string (second parameter does not contain valid ipaddress");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }

   if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
   {
     perror("connect failed");
     close(SocketFD);
     exit(EXIT_FAILURE);
   }


   //imprimir();
   std::thread t1 (readS);
   std::thread t2 (writeS);
   t1.join();
   t2.join();
     /*t1.join();
     t2.join();
     t3.join();*/

   shutdown(SocketFD, SHUT_RDWR);
   close(SocketFD);
   endwin ();
   return 0;
 }
