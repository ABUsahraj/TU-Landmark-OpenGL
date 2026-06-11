#include <GL/freeglut.h>
#include <cmath>

static const float WORLD_W = 900.0f, WORLD_H = 650.0f;
static int   winW = 900, winH = 650;
static bool  night = false, lightsOn = false;
static float carX = 250.0f, anim = 0.0f;
static const float CAR_SPEED = 18.0f;

static void rect(float x, float y, float w, float h){
    glBegin(GL_QUADS); glVertex2f(x,y); glVertex2f(x+w,y); glVertex2f(x+w,y+h); glVertex2f(x,y+h); glEnd();
}
static void circle(float cx, float cy, float r){
    float fix = (static_cast<float>(winW)/static_cast<float>(winH)) / (WORLD_W/WORLD_H);
    glBegin(GL_TRIANGLE_FAN); glVertex2f(cx,cy);
    for(int i=0;i<=60;++i){ float a=6.2831853f*static_cast<float>(i)/60.0f;
        glVertex2f(cx+(r*cosf(a))/fix, cy+r*sinf(a)); }
    glEnd();
}
static void glow(float cx,float cy,float r,float gr,float gg,float gb,float a){
    float fix = (static_cast<float>(winW)/static_cast<float>(winH)) / (WORLD_W/WORLD_H);
    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(gr,gg,gb,a); glVertex2f(cx,cy);
    glColor4f(gr,gg,gb,0.0f);
    for(int i=0;i<=40;++i){ float ang=6.2831853f*static_cast<float>(i)/40.0f;
        glVertex2f(cx+(r*cosf(ang))/fix, cy+r*sinf(ang)); }
    glEnd();
    glDisable(GL_BLEND);
}
static void triangle(float x1,float y1,float x2,float y2,float x3,float y3){
    glBegin(GL_TRIANGLES); glVertex2f(x1,y1); glVertex2f(x2,y2); glVertex2f(x3,y3); glEnd();
}
static void strokeText(float x,float y,float s,float lw,const char* t){
    glLineWidth(lw); glPushMatrix(); glTranslatef(x,y,0.0f); glScalef(s,s,1.0f);
    for(const char* c=t; *c; ++c) glutStrokeCharacter(GLUT_STROKE_ROMAN, static_cast<int>(static_cast<unsigned char>(*c)));
    glPopMatrix(); glLineWidth(1.0f);
}
static void drawArchWindow(float x,float y,float w,float h){
    float arch=18.0f, mid=x+w*0.5f;
    glColor3f(0.30f,0.32f,0.36f); rect(x-3,y-3,w+6,h+3);
    triangle(x-3,y+h, x+w+3,y+h, mid,y+h+arch+4);
    if(night||lightsOn) glColor3f(1.0f,0.90f,0.40f); else glColor3f(0.58f,0.74f,0.86f);
    rect(x,y,w,h); triangle(x,y+h, x+w,y+h, mid,y+h+arch);
    glColor3f(0.30f,0.32f,0.36f);
    glBegin(GL_LINES); glVertex2f(mid,y); glVertex2f(mid,y+h);
        glVertex2f(x,y+h*0.5f); glVertex2f(x+w,y+h*0.5f); glEnd();
    if(night||lightsOn) glow(mid, y+h*0.45f, 55.0f, 1.0f,0.85f,0.40f, 0.5f);
}
static void drawBench(float cx,float by){
    glColor3f(0.45f,0.30f,0.16f); rect(cx-30,by,6,16); rect(cx+24,by,6,16);
    glColor3f(0.55f,0.38f,0.20f); rect(cx-36,by+16,72,8);
    glColor3f(0.45f,0.30f,0.16f); rect(cx-30,by+24,5,18); rect(cx+25,by+24,5,18);
    glColor3f(0.55f,0.38f,0.20f); rect(cx-36,by+30,72,5); rect(cx-36,by+38,72,5);
}
static void drawBush(float cx,float by){
    glColor3f(0.16f,0.50f,0.20f); rect(cx-22,by,44,16);
    circle(cx-13,by+14,12); circle(cx,by+18,14); circle(cx+13,by+14,12);
}
static void drawSky(){
    if(night) glColor3f(0.05f,0.05f,0.20f); else glColor3f(0.40f,0.70f,0.95f);
    rect(0,0,WORLD_W,WORLD_H);
}
static void drawCelestial(){
    if(night){
        glColor3f(0.95f,0.95f,0.85f); circle(810,580,36);
        glColor3f(1,1,1);
        const float sx[6]={90,200,320,470,640,760}, sy[6]={600,560,615,580,615,520};
        for(int i=0;i<6;++i) circle(sx[i],sy[i],4);
    } else {
        glColor3f(1.0f,0.93f,0.45f); circle(810,580,50);
        glColor3f(1.0f,0.84f,0.10f); circle(810,580,38);
    }
}
static void drawGroundAndRoad(){
    glColor3f(0.40f,0.66f,0.26f); rect(0,0,WORLD_W,200);
    glColor3f(0.25f,0.25f,0.25f); rect(0,60,WORLD_W,80);
    glColor3f(1.0f,0.85f,0.10f); for(float x=20;x<WORLD_W;x+=70) rect(x,96,40,8);
}
static void drawFlag(){
    float px=60, py=200, ph=235;
    glColor3f(0.55f,0.55f,0.58f); rect(px,py,6,ph);
    glColor3f(0.85f,0.85f,0.88f); circle(px+3,py+ph,5);
    float fx=px+6, fy=360, fw=90, fh=58;
    glColor3f(0.0f,0.42f,0.18f);
    glBegin(GL_QUAD_STRIP);
    for(int i=0;i<=24;++i){
        float t=static_cast<float>(i)/24.0f, x=fx+fw*t, off=sinf(anim+t*6.0f)*6.0f;
        glVertex2f(x, fy+off); glVertex2f(x, fy+fh+off);
    }
    glEnd();
}
static void drawBuilding(){
    const float bx=160, by=200, bw=600, bh=235, top=by+bh;
    glColor3f(0.93f,0.88f,0.76f); rect(bx,by,bw,bh);
    glColor3f(0.82f,0.75f,0.62f); rect(bx,top-16,bw,16);
    glColor3f(0.88f,0.82f,0.70f); rect(bx-6,top,bw+12,8);
    const float colX[6]={185,250,315, 559,624,689}, rowY[2]={225,318};
    for(int r=0;r<2;++r) for(int c=0;c<6;++c) drawArchWindow(colX[c],rowY[r],46,58);
    glColor3f(0.28f,0.30f,0.34f); rect(406,200,108,100);
    if(night||lightsOn) glColor3f(0.95f,0.85f,0.45f); else glColor3f(0.20f,0.24f,0.30f);
    rect(412,200,96,92);
    glColor3f(0.45f,0.47f,0.50f); rect(458,200,4,92);
    glColor3f(0.78f,0.78f,0.80f); rect(408,188,104,12);
    glColor3f(0.72f,0.72f,0.74f); rect(398,176,124,12);
    glColor3f(0.66f,0.66f,0.68f); rect(388,164,144,12);
    glColor3f(0.72f,0.20f,0.14f);
    glBegin(GL_QUADS); glVertex2f(404,298); glVertex2f(516,298); glVertex2f(498,326); glVertex2f(422,326); glEnd();
    glColor3f(0.75f,0.15f,0.12f);
    float cw = static_cast<float>(glutStrokeLength(GLUT_STROKE_ROMAN,
               reinterpret_cast<const unsigned char*>("CIT"))) * 0.5f;
    strokeText(460.0f - cw*0.5f, 332.0f, 0.5f, 5.0f, "CIT");
    if(night||lightsOn) glow(460.0f, 250.0f, 75.0f, 1.0f,0.85f,0.45f, 0.5f);
}
static void drawCar(float x){
    float y=80;
    glColor3f(0.15f,0.35f,0.85f); rect(x,y,130,32);
    glColor3f(0.20f,0.45f,0.95f); rect(x+28,y+32,70,28);
    glColor3f(0.75f,0.88f,1.0f); rect(x+34,y+38,26,18); rect(x+66,y+38,26,18);
    glColor3f(0.05f,0.05f,0.05f); circle(x+30,y,16); circle(x+100,y,16);
    glColor3f(0.55f,0.55f,0.55f); circle(x+30,y,6); circle(x+100,y,6);
    if(night||lightsOn) glColor3f(1.0f,1.0f,0.6f); else glColor3f(0.85f,0.85f,0.4f);
    circle(x+128,y+12,6);
    if(night||lightsOn) glow(x+140,y+12, 42.0f, 1.0f,1.0f,0.6f, 0.6f);
}
static void drawHUD(){
    glColor3f(1,1,1);
    strokeText(12,624,0.10f,1.2f,"N: Night   D: Day   Arrows: Move Car   Mouse: Lights   ESC: Exit");
}
static void box3D(float x,float y,float w,float h,float dx,float dy,float r,float g,float b){
    glColor3f(r*0.65f,g*0.65f,b*0.65f);
    glBegin(GL_QUADS); glVertex2f(x+w,y); glVertex2f(x+w+dx,y+dy); glVertex2f(x+w+dx,y+h+dy); glVertex2f(x+w,y+h); glEnd();
    glColor3f(r*0.82f,g*0.82f,b*0.82f);
    glBegin(GL_QUADS); glVertex2f(x,y+h); glVertex2f(x+w,y+h); glVertex2f(x+w+dx,y+h+dy); glVertex2f(x+dx,y+h+dy); glEnd();
    glColor3f(r,g,b); rect(x,y,w,h);
}
static void drawSign3D(){
    box3D(817,142,16,46, 8,5, 0.55f,0.55f,0.58f);      // العمود (3D)
    box3D(775,185,100,52, 16,10, 0.12f,0.27f,0.50f);   // اللوحة (3D)
    glColor3f(1.0f,0.84f,0.0f);
    float cw = static_cast<float>(glutStrokeLength(GLUT_STROKE_ROMAN, reinterpret_cast<const unsigned char*>("CIT"))) * 0.20f;
    strokeText(825.0f - cw*0.5f, 200.0f, 0.20f, 2.5f, "CIT");
}
static void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    drawSky(); drawCelestial(); drawGroundAndRoad();
    drawFlag();
    drawBuilding();
    drawSign3D();
    drawBench(300,150); drawBench(620,150);
    drawBush(225,150); drawBush(695,150);
    drawCar(carX);
    drawHUD();
    glutSwapBuffers();
}
static void reshape(int w,int h){
    if(h==0) h=1;
    winW=w; winH=h;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluOrtho2D(0.0, static_cast<double>(WORLD_W), 0.0, static_cast<double>(WORLD_H));
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}
static void keyboard(unsigned char key,int,int){
    if(key=='n'||key=='N') night=true;
    else if(key=='d'||key=='D') night=false;
    else if(key==27) glutLeaveMainLoop();
}
static void special(int key,int,int){
    if(key==GLUT_KEY_LEFT)  carX-=CAR_SPEED;
    if(key==GLUT_KEY_RIGHT) carX+=CAR_SPEED;
    if(carX>WORLD_W) carX=-130;
    if(carX<-130) carX=WORLD_W;
}
static void mouse(int button,int state,int,int){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) lightsOn=!lightsOn;
}
static void timer(int){
    anim += 0.12f;
    carX += 1.2f; if(carX>WORLD_W) carX=-130;
    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}
int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(winW,winH);
    glutCreateWindow("Taif University - CIT College - Computer Graphics Project");
    glutDisplayFunc(display); glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard); glutSpecialFunc(special); glutMouseFunc(mouse);
    glutTimerFunc(16,timer,0);
    glutMainLoop();
    return 0;
}
