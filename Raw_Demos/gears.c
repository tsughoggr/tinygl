/* sdlGears.c */
/*
 * 3-D gear wheels by Brian Paul. This program is in the public domain.
 *
 * ported to libSDL/TinyGL by Gerald Franz (gfz@o2online.de)
 */

//#define PLAY_MUSIC

//Only C standard library includes.
//These are ALL the external dependencies of this program!!! ALL of them!!!
#include <u.h>
#include <libc.h>
#include <draw.h>
#include <event.h>
#include <keyboard.h>
#include <memdraw.h>
#include <tinygl.h>

//#define CHAD_MATH_IMPL

//Drags in Math and String (which are already dragged in above.)
//#include "../include-demo/3dMath.h"

//Requires 
/*
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

and
#include <assert.h>
if I didn't define STBIW_ASSERT
*/

void initScene(void);
void drawgear(void);


#ifndef M_PI
#define M_PI 3.14159265
#endif
int override_drawmodes = 0;
GLubyte stipplepattern[128] = {0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,

							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,

							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,

							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55,
							   0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55, 0xAA, 0xAA, 0xAA, 0xAA, 0x55, 0x55, 0x55, 0x55};

/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 *         outer_radius - radius at center of teeth
 *         width - width of gear
 *         teeth - number of teeth
 *         tooth_depth - depth of tooth
 */
static void gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width, GLint teeth, GLfloat tooth_depth) {
	GLint i;
	GLfloat r0, r1, r2;
	GLfloat angle, da;
	GLfloat u, v, len;

	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0;
	r2 = outer_radius + tooth_depth / 2.0;

	da = 2.0 * M_PI / teeth / 4.0;

	glNormal3f(0.0, 0.0, 1.0);

	/* draw front face */
if(override_drawmodes == 1)
	glBegin(GL_LINES);
else if (override_drawmodes == 2)
	glBegin(GL_POINTS);
else
	{glBegin(GL_QUAD_STRIP);}
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	/* draw front sides of teeth */
	if(override_drawmodes == 1)
		glBegin(GL_LINES);
	else if (override_drawmodes == 2)
		glBegin(GL_POINTS);
	else
		glBegin(GL_QUADS);
	da = 2.0 * M_PI / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
	}
	glEnd();

	glNormal3f(0.0, 0.0, -1.0);

	/* draw back face */
	if(override_drawmodes == 1)
		glBegin(GL_LINES);
	else if (override_drawmodes == 2)
		glBegin(GL_POINTS);
	else
		glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
	}
	glEnd();

	/* draw back sides of teeth */
	if(override_drawmodes == 1)
		glBegin(GL_LINES);
	else if (override_drawmodes == 2)
		glBegin(GL_POINTS);
	else
		glBegin(GL_QUADS);
	da = 2.0 * M_PI / teeth / 4.0;
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;

		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
	}
	glEnd();

	/* draw outward faces of teeth */
	if(override_drawmodes == 1)
		glBegin(GL_LINES);
	else if (override_drawmodes == 2)
		glBegin(GL_POINTS);
	else
		glBegin(GL_QUAD_STRIP);
	for (i = 0; i < teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;

		glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
		glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
		u = r2 * cos(angle + da) - r1 * cos(angle);
		v = r2 * sin(angle + da) - r1 * sin(angle);
		len = sqrt(u * u + v * v);
		u /= len;
		v /= len;
		glNormal3f(v, -u, 0.0);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
		glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), width * 0.5);
		glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da), -width * 0.5);
		u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
		v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
		glNormal3f(v, -u, 0.0);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), width * 0.5);
		glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da), -width * 0.5);
		glNormal3f(cos(angle), sin(angle), 0.0);
	}

	glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
	glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

	glEnd();


	/* draw inside radius cylinder */
	if(override_drawmodes == 1)
		glBegin(GL_LINES);
	else if (override_drawmodes == 2)
		glBegin(GL_POINTS);
	else
		glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= teeth; i++) {
		angle = i * 2.0 * M_PI / teeth;
		glNormal3f(-cos(angle), -sin(angle), 0.0);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
		glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	}
	glEnd();
}

GLfloat view_rotx = 20.0, view_roty = 45.0;
static GLint gear1, gear2, gear3;
static GLfloat angle = 0.0;

