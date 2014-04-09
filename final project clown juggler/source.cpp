//Dan Lustig
//12/8/13
//juggling snowman


#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <GL/glut.h>

using namespace std;

/* BMP file loader */
#ifndef FILE_STUFF
#define FILE_STUFF


struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;
#endif

#define DEGREES_PER_PIXEL	0.6f
#define UNITS_PER_PIXEL		0.1f
#define ZOOM_FACTOR		0.04f

#define BULB_RADIUS		0.3f
#define SPOTLIGHT_RADIUS	0.5f
#define ROOM_SIZE			4.0f
#define WALL_THICKNESS		0.05f

void idleCallBack();


void LoadGLTextures();

GLuint texture[5];




bool paused = false;

bool turnA = true;
bool turnB = false;
bool turnC = false;
bool turnD = false;

bool turnA2 = false;
bool turnB2 = false;
bool turnC2 = true; //true
bool turnD2 = false;

bool turnA3 = false;
bool turnB3 = false;
bool turnC3 = false;
bool turnD3 = true; //true

bool turnSingleBallAnimationOn = false;

bool turnBall1On = true;
bool turnBall2On = false;
bool turnBall3On = false;

bool spotlight = false;

int num_segments = 180;
float ii = 0;
float ballX;
float ballY;
float loopSpeed = 2.000f;
float ball3LoopDelay = 0.500f;



//angle of rotation
GLfloat angle = 0.0;

//diffuse light color variables
GLfloat dlr = .10;
GLfloat dlg = .10;
GLfloat dlb = .10;

//ambient light color variables
GLfloat alr = .10;
GLfloat alg = .10;
GLfloat alb = .10;

//light position variables
GLfloat lx = 0.0;
GLfloat ly = 0.0;
GLfloat lz = 1.0;
GLfloat lw = 0.0;

GLfloat lx2 = 0.0;
GLfloat ly2 = 0.0;

GLfloat lx3 = 0.0;
GLfloat ly3 = 0.0;


GLfloat density = 0.0; //set the density  = .05

GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; //set the for color to grey



typedef struct
{
	bool leftButton;
	bool rightButton;
	int x;
	int y;
} MouseState;

MouseState mouseState = {false, false, 0, 0};

/* scene rotation angles to alter interactively */
float xRotate = 0, yRotate = 0;

/* quadrics pointers for GLU spheres and cylinders */
GLUquadricObj *bulbObj;


/* camera position and orientation -- used by gluLookAt */
GLfloat eye[] = {0, 8, 8};
GLfloat at[] = {0, 0, 0}; 

/* The room (composed of three orthogonal walls) is 
* centered at the origin of the coordinate system. 
*/




static unsigned int getint(FILE *fp)

