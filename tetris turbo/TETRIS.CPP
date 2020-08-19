#include<iostream.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<graphics.h>
#include<fstream.h>

//made by manas makde

///////////////////some basic variables/////////////
union REGS in,out;
int i,j;
int ttp=0;
int mouse_on=1;
float tim=0;
char shp,prev_shp;
int speed=150;
char key='o'; //which key is pressed
char shapes[]="oiszljt";
int shp_x,shp_y,shp_col,prev_shp_col;
int play=1,game_act=1,action=0,reset=0,q=0,cheats=0;
long int score=0;
int side=15; //side length of single box

/////////////////mouse functions//////////////////////
void show_mouse()//shows mouse
{
 in.x.ax=1;
 int86(0x33,&in,&out);
}
void hide_mouse()//hides mouse
{
 in.x.ax=2;
 int86(0x33,&in,&out);
}
void limit_mouse(int x1,int y1,int x2,int y2)//limits mouse movement
{                                    //by  2 x-axis and 2 y-axis(min-max each)
 in.x.ax=7;
 in.x.cx=x1;
 in.x.dx=x2;
 int86(0x33,&in,&out);
 in.x.ax=8;
 in.x.cx=y1;
 in.x.dx=y2;
 int86(0x33,&in,&out);
}
int mouse_in(int x1,int y1,int x2,int y2)// checks if mouse is inside box
{
in.x.ax=3;
int86(0x33,&in,&out);
if(out.x.bx==1&&x1<out.x.cx&&x2>out.x.cx&&y1<out.x.dx&&y2>out.x.dx)
return 1;   //clicked inside

else if(out.x.bx==0&&x1<out.x.cx&&x2>out.x.cx&&y1<out.x.dx&&y2>out.x.dx)
return -1;  //not clicked
else
return 0;   //not in box
}

int mouse_stat()    //gives if button is clicked
{                   //and if clicked thne which one
 in.x.ax=3;
int86(0x33,&in,&out);
return out.x.bx;//left click =1 right click=2
}

///////////////////////helpful functions//////////////////////////////
int ins(int i,int j,int a[10])  //element(s) inside given matrix
{                               //(only to spell "TETRIS" irrelevant
 for(int p=1;p<=a[0];p++)       //                      to actual game)
 {
  if(a[p*2-1]-1==i&&a[p*2]==j)
  return 1;
 }
 return 0;
}

