#define _CRT_SECURE_NO_WARNINGS

#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>

// --- VARIABEL GLOBAL KAMERA ---
float x = 20.0f, y = 30.0f, z = 180.0f;
float lx = 0.0f, lz = -1.0f;
float angle = 0.0f;
float pitch = -0.1f;

// --- VARIABEL INPUT ---
bool keyStates[256] = { false };

// --- FUNGSI BANTUAN BENTUK ---
static void drawCylinder(float baseRadius, float topRadius, float height, int slices) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluCylinder(quadric, baseRadius, topRadius, height, slices, 1);
    gluDeleteQuadric(quadric);
}

static void drawDisk(float innerRadius, float outerRadius, int slices) {
    GLUquadricObj* quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    gluDisk(quadric, innerRadius, outerRadius, slices, 1);
    gluDeleteQuadric(quadric);
}

// --- FUNGSI GAMBAR GEDUNG KOTAK (GENERIC) ---
static void drawBuilding(float px, float py, float pz, float sx, float sy, float sz) {
    glPushMatrix();
    glTranslatef(px, py + (sy / 2), pz);
    glScalef(sx, sy, sz);
    glColor3f(0.65f, 0.65f, 0.65f);
    glutSolidCube(1.0f);
    glLineWidth(1.5f);
    glColor3f(0.2f, 0.2f, 0.2f);
    glutWireCube(1.002f);
    glLineWidth(1.0f);
    glPopMatrix();
}

// ==================================================================================
// FUNGSI MONUMEN DEPAN (POSISI & ARAH DIPERBAIKI)
// ==================================================================================
static void drawMonumentDepan(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);

    // PERBAIKAN: Putar 200 derajat (180 + 20) agar menghadap ke depan (Jalan)
    glRotatef(200.0f, 0.0f, 1.0f, 0.0f);

    // --- 1. PAPAN "LIGAIKU" (KUNING & BIRU TUA) ---
    glPushMatrix();
    glTranslatef(-8.0f, 0.0f, 0.0f); // Geser ke kiri dari pusat monumen

    // Dudukan Bawah (Kuning)
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, 0.0f);
    glScalef(8.0f, 4.0f, 2.0f);
    glColor3f(1.0f, 0.8f, 0.0f); // Kuning
    glutSolidCube(1.0f);
    glPopMatrix();

    // Bagian Atas (Biru Tua)
    glPushMatrix();
    glTranslatef(0.0f, 6.0f, 0.0f);
    glScalef(8.0f, 4.0f, 2.0f);
    glColor3f(0.0f, 0.1f, 0.4f); // Biru Tua
    glutSolidCube(1.0f);
    glPopMatrix();

    // Logo Bulat di Atas (Kuning & Putih)
    // PERBAIKAN: Hapus rotasi X agar disk tegak lurus menghadap depan
    glPushMatrix();
    glTranslatef(0.0f, 9.5f, 1.1f); // Majukan sedikit Z biar timbul

    glColor3f(1.0f, 0.8f, 0.0f); // Lingkaran Luar Kuning
    drawDisk(0.0f, 1.5f, 20);

    glTranslatef(0.0f, 0.0f, 0.1f); // Tumpuk sedikit
    glColor3f(1.0f, 1.0f, 1.0f); // Lingkaran Dalam Putih
    drawDisk(0.0f, 1.0f, 20);
    glPopMatrix();

    glPopMatrix(); // End Papan LIGAIKU

    // --- 2. PAPAN BATU HITAM (DI SEBELAHNYA) ---
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 0.0f); // Geser ke kanan

    // Dudukan Batu Panjang
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, 0.0f);
    glScalef(12.0f, 3.0f, 2.5f);
    glColor3f(0.2f, 0.2f, 0.2f); // Batu Abu Gelap
    glutSolidCube(1.0f);
    glPopMatrix();

    // Papan Tulisan di Atasnya
    glPushMatrix();
    glTranslatef(0.0f, 4.5f, 0.0f);
    glScalef(10.0f, 3.0f, 0.5f);
    glColor3f(0.1f, 0.1f, 0.1f); // Hitam Pekat
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix(); // End Papan Batu

    glPopMatrix(); // End Global Monument
}