{
	int c, c1, c2, c3;

	/*  get 4 bytes */ 
	c = getc(fp);  
	c1 = getc(fp);  
	c2 = getc(fp);  
	c3 = getc(fp);

	return ((unsigned int) c) +   
		(((unsigned int) c1) << 8) + 
		(((unsigned int) c2) << 16) +
		(((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE *fp)

{
	int c, c1;

	/* get 2 bytes*/
	c = getc(fp);  
	c1 = getc(fp);

	return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

void idleCallBack(void)
{


	glutPostRedisplay();
}

void room()
{
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;


	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);


	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0,0.0);glVertex3f(-40, 0, -45);
	glTexCoord2f(1.0,0.0);glVertex3f(40, 0, -45);
	glTexCoord2f(1.0,1.0);glVertex3f(40, 0, 45);
	glTexCoord2f(0.0,1.0);glVertex3f(-40, 0, 45);
	glEnd();


	/*glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,.15,0);
	glScalef(ROOM_SIZE * 1.5, WALL_THICKNESS * 1.5, ROOM_SIZE);
	glutSolidCube( 10.0 );
	glPopMatrix();
	*/



	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);



}

void DrawBackground()
{
	/*GLfloat mat_specular[]  = {0.393548,0.271906,0.166721,1};  
	GLfloat mat_diffuse[]  = {0.714,0.4284,0.18144,1};  
	GLfloat mat_ambient[]  = {0.2125,0.1275,0.054,1}; 
	GLfloat mat_shininess  = .2;


	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	*/


	glColor3f(1.0,0.0,0.0);

	glPushMatrix();

	glTranslatef(-3,15,-25);
	glScalef(20,15,1);


	//FIELDS - gl quad strip
	glColor3f(.5,.25,0);
	glBegin(GL_QUAD_STRIP);
	glVertex2f(-1,-1); //bottom left
	glVertex2f(-1,-.98);//top left
	//peaks & bottom peaks
	glVertex2f(-.90,-.95);
	glVertex2f(-.90, -1);

	glVertex2f(-.55,-.93);
	glVertex2f(-.55,-1);

	glVertex2f(-.40, -.90);
	glVertex2f(-.40, -1);

	glVertex2f(-.28, -.95);
	glVertex2f(-.28, -1);

	glVertex2f(0.1,-.89);
	glVertex2f(0.1, -1);

	glVertex2f(.38,-.85);
	glVertex2f(.38,-1);

	glVertex2f(.70, -.93);
	glVertex2f(.70,-1);

	glVertex2f(1,-.95);//top right
	glVertex2f(1,-1);//bottom right

	glVertex2f(-.95, -1);
	glEnd();




	glTranslatef(0,-.05,15);
	glRotatef(-50,0,1,0);

	//ferris wheel circle - gl line strip
	glColor3f(1, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex2f(.5,-.58);//top left
	glVertex2f(.60,-.58);//top right
	glVertex2f(.66,-.63);
	glVertex2f(.69,-.70);//mid right top
	glVertex2f(.69,-.80);//mid right bottom
	glVertex2f(.66,-.87);
	glVertex2f(.6,-.92);//right bottom
	glVertex2f(.5,-.92);//left bottom
	glVertex2f(.44,-.87);
	glVertex2f(.41,-.80);//mid left bottom
	glVertex2f(.41,-.70);//mid left top
	glVertex2f(.44,-.63);
	glVertex2f(.5,-.58);//top left connect
	glEnd();


	//ferris wheel legs - gl lines
	glBegin(GL_LINES);
	glVertex2f(.55,-.75);//left bottom
	glVertex2f(.50,-.95);//right bottom
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(.55,-.75);//left bottom
	glVertex2f(.60,-.95);//right bottom
	glEnd();

	//ferris wheel center - gl line loop
	float cx =.55,cy=-.75, r=.05;
	int num_segments = 180;
	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments;ii++)
	{
		float theta = 2.0 * 3.1415926f * float(ii)/ float(num_segments);//get the current angle
		float x = r * cosf(theta);//calc the x component
		float y = r * sinf(theta);//calc the y component
		glVertex2f(x+cx,y+cy);//outputvertex
	}
	glEnd();

	//ferris wheel seats (animation)
	const float fLoopDuration = 5.0f;
	const float fScale = 3.14159f * 2.0f / fLoopDuration;
	float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float radiusSeat = .15f;
	float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);
	float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * radiusSeat;
	float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * radiusSeat;
	//moving ball
	float X = .55 + fXOffset;
	float Y = -.75 + fYOffset;
	glBegin(GL_QUADS);
	glVertex2f(X,Y);
	glVertex2f(X, Y+.05);
	glVertex2f(X+.05,Y+.05);
	glVertex2f(X+.05,Y);
	glEnd();


	//tent - gl triangles
	glBegin(GL_TRIANGLES);
	glVertex2f(.30,-.95);//left 
	glVertex2f(.35,-.75);//top
	glVertex2f(.40,-.95);//right
	glEnd();

	//tent 3d front - gl triangle fan
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(.35,-.75);
	glVertex2f(.38,-.95);
	glVertex2f(.36,-.96);
	glVertex2f(.34,-.96);
	glVertex2f(.32,-.95);
	glEnd();

	//tent flag - gl triangle strip
	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(.35,-.75);// bottom
	glVertex2f(.35,-.65);// top
	glEnd();
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(.35,-.65);//top left
	glVertex2f(.38,-.65);//top right
	glVertex2f(.38,-.70);//bottom right
	glVertex2f(.35,-.70);//left bottom
	glVertex2f(.365,-.675);//middle
	glEnd();

	glPopMatrix();

}

void DrawMoon(){
	GLfloat mat_specular[]  = {0.393548,0.271906,0.166721,1};  
	GLfloat mat_diffuse[]  = {0.714,0.4284,0.18144,1};  
	GLfloat mat_ambient[]  = {0.2125,0.1275,0.054,1}; 
	GLfloat mat_shininess  = .2;


	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);


	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[2]);
	glTranslatef(-2,8,-10);
	glutSolidSphere(3,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void DrawBody()
{
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);


	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);



	//pointed buttons
	glPushMatrix();
	glTranslatef(0,2.5,.65);
	glutSolidCone(.15,.35, 40, 40);
	glPopMatrix();

	//pointed buttons
	glPushMatrix();
	glTranslatef(0,2.1,.65);
	glutSolidCone(.15,.35, 40, 40);
	glPopMatrix();

	//pointed buttons
	glPushMatrix();
	glTranslatef(0,1.7,.9);
	glutSolidCone(.15,.35, 40, 40);
	glPopMatrix();

	//pointed buttons
	glPushMatrix();
	glTranslatef(0,1.3,1.1);
	glutSolidCone(.15,.35, 40, 40);
	glPopMatrix();

	//pointed buttons
	glPushMatrix();
	glTranslatef(0,.9,1.18);
	glutSolidCone(.15,.35, 40, 40);
	glPopMatrix();

}

void DrawS1(){
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);

	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

}

void DrawS2(){
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);

	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

}

void DrawS3(){
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);

	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

}

void DrawS4(){
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);

	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

}

void DrawS5(){
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);

	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

}