/////////////////////////graphical functions///////////////////////////
void box(int x,int y,int col) //creates square box of color at given x y
{
int temp=getcolor();
setcolor(BLACK);
 rectangle(x,y,x+side,y+side);
 setcolor(col);
for(int j=1;j<side;j++)
line(x+1,y+j,x+side-1,y+j);
setcolor(temp);
}
void box(int x,int y,int col,int side,int col_2=BLACK)//same but custom outline color(2)
{
int temp=getcolor();
setcolor(col_2);
 rectangle(x,y,x+side,y+side);
 setcolor(col);
for(int j=1;j<side;j++)
line(x+1,y+j,x+side-1,y+j);
setcolor(temp);
}
void shape(int x,int y,char c,int sid,int col=WHITE)//places shapes at x,y
{                                       //options=> O T S Z I L J(all small)
 if(c=='o')
 {
  box(x,y,col,sid);               //1                            {1}{2}
  box(x+sid+1,y,col,sid);         //2                            {4}{3}
  box(x+sid+1,y+sid+1,col,sid);   //3
  box(x,y+sid+1,col,sid);         //4
 }
 else
 if(c=='i')
 {

 box(x,y,col,sid);
 box(x,y+sid+1,col,sid);
 box(x,y+sid*2+2,col,sid);
 box(x,y+sid*3+3,col,sid);

 }
 else
 if(c=='s')
 {
 box(x+sid+1,y,col,sid);
 box(x+sid*2+2,y,col,sid);
 box(x+sid+1,y+sid+1,col,sid);
 box(x,y+sid+1,col,sid);
 }

 else
 if(c=='z')
 {
 box(x,y,col,sid);
 box(x+sid+1,y,col,sid);
 box(x+sid+1,y+sid+1,col,sid);
 box(x+sid*2+2,y+sid+1,col,sid);
 }
 else
 if(c=='l')
 {
  box(x,y,col,sid);
 box(x,y+sid+1,col,sid);
 box(x,y+sid*2+2,col,sid);
 box(x+sid+1,y+sid*2+2,col,sid);
 }
 else
 if(c=='j')
 {
  box(x+sid+1,y,col,sid);
 box(x+sid+1,y+sid+1,col,sid);
 box(x+sid+1,y+sid*2+2,col,sid);
 box(x,y+sid*2+2,col,sid);
 }
 else
 if(c=='t')
 {
 box(x,y,col,sid);
 box(x+sid*1+1,y,col,sid);
 box(x+sid*2+2,y,col,sid);
 box(x+sid*1+1,y+sid+1,col,sid);

 }
}
void shape_mod(int x_old,int y_old,int x,int y) //erases old shape location x_old y_old
{                                               //and creates new shape at new location
 shape(x_old,y_old,shp,side,BLACK);
 shape(x,y,shp,side,shp_col);
}
void intro()//fancy intro
{
int TETRIS[]={66,
/*T*/10,10,9,10,8,10,7,10,6,10,8,11,8,12,8,13,8,14,
/*E*/12,10,13,10,14,10,15,10,12,12,13,12,14,12,15,12,12,14,13,14,14,14,15,14,12,11,12,13,17,10,
/*T*/18,10,19,10,20,10,21,10,19,11,19,12,19,13,19,14,
/*R*/23,10,24,10,25,10,23,12,24,12,23,11,25,11,23,13,23,14,25,13,25,14,
/*I*/27,10,28,10,29,10,28,11,28,12,28,13,28,14,27,14,29,14,31,10,
/*S*/32,10,33,10,34,10,31,11,31,12,32,12,33,12,34,12,34,13,31,14,32,14,33,14,34,14
};
gotoxy(30,16);
cout<<"press any key to start...";
line(0,241,getmaxx(),241);
line(0,254,getmaxx(),254);//2 lines above and below "press any key to start"

while(!kbhit())
{
randomize();
for(int j=0;j<30;j++)
for(int i=0;i<40;i++)            //changing color boxes in intro
{
if(ins(i,j,TETRIS)==1)
box(side*i+i,side*j+j,WHITE);
else if(j>15||j<10)
box(side*i+i,side*j+j,random(14)+1);
}
}
getch();
}
void animate(int i=0)  //opening clearing lines
{
if(i==0)               //clearing lines start from a given gap
{
gotoxy(30,16);
cout<<"                         ";  //to clear middle text
line(0,241,getmaxx(),241);
line(0,254,getmaxx(),254);

for(int i=241,j=254;i>=0;i--)
{                                 //actual clearing lines animation
 setcolor(WHITE);
 line(0,i,getmaxx(),i);
 setcolor(BLACK);
 line(0,i+1,getmaxx(),i+1);
 j++;
 setcolor(WHITE);
 line(0,j,getmaxx(),j);
 setcolor(BLACK);
 line(0,j-1,getmaxx(),j-1);
 delay(2);
}
}
else          //clearing starts from same line  i.e. "restart" animation
	      // (i added this one later)
{
 line(0,248,getmaxx(),248);
line(0,248,getmaxx(),248);
for(int i=248,j=248;i>=0;i--)
{
 setcolor(WHITE);
 line(0,i,getmaxx(),i);
 setcolor(BLACK);
 line(0,i+1,getmaxx(),i+1);
 j++;
 setcolor(WHITE);
 line(0,j,getmaxx(),j);
 setcolor(BLACK);
 line(0,j-1,getmaxx(),j-1);
 delay(2);
}
}

}
void interface()           //entire gameplay interface buttons and all
{
setcolor(WHITE);
				      //////////////////////
 for(int j=1;j<=3;j++)
 for(int i=0;i<12;i++)
  box(side*(30+i)+i,side*(1+j)+j,random(14)+1);
					       //those blinking boxes
 for( j=1;j<=3;j++)                            //above and below "upcoming shape"
 for( i=0;i<12;i++)
  box(side*(30+i)+i,side*(11+j)+j,random(14)+1);
				     ////////////////////////

 line(side*30-1,0,side*30-1,getmaxy());
  gotoxy(63,1);
 cout<<">>> Score <<<";                       //score interface (duh!)
 gotoxy(58,2);
 cout<<score;
 line(side*30,15,getmaxx(),15);
 line(side*30,30,getmaxx(),30);

 gotoxy(67,17);
 (play==1?cout<<"pause":cout<<"play");        // play/pause button desgin
 line(side*30,side*17+1,side*43,side*17+1);
 line(side*30,side*17+17,side*43,side*17+17);

 gotoxy(66,21);
 cout<<"restart";                            // restart button desgin
   line(side*30,side*21+4,side*43,side*21+4);
   line(side*30,side*22+4,side*43,side*22+4);


 gotoxy(63,25);
 cout<<"?";                                   //  ? button desgin
 line(side*31,side*25+9,side*36-8,side*25+9);
 line(side*31,side*26+7,side*36-8,side*26+7);

 gotoxy(73,25);
 cout<<"cheat";                               // cheat button desgin
 line(side*37+5,side*25+9,side*42,side*25+9);
 line(side*37+5,side*26+7,side*42,side*26+7);

}