void drawgear() {
	angle += 2.0;
	glPushMatrix();
	glRotatef(view_rotx, 1.0, 0.0, 0.0);
	glRotatef(view_roty, 0.0, 1.0, 0.0);
	//glRotatef( view_rotz, 0.0, 0.0, 1.0 );

	glPushMatrix();
	glTranslatef(-3.0, -2.0, 0.0);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glCallList(gear1);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.1, -2.0, 0.0);
	glRotatef(-2.0 * angle - 9.0, 0.0, 0.0, 1.0);
	glCallList(gear2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-3.1, 4.2, 0.0);
	glRotatef(-2.0 * angle - 25.0, 0.0, 0.0, 1.0);
	glCallList(gear3);
	glPopMatrix();

	glPopMatrix();
}

void initScene() {
	// static GLfloat pos[4] = {0.408248290463863, 0.408248290463863, 0.816496580927726, 0.0 }; //Light at infinity.
	static GLfloat pos[4] = {5, 5, 10, 0.0}; // Light at infinity.
	//static GLfloat pos[4] = {5, 5, -10, 0.0}; // Light at infinity.

	static GLfloat red[4] = {1.0, 0.0, 0.0, 0.0};
	static GLfloat green[4] = {0.0, 1.0, 0.0, 0.0};
	static GLfloat blue[4] = {0.0, 0.0, 1.0, 0.0};
	static GLfloat white[4] = {1.0, 1.0, 1.0, 0.0};
	static GLfloat shininess = 5;
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	// glLightfv( GL_LIGHT0, GL_AMBIENT, white);
	glLightfv( GL_LIGHT0, GL_SPECULAR, white);
	glEnable(GL_CULL_FACE);
	
	glEnable(GL_LIGHT0);
	//glEnable(GL_DEPTH_TEST);
	

	glEnable(GL_POLYGON_STIPPLE);
//	glDisable(GL_POLYGON_STIPPLE);
	glPolygonStipple(stipplepattern);
	glPointSize(10.0f);
	glTextSize(GL_TEXT_SIZE24x24);
	/* make the gears */
	gear1 = glGenLists(1);
	glNewList(gear1, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	glColor3fv(blue);
	gear(1.0, 4.0, 1.0, 20, 0.7); // The largest gear.
	glEndList();

	gear2 = glGenLists(1);
	glNewList(gear2, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glColor3fv(red);
	gear(0.5, 2.0, 2.0, 10, 0.7); // The small gear with the smaller hole, to the right.
	glEndList();

	gear3 = glGenLists(1);
	glNewList(gear3, GL_COMPILE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glColor3fv(green);
	gear(1.3, 2.0, 0.5, 10, 0.7); // The small gear above with the large hole.
	glEndList();
	// glEnable( GL_NORMALIZE );
}

/*
int main(int argc, char** argv) {
	// initialize SDL video:
	int wsx = 640;
	int wsy = 480;
	PIXEL* imbuf = NULL;
	uchar* pbuf = NULL;
	unsigned int flat = 0;
	unsigned int setenspec = 1;
	unsigned int dotext = 1;
	unsigned int blending = 0;
	if (argc > 1) {
		char* larg = "";
		for (int i = 1; i < argc; i++) {
			if (!strcmp(larg, "-w"))
				wsx = atoi(argv[i]);
			if (!strcmp(larg, "-h"))
				wsy = atoi(argv[i]);
			if (!strcmp(argv[i],"-flat"))
				flat = 1;
			if (!strcmp(argv[i],"-smooth"))
				flat = 0;
			if (!strcmp(argv[i],"-blend"))
				blending = 1;
			if (!strcmp(argv[i],"-nospecular"))
				setenspec = 0;
			if (!strcmp(argv[i],"-lines"))
				override_drawmodes = 1;
			if (!strcmp(argv[i],"-points"))
				override_drawmodes = 2;
			if (!strcmp(argv[i],"-notext"))
				dotext = 0;
			larg = argv[i];
		}
	}


	fflush(stdout);
	imbuf = calloc(1,sizeof(PIXEL) * wsx * wsy);
	// initialize TinyGL:
	// unsigned int pitch;
	//int mode;
	ZBuffer* frameBuffer = NULL;
	if(TGL_FEATURE_RENDER_BITS == 32)
	 frameBuffer = ZB_open(wsx, wsy, ZB_MODE_RGBA, 0);
	else
	 frameBuffer = ZB_open(wsx, wsy, ZB_MODE_5R6G5B, 0);
	if(!frameBuffer){printf("\nZB_open failed!");exit(1);}
	glInit(frameBuffer);

	//Print version info
	printf("\nVersion string:\n%s",glGetString(GL_VERSION));
	printf("\nVendor string:\n%s",glGetString(GL_VENDOR));
	printf("\nRenderer string:\n%s",glGetString(GL_RENDERER));
	printf("\nExtensions string:\n%s",glGetString(GL_EXTENSIONS));
	printf("\nLicense string:\n%s",glGetString(GL_LICENSE));
	// initialize GL:
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, wsx, wsy);
if(flat)	glShadeModel(GL_FLAT); else glShadeModel(GL_SMOOTH);
//TESTING BLENDING...
	//glDisable(GL_DEPTH_TEST);


	//glDisable( GL_LIGHTING );
	glEnable(GL_LIGHTING);
	//glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);
	glBlendEquation(GL_FUNC_ADD);
	if(blending){
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR);
		glBlendEquation(GL_FUNC_ADD);
	}else{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
	GLfloat h = (GLfloat)wsy / (GLfloat)wsx;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -45.0);

	initScene();
	if(setenspec) glSetEnableSpecular(GL_TRUE); else glSetEnableSpecular(GL_FALSE);
	// variables for timing:
	unsigned int frames = 0;
	//unsigned int tLastFps = tNow;

	// main loop:
	int isRunning = 1;
	//float test = 0;
	
	while (isRunning) {
		++frames;
		//Depending on SDL to give us ticks even without a window open...
	//	test = TEST_fastInvSqrt(tNow);
	//	printf("\n%f",test);
		// do event handling:
		
		// draw scene:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawgear();
		if(dotext){
			glDrawText((unsigned char*)"RED text", 0, 0, 0xFF0000);

			glDrawText((unsigned char*)"GREEN text", 0, 24, 0x00FF00);

			glDrawText((unsigned char*)"BLUE text", 0, 48, 0xFF);
		}
		// swap buffers:
		// Quickly convert all pixels to the correct format
	
		 ZB_copyFrameBuffer(frameBuffer, imbuf, wsx * sizeof(PIXEL));
		if(frames > 0) break;
	}
	if(TGL_FEATURE_RENDER_BITS == 32){ //very little conversion.
		pbuf = malloc(3 * wsx * wsy);
		for(int i = 0; i < wsx * wsy; i++){
			//pbuf[3*i+0] = (imbuf[i]&0xff0000)>>16;
			//pbuf[3*i+1] = (imbuf[i]&0x00ff00)>>8;
			//pbuf[3*i+2] = (imbuf[i]&0x0000ff);
			pbuf[3*i+0] = GET_RED(imbuf[i]);
			pbuf[3*i+1] = GET_GREEN(imbuf[i]);
			pbuf[3*i+2] = GET_BLUE(imbuf[i]);
		}
		stbi_write_png("render.png", wsx, wsy, 3, pbuf, 0);
		free(imbuf);
		free(pbuf);
	} else if(TGL_FEATURE_RENDER_BITS == 16){
		puts("\nTesting 16 bit rendering...\n");
		pbuf = malloc(3 * wsx * wsy);
		for(int i = 0; i < wsx * wsy; i++){
			pbuf[3*i+0] = GET_RED(imbuf[i]);
			pbuf[3*i+1] = GET_GREEN(imbuf[i]);
			pbuf[3*i+2] = GET_BLUE(imbuf[i]);
		}
		stbi_write_png("render.png", wsx, wsy, 3, pbuf, 0);
		free(imbuf);
		free(pbuf);
	}
	// cleanup:
	glDeleteList(gear1);
	glDeleteList(gear2);
	glDeleteList(gear3);
	//ZB_close(frameBuffer);
	glClose();
	glInit(frameBuffer);
	glClose();
	glInit(frameBuffer);
	glClose();
	glInit(frameBuffer);
	glClose();
	glInit(frameBuffer);
	ZB_close(frameBuffer);
	glClose();
	return 0;
}
*/
void
usage(){
	sysfatal("Usage: gears [-w width] [-h height] [-r rate] [-fsbnlpt]");
}
void
eresized(int new){
	if(new && getwindow(display, Refnone) < 0)
		sysfatal("Resize");
}
void
main(int argc, char** argv){
	int wsx = 640;
	int wsy = 480;
	PIXEL* imbuf;
	uchar* pbuf;
	Event Event;
	uint Etimer = 4;
	ZBuffer *framebuffer;
	GLfloat h;
	Image* tmp, *wc, *bc;

	uint flat, setenspec, dotext, blend, trate;
	dotext = setenspec = 1;
	trate = 20;
	imbuf = nil;
	pbuf = nil;
	framebuffer = nil;
	int i,k;

	ARGBEGIN {
		case 'w':
			wsx = atoi(EARGF(usage));
			break;
		case 'r':
			trate = atoi(EARGF(usage));
			break;
		case 'h':
			wsy = atoi(EARGF(usage));
			break;
		case 'f':
			flat = 1;
			break;
		case 'b':
			blend = 1;
			break;
		case 'n':
			setenspec = 0;
			break;
		case 'p':
			++override_drawmodes;
		case 'l':
			++override_drawmodes;
			break;
		case 't':
			dotext = 0;
			break;
		
	}ARGEND

	memimageinit();

	if(initdraw(nil, nil, "GLXGears") < 0)
		sysfatal("Initdraw");
	if(getwindow(display, Refmesg) < 0)
		sysfatal("getwindow");


	if(!(framebuffer = alloczbuffer(Rect(0,0,wsx,wsy),ZB_MODE_RGBA)))
		sysfatal("fballoc");
	tmp = allocimage(display,Rect(0,0,wsx,wsy), ABGR32 ,0, DTransparent);
	wc = allocimagemix(display, DWhite, DWhite);
	bc = allocimagemix(display, DBlue, DBlue);

	glInit((void*)framebuffer);
	fprint(2,"\nVersion string:\n%s",glGetString(GL_VERSION));
	fprint(2,"\nVendor string:\n%s",glGetString(GL_VENDOR));
	fprint(2,"\nRenderer string:\n%s",glGetString(GL_RENDERER));
	fprint(2,"\nExtensions string:\n%s",glGetString(GL_EXTENSIONS));
	fprint(2,"\nLicense string:\n%s",glGetString(GL_LICENSE));
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glViewport(0, 0, wsx, wsy);
	if(flat)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	if(blend){
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_DST_COLOR);
		glBlendEquation(GL_FUNC_ADD);
	}else{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
	h = (GLfloat)wsy / (GLfloat)wsx;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -45.0);
	initScene();
	if(setenspec)
		glSetEnableSpecular(GL_TRUE);
	else
		glSetEnableSpecular(GL_FALSE);
	/*Esoteric knowledge: window resizing with eresized() is a function of Emouse (not documented in manpage)*/
	einit(Ekeyboard|Emouse);
	etimer(Etimer, trate);
	for(;;)
		switch(event(&Event)){
			case Ekeyboard:
				switch(Event.kbdc){
					case Kup:
						view_rotx += 1.0;
						break;
					case Kdown:
						view_rotx -=1.0;
						break;
					case Kright:
						view_roty += 1.0;
						break;
					case Kleft:
						view_roty -=1.0;
						break;
					case 'q':
					case 'Q':
					case Kdel:
						exits(nil);
		
					default:
						break;
				}
				break;
			case 4:
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				drawgear();
				if(dotext){
					glDrawText((unsigned char*)"RED text", 0, 0, 0xFF0000FF);
		
					glDrawText((unsigned char*)"GREEN text", 0, 24, 0xFF00FF00);
		
					glDrawText((unsigned char*)"BLUE text", 0, 48, 0xFFFF0000);
				}
				// swap buffers:
				// Quickly convert all pixels to the correct format
			
				//ZB_copyFrameBuffer(framebuffer, imbuf, wsx * sizeof(PIXEL));
				loadimage(tmp, tmp->r, framebuffer->data->bdata,  Dx(framebuffer->r) * Dy(framebuffer->r) * framebuffer->depth);
				draw(screen, screen->r, wc, nil, ZP);
				draw(screen, screen->r, tmp, nil, ZP);
				break;
		}
teardown:
	free(imbuf);
	// cleanup:
	glDeleteList(gear1);
	glDeleteList(gear2);
	glDeleteList(gear3);
	ZB_close(framebuffer);
	glClose();
}