void DrawS6(){
	GLfloat mat_specular[]  = {1,1,1,1};  
	GLfloat mat_diffuse[]  = {.4,.4,.4,1};  
	GLfloat mat_ambient[]  = {.774597,.774597,.774597,1};  
	GLfloat mat_shininess  = 76.8;
	//GLfloat mat_emission[] = [1.0,1.0,1.0,1.0];

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,mat_emission);

	//lower body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,1,0);
	glutSolidSphere(1.25,40,40);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	//middle body
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	glTranslatef(0,2.3,0);
	glutSolidSphere(.8,40,40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);


	//head
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glTranslatef(0,3.3,0);
	glutSolidSphere(.5, 40, 40);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);

}

void DrawRightArm()
{


	//right upper arm
	glPushMatrix();
	glTranslatef(-.95,2.3,.35);
	glRotatef(-35,0,0,1); //away from body
	glRotatef(-25,1,0,0); //toward front
	glScalef(.15,1.75,.15);
	glutSolidSphere(.3,20,20);
	glPopMatrix();


	//right lower arm
	glPushMatrix();
	glTranslatef(-1.18,1.72,.72);
	glRotatef(-10,0,0,1);
	glRotatef(-45,1,0,0);
	glScalef(.15,1.4,.15);
	glutSolidSphere(.3,20,20);
	glPopMatrix();


	//hand - finger 1
	glPushMatrix();
	glTranslatef(-1.18,1.55,1.0);
	glRotatef(-25,0,1,0);
	glRotatef(-150,0,0,1);
	glRotatef(75,1,0,0);
	glScalef(.15,1.0,.15);
	glutSolidSphere(.2,20,20);
	glPopMatrix();

	//hand - finger 2
	glPushMatrix();
	glTranslatef(-1.20,1.65,1.0);
	glRotatef(-25,0,1,0);
	glRotatef(-25,0,0,1);
	glRotatef(75,1,0,0);
	glScalef(.15,1.0,.15);
	glutSolidSphere(.2,20,20);
	glPopMatrix();

	//hand - finger 3
	glPushMatrix();
	glTranslatef(-1.20,1.50,.95);
	glRotatef(25,0,0,1);
	glRotatef(45,0,1,0);
	glScalef(.15,1.0,.15);
	glutSolidSphere(.2,20,20);
	glPopMatrix();



}

void DrawLeftArm()
{
	//left upper arm
	glPushMatrix();
	glTranslatef(.95,2.3,.35);
	glRotatef(35,0,0,1); //away from body
	glRotatef(-25,1,0,0); //toward front
	glScalef(.15,1.75,.15);
	glutSolidSphere(.3,20,20);
	glPopMatrix();


	//left lower arm
	glPushMatrix();
	glTranslatef(1.18,1.72,.72);
	glRotatef(-10,0,0,1);
	glRotatef(-45,1,0,0);
	glScalef(.15,1.4,.15);
	glutSolidSphere(.3,20,20);
	glPopMatrix();


	//hand - finger 1
	glPushMatrix();
	glTranslatef(1.18,1.55,1.0);
	glRotatef(-25,0,1,0);
	glRotatef(-150,0,0,1);
	glRotatef(75,1,0,0);
	glScalef(.15,1.0,.15);
	glutSolidSphere(.2,20,20);
	glPopMatrix();

	//hand - finger 2
	glPushMatrix();
	glTranslatef(1.20,1.65,1.0);
	glRotatef(-25,0,1,0);
	glRotatef(-25,0,0,1);
	glRotatef(75,1,0,0);
	glScalef(.15,1.0,.15);
	glutSolidSphere(.2,20,20);
	glPopMatrix();

	//hand - finger 3
	glPushMatrix();
	glTranslatef(1.20,1.50,.95);
	glRotatef(25,0,0,1);
	glRotatef(45,0,1,0);
	glScalef(.15,1.0,.15);
	glutSolidSphere(.2,20,20);
	glPopMatrix();


}

double trunc(double d){
	return (d>0) ? floor(d) : ceil(d) ; 
}