int pattern()//keeps gameplay interface active and modified (use in loop)
{
if(tim==speed)      //colorful blinking boxes
{
 randomize();
 for(int j=1;j<=3;j++)
 for(int i=0;i<12;i++)
  box(side*(30+i)+i,side*(1+j)+j,random(14)+1);

 for( j=1;j<=3;j++)
 for( i=0;i<12;i++)
  box(side*(30+i)+i,side*(11+j)+j,random(14)+1);
}
   gotoxy(58,2);
 cout<<score;
  if(action==0&&mouse_in(side*30-1,side*17+1,side*43,side*17+17)!=0)
  {                              //mouse inside play/pause button
  hide_mouse();                  //modifies the button
  setcolor(BLACK);
   line(side*30,side*17+1,side*43,side*17+1);
  line(side*30,side*17+17,side*43,side*17+17);

  setcolor(WHITE);
   rectangle(side*30-1,side*16,side*43,side*19);
  action=1;
  show_mouse();
  }
  else
  if(action==1&&mouse_in(side*30-1,side*16,side*43,side*19)==1&&mouse_on==1)
  {   if(play==1)             //clicked inside play/pause
      play=0;
      else
      play=1;
      hide_mouse();
	 gotoxy(67,17);
 (play==1?cout<<"pause":cout<<"play ");
      show_mouse();
       mouse_on=0;
  }

  else
  if(action==0&&mouse_in(side*30-1,side*21+4,side*43,side*22+4)!=0)
  {                                           //mouse inside reset button
  hide_mouse();
  setcolor(BLACK);
  line(side*30,side*21+4,side*43,side*21+4);
  line(side*30,side*22+4,side*43,side*22+4);

  setcolor(WHITE);
  rectangle(side*30-1,side*21-5,side*43,side*24-10);
  action=1;
  show_mouse();
  }
  else  if(action==1&&mouse_in(side*30-1,side*21-5,side*43,side*24-10)==1&&mouse_on==1)
  {   reset=1; mouse_on=0;                  //mouse clicked inside reset button
  }
  else
  if(action==0&&mouse_in(side*31,side*25+9,side*36-8,side*26+7)!=0)
  {
  hide_mouse();                             //mouse inside ? button
   gotoxy(73,25);
 cout<<"      ";
  gotoxy(63,25);
  cout<<" ";
  gotoxy(69,25);
  cout<<"?";
  setcolor(BLACK);
 line(side*31,side*25+9,side*36-8,side*25+9);
 line(side*31,side*26+7,side*36-8,side*26+7);
 line(side*37+5,side*25+9,side*42,side*25+9);
 line(side*37+5,side*26+7,side*42,side*26+7);

  setcolor(WHITE);
  rectangle(side*30-1,side*25-5,side*43,side*28-10);
  action=1;
  show_mouse();
  }
  else   if(action==1&&mouse_in(side*30-1,side*25-5,side*43,side*28-10)==1&&mouse_on==1)
  {   q=1; mouse_on=0;            //mouse clicked inside ? button
  }
  else
  if(action==0&&mouse_in(side*37+5,side*25+9,side*42,side*26+7)!=0) //cheats
  {                               //mouse inside cheat button
  hide_mouse();
  gotoxy(63,25);
  cout<<" ";
   gotoxy(73,25);
   cout<<"      ";
 gotoxy(67,25);
 cout<<"cheats";
					   //erasing previous lines
					   //which ones? i have no idea
  setcolor(BLACK);
 line(side*31,side*25+9,side*36-8,side*25+9);
 line(side*31,side*26+7,side*36-8,side*26+7);
 line(side*37+5,side*25+9,side*42,side*25+9);
 line(side*37+5,side*26+7,side*42,side*26+7);

  setcolor(WHITE);
  rectangle(side*30-1,side*25-5,side*43,side*28-10);
  action=1;
  show_mouse();
  }
  else
    if(action==1&&mouse_in(side*37+5,side*25+9,side*42,side*26+7)==1&&mouse_on==1)
  {   cheats=1; mouse_on=0;   }     //mouse clicked inside cheats button

  else
  if(
  action==1&&mouse_in(side*30-1,side*25-5,side*43,side*28-10)==0
  &&mouse_in(side*30-1,side*21-5,side*43,side*24-10)==0
  &&mouse_in(side*30-1,side*16,side*43,side*19)==0 )
  action=3;                //if mouse not in any button (sloppy i know)

 else if(action==3) //sets everything back to defualt under non mouse action
 {                  //conditions
  hide_mouse();
   gotoxy(67,17);
 (play==1?cout<<"pause":cout<<"play");
    gotoxy(63,25);
  cout<<"?";
   gotoxy(69,25);
  cout<<" ";
   gotoxy(67,25);
 cout<<"      ";

 gotoxy(73,25);
 cout<<"cheat";
 setcolor(BLACK);
 rectangle(side*30,side*16,side*43,side*19); // play/pause
 rectangle(side*30,side*21-5,side*43,side*24-10);//reset
 rectangle(side*30,side*25-5,side*43,side*28-10);//?
 rectangle(side*37+4,side*25-5,side*42,side*28-10);//cheats

 setcolor(WHITE);
 line(side*30,side*17+1,side*43,side*17+1);
 line(side*30,side*17+17,side*43,side*17+17);
  line(side*30,side*21+4,side*43,side*21+4);
  line(side*30,side*22+4,side*43,side*22+4);
 line(side*31,side*25+9,side*36-8,side*25+9);
 line(side*31,side*26+7,side*36-8,side*26+7);
  line(side*37+5,side*25+9,side*42,side*25+9);
  line(side*37+5,side*26+7,side*42,side*26+7);
 action=0;
 show_mouse();
 }
if(mouse_stat()==0)
mouse_on=1;

//custom shape size for every upcoming shape visual
if(prev_shp=='i')
shape(side*36-4,side*6-2,'i',20,WHITE);
else
if(prev_shp=='o')
shape(side*35,side*7,'o',20,WHITE);
else
if(prev_shp=='l')
shape(side*36-3,side*6+5,'l',20,WHITE);
else
if(prev_shp=='s')
shape(side*35,side*7,'s',20,WHITE);
else
if(prev_shp=='z')
shape(side*35,side*7,'z',20,WHITE);
else
if(prev_shp=='j')
shape(side*35,side*6+5,'j',20,WHITE);
else
if(prev_shp=='t')
shape(side*35-8,side*7,'t',20,WHITE);
return 0;
}
////////////////////////////actual game logic/////////////////////
int check(int x,int y) //checks if next down block is empty  1=yes 0=no
{
 if(getpixel(x+2,y+side*1.5)==BLACK)
 return 1;
 return 0;
}
int check_right(int x,int y) //checks if next down block is empty  1=yes 0=no
{
 if(getpixel(x+side*1.5,y+2)==BLACK)
 return 1;
 return 0;
}