// --- FUNGSI GAPURA FEB ---
static void drawGapuraFEB(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

    float radius = 24.0f;
    int segments = 60;
    float totalAngle = 100.0f;

    // 1. DINDING
    for (int i = 0; i < segments; i++) {
        float angle = -totalAngle / 2 + (i * (totalAngle / segments));
        glPushMatrix();
        glRotatef(angle, 0, 1, 0);
        glTranslatef(0.0f, 0.0f, -radius);

        float segWidth = 1.3f;
        glColor3f(0.0f, 0.6f, 0.8f); glPushMatrix(); glTranslatef(0.0f, 0.5f, 0.0f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 1.5f, -0.8f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 2.0f, -0.8f); glScalef(segWidth, 0.2f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 2.5f, -1.8f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 3.0f, -1.8f); glScalef(segWidth, 0.2f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 3.5f, -2.8f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 4.0f, -2.8f); glScalef(segWidth, 0.2f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.95f, 0.95f, 0.95f); glPushMatrix(); glTranslatef(0.0f, 6.5f, -3.8f); glScalef(segWidth, 6.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.0f, 0.6f, 0.8f); glPushMatrix(); glTranslatef(0.0f, 5.5f, -3.3f); glScalef(segWidth, 0.6f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
        glPopMatrix();
    }

    // 2. TULISAN
    const char* textString = "FAKULTAS EKONOMIKA DAN BISNIS";
    int len = strlen(textString);
    float textZ = -27.0f;
    float textScaleX = 0.018f;
    float textScaleY = 0.018f;
    float charGap = 15.0f;

    float totalWidthUnits = 0.0f;
    for (int i = 0; i < len; i++) {
        float w = glutStrokeWidth(GLUT_STROKE_ROMAN, textString[i]);
        if (textString[i] == ' ') w = 80.0f;
        totalWidthUnits += w;
    }
    if (len > 0) totalWidthUnits += (len - 1) * charGap;

    float totalArcLength = totalWidthUnits * textScaleX;
    float totalTextAngleRad = totalArcLength / 27.0f;
    float totalTextAngleDeg = totalTextAngleRad * (180.0f / 3.14159f);
    float currentAngle = totalTextAngleDeg / 2.0f;

    glColor3f(0.0f, 0.5f, 0.5f);
    glLineWidth(2.5f);

    for (int j = 0; j < len; j++) {
        char c = textString[j];
        float w = glutStrokeWidth(GLUT_STROKE_ROMAN, c);
        if (c == ' ') w = 80.0f;
        float charArcLen = (w + charGap) * textScaleX;
        float charAngleDeg = (charArcLen / 27.0f) * (180.0f / 3.14159f);

        glPushMatrix();
        glRotatef(currentAngle - (charAngleDeg / 2.0f), 0, 1, 0);
        glTranslatef(0.0f, 6.5f, textZ);
        glScalef(textScaleX, textScaleY, 1.0f);
        glTranslatef(-w / 2.0f, 0.0f, 0.0f);
        if (c != ' ') glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
        glPopMatrix();
        currentAngle -= charAngleDeg;
    }
    glLineWidth(1.0f);

    // 3. LOGO
    float logoAngle = (totalTextAngleDeg / 2.0f) + 5.0f;
    glPushMatrix();
    glRotatef(logoAngle, 0, 1, 0);
    glTranslatef(0.0f, 6.8f, textZ);
    glColor3f(1.0f, 0.8f, 0.0f);
    glPushMatrix(); drawDisk(0.0f, 1.5f, 30); drawCylinder(1.5f, 1.5f, 0.3f, 30);
    glTranslatef(0.0f, 0.0f, 0.3f); drawDisk(0.0f, 1.5f, 30); glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

// --- FUNGSI SPESIAL: GEDUNG G3 (DETAIL) ---
static void drawGedungG3Unesa(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    // 1. MENARA KACA
    float glassX = 35.0f; float glassZ = 10.0f;
    float radius = 14.0f; float towerH = 45.0f;

    for (int i = 0; i < 100; i++) {
        glPushMatrix();
        float theta = (i * 15.0f) * (3.14159f / 180.0f);
        float sx = glassX + cos(theta + 3.14f) * 9.0f;
        float sz = glassZ + sin(theta + 3.14f) * 9.0f;
        float sy = (i * (towerH / 100.0f));
        glTranslatef(sx, sy, sz); glRotatef(-i * 15.0f, 0.0f, 1.0f, 0.0f);
        glScalef(7.0f, 0.5f, 2.5f); glColor3f(0.9f, 0.9f, 0.9f); glutSolidCube(1.0f);
        glPopMatrix();
    }
    glPushMatrix(); glTranslatef(glassX, 0.0f, glassZ); glRotatef(-90, 1, 0, 0);
    glColor3f(0.8f, 0.8f, 0.8f); drawCylinder(2.5f, 2.5f, towerH, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(glassX, towerH, glassZ); glRotatef(-90, 1, 0, 0);
    glColor3f(1.0f, 1.0f, 1.0f); drawDisk(0.0f, radius + 2.0f, 30); glPopMatrix();

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
    glPushMatrix(); glTranslatef(glassX, 0.0f, glassZ); glRotatef(-90, 1, 0, 0);
    glColor4f(0.7f, 0.9f, 1.0f, 0.25f); drawCylinder(radius, radius, towerH, 32);
    glLineWidth(3.0f); glColor4f(0.0f, 0.6f, 0.5f, 0.9f);
    for (int k = 0; k < 8; k++) {
        glPushMatrix(); glRotatef(k * 45.0f, 0, 0, 1); glTranslatef(radius, 0, 0); drawCylinder(0.4f, 0.4f, towerH, 4); glPopMatrix();
    }
    drawDisk(radius, radius + 0.5f, 32);
    glTranslatef(0, 0, towerH / 2); drawDisk(radius, radius + 0.5f, 32);
    glTranslatef(0, 0, towerH / 2); drawDisk(radius, radius + 0.5f, 32);
    glPopMatrix();
    glDepthMask(GL_TRUE); glDisable(GL_BLEND);

    // 2. BAGIAN TENGAH
    glPushMatrix(); glTranslatef(8.0f, 20.0f, 5.0f); glScalef(25.0f, 40.0f, 20.0f);
    glColor3f(0.35f, 0.35f, 0.35f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(15.0f, 14.0f, 25.0f); glScalef(50.0f, 1.0f, 30.0f);
    glColor3f(0.8f, 0.4f, 0.3f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix(); glTranslatef(30.0f, 0.0f, 35.0f); glRotatef(-90, 1, 0, 0); drawCylinder(1.5f, 1.5f, 14.0f, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.0f, 35.0f); glRotatef(-90, 1, 0, 0); drawCylinder(1.5f, 1.5f, 14.0f, 16); glPopMatrix();

    // 3. SAYAP KIRI
    float leftWingX = -40.0f;
    glPushMatrix(); glTranslatef(leftWingX, 18.0f, 0.0f); glScalef(70.0f, 36.0f, 25.0f);
    glColor3f(0.95f, 0.95f, 0.95f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.6f);
    glPushMatrix(); glTranslatef(leftWingX - 32.0f, 23.0f, 13.0f); glScalef(4.0f, 26.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(leftWingX, 35.0f, 13.0f); glScalef(68.0f, 2.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(leftWingX, 23.0f, 13.0f); glScalef(68.0f, 2.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.2f, 0.3f, 0.4f);
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 6; col++) {
            glPushMatrix();
            float jx = (leftWingX - 25.0f) + (col * 10.0f); float jy = 29.0f - (row * 12.0f);
            glTranslatef(jx, jy, 12.8f); glScalef(6.0f, 6.0f, 1.0f); glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    // 4. AKSESORIS TAMAN (BENDERA FIX)
    glPushMatrix();
    glTranslatef(15.0f, 0.0f, 50.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix(); glRotatef(-90, 1, 0, 0); drawCylinder(0.3f, 0.1f, 35.0f, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(6.0f, 33.5f, 0.0f); glScalef(12.0f, 3.0f, 0.2f); glColor3f(0.9f, 0.1f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(6.0f, 30.5f, 0.0f); glScalef(12.0f, 3.0f, 0.2f); glColor3f(1.0f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    glPopMatrix();

    glPopMatrix();
}

static void drawEnvironment() {
    glPushMatrix();
    glColor3f(0.1f, 0.8f, 0.1f); // Rumput
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-300.0f, 0.0f, -400.0f); glVertex3f(-300.0f, 0.0f, 300.0f);
    glVertex3f(300.0f, 0.0f, 300.0f); glVertex3f(300.0f, 0.0f, -400.0f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f); // Jalan Aspal
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-55.0f, 0.1f, -350.0f); glVertex3f(-55.0f, 0.1f, 150.0f);
    glVertex3f(-25.0f, 0.1f, 150.0f); glVertex3f(-25.0f, 0.1f, -350.0f);
    glEnd();
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-25.0f, 0.1f, 50.0f); glVertex3f(-25.0f, 0.1f, 80.0f);
    glVertex3f(200.0f, 0.1f, 80.0f); glVertex3f(200.0f, 0.1f, 50.0f);
    glEnd();
    glPopMatrix();
}

static void drawText(float x, float y, const char* string) {
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL);
    glRasterPos2f(x, y);
    int len = (int)strlen(string);
    for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    glEnable(GL_COLOR_MATERIAL); glEnable(GL_LIGHTING);
}

static void displayTextParams() {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    char buffer[100];
    sprintf(buffer, "Pos: %.0f, %.0f, %.0f | WASD = Jalan | Spasi/C = Naik/Turun", x, y, z);
    drawText(10, 690, buffer);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

void handleMovement() {
    float speed = 0.2f;
    if (keyStates['w']) { x += lx * speed; z += lz * speed; }
    if (keyStates['s']) { x -= lx * speed; z -= lz * speed; }
    if (keyStates['a']) { x += lz * speed; z -= lx * speed; }
    if (keyStates['d']) { x -= lz * speed; z += lx * speed; }
    if (keyStates[32]) { y += 1.0f; }
    if (keyStates['c']) { y -= 1.0f; if (y < 1.75f) y = 1.75f; }
}

static void display() {
    handleMovement();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(x, y, z, x + lx, y + sin(pitch), z + lz, 0.0f, 1.0f, 0.0f);

    GLfloat light_pos[] = { 0.0f, 100.0f, 100.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    drawEnvironment();

    // GEDUNG UTAMA G3
    drawGedungG3Unesa(40.0f, 0.0f, -250.0f);

    // --- MONUMEN DEPAN (DIPINDAH KE DEPAN & DIPUTAR 180) ---
    // Posisi Z diubah dari -190 jadi -300 agar ada di depan gedung
    drawMonumentDepan(10.0f, 0.0f, -300.0f);


    // BACKGROUND BUILDINGS
    drawBuilding(20.0f, 0.0f, -320.0f, 5.0f, 20.0f, 5.0f);
    drawBuilding(60.0f, 0.0f, -320.0f, 5.0f, 20.0f, 5.0f);
    drawBuilding(40.0f, 18.0f, -320.0f, 45.0f, 4.0f, 5.0f);
    drawBuilding(50.0f, 0.0f, -160.0f, 60.0f, 25.0f, 60.0f);
    drawBuilding(10.0f, 0.0f, -160.0f, 15.0f, 15.0f, 20.0f);
    drawBuilding(50.0f, 0.0f, -90.0f, 60.0f, 15.0f, 30.0f);
    drawBuilding(50.0f, 0.0f, -40.0f, 60.0f, 15.0f, 30.0f);
    drawBuilding(50.0f, 0.0f, 10.0f, 60.0f, 15.0f, 30.0f);

    drawGapuraFEB(-16.0f, 0.0f, 44.0f);
    drawBuilding(0.0f, 0.0f, 90.0f, 4.0f, 15.0f, 4.0f);
    drawBuilding(50.0f, 0.0f, 120.0f, 60.0f, 20.0f, 40.0f);

    displayTextParams();
    glutSwapBuffers();
}

static void keyDown(unsigned char key, int x, int y) {
    keyStates[key] = true;
    if (key == 27) exit(0);
}

static void keyUp(unsigned char key, int x, int y) {
    keyStates[key] = false;
}

static void processMouseMove(int xx, int yy) {
    int centerX = 1280 / 2;
    int centerY = 720 / 2;
    float deltaX = (xx - centerX) * 0.002f;
    float deltaY = (yy - centerY) * 0.002f;
    angle += deltaX;
    pitch -= deltaY;
    if (pitch > 1.5f) pitch = 1.5f;
    if (pitch < -1.5f) pitch = -1.5f;
    lx = sin(angle); lz = -cos(angle);
    if (xx != centerX || yy != centerY) glutWarpPointer(centerX, centerY);
}

static void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING); glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE); glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

    GLfloat ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutSetCursor(GLUT_CURSOR_NONE);
}

static void reshape(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0f / h;
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0f, ratio, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1280, 720);
    glutCreateWindow("GEDUNG FEB UNESA");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(processMouseMove);

    glutMainLoop();
    return 0;
}