void DrawBall1()
{
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);





	if (!paused && turnBall1On == true){
		//turn 1!!!!!!!!!!!!!!!
		if (turnA == true){


			//***********************//

			//If I change the floopDuration to 2.5 instead of 5, the following loop is skipped. why is that?
			const float fLoopDuration = loopSpeed;


			//***********************//

			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.45f;

			float X,Y;
			//moving ball
			X = .7 + fXOffset;
			Y = 2 - fYOffset;
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D,texture[1]);
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 100);
			int nearestY = floorf(Y);

			if (nearestX == 25){
				turnB = true;
				turnA = false;
				turnBall3On = true;
				turnBall2On = true;
			}
			lx = X;
			ly = Y;
		}



		//turn 2!!!!!!!!!!!!!!!!!!!!!!
		if (turnB == true){

			const float fLoopDuration1 = loopSpeed;
			const float fScale1 = (3.14159f * 2.0f / fLoopDuration1);

			float fElapsedTime1 = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);


			float fCurrTimeThroughLoop1 = fmodf(fElapsedTime1, fLoopDuration1);

			float fXOffset = cosf(fCurrTimeThroughLoop1 * fScale1) * 0.7f;
			float fYOffset = sinf(fCurrTimeThroughLoop1 * fScale1) * 1.5f;

			float X,Y;

			// minus minus starts on right and goes counter clockwise
			X = -.45 - fXOffset; 
			Y = 2 - fYOffset;
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D,texture[1]);
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();

			int nearestX = trunc(X * 1000);
			int nearestY = floorf(Y);

			if (nearestX <= -1149 ){
				turnB = false;
				turnC = true;

			}
			lx = X;
			ly = Y;
		}



		//turn 3!!!!!!!!!!!!!!!
		if (turnC == true){

			const float fLoopDuration = loopSpeed;
			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.45f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//moving ball
			X = -.7 - fXOffset;
			Y = 2.1 - fYOffset;
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D,texture[1]);
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 100);
			int nearestY = floorf(Y);
			//cout <<nearestX << "   " << X <<endl;
			if (nearestX == -25){
				turnD= true;
				turnC = false;

			}
			lx = X;
			ly = Y;
		}




		//turn 4!!!!!!!!!!!!!!!!!!!!!!
		if (turnD == true){


			const float fLoopDuration = loopSpeed;
			const float fScale = (3.14159f * 2.0f / fLoopDuration);

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.7f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 1.5f;

			float X,Y;

			X = .45 + fXOffset; 
			Y = 2 - fYOffset;
			glPushMatrix();
			glBindTexture(GL_TEXTURE_2D,texture[1]);
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 1000);
			int nearestY = floorf(Y);
			//cout <<nearestX << "   " << X <<endl;
			if (nearestX >= 1149){
				turnD = false;
				turnA = true;

			}
			lx = X;
			ly = Y;
		}


	}
}

void DrawBall2()
{
	if (!paused && turnBall2On == true){
		//turn 1!!!!!!!!!!!!!!!
		if (turnA2 == true){



			//***********************//


			const float fLoopDuration = loopSpeed;


			//***********************//
			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float X,Y;

			//moving ball
			X = .7 - fXOffset;
			Y = 2 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 100);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX == 25 && nearestY == 2){
				turnB2 = true;
				turnA2 = false;
			}
			lx2 = X;
			ly2 = Y;
		}


		//turn 2!!!!!!!!!!!!!!!!!!!!!!
		if (turnB2 == true){
			const float fLoopDuration = loopSpeed;
			const float fScale = (3.14159f * 2.0f / fLoopDuration);

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.7f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 1.5f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//cout << fCurrTimeThroughLoop * fScale <<endl;
			// minus minus starts on right and goes counter clockwise
			X = -.45 + fXOffset; 
			Y = 2 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();

			int nearestX = trunc(X * 1000);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX <= -1149 && nearestY == 2){
				turnB2 = false;
				turnC2 = true;
			}
			lx2 = X;
			ly2 = Y;

		}



		//turn 3!!!!!!!!!!!!!!!
		if (turnC2 == true){
			//turnBall1On = false;

			const float fLoopDuration = loopSpeed;
			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.45f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//moving ball
			X = -.7 + fXOffset;
			Y = 2.1 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 100);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX == -25 && nearestY == 2){
				turnD2 = true;
				turnC2 = false;
			}
			lx2 = X;
			ly2 = Y;
		}




		//turn 4!!!!!!!!!!!!!!!!!!!!!!
		if (turnD2 == true){

			const float fLoopDuration = loopSpeed;
			const float fScale = (3.14159f * 2.0f / fLoopDuration);

			float fElapsedTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.7f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 1.5f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//cout << fCurrTimeThroughLoop * fScale <<endl;
			// minus minus starts on right and goes counter clockwise
			X = .45 - fXOffset; 
			Y = 2 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 1000);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX >= 1149 && nearestY == 2){
				turnD2 = false;
				turnA2 = true;
			}
			lx2 = X;
			ly2 = Y;
		}
	}

}

