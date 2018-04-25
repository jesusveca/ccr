#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <ncurses.h>
#include <unistd.h>
using namespace std;

vector< vector<char> > matrix;
int dimension=40;
int dispara=0;
int direccion=0;
vector <char> naves;
int tecla=0;
char space= ' ';
char hurt;

int mod (int a,int b )
{
    if(a<0)
    {
        return a+b;
    }
    return a%b;
}

void inicia(int dim)
{
     vector<char> linea;

     matrix.clear();
     for(int i =0; i<dim; i++)
     {
        linea.push_back(space);

     }
     for(int i =0; i<dim; i++)
     {
        matrix.push_back(linea);
     }
}

void imprimir ()
{
    clear();
    for(int i =0; i<dimension; i++)
     {
        for(int j =0; j<dimension; j++)
         {
            printw ("%c", matrix[i][j]);
         }
         printw("\n");
     }
     cbreak();
     refresh();
}

int colisiones[10]={0,0,0,0,0,0,0,0,0,0};
int vidas[10]={4,4,4,4,4,4,4,4,4,4};

void balas (int a , int b, int direccion,char buffer1[2])
{
    hurt=0;
    char bala= '*';
    int dir=direccion-48;

    switch (dir)
    {
        case 8://Arriba

          for(int i=a-1; i>0;i--)///a es y
            {
                if(matrix[i][b]!= space)
                {
                    hurt=matrix[i][b]-'A';
                    matrix[i][b]=hurt;
                    colisiones[hurt]++;
                    vidas[hurt]--;
                    cout<<"VIDAS "<<vidas[hurt]<<endl;
                    break;
                }
                else{
                    matrix[i][b]=bala;
                    imprimir();
                    sleep(1);
                    matrix[i][b]=' ';
                  }
            }
          break;
        case 2:///abajo
          for(int i=a+2; i<dimension ;i++)
          {
              if(matrix[i][b]!= space)
              {
                 hurt= matrix[i][b] -'A';
                 matrix[i][b]=hurt;
                 colisiones[hurt]++;
                 vidas[hurt]--;
                 cout<<"VIDAS "<<vidas[hurt]<<endl;
                 break;
              }
              else{
                 matrix[i][b]=bala;
                 imprimir();
                 sleep(1);
                 matrix[i][b]=' ';
               }

          }
          break;
        case 6: //derecha
          for(int i=b+2; i<dimension ;i++)
          {
              if(matrix[a][i]!=space)
              {
                hurt= matrix[a][i]-'A';
                matrix[a][i]=hurt;
                colisiones[hurt]++;
                vidas[hurt]--;
                cout<<"VIDAS "<<vidas[hurt]<<endl;
                break;
              }
              else{
                matrix[a][i]=bala;
                imprimir();
                sleep(1);
                matrix[a][i]=' ';
              }
          }
          break;
        case 4: ///izquierda
          for(int i=b-1; i>0 ;i--)
          {
              if(matrix[a][i]!=space)
              {
                hurt= matrix[a][i]-'A';
                matrix[a][i]=hurt;
                colisiones[hurt]++;
                vidas[hurt]--;
                cout<<"VIDAS "<<vidas[hurt]<<endl;
                break;
              }
              else{
                 matrix[a][i]=bala;
                 imprimir();
                 sleep(1);
                 matrix[a][i]=' ';
              }
          }
           break;
        case 9:
          for(int i=b+2; i<dimension;i++)//derecha
            for(int j=a; j>=0; j--)//arriba
                if(i+j==a+b+1){
                    matrix[j][i]=bala;
                    imprimir();
                    sleep(1);
                    matrix[j][i]=' ';
                  }
          break;
        case 3:
          for(int i=b+2; i<dimension;i++)//derecha
            for(int j=a; j<dimension; j++)//abajo
                if(j-i==a-b){
                    matrix[j][i]=bala;
                    imprimir();
                    sleep(1);
                    matrix[j][i]=' ';
                  }
          break;
        case 1:
          for(int i=b-1; i>=0;i--)//izquierda
            for(int j=a; j<dimension; j++)//abajo
                if(i+j==a+b+1){
                    matrix[j][i]=bala;
                    imprimir();
                    sleep(1);
                    matrix[j][i]=' ';
                  }
          break;
        case 7:
          for(int i=b-1; i>=0;i--)//izquierda
            for(int j=a; j>=0; j--)//arriba
                if(j-i==a-b){
                    matrix[j][i]=bala;
                    imprimir();
                    sleep(1);
                    matrix[j][i]=' ';
                  }
          break;
        default:
          break;


    }
    if(hurt!=0)
    {

    buffer1[0]='H';
    buffer1[1]=hurt;

    }
    else buffer1[0]='P';
    //imprimir();
}

int a=0;
int b=0;
vector< pair <int, pair<int, int> > > usuarios;
int teclas ()
{

    //initscr ();
    //cbreak ();
    tecla = getch ();

    if(tecla==66)
    {
        a=(a+1)%(dimension-1);//S//abajo
        direccion=5;
    }
    if(tecla==68)
    {   b=(b-1)%(dimension-1);//A//izq
        direccion=4;
    }
    if(tecla==65)
    {
        a=(a-1)%(dimension-1);//W//arriba//
        direccion=8;
    }
    if(tecla==67)
    {
        b=(b+1)%(dimension-1);//D//derecha
        direccion=6;
    }
    if(tecla=='w')dispara=8; //8//arriba
    if(tecla=='e')dispara=9; //9//arr derecha
    if(tecla=='d')dispara=6; //6//derecha
    if(tecla=='c')dispara=3; //3//abajo derecha
    if(tecla=='x')dispara=2; //2//abajo
    if(tecla=='z')dispara=1; //1//abajo izquierda
    if(tecla=='a')dispara=4; //4//izquierda
    if(tecla=='q')dispara=7; //7//arriba izq
    if(a<0)a=dimension-2;
    if(b<0)b=dimension-2;
    imprimir();
    //endwin ();
}
void cuadrado( int x, int y, char letra , int col)
{   //inicia(dimension);
    if(col==0)
        {matrix[x][y]=letra;
        matrix[x+1][y]=letra;
        matrix[x][y+1]=letra;
        matrix[x+1][y+1]=letra;}
    if(col==1){
      matrix[x][y]=letra;
      matrix[x+1][y]=letra;
      matrix[x][y+1]=letra;
    }
    if(col==2){
      matrix[x+1][y]=letra;
      matrix[x][y+1]=letra;
    }
    if(col==3){
      matrix[x][y+1]=letra;
    }

    //teclas();
    //imprimir();
}
void main_no_main(char buffer[7])
{
    //cout<<"HEREL"<<endl;
    //teclas();
    char integer_string[32];


    if(dispara>0)buffer[0]='S';//strcat(buffer,"1");
    else         buffer[0]='M';//strcat(buffer,"0");

    if(a<=9) strcat(buffer,"0");
    sprintf(integer_string, "%d", a);
    strcat(buffer,integer_string);

    if(b<=9) strcat(buffer,"0");
    sprintf(integer_string, "%d", b);
    strcat(buffer,integer_string);

    if(dispara>0)
    {
        sprintf(integer_string, "%d", dispara);
        strcat(buffer,integer_string);
    }
    else
    {
        sprintf(integer_string, "%d", direccion);
        strcat(buffer,integer_string);
    }

    //if(dispara>0) balas(a,b, dispara);
    dispara=0;
    //imprimir();

}