int check_left(int x,int y) //checks if next down block is empty  1=yes 0=no
{
 if(getpixel(x-side/2,y+2)==BLACK)
 return 1;
 return 0;
}

int go_left()// 0=nope 1=shape can move right i.e blank space available
{

char c=shp;
int x=shp_x,y=shp_y,sid=side;

 if(x==0)
 return 0;

 if(c=='o'&&check_left(x,y)==1&&
    check_left(x,y+sid+1)==1)
 return 1;

 else
 if(c=='i'&&
 check_left(x,y)==1&&
 check_left(x,y+sid+1)==1&&
 check_left(x,y+sid*2+2)==1&&
 check_left(x,y+sid*3+3)==1)
 return 1;

 else
 if(c=='s'&&
 check_left(x+sid+1,y)==1&&
 check_left(x,y+sid+1)==1)
 return 1;

 else
 if(c=='z'&&
 check_left(x,y)==1&&
 check_left(x+sid+1,y+sid+1)==1)
 return 1;

 else
 if(c=='l'&&
 check_left(x,y)==1&&
 check_left(x,y+sid+1)==1&&
 check_left(x,y+sid*2+2)==1)
 return 1;

 else
 if(c=='j'&&
 check_left(x+sid+1,y)==1&&
 check_left(x+sid+1,y+sid+1)==1&&
 check_left(x,y+sid*2+2)==1)
 return 1;

 else
 if(c=='t'&&
 check_left(x,y)==1&&
 check_left(x+sid*1+1,y+sid+1)==1)
 return 1;


 return 0;
}
int go_right() // 0=nope 1=shape can move right i.e blank space available
{
char c=shp;
int x=shp_x,y=shp_y,sid=side;

 if(c=='o'&&
  check_right(x+sid+1,y)==1&&
  check_right(x+sid+1,y+sid+1)==1&&x+sid+1!=side*27+27)
  return 1;

 else
 if(c=='i'&&
 check_right(x,y)==1&&
 check_right(x,y+sid+1)==1&&
 check_right(x,y+sid*2+2)==1&&
 check_right(x,y+sid*3+3)==1&&x!=side*27+27)
 return 1;

 else
 if(c=='s'&&
 check_right(x+sid*2+2,y)==1&&
 check_right(x+sid+1,y+sid+1)==1&&x+sid*2+2!=side*27+27)
 return 1;

 else
 if(c=='z'&&
 check_right(x+sid+1,y)==1&&
 check_right(x+sid*2+2,y+sid+1)==1&&x+sid*2+2!=side*27+27)
 return 1;


 else
 if(c=='l'&&
  check_right(x,y)==1&&
 check_right(x,y+sid+1)==1&&
 check_right(x+sid+1,y+sid*2+2)==1&&x+sid+1!=side*27+27)
 return 1;

 else
 if(c=='j'&&
 check_right(x+sid+1,y)==1&&
 check_right(x+sid+1,y+sid+1)==1&&
 check_right(x+sid+1,y+sid*2+2)==1&&x+sid+1!=side*27+27)
 return 1;

 else
 if(c=='t'&&
 check_right(x+sid*2+2,y)==1&&
 check_right(x+sid*1+1,y+sid+1)==1&&x+sid*2+2!=side*27+27)
 return 1;

return 0;
}