void DrawBall3()
{
	if (!paused && turnBall3On == true){
		//turn 1!!!!!!!!!!!!!!!
		if (turnA3 == true){


			//***********************//

			//If I change the floopDuration to 2.5 instead of 5, the following loop is skipped. why is that?
			const float fLoopDuration = loopSpeed;


			//***********************//

			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f)-ball3LoopDelay;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.45f;

			float X,Y;
			//moving ball
			X = .7 - fXOffset;
			Y = 2 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();

			int nearestX = trunc(X * 100);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX == 25 && nearestY == 2){
				turnB3 = true;
				turnA3 = false;
			}
			lx3 = X;
			ly3 = Y;
		}


		//turn 2!!!!!!!!!!!!!!!!!!!!!!
		if (turnB3 == true){
			const float fLoopDuration = loopSpeed;
			const float fScale = (3.14159f * 2.0f / fLoopDuration);

			float fElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f)-ball3LoopDelay;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.7f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 1.5f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//cout << fCurrTimeThroughLoop * fScale <<endl;
			// minus minus starts on right and goes counter clockwise
			X = -.45 + fXOffset; 
			Y = 2 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();

			int nearestX = trunc(X * 1000);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX <= -1149 && nearestY == 2){
				turnB3 = false;
				turnC3 = true;
			}
			lx3 = X;
			ly3 = Y;

		}



		//turn 3!!!!!!!!!!!!!!!
		if (turnC3 == true){

			const float fLoopDuration = loopSpeed;
			const float fScale = 3.14159f * 2.0f / fLoopDuration;

			float fElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f)-ball3LoopDelay;

			float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

			float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 0.45f;
			float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 0.45f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//moving ball
			X = -.7 + fXOffset;
			Y = 2.1 + fYOffset;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 100);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX == -25 && nearestY == 2){
				turnD3 = true;
				turnC3 = false;
			}
			lx3 = X;
			ly3 = Y;
		}




		//turn 4!!!!!!!!!!!!!!!!!!!!!!
		if (turnD3 == true){

			const float fLoopDuration2 = loopSpeed;
			const float fScale2 = (3.14159f * 2.0f / fLoopDuration2);

			float fElapsedTime2 = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f)-ball3LoopDelay;

			float fCurrTimeThroughLoop2 = fmodf(fElapsedTime2, fLoopDuration2);

			float fXOffset2 = cosf(fCurrTimeThroughLoop2 * fScale2) * 0.7f;
			float fYOffset2 = sinf(fCurrTimeThroughLoop2 * fScale2) * 1.5f;
			//cout << fXOffset << "   "<< fYOffset <<endl;

			float X,Y;
			//cout << fCurrTimeThroughLoop * fScale <<endl;
			// minus minus starts on right and goes counter clockwise
			X = .45 - fXOffset2; 
			Y = 2 + fYOffset2;
			glPushMatrix();
			glTranslatef(X,Y,1.25);
			glutSolidSphere(.2,20,20);
			glPopMatrix();
			int nearestX = trunc(X * 1000);
			float nearestY = floorf(Y);
			//cout <<nearestX << "   " << tempNumY <<endl;
			if (nearestX >= 1149 && nearestY == 2){
				turnD3 = false;
				turnA3 = true;
			}
			lx3 = X;
			ly3 = Y;
		}
	}
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_2D);
}

void DrawSingleBallAnimation(){
	glEnable(GL_TEXTURE_2D);
	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);


	if (!paused){

		const float fLoopDuration = loopSpeed;


		//***********************//

		const float fScale = 3.14159f * 2.0f / fLoopDuration;

		float fElapsedTime = (glutGet(GLUT_ELAPSED_TIME) / 1000.0f);
		float fCurrTimeThroughLoop = fmodf(fElapsedTime, fLoopDuration);

		float fXOffset = cosf(fCurrTimeThroughLoop * fScale) * 1.0f;
		float fYOffset = sinf(fCurrTimeThroughLoop * fScale) * 1.0f;

		float X,Y;
		//moving ball
		X = 0 + fXOffset;
		Y = 3 - fYOffset;
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D,texture[1]);
		glTranslatef(X,Y,1.25);
		glutSolidSphere(.2,20,20);
		glPopMatrix();
		lx = X;
		ly = Y;
	}

}

void DrawScene()
{


	/* Rotate the scene in the x and y directions */
	glRotatef(xRotate, 0,1,0);
	glRotatef(yRotate, 1,0,0);

	glPushMatrix();
	glScalef(1,1,.60);
	room();
	glPopMatrix();



	//glPushMatrix();

	//glRotatef(turnBody,0,1,0);
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();

	glTranslatef(0,0,-25);
	glScalef(2,2,NULL);
	DrawBackground();

	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);


	DrawBody();
	DrawRightArm();
	DrawLeftArm();
	DrawMoon();
	glPushMatrix();
	glScalef(.75,.75,.75);
	glTranslatef(4,0,-5);
	DrawS1();
	glPopMatrix();
	glPushMatrix();
	glScalef(.90,.70,.90);
	glTranslatef(6,0,-6);
	DrawS2();
	glPopMatrix();
	glPushMatrix();
	glScalef(.65,.65,.65);
	glTranslatef(-5,0,-8);
	DrawS3();
	glPopMatrix();
	glPushMatrix();
	glScalef(.75,.75,1);
	glTranslatef(-4,0,-3);
	DrawS4();
	glPopMatrix();
	glPushMatrix();
	glScalef(.75,.75,.75);
	glTranslatef(-8,0,-10);
	DrawS5();
	glPopMatrix();
	glPushMatrix();
	glScalef(.8,.8,.8);
	glTranslatef(-7,0,-14);
	DrawS6();
	glPopMatrix();


	if (turnSingleBallAnimationOn == true){
		DrawSingleBallAnimation();
	}
	else{
		DrawBall1();
		DrawBall2();
		DrawBall3();
	}

}

