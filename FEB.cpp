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

// --- VARIABEL INPUT (SMOOTH MOVEMENT) ---
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

// --- FUNGSI GAMBAR GEDUNG GENERIK ---
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

// --- FUNGSI SPESIAL: GEDUNG G3 FEB UNESA (REVISI TOTAL - REALISTIS) ---
static void drawGedungG3Unesa(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);

    // Putar 180 derajat agar menghadap kamera
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    // ==========================================
    // 1. BAGIAN KANAN: MENARA KACA (TOWER)
    // ==========================================
    float glassX = 35.0f;
    float glassZ = 10.0f;
    float radius = 14.0f;
    float towerH = 45.0f;

    // --- INTERIOR: TANGGA SPIRAL ---
    float numSteps = 100.0f;
    float anglePerStep = 15.0f;
    float stairDist = 9.0f;

    for (int i = 0; i < numSteps; i++) {
        glPushMatrix();
        float theta = (i * anglePerStep) * (3.14159f / 180.0f);
        float sx = glassX + cos(theta + 3.14f) * stairDist;
        float sz = glassZ + sin(theta + 3.14f) * stairDist;
        float sy = (i * (towerH / numSteps));

        glTranslatef(sx, sy, sz);
        glRotatef(-i * anglePerStep, 0.0f, 1.0f, 0.0f);
        glScalef(7.0f, 0.5f, 2.5f);
        glColor3f(0.9f, 0.9f, 0.9f); // Putih
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // Lantai & Tiang Pusat
    glColor3f(0.8f, 0.8f, 0.8f);
    for (int i = 0; i < 5; i++) {
        glPushMatrix();
        glTranslatef(glassX, (i * 9.0f) + 0.1f, glassZ);
        glRotatef(-90, 1, 0, 0);
        drawDisk(0.0f, radius - 0.5f, 24);
        glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(glassX, 0.0f, glassZ);
    glRotatef(-90, 1, 0, 0);
    drawCylinder(2.5f, 2.5f, towerH, 16);
    glPopMatrix();

    // Atap Menara (Topi Putih)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f); // Putih Bersih
    glTranslatef(glassX, towerH, glassZ);
    glRotatef(-90, 1, 0, 0);
    drawDisk(0.0f, radius + 2.0f, 30);
    glPopMatrix();

    // --- EKSTERIOR: KACA TRANSPARAN ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glPushMatrix();
    glTranslatef(glassX, 0.0f, glassZ);
    glRotatef(-90, 1, 0, 0);
    glColor4f(0.7f, 0.9f, 1.0f, 0.25f); // Kaca Bening Kebiruan
    drawCylinder(radius, radius, towerH, 32);

    // Bingkai Hijau Tosca
    glLineWidth(3.0f);
    glColor4f(0.0f, 0.6f, 0.5f, 0.9f); // Hijau Tosca
    for (int k = 0; k < 8; k++) {
        glPushMatrix();
        glRotatef(k * 45.0f, 0, 0, 1);
        glTranslatef(radius, 0, 0);
        drawCylinder(0.4f, 0.4f, towerH, 4);
        glPopMatrix();
    }
    // Cincin Bingkai
    drawDisk(radius, radius + 0.5f, 32);
    glTranslatef(0, 0, towerH / 2);
    drawDisk(radius, radius + 0.5f, 32);
    glTranslatef(0, 0, towerH / 2);
    drawDisk(radius, radius + 0.5f, 32);

    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);


    // ==========================================
    // 2. BAGIAN TENGAH: LOBI BATU & KANOPI MERAH
    // ==========================================

    // Dinding Tengah (Tekstur Batu Alam - Abu Gelap)
    glPushMatrix();
    glTranslatef(8.0f, 20.0f, 5.0f);
    glScalef(25.0f, 40.0f, 20.0f);
    glColor3f(0.35f, 0.35f, 0.35f); // Abu-abu Batu
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tulisan FAKULTAS (Background Putih di atas)
    glPushMatrix();
    glTranslatef(8.0f, 35.0f, 15.5f);
    glScalef(20.0f, 4.0f, 1.0f);
    glColor3f(0.95f, 0.95f, 0.95f); // Putih
    glutSolidCube(1.0f);
    glPopMatrix();

    // --- KANOPI (ATAP TERAS) ---
    // Ini yang sebelumnya menghalangi pandangan, sekarang dibuat tipis
    glPushMatrix();
    glColor3f(0.8f, 0.4f, 0.3f); // Merah Bata / Terracotta
    glTranslatef(15.0f, 14.0f, 25.0f); // Maju ke depan
    glScalef(50.0f, 1.0f, 30.0f); // Tipis dan Lebar
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tiang Penyangga Kanopi (Bulat)
    glColor3f(0.7f, 0.7f, 0.7f); // Abu terang
    // Tiang Kiri
    glPushMatrix();
    glTranslatef(30.0f, 0.0f, 35.0f);
    glRotatef(-90, 1, 0, 0);
    drawCylinder(1.5f, 1.5f, 14.0f, 16);
    glPopMatrix();
    // Tiang Kanan
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 35.0f);
    glRotatef(-90, 1, 0, 0);
    drawCylinder(1.5f, 1.5f, 14.0f, 16);
    glPopMatrix();


    // ==========================================
    // 3. BAGIAN KIRI: GEDUNG KOTAK PUTIH
    // ==========================================
    float leftWingX = -40.0f;

    // Gedung Utama Putih
    glPushMatrix();
    glTranslatef(leftWingX, 18.0f, 0.0f);
    glScalef(70.0f, 36.0f, 25.0f);
    glColor3f(0.95f, 0.95f, 0.95f); // PUTIH BERSIH
    glutSolidCube(1.0f);
    glPopMatrix();

    // Lis/Frame Hijau Tosca (Kotak Besar di Fasad)
    glColor3f(0.0f, 0.6f, 0.6f); // Tosca

    // Frame Kiri
    glPushMatrix();
    glTranslatef(leftWingX - 32.0f, 23.0f, 13.0f);
    glScalef(4.0f, 26.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Frame Atas
    glPushMatrix();
    glTranslatef(leftWingX, 35.0f, 13.0f);
    glScalef(68.0f, 2.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Frame Tengah Horizontal
    glPushMatrix();
    glTranslatef(leftWingX, 23.0f, 13.0f);
    glScalef(68.0f, 2.0f, 2.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Jendela-Jendela Kaca Gelap
    glColor3f(0.2f, 0.3f, 0.4f); // Kaca Gelap
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 6; col++) {
            glPushMatrix();
            float jx = (leftWingX - 25.0f) + (col * 10.0f);
            float jy = 29.0f - (row * 12.0f);
            glTranslatef(jx, jy, 12.8f);
            glScalef(6.0f, 6.0f, 1.0f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }


    // ==========================================
    // 4. AKSESORIS: BENDERA & PAPAN NAMA (TAMAN)
    // ==========================================

    // Tiang Bendera
    glPushMatrix();
    glTranslatef(15.0f, 0.0f, 50.0f); // Di depan kanopi
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    drawCylinder(0.3f, 0.1f, 35.0f, 8);
    glPopMatrix();

    // Bendera Merah Putih (Berkibar)
    glRotatef(-90, 0, 1, 0); // Menghadap samping
    // Merah
    glPushMatrix();
    glTranslatef(0.0f, 33.5f, 0.0f);
    glScalef(12.0f, 3.0f, 0.2f);
    glColor3f(0.9f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    // Putih
    glPushMatrix();
    glTranslatef(0.0f, 30.5f, 0.0f);
    glScalef(12.0f, 3.0f, 0.2f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPopMatrix();

    // Papan Nama Batu (Pendek di Taman)
    glPushMatrix();
    glTranslatef(-30.0f, 4.0f, 60.0f); // Di depan kiri

    // Dudukan
    glPushMatrix();
    glScalef(35.0f, 6.0f, 3.0f);
    glColor3f(0.2f, 0.2f, 0.2f); // Batu Hitam
    glutSolidCube(1.0f);
    glPopMatrix();

    // Tulisan Emas (Simulasi)
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.6f);
    glScalef(30.0f, 4.0f, 0.2f);
    glColor3f(0.8f, 0.7f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix(); // End DrawGedung
}


static void drawEnvironment() {
    glPushMatrix();
    glColor3f(0.1f, 0.8f, 0.1f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-300.0f, 0.0f, -400.0f); glVertex3f(-300.0f, 0.0f, 300.0f);
    glVertex3f(300.0f, 0.0f, 300.0f); glVertex3f(300.0f, 0.0f, -400.0f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.2f, 0.2f, 0.2f);
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
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glRasterPos2f(x, y);
    int len = (int)strlen(string);
    for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
}

static void displayTextParams() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    char buffer[100];
    sprintf(buffer, "Pos: %.0f, %.0f, %.0f | WASD = Jalan | Spasi/C = Naik/Turun", x, y, z);
    drawText(10, 690, buffer);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
}

// --- LOGIKA PERGERAKAN ---
void handleMovement() {
    float speed = 0.1f; // Kecepatan normal

    if (keyStates['w']) {
        x += lx * speed;
        z += lz * speed;
    }
    if (keyStates['s']) {
        x -= lx * speed;
        z -= lz * speed;
    }
    if (keyStates['a']) {
        x += lz * speed;
        z -= lx * speed;
    }
    if (keyStates['d']) {
        x -= lz * speed;
        z += lx * speed;
    }
    if (keyStates[32]) {
        y += 1.0f;
    }
    if (keyStates['c']) {
        y -= 1.0f;
        if (y < 1.75f) y = 1.75f;
    }
}
static void drawGapuraFEB(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);

    // SESUAI REQUEST: Rotasi -45 derajat
    glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);

    float radius = 24.0f;
    int segments = 60;
    float totalAngle = 100.0f;

    // 1. GAMBAR DINDING
    for (int i = 0; i < segments; i++) {
        float angle = -totalAngle / 2 + (i * (totalAngle / segments));

        glPushMatrix();
        glRotatef(angle, 0, 1, 0);
        glTranslatef(0.0f, 0.0f, -radius); // Mundur radius 24

        float segWidth = 1.3f;

        // List Biru Bawah
        glColor3f(0.0f, 0.6f, 0.8f); glPushMatrix(); glTranslatef(0.0f, 0.5f, 0.0f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        // Tingkat 1
        glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 1.5f, -0.8f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 2.0f, -0.8f); glScalef(segWidth, 0.2f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
        // Tingkat 2
        glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 2.5f, -1.8f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 3.0f, -1.8f); glScalef(segWidth, 0.2f, 1.2f); glutSolidCube(1.0f); glPopMatrix();
        // Tingkat 3
        glColor3f(0.6f, 0.4f, 0.2f); glPushMatrix(); glTranslatef(0.0f, 3.5f, -2.8f); glScalef(segWidth, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(1.0f, 1.0f, 1.0f); glPushMatrix(); glTranslatef(0.0f, 4.0f, -2.8f); glScalef(segWidth, 0.2f, 1.2f); glutSolidCube(1.0f); glPopMatrix();

        // Dinding Belakang Putih
        // PENTING: Posisinya mundur 3.8 dari pusat segmen
        glColor3f(0.95f, 0.95f, 0.95f);
        glPushMatrix();
        glTranslatef(0.0f, 6.5f, -3.8f);
        glScalef(segWidth, 6.0f, 1.0f);
        glutSolidCube(1.0f);
        glPopMatrix();

        // List Biru Tengah
        glColor3f(0.0f, 0.6f, 0.8f); glPushMatrix(); glTranslatef(0.0f, 5.5f, -3.3f); glScalef(segWidth, 0.6f, 0.2f); glutSolidCube(1.0f); glPopMatrix();

        glPopMatrix();
    }

    // 2. TULISAN "FAKULTAS EKONOMIKA DAN BISNIS" (DIJAMIN NEMPEL)
    const char* textString = "FAKULTAS EKONOMIKA DAN BISNIS";
    int len = strlen(textString);

    // HITUNGAN MATEMATIKA POSISI:
    // Radius Lengkungan (-24.0) + Mundur Dinding (-3.8) + Setengah Tebal Dinding (+0.5)
    // = -27.3 (Permukaan Wajah Dinding)
    // Kita taruh di -27.2 (Maju 0.1) biar nempel pas.
    float textZ = -27.2f;

    float angleStep = 3.0f;
    float totalTextAngle = len * angleStep;
    float startAngle = totalTextAngle / 2.0f;

    glColor3f(0.0f, 0.5f, 0.5f); // Hijau Tosca
    glLineWidth(2.0f);

    for (int j = 0; j < len; j++) {
        if (textString[j] == ' ') continue;

        // Hitung sudut huruf
        float currentAngle = startAngle - (j * angleStep);

        glPushMatrix();
        // 1. Putar sesuai lengkungan tembok (Matrix Rotation)
        glRotatef(currentAngle, 0, 1, 0);

        // 2. Geser kedalaman sama persis dengan tembok (textZ)
        glTranslatef(0.0f, 6.5f, textZ);

        // 3. Atur Ukuran & Posisi Tengah Huruf
        glScalef(0.015f, 0.018f, 0.015f); // Scale positif
        glTranslatef(-50.0f, 0.0f, 0.0f); // Center align

        glutStrokeCharacter(GLUT_STROKE_ROMAN, textString[j]);
        glPopMatrix();
    }
    glLineWidth(1.0f);

    // 3. LOGO KUNING
    float logoAngle = startAngle + 8.0f;

    glPushMatrix();
    glRotatef(logoAngle, 0, 1, 0);
    glTranslatef(0.0f, 6.8f, textZ); // Z sama dengan teks

    glColor3f(1.0f, 0.8f, 0.0f);
    glPushMatrix();
    drawDisk(0.0f, 1.5f, 30);
    drawCylinder(1.5f, 1.5f, 0.3f, 30);
    glTranslatef(0.0f, 0.0f, 0.3f);
    drawDisk(0.0f, 1.5f, 30);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}
static void display() {
    handleMovement();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Kamera
    gluLookAt(x, y, z,
        x + lx, y + sin(pitch), z + lz,
        0.0f, 1.0f, 0.0f);

    // Pencahayaan
    GLfloat light_pos[] = { 0.0f, 100.0f, 100.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    // 1. Gambar Lingkungan (Rumput & Jalan)
    drawEnvironment();

    // 2. Gedung Utama G3 (Yang detail ada menara kacanya)
    drawGedungG3Unesa(40.0f, 0.0f, -250.0f);

    // --- DAFTAR GEDUNG BACKGROUND (KUBUS SEDERHANA) ---

    // A. Gapura Belakang (Paling Jauh)
    // Dua tiang di ujung jalan belakang
    drawBuilding(20.0f, 0.0f, -320.0f, 5.0f, 20.0f, 5.0f); // Tiang Kiri
    drawBuilding(60.0f, 0.0f, -320.0f, 5.0f, 20.0f, 5.0f); // Tiang Kanan
    drawBuilding(40.0f, 18.0f, -320.0f, 45.0f, 4.0f, 5.0f); // Palang Atas

    // B. Gedung Rektorat (Gedung Putih Besar)
    drawBuilding(50.0f, 0.0f, -160.0f, 60.0f, 25.0f, 60.0f); // Bangunan Utama
    drawBuilding(10.0f, 0.0f, -160.0f, 15.0f, 15.0f, 20.0f); // Annex Kecil

    // C. Gedung Kuliah (Deretan Gedung Oranye/Abu)
    drawBuilding(50.0f, 0.0f, -90.0f, 60.0f, 15.0f, 30.0f); // Gedung G3/G2
    drawBuilding(50.0f, 0.0f, -40.0f, 60.0f, 15.0f, 30.0f); // Gedung G2

    // D. Gedung G1 (Paling dekat dengan pertigaan)
    drawBuilding(50.0f, 0.0f, 10.0f, 60.0f, 15.0f, 30.0f);  // Gedung G1

    // E. GAPURA DEPAN (AREA MASUK)
// Posisikan di pinggir jalan (X=1, Z=40)
    drawGapuraFEB(-16.0f, 0.0f, 44.0f);

    // Tiang Gapura Sisi Satunya (Masih Tiang Biasa)
    drawBuilding(0.0f, 0.0f, 90.0f, 4.0f, 15.0f, 4.0f);

    // F. Gedung Pascasarjana (Di seberang jalan depan)
    drawBuilding(50.0f, 0.0f, 120.0f, 60.0f, 20.0f, 40.0f); // Gedung Pasca

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
    lx = sin(angle);
    lz = -cos(angle);

    if (xx != centerX || yy != centerY) {
        glutWarpPointer(centerX, centerY);
    }
}

static void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
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
