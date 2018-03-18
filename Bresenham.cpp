#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
#ifdef __APPLE__
// #include <GL/glew.h>   // We don't need the GLEW library for now
#include <GLUT/glut.h>
#else
// #include <GL/glew.h>   // We don't need the GLEW library for now
#include <GL/glut.h>
#endif

#define XOFF          50
#define YOFF          50
#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 600

float K=225.0;
int o=1;
int pntx,pnty,r;
int a[100],arr_x[100],arr_y[100],arr_r[100],arr_max_x[100],arr_max_y[100];
int size,maximum=0;
void display(void);
void myinit(void);
void idle(void);
char choice;

void plot(int x, int y, int pntx, int pnty)
{
	glBegin(GL_POINTS);
        glVertex2i(x+pntx, y+pnty);
	glEnd();
}
void MidPointCircle()
{       
        switch(choice)
	{
	case 'c':
	{
		int x = r;
                int y = 0;
                int h = 1-r;
                plot(x, y,pntx,pnty);
                while (x > y)
                {
                        if (h < 0)
                        {
                                y++;
                                h += 2*y+3;
                        }       
                        else
                        {
                                x--;
                                y++;
                                h += 2*(y-x)+5;
                        }       
                        plot(x, y,pntx,pnty);
                        plot(x, -y,pntx,pnty);
                        plot(-x, y,pntx,pnty);
                        plot(-x, -y,pntx,pnty);
                        plot(y, x,pntx,pnty);
                        plot(-y, x,pntx,pnty);
                        plot(y, -x,pntx,pnty);
                        plot(-y, -x,pntx,pnty);
                }       
		break;
	}
        case 'd':
        {
        for(int k=0;k<size;k++)
        {
                pntx=arr_x[k];
                pnty=arr_y[k];
                r=arr_r[k];
                int x = r;
                int y = 0;
                int h = 1-r;
                plot(x, y,pntx,pnty);
                while (x > y)
                {
                        if (h < 0)
                        {
                                y++;
                                h += 2*y+3;
                        }
                        else
                        {
                                x--;
                                y++;
                                h += 2*(y-x)+5;
                        }
                        plot(x, y,pntx,pnty);
                        plot(x, -y,pntx,pnty);
                        plot(-x, y,pntx,pnty);
                        plot(-x, -y,pntx,pnty);
                        plot(y, x,pntx,pnty);
                        plot(-y, x,pntx,pnty);
                        plot(y, -x,pntx,pnty);
                        plot(-y, -x,pntx,pnty);
                }


        }break;
        }

        case 'e':
	{
        for(int k=0;k<size;k++)
        {
		pntx=arr_x[k];
 		pnty=arr_y[k];
                r=floor((arr_r[k]*o)/K);
        	int x = r;
		int y = 0;
        	int h = 1-r;
		plot(x, y,pntx,pnty);
         	while (x > y)
		{
			if (h < 0)
			{
				y++; 
				h += 2*y+3;
			}
			else
			{
				x--;
				y++;
				h += 2*(y-x)+5;
			}
			plot(x, y,pntx,pnty);
			plot(x, -y,pntx,pnty);
			plot(-x, y,pntx,pnty);
			plot(-x, -y,pntx,pnty);
			plot(y, x,pntx,pnty);
			plot(-y, x,pntx,pnty);
			plot(y, -x,pntx,pnty);
			plot(-y, -x,pntx,pnty);
		}
		
	
	}break;
	}}

}

/* Function to handle file input; modification may be needed */
void file_in(void);

/*-----------------
The main function
------------------*/
int main(int argc, char **argv)
{   
    cout << "Enter c, d or e :"; cin>>choice;
    if (choice=='c')
    {
	cout << "X coordinate :"; cin>>pntx;
        cout << "\n Y coordinate :"; cin>>pnty;
        cout << "\n Enter Radius :"; cin>>r;
    }

    glutInit(&argc, argv);

    /* Use both double buffering and Z buffer */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(XOFF, YOFF);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("CS6533/CS4533 Assignment 1");
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    
    /* Function call to handle file input here */
    file_in();

    myinit();
    glutMainLoop();
    
    return 0;
}

/*----------
file_in(): file input function. Modify here.
------------*/
void file_in(void)
{
    ifstream input_file;
    int l=0,m=0,n=0,maximum1=0,maximum2=0;
    input_file.open("/Users/suchethas/input.txt");
    if (input_file.is_open())
    {
        
        for (int z=0;z<19;z++)
        { 
            input_file >> a[z];
        }
        size=a[0];
        for(int p=1;p<3*size;p=p+3)
        {
            arr_x[l]=a[p];
            l++;
        }
        for(int q=2;q<3*size;q=q+3)
        {
            arr_y[m]=a[q];
            m++;
        }
        for(int r=3;r<=3*size;r=r+3)
        {
            arr_r[n]=a[r];
	    n++;
        }
        for (int e=0;e<size;e++)
        {       
                
		arr_max_x[e]=max((arr_x[e]+arr_r[e]),abs(arr_x[e]-arr_r[e]));
                arr_max_y[e]=max((arr_y[e]+arr_r[e]),abs(arr_y[e]-arr_r[e]));

        }
        //maximum=max(max_element(begin(arr_max_x),end(arr_max_x)),max_element(begin(arr_max_y),end(arr_max_y)));
	for (int g=0;g<size;g++)
	{
		if(arr_max_x[g]>maximum1)
		{
			maximum1=arr_max_x[g];
		}
	}
	for (int h=0;h<size;h++)
        {
                if(arr_max_y[h]>maximum2)
                {
                        maximum2=arr_max_y[h];
                }
        }
        maximum=max(maximum1,maximum2);

        if (maximum>300);
        {
		for (int f=0;f<size;f++)
		{
			arr_x[f]=300+floor(((arr_x[f])*300)/maximum);
			arr_y[f]=300+floor(((arr_y[f])*300)/maximum);
			arr_r[f]=floor((arr_r[f]*300)/maximum);
		}
        }
    }
    else
    {
        cout << "File Not Found";
    }
}

/*---------------------------------------------------------------------
display(): This function is called once for _every_ frame. 
---------------------------------------------------------------------*/
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glColor3f(1.0, 0.84, 0);              /* draw in golden yellow */
    glPointSize(1.0);                     /* size of each point */
    
    MidPointCircle();
    //glBegin(GL_POINTS);
    //glVertex2i(300, 300);               /* draw a vertex here */

    glEnd();

    glFlush();                            /* render graphics */

    glutSwapBuffers();                    /* swap buffers */
}

/*---------------------------------------------------------------------
myinit(): Set up attributes and viewing
---------------------------------------------------------------------*/
void myinit()
{
    glClearColor(0.0, 0.0, 0.92, 0.0);    /* blue background*/

    /* set up viewing */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
}
void idle(void)
{       
	o++;
	if (o>K) o=1;
 	glutPostRedisplay();
}