/* 
*  This function is called whenever the display needs 
*  to be redrawn. First called when program starts.
*/
void Display(void)
{

	glClearColor(0.0,0.0,0.0,1.0); //clear the screen to black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer
	glLoadIdentity();  
	GLfloat DiffuseLight[] = {dlr, dlg, dlb}; //set DiffuseLight[] to the specified values
	GLfloat AmbientLight[] = {alr, alg, alb}; //set AmbientLight[] to the specified values
	glLightfv (GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
	glLightfv (GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
	GLfloat LightPosition[] = {lx, ly, lz, lw}; //set the LightPosition to the specified values
	glLightfv (GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly

	glLightfv (GL_LIGHT2, GL_DIFFUSE, DiffuseLight);
	glLightfv (GL_LIGHT3, GL_AMBIENT, AmbientLight); //change the light accordingly
	GLfloat LightPosition2[] = {lx2, ly2, lz, lw}; //set the LightPosition to the specified values
	glLightfv (GL_LIGHT2, GL_POSITION, LightPosition2); //change the light accordingly

	glLightfv (GL_LIGHT4, GL_DIFFUSE, DiffuseLight);
	glLightfv (GL_LIGHT5, GL_AMBIENT, AmbientLight); //change the light accordingly
	GLfloat LightPosition3[] = {lx3, ly3, lz, lw}; //set the LightPosition to the specified values
	glLightfv (GL_LIGHT4, GL_POSITION, LightPosition3); //change the light accordingly

	

	if (spotlight == true){
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT6);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

	GLfloat light6_diffuse[]  = {1,1,1,1};  // bright white
	GLfloat light6_specular[]  = {1,1,1,1}; 
	GLfloat light6_ambient[]  = {.8,.8,.8,1};  // soft white


	glLightfv(GL_LIGHT6,GL_DIFFUSE,light6_diffuse);
	glLightfv(GL_LIGHT6,GL_SPECULAR,light6_specular);
	glLightfv(GL_LIGHT6,GL_AMBIENT,light6_ambient);

	GLfloat mat_specular[]  = {1,1,1,1};  // bright white
	GLfloat mat_diffuse[]  = {1,.5,0,1};  // orange color
	GLfloat mat_ambient[]  = {1,.5,0,1};  // same as diffuse
	GLfloat mat_shininess  = 5;

	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialf(GL_FRONT,GL_SHININESS,mat_shininess);
	GLfloat spot_direction[] = {0,2,0};
	GLfloat light_position6[] = {0,2,5};
	//glLightf(GL_LIGHT6,GL_SPOT_EXPONENT,8.0f);
	//glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 45.0);
	//GLfloat light6_position[]  = {0,0,1,1};
	glLightfv(GL_LIGHT6,GL_POSITION,light_position6);
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, spot_direction);
	}
	else{
		glDisable(GL_LIGHT6);
	}



	glEnable (GL_FOG); //enable the fog
	glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2
	glFogfv (GL_FOG_COLOR, fogColor); //set the fog color to our color chosen above
	glFogf (GL_FOG_DENSITY, density); //set the density to the value above
	glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the nicest, may slow down on older cards




	gluLookAt(eye[0], eye[1], eye[2], at[0], at[1], at[2], 0,1,0);
	//gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); //camera position, x,y,z, looking at x,y,z, Up Positions of the camera
	//glMatrixMode( GL_MODELVIEW );
	DrawScene(); 

	glFlush();
	glutSwapBuffers(); //swap the buffers
	//angle++; //increase the angle
	//
	//



	/* Draw to the back buffer */
	//glDrawBuffer( GL_BACK );	

	/* Clear the display */
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* (Re)position the camera */
	/*
	glLoadIdentity();




	/* Before returning, flush the graphics buffer,   
	* so all graphics appear in the window */
	//glFlush();
	//glutSwapBuffers();
}

/*
* A special keyboard event occurs when the user presses a 
* special key (arrows, F? keys). Arrows cause the scene to 
* move in the direction indicated; this is accomplished by 
* moving camera position (and maintaining the orientation).
* HOME and END keys should cause the scene to zoom in and 
* out; this is accomplished by moving the camera along the 
* vector between the eye and the lookat point.
*/
void SpecialKey(int key, int x, int y) 
{
	switch(key) {
	case GLUT_KEY_LEFT:
		/* as camera moves to the right, the image moves to the left */
		eye[0] = eye[0] + UNITS_PER_PIXEL; 
		at[0] = at[0] + UNITS_PER_PIXEL;
		break;
	case GLUT_KEY_RIGHT:	
		eye[0] = eye[0] - UNITS_PER_PIXEL; 
		at[0] = at[0] - UNITS_PER_PIXEL;
		break;
	case GLUT_KEY_UP:
		eye[1] = eye[1] - UNITS_PER_PIXEL; 
		at[1] = at[1] - UNITS_PER_PIXEL;
		break;
	case GLUT_KEY_DOWN:	
		eye[1] = eye[1] + UNITS_PER_PIXEL; 
		at[1] = at[1] + UNITS_PER_PIXEL;
		break;
	case GLUT_KEY_END:	/* zoom out */
		eye[0] = (1 + ZOOM_FACTOR) * eye[0] - at[0] * ZOOM_FACTOR;
		eye[1] = (1 + ZOOM_FACTOR) * eye[1] - at[1] * ZOOM_FACTOR;
		eye[2] = (1 + ZOOM_FACTOR) * eye[2] - at[2] * ZOOM_FACTOR;
		break;  
	case GLUT_KEY_HOME: /* zoom in */		
		eye[0] = (1 - ZOOM_FACTOR) * eye[0] + at[0] * ZOOM_FACTOR;
		eye[1] = (1 - ZOOM_FACTOR) * eye[1] + at[1] * ZOOM_FACTOR;
		eye[2] = (1 - ZOOM_FACTOR) * eye[2] + at[2] * ZOOM_FACTOR;
		break;        
	}
	glutPostRedisplay();
}

/*
* A keyboard event occurs when the user presses a key:
* '+' should cause the scene to zoom in
* '-' should cause the scene to zoom out
*/
void Keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
	case '+':	SpecialKey(GLUT_KEY_HOME, 0, 0);
		break;
	case '-':	SpecialKey(GLUT_KEY_END, 0, 0);
		break;
	case 'l':	xRotate += 10;
		break;
	case 'k':	xRotate -= 10;
		break;

	case 'p':	if (paused == false){
		paused = true;
				}
				else(paused = false);
				break;
	case 'f':	if (density > 0) //if fog is greater than 0 then turn it to 0
				{
					density = 0.0;
				}
				else{ // else turn it to .05
					density = 0.05;
				}
				break;

	case 'o':	if (turnSingleBallAnimationOn == false){
		turnSingleBallAnimationOn = true;
				}
				else(turnSingleBallAnimationOn = false);
				break;
	case 'x':  
		loopSpeed = loopSpeed * 1.5;
		ball3LoopDelay = loopSpeed / 4;
		break;
	case 'c':  
		loopSpeed = loopSpeed / 1.5;
		ball3LoopDelay = loopSpeed / 4;
		break;
	case 'q':
		loopSpeed = 2.00f;
		ball3LoopDelay =  .500f;
		dlr = 0.0; //change light to grey
		dlg = 0.0;
		dlb = 0.0;
		turnSingleBallAnimationOn = false;
		xRotate = 0;
		yRotate = 0;
		density = 0.0;
		break;
	}
	if (key=='r') { 
		dlr = 1.0; //change light to red
		dlg = 0.0;
		dlb = 0.0;
	}
	if (key=='g') { 
		dlr = 0.0; //change light to green
		dlg = 1.0;
		dlb = 0.0;
	}
	if (key=='b') { 
		dlr = 0.0; //change light to blue
		dlg = 0.0;
		dlb = 1.0;
	}
	if (key == 't'){
		//if spotlight is on, turn it off
		if(spotlight == true){
			spotlight = false;
		}
		//else turn it on.
		else{
			spotlight = true;
		}
	}
	
}

