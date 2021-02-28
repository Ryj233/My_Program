#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <time.h>


#define UP    1
#define DOWN  2
#define LEFT  3
#define RIGHT 4

struct Snake //贪吃🐍结构体
{
	int hang;
	int lie;
	struct Snake *next;
};

//全局变量
struct Snake* head =NULL;//蛇头
struct Snake* tail =NULL;//蛇尾
struct Snake food;	 //食物
int dir;                 //方向
int fen=0;               //分数

void initFood()//食物位置生成
{
	int x;
	int y;
    while(1){	
        x=rand()%19;

        y=rand()%19;	
               

	if (x!=0 && y !=0)
	{
         break;	        
	}
      } //防止x=0或y=0            

	food.hang=x;
	food.lie =y;
	food.next=NULL;
	printw("Next Food is: x:%d y:%d\n",food.hang,food.lie);

}


void initScr()//初始化curse界面
{
	initscr();
	keypad(stdscr,1);
}


int hasSnakeNode(int i,int j)//检测链表节点
{
	struct Snake * point=head;
	while(point!=NULL)
	{
		if(point->hang==i && point->lie==j)
		{
			return 1;
		}
	  point=point->next;
	}//历遍链表判断当前位置是否为蛇身节点
	return 0;
}

int hasFood(int i,int j)//判断当前位置是否为食物位置
{
	if(food.hang==i && food.lie==j)
	{
		return 1;
	}

	return 0;
}

void gamPic()//初始化地图
{      
	int hang=0;
	int lie;
        move(0,0);
	while(hang<21){

		if(hang==0)
		{
			for(lie=0;lie<20;lie++)
			{
			 printw("--");
			}
		}else
		 {

			if(hang==20)
			{
				for(lie=0;lie<20;lie++)
				{
				 printw("--");
				}
			}else
			{
			
			for(lie=0;lie<21;lie++){
				if (lie==0||lie==20)
				{
                                   printw("|");
			        }else
			         {
			             if(hasSnakeNode(hang,lie))
					 {
						 printw("[]");
					 }else if(hasFood(hang,lie))
					 {
						 printw("##");
					 }
				     
				     else{
			         		  printw("  ");
					}
				 }	 

			 }
		        }
		}
			printw("\n");
                        hang++;
	        
	}
	
}



void addNode()//增加节点
{
	struct Snake *new =(struct Snake*)malloc(sizeof(struct Snake));

     switch(dir)
     {
	     case UP:
		    new->hang=tail->hang-1;
                    new->lie=tail->lie;
                    new->next=NULL;
		    break;
             case DOWN:
                    new->hang=tail->hang+1;
                    new->lie=tail->lie;
                    new->next=NULL;
		    break;
	     case LEFT:
                    new->hang=tail->hang;
                    new->lie=tail->lie-1;
                    new->next=NULL;
		    break;
	     case RIGHT:
                    new->hang=tail->hang;
                    new->lie=tail->lie+1;
                    new->next=NULL;
		    break;   

     }


	tail->next =new;
	tail=new;
}


void initSnake()//蛇身的初始化
{
        initFood();
        dir=RIGHT;
	struct Snake *p;
	while(head!=NULL){
	        p=head;
		head=head->next;
		free(p);
	}

	head =(struct Snake *)malloc(sizeof(struct Snake));
	head->hang=2;
	head->lie=2;
	head->next=NULL;
	tail=head;
	head->next=tail;

	addNode();
	addNode();
	addNode();
	addNode();
	addNode();
}	

int ifSnakedie()//判断蛇是否撞到自己
{
    struct Snake *p;
    p=head;



    while(p->next!=NULL)
    {
	  if (p->hang == tail->hang && p->lie == tail->lie)
	  {
		 return 1;
	  }

	  p=p->next;
    }

    return 0;
}



void movesnake()//蛇身移动，没吃食物移动为尾增节点、头减节点；吃食物后不删除头部节点即可！
{
	addNode();
	if(hasFood(tail->hang,tail->lie)){
	     initFood();
	     fen=fen+1;
	}else{
	
	head=head->next;//删除节点
	}
		if((tail->hang==0||tail->lie==0||tail->hang==20||tail->lie==20) ||(ifSnakedie()) )//撞墙、撞自己检测
		{
			initSnake();
			fen=0;
		}

}


void* refreshjiemian()//刷新界面
{
	        while(1)
             {
                        movesnake();
                        gamPic();
                        refresh();
	        	printw("Game score is:%d \n",fen*10);
                        usleep(100000);

            }

}


void* changeDirction() //检测键入的方向
{

	int c;
	
	        while(1)
             {   c=getch();
                switch(c){

                case 0402:
			if (dir!=UP)
	         	{
			 dir=DOWN;
			}
                       // printw("DOWN\n");
                        break;

                case 0403:
	         	if(dir!=DOWN)
			{
			dir=UP;
			}
                       // printw("UP\n");
                        break;

                case 0404:
		       if(dir!=RIGHT)
		       {
			dir=LEFT;
			}
                       // printw("LEFT\n");
                        break;

                case 0405:
			if (dir!=LEFT)
			{
			dir=RIGHT;
			}
                       // printw("RIGTH\n");
                        break;
            }
	     }
}


int main()
{     
	pthread_t th1;
	pthread_t th2;

	initScr();

        initSnake();

	gamPic();
	//多线程
	pthread_create(&th1,NULL,changeDirction,NULL);
	pthread_create(&th2,NULL,refreshjiemian,NULL);
   
       while(1);
  
        getch();
	endwin();
   
	return 0;
}