int allowed_down() // 0=no 1=shape can move down blank space available
{
char c=shp;
int x=shp_x,y=shp_y,sid=side;
  if(c=='o'&&
  check(x+sid+1,y+sid+1)==1&&
  check(x,y+sid+1)==1&&y+sid*2+2<sid*30+30)
  return 1;

 else
 if(c=='i'&&check(x,y+sid*3+3)==1&&y+sid*4+4<sid*30+30)
 return 1;

 else
 if(c=='s'&&
 check(x+sid*2+2,y)==1&&
 check(x+sid+1,y+sid+1)==1&&
 check(x,y+sid+1)==1&&y+sid*2+2<sid*30+30)
 return 1;

 else
 if(c=='z'&&
 check(x,y)==1&&
 check(x+sid+1,y+sid+1)==1&&
 check(x+sid*2+2,y+sid+1)==1&&y+sid*2+2<sid*30+30)
 return 1;

 else
 if(c=='l'&&
 check(x,y+sid*2+2)==1&&
 check(x+sid+1,y+sid*2+2)==1&&y+sid*3+3<sid*30+30)
 return 1;

 else
 if(c=='j'&&
 check(x+sid+1,y+sid*2+2)==1&&
 check(x,y+sid*2+2)==1&&y+sid*3+3<sid*30+30)
 return 1;

 else
 if(c=='t'&&
 check(x,y)==1&&
 check(x+sid*2+2,y)==1&&
 check(x+sid*1+1,y+sid+1)==1&&y+sid*2+2<sid*30+30)
 return 1;

return 0;
}

void clear(int n) //actually modifies boxes when filled horizontally
{
 for(j=n-1;j>=0;j--)
  for(i=0;i<28;i++)
   box(side*i+i,side*(j+1)+j+1,getpixel(side*i+i+3,side*j+j+5));
 score+=100;
}

void clean_old()     //checks if row is compltely filled and modifies
{
  for(j=0;j<30;j++)
 {
  for(i=0;i<28;i++)
  if(getpixel(side*i+i+3,side*j+j+5)==BLACK)
  goto cont;
  clear(j);
  cont:;
 }
}