void Mouse(int button, int state, int x, int y)
{
	// update the button state
	if(button == GLUT_LEFT_BUTTON)
	{
		if(state == GLUT_DOWN)
			mouseState.leftButton = true;
		else
			mouseState.leftButton = false;
	}
	if(button == GLUT_RIGHT_BUTTON)
	{
		if(state == GLUT_DOWN)
			mouseState.rightButton = true;
		else
			mouseState.rightButton = false;
	}

	// update the mouse position, so we can track the mouse move
	mouseState.x = x;
	mouseState.y = y;
}

void MouseMove(int x, int y)
{
	/* Calculate the displacement in movement */
	int xDelta = mouseState.x - x;
	int yDelta = mouseState.y - y;

	/* Commit the mouse position */
	mouseState.x = x;
	mouseState.y = y;

	/* If left button is down, rotate when mouse is moved */
	if(mouseState.leftButton) {
		xRotate -= xDelta * DEGREES_PER_PIXEL;
		yRotate -= yDelta * DEGREES_PER_PIXEL;
	}
	/* If right button is down, translate when mouse is moved.
	* Simulate translation by moving the eye--lookat vector:
	* if vector moves to the left, scene moves to the right.
	*/
	else if(mouseState.rightButton) {
		eye[0] += xDelta * UNITS_PER_PIXEL;
		eye[1] -= yDelta * UNITS_PER_PIXEL;
		at[0] += xDelta * UNITS_PER_PIXEL;
		at[1] -= yDelta * UNITS_PER_PIXEL;
	}


	glutPostRedisplay();
}

