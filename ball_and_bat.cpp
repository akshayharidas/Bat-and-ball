#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

static GLfloat spin = 0.0;
static GLint paddle_height = 20, paddle_width = 150; //paddile_velocity = 8.0;
static GLint ball_radius=20, ball_x=rand()%1200, ball_y=750,ball_velocity_y=rand()%5,ball_velocity_x=rand()%3-rand()%1;
static int  paddle_x=0,playerResult=0,pcResult=0;

char string [100];
void init(void)
{
 glClearColor (0.0, 0.0, 0.0, 0.0);
 glShadeModel (GL_FLAT);
}
void drawText(char*string,int x,int y)
{
	  char *c;
	  glPushMatrix();
	  glTranslatef(x, y,0);
	  glScalef(0.1,0.1,1);
  
	  for (c=string; *c != '\0'; c++)
	  {
    		glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
	  }
	  glPopMatrix();

}
void drawPaddle(int x, int y) {
    glPushMatrix();

    glTranslatef(x, y, 0);

    glBegin(GL_QUADS);
    glColor3f(1.0, 1.0, 1.0);
    //int height = paddle_height / 2;
    glVertex2f( 0, paddle_height);
    glVertex2f(paddle_width , paddle_height);
    glVertex2f(paddle_width , 0);
    glVertex2f(0, 0);
    glEnd();

    glPopMatrix();
}
void drawBall(int x, int y) {
    glPushMatrix();

    glTranslatef(x, y, 0);
    glColor3f(1.0, 1.0, 1.0);
    glutSolidSphere (ball_radius, 20, 16);

    glPopMatrix();
}


void display(void)
{
 glClear(GL_COLOR_BUFFER_BIT);
 
    glColor3f(1,1,1);
	sprintf(string,"PC : %d ",pcResult); 
	drawText(string,10,600); 
	sprintf(string,"YOU : %d ",playerResult);
	drawText(string,10,550);
 glPushMatrix();
 //glRotatef(spin, 0.0, 0.0, 1.0);
 glColor3f(1.0, 1.0, 1.0);
 //glRectf(-25.0, -25.0, 25.0, 25.0);
 glPopMatrix();
 drawPaddle(paddle_x,0);
 drawBall(ball_x,ball_y);

 
  if(pcResult==2 || playerResult==2){
         if(pcResult==2){
             glColor3f(1,0,0);
             sprintf(string,"GAME OVER!! ");
             //glScalef(3, 5, 0);
             drawText(string,500,500);
             sprintf(string,"PC WON "); 
             drawText(string,500,480);
             glColor3f(1,0,0);
             sprintf(string,"Press ESC to exit! "); 
             drawText(string,500,450 );
             usleep(2000000);
             //exit(0);
            }
          else{
            glColor3f(1,0,0);
            sprintf(string,"GAME OVER!! ");  
            drawText(string,500,500);
            //glScalef(3, 5, 0);
            sprintf(string,"YOU WON "); 
             drawText(string,500,480);
             glColor3f(1,0,0);
             sprintf(string,"Press ESC to exit! "); 
             drawText(string,500,450);
             usleep(2000000);
             //exit(0);
            }  
  }
 
 glutSwapBuffers();
}

void spinDisplay(void)
{
 spin = spin + 2.0;
 if (spin > 360.0)
 spin = spin - 360.0;
 ball_y=ball_y - ball_velocity_y;
 ball_x=ball_x + ball_velocity_x;
/* if(ball_y<0){
     ball_y=800;
     ball_x= rand()%1200;
}*/
if((ball_y - ball_radius < paddle_height) && (ball_y - ball_radius>0)){ //bat
    if((ball_x > paddle_x) && (ball_x < paddle_width + paddle_x)){
        ball_velocity_y=-(ball_velocity_y+1);
        playerResult++;
        //ball_velocity_y += 0.3;
        //ball_y=ball_y - ball_velocity_y+1;
        
    }
}

if(ball_y - ball_radius<0){                         //bottom
            ball_velocity_y=-(ball_velocity_y+1);
            pcResult++;
}
if(ball_y + ball_radius>800){                          //top
            ball_velocity_y=-ball_velocity_y;

}
if((ball_x- ball_radius<0)||(ball_x+ ball_radius>1200)){
     ball_velocity_x=-ball_velocity_x;
}
 glutPostRedisplay();
}
void reshape(int w, int h)
{
 glViewport (0, 0, (GLsizei) w, (GLsizei) h);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0, 1200, 0, 800, 0, 100);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}
void mouse(int button, int state, int x, int y)
{
 switch (button) {
 case GLUT_LEFT_BUTTON:
 if (state == GLUT_DOWN){
    
     
 glutIdleFunc(spinDisplay);
 }
 break;
 case GLUT_MIDDLE_BUTTON:
 if (state == GLUT_DOWN)
 glutIdleFunc(NULL);
 break;
 default:
 break;
 }
}
void keyboard (unsigned char key, int x, int y)
{
 switch (key) {
 case 'd':
    if(paddle_x<1200){
     paddle_x=paddle_x+10;
        
    }
 glutPostRedisplay();
 break;
 case 'a':
    if(paddle_x>0){
     paddle_x=paddle_x-10;
        
    }
    break;
    case 27:
         exit(0);
         break;
 glutPostRedisplay();
 break;
 default:
 break;
 }
}
/*
 * Request double buffer display mode.
 * Register mouse input callback functions
 */
int main(int argc, char** argv)
{
 int buf;
 int fd = open("/dev/urandom", O_RDONLY);
 read(fd, &buf, sizeof(buf));
 srand(buf);
 glutInit(&argc, argv);
 glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
 glutInitWindowSize (800, 1200);
 glutInitWindowPosition (10, 20);
 glutCreateWindow (argv[0]);
 init ();
 glutDisplayFunc(display);
 glutReshapeFunc(reshape);
 glutMouseFunc(mouse);
 glutKeyboardFunc(keyboard);
 glutMainLoop();
 return 0;
}