int update()  //1= continue  0=new shape  -1=game over
{
 if(allowed_down()==1) //there is space to go down
 return 1;

 for(i=0;i<28;i++)  //to check if game over height is achived
 {
  if(getpixel(side*i+i+3,5)!=BLACK)
   return -1;
 }
 if(allowed_down()==0&&tim==speed)
 return 0;     //game over height not achived but next block filled
 else
 return 1;
}


int game()  // -1=game over   0=everything cool go on
{
if(play==0) //check if game is on play or pause
{
if(kbhit())    //if paused and  want to exit
{
key=getch();
 if(int(key)==0)
 key=getch();
 if(key=='x')
 return -1;
}
 goto skip;    //if paused and NOT want to exit
}

if(update()==1)  //yep your allowed to move shape  if update()==1
{
if(kbhit())
 {
 key=getch();
 if(int(key)==0)
 key=getch();

 if(key=='x')
   return -1;

 if(int(key)==75&&go_left()==1)//left arrow key pressed
   {
    shape_mod(shp_x,shp_y,shp_x-side-1,shp_y);
   shp_x=shp_x-side-1;              //to be modified
   }
 else if(int(key)==77&&go_right()==1)//pressed right arrow key
   {
   shape_mod(shp_x,shp_y,shp_x+side+1,shp_y);
   shp_x+=side+1;	             //to be modified
   }
   else if(int(key)==80&&allowed_down()==1)//down key pressed
   {
   shape_mod(shp_x,shp_y,shp_x,shp_y+side+1);
  shp_y+=side+1;
   tim=0;
   }
 }
 if(tim==speed)    //times up! now the shape will automatically go down
 {
 shape_mod(shp_x,shp_y,shp_x,shp_y+side+1);
 shp_y+=side+1;
 }
}

if(update()==0)//creates new shap after placing old block
 {
 clean_old();

 j=getcolor();             //to replace white shape
 setcolor(BLACK);
 for(i=0;i<7*side;i++)
 line(side*29+29,side*5+3+i,getmaxx(),side*5+3+i);
 setcolor(j);


randomize();
shp=prev_shp;
prev_shp=shapes[(shp_col+random(7)+int(shp)+int(tim))%7];
shp_x=side*10+10;
shp_y=0;
shp_col=(shp_col+random(14))%14+1;
shape(shp_x,shp_y,shp,side,shp_col);
 }
else if(update()==-1)
return -1;

(tim==501?tim=0:tim+=1);//+0.001 sec
delay(1);

skip:;//time is frozen i.e paused game
return 0;
}
int main()
{
clrscr();
int d=DETECT;
initgraph(&d,0,"c:\\turboc3\\bgi"); //EDIT HERE
cleardevice();
limit_mouse(side*30,side*15+3,getmaxx(),getmaxy());
intro();
animate();
again:;

randomize();
prev_shp=shapes[random(7)];
prev_shp_col=random(14)+1;

///new shape initialization///
shp=shapes[random(7)];
shp_col=random(14)+1;
shp_x=side*10+10;
shp_y=0;
shape(shp_x,shp_y,shp,side,shp_col);
/////////////////

qe:;
if(q==1)
{
animate(1);
q=0;
hide_mouse();
 gotoxy(1,1);
 cout<<"Operating Keys:"<<endl<<"WASD keys as usual and 'x' to exit"<<endl<<endl;
 cout<<"Cheats:"<<endl<<"ask the creator"<<endl<<endl;
 cout<<"Creator:"<<endl<<"Manas Makde"<<endl<<endl;
 cout<<"Contact:"<<endl<<"pfff i have a unique name, find me"<<endl<<endl;
 cout<<"<cheats not yet added>"<<endl<<endl;
 cout<<"press any key to continue...";
 getch();
 animate(1);
 show_mouse();
}
hide_mouse();
interface();
show_mouse();
show_mouse();
do
{
pattern();
 if(reset==1)
 {
 tim=1;
 play=1,action=0,score=0,reset=0,q=0,cheats=0;
 hide_mouse();
 animate(1);
 goto again;
 }
 if(q==1)
  goto qe;
}
while(game()!=-1);  //even i don't know why i used a do while loop xD

if(key!='x')
{
gotoxy(25,1);
cout<<"game over!";  //yep you guessed it... GAME OVER
getch();
}
return 1;
}
//wow you actually sat down and read that? kudos to that!
//oh and just so you know this is a 2 days sloppy weekends work
//i can do much better don't have time tho (no actually i'm lazy)
///anyways have a fantastically awesome day!