int ImageLoad(char *filename, Image *image) {
	FILE *file;
	unsigned long size;                 /*  size of the image in bytes. */
	unsigned long i;                    /*  standard counter. */
	unsigned short int planes;          /*  number of planes in image (must be 1)  */
	unsigned short int bpp;             /*  number of bits per pixel (must be 24) */
	char temp;                          /*  used to convert bgr to rgb color. */

	/*  make sure the file is there. */
	if ((file = fopen(filename, "rb"))==NULL) {
		printf("File Not Found : %s\n",filename);
		system("pause");
		return 0;
	}

	/*  seek through the bmp header, up to the width height: */
	fseek(file, 18, SEEK_CUR);

	/*  No 100% errorchecking anymore!!! */

	/*  read the width */    image->sizeX = getint (file);

	/*  read the height */ 
	image->sizeY = getint (file);

	/*  calculate the size (assuming 24 bits or 3 bytes per pixel). */
	size = image->sizeX * image->sizeY * 3;

	/*  read the planes */    
	planes = getshort(file);
	if (planes != 1) {
		printf("Planes from %s is not 1: %u\n", filename, planes);
		system("pause");
		return 0;
	}

	/*  read the bpp */    
	bpp = getshort(file);
	if (bpp != 24) {
		printf("Bpp from %s is not 24: %u\n", filename, bpp);
		system("pause");
		return 0;
	}

	/*  seek past the rest of the bitmap header. */
	fseek(file, 24, SEEK_CUR);

	/*  read the data.  */
	image->data = (char *) malloc(size);
	if (image->data == NULL) {
		printf("Error allocating memory for color-corrected image data");
		system("pause");
		return 0;	
	}

	if ((i = fread(image->data, size, 1, file)) != 1) {
		printf("Error reading image data from %s.\n", filename);
		system("pause");
		return 0;
	}

	for (i=0;i<size;i+=3) { /*  reverse all of the colors. (bgr -> rgb) */
		temp = image->data[i];
		image->data[i] = image->data[i+2];
		image->data[i+2] = temp;
	}

	fclose(file); /* Close the file and release the filedes */

	/*  we're done. */
	return 1;
}

GLvoid LoadTexture(char *filename, int texturenumber)
{	
	Image *TextureImage;

	TextureImage = (Image *) malloc(sizeof(Image));
	if (TextureImage == NULL) {
		printf("Error allocating space for image");
		system("pause");
		exit(1);
	}

	if (!ImageLoad(filename, TextureImage)) {
		exit(1);
	}  

	/*  2d texture, level of detail 0 (normal), 3 components (red, green, blue),            */
	/*  x size from image, y size from image,                                              */    
	/*  border 0 (normal), rgb color data, unsigned byte data, and finally the data itself. */ 
	glBindTexture(GL_TEXTURE_2D,texture[texturenumber]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);



	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		TextureImage->sizeX, TextureImage->sizeY,
		0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);

	free( TextureImage->data);
	free( TextureImage );

}

void myInit()
{
	glGenTextures(3, texture);
	LoadTexture("snowBody.bmp",0);
	LoadTexture("snowFace.bmp",1);
	LoadTexture("moon.bmp",2);
	glEnable(GL_TEXTURE_2D);

	glEnable (GL_DEPTH_TEST); //enable the depth testing
	glEnable (GL_LIGHTING); //enable the lighting
	glEnable (GL_LIGHT0); //enable LIGHT0, our Diffuse Light
	glEnable (GL_LIGHT1); //enable LIGHT1, our Ambient Light
	glEnable (GL_LIGHT2); //enable LIGHT2, our Diffuse Light
	glEnable (GL_LIGHT3); //enable LIGHT3, our Diffuse Light
	glEnable (GL_LIGHT4); //enable LIGHT4, our Diffuse Light
	glEnable (GL_LIGHT5); //enable LIGHT5, our Diffuse Light
	glShadeModel (GL_SMOOTH); //set the shader to smooth shader


	//spotlight
	if(spotlight == true){
	glEnable(GL_LIGHT6);
	}
	else{
		glDisable(GL_LIGHT6);
	}

	
}

void reshape (int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h); //set the viewport to the current window specifications
	glMatrixMode(GL_PROJECTION); //set the matrix to projection

	glLoadIdentity();
	gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 50.0); //set the perspective (angle of sight, width, height, , depth)
	glMatrixMode(GL_MODELVIEW); //set the matrix back to model

}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("juggling snowman");
	/* set window attributes */
	myInit();
	/* register callback functions */
	glutDisplayFunc(Display);
	glutIdleFunc(idleCallBack);
	glutReshapeFunc(reshape); //reshape the window accordingly
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutMouseFunc(Mouse);
	glutMotionFunc(MouseMove);
	/* start event processing */
	cout << "Creepy Snowman 1.0 " <<endl << "Daniel Lustig" <<endl << endl <<endl <<endl;
	cout << "Use the mouse to drag the camera position" <<endl <<endl;

	cout << "press key : " << endl;
	cout << "l = rotate left" << endl;
	cout << "k = rotate right" <<endl;
	cout << "p = pause animation" << endl;
	cout << "f = fog on/off" <<endl;
	cout << "o = change animation" << endl;
	cout << "r = change lighting ball light to red" <<endl;
	cout << "g = change lighting ball light to green" <<endl;
	cout << "b = change lighting ball light to blue" <<endl;
	cout << "t = turn spotlight on and off" <<endl;
	cout << "x = increase juggling speed" << endl;
	cout << "c = decrease juggling speed" << endl;
	cout << "q = reset" << endl;

	glutMainLoop();
}