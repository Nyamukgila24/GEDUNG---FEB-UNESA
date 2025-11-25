#define _CRT_SECURE_NO_WARNINGS

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// --- VARIABEL GLOBAL KAMERA ---
float x = 0.0f, y = 50.0f, z = 150.0f;
float lx = 0.0f, lz = -1.0f;
float angle = 0.0f;
float pitch = -0.2f;

// --- FUNGSI GAMBAR GEDUNG (KUBUS) ---
void drawBuilding(float px, float py, float pz, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(px, py + (sy / 2), pz);
    glScalef(sx, sy, sz);

    // WARNA GEDUNG: ABU-ABU
    glColor3f(0.6f, 0.6f, 0.6f);

    glutSolidCube(1.0f);
    glPopMatrix();
}

// --- FUNGSI LINGKUNGAN ---
void drawEnvironment() {
    // 1. RUMPUT LUAS (HIJAU)
    glPushMatrix();
    glColor3f(0.1f, 0.8f, 0.1f); // Hijau Segar

    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-300.0f, 0.0f, -400.0f);
    glVertex3f(-300.0f, 0.0f, 300.0f);
    glVertex3f(300.0f, 0.0f, 300.0f);
    glVertex3f(300.0f, 0.0f, -400.0f);
    glEnd();
    glPopMatrix();

    // 2. JALAN (ASPAL - Abu Gelap)
    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f); // <-- UBAH KE WARNA ASPAL

    // A. JALAN VERTIKAL (KIRI)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-55.0f, 0.1f, -350.0f);
    glVertex3f(-55.0f, 0.1f, 150.0f);
    glVertex3f(-25.0f, 0.1f, 150.0f);
    glVertex3f(-25.0f, 0.1f, -350.0f);
    glEnd();

    // B. JALAN HORIZONTAL (BAWAH)
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-25.0f, 0.1f, 50.0f);
    glVertex3f(-25.0f, 0.1f, 80.0f);
    glVertex3f(200.0f, 0.1f, 80.0f);
    glVertex3f(200.0f, 0.1f, 50.0f);
    glEnd();
    glPopMatrix();
}

// --- FUNGSI TEKS HUD ---
void drawText(float x, float y, char* string) {
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);

    glRasterPos2f(x, y);
    int len = (int)strlen(string);
    for (int i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    }

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

void displayTextParams() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f); // Warna Teks Putih
    char buffer[100];

    // --- UPDATE TEKS DISINI ---
    sprintf(buffer, "Pos: %.1f, %.1f, %.1f | WASD = Jalan | Space = Naik | C = Turun", x, y, z);

    drawText(10, 690, buffer);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// --- DISPLAY UTAMA ---
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(x, y, z,
        x + lx, y + sin(pitch), z + lz,
        0.0f, 1.0f, 0.0f);

    // POSISI CAHAYA
    GLfloat light_pos[] = { 0.0f, 100.0f, 100.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    drawEnvironment();

    // --- SEMUA GEDUNG WARNA ABU-ABU ---

    // 1. GAPURA ATAS
    drawBuilding(20.0f, 0.0f, -320.0f, 5.0f, 20.0f, 5.0f);
    drawBuilding(60.0f, 0.0f, -320.0f, 5.0f, 20.0f, 5.0f);
    drawBuilding(40.0f, 18.0f, -320.0f, 45.0f, 4.0f, 5.0f);

    // 2. GEDUNG "NGAREP DEWE"
    drawBuilding(40.0f, 0.0f, -250.0f, 70.0f, 20.0f, 40.0f);

    // 3. REKTORAT
    drawBuilding(50.0f, 0.0f, -160.0f, 60.0f, 25.0f, 60.0f);
    drawBuilding(10.0f, 0.0f, -160.0f, 15.0f, 15.0f, 20.0f);

    // 4. G3
    drawBuilding(50.0f, 0.0f, -90.0f, 60.0f, 15.0f, 30.0f);

    // 5. G2
    drawBuilding(50.0f, 0.0f, -40.0f, 60.0f, 15.0f, 30.0f);

    // 6. G1
    drawBuilding(50.0f, 0.0f, 10.0f, 60.0f, 15.0f, 30.0f);

    // 7. GAPURA BAWAH
    drawBuilding(0.0f, 0.0f, 40.0f, 4.0f, 15.0f, 4.0f);
    drawBuilding(0.0f, 0.0f, 90.0f, 4.0f, 15.0f, 4.0f);

    // 8. PASCA
    drawBuilding(50.0f, 0.0f, 120.0f, 60.0f, 20.0f, 40.0f);

    displayTextParams();
    glutSwapBuffers();
}

// --- LOGIKA INPUT ---
void processNormalKeys(unsigned char key, int xx, int yy) {
    float speed = 2.0f;
    switch (key) {
    case 'w': z += lz * speed; x += lx * speed; break;
    case 's': z -= lz * speed; x -= lx * speed; break;
    case 'a': x += lz * speed; z -= lx * speed; break;
    case 'd': x -= lz * speed; z += lx * speed; break;
    case 32: y += 1.0f; break;
    case 'c': y -= 1.0f; if (y < 1.75f) y = 1.75f; break;
    case 27: exit(0); break;
    }
}

void processMouseMove(int xx, int yy) {
    int centerX = 1280 / 2;
    int centerY = 720 / 2;
    float deltaX = (xx - centerX) * 0.002f;
    float deltaY = (yy - centerY) * 0.002f;
    angle += deltaX;
    pitch -= deltaY;
    if (pitch > 1.5f) pitch = 1.5f;
    if (pitch < -1.5f) pitch = -1.5f;
    lx = sin(angle);
    lz = -cos(angle);
    if (xx != centerX || yy != centerY) glutWarpPointer(centerX, centerY);
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    glutSetCursor(GLUT_CURSOR_NONE);
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("GEDUNG FEB");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(processNormalKeys);
    glutPassiveMotionFunc(processMouseMove);

    glutMainLoop();
    return 0;
}