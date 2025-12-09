#define _CRT_SECURE_NO_WARNINGS
#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>

GLuint textures[20]; // bisa 20 gambar
int textureCount = 0;
// Load gambar jadi tekstur
GLuint loadTexture(const char* file) {
    int w, h, ch;
    unsigned char* data = stbi_load(file, &w, &h, &ch, 4);
    if (!data) {
        printf("Gagal load: %s\n", file);
        return 0;
    }
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    return tex;
}

// Menggambar objek foto mandiri
void drawTexture(GLuint tex, float x, float y, float z,
    float w, float h,
    float rotX, float rotY, float rotZ)
{
    glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rotX, 1, 0, 0); glRotatef(rotY, 0, 1, 0); glRotatef(rotZ, 0, 0, 1);
    glDisable(GL_LIGHTING); glEnable(GL_TEXTURE_2D); glBindTexture(GL_TEXTURE_2D, tex); glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-w / 2, 0, 0); glTexCoord2f(1, 0); glVertex3f(w / 2, 0, 0); glTexCoord2f(1, 1); glVertex3f(w / 2, h, 0); glTexCoord2f(0, 1); glVertex3f(-w / 2, h, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);  glEnable(GL_LIGHTING); glPopMatrix();
}

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


// PASCA & G1 Fazrin Mauza
// --- FUNGSI GAPURA PASCA ---
static void drawGapuraPASCA(float px, float py, float pz) {
    glPushMatrix(); glTranslatef(px, py, pz); glRotatef(-135.0f, 0.0f, 1.0f, 0.0f);

    float radius = 24.0f; int segments = 60; float totalAngle = 100.0f;

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
    const char* textString = "BEREPUTASI GLOBAL";
    int len = strlen(textString); float textZ = -27.0f; float textScaleX = 0.018f; float textScaleY = 0.018f; float charGap = 15.0f;

    float totalWidthUnits = 0.0f;
    for (int i = 0; i < len; i++) {
        float w = glutStrokeWidth(GLUT_STROKE_ROMAN, textString[i]);
        if (textString[i] == ' ') w = 80.0f;
        totalWidthUnits += w;
    }
    if (len > 0) totalWidthUnits += (len - 1) * charGap;
    float totalArcLength = totalWidthUnits * textScaleX; float totalTextAngleRad = totalArcLength / 27.0f; float totalTextAngleDeg = totalTextAngleRad * (180.0f / 3.14159f); float currentAngle = totalTextAngleDeg / 2.0f;
    glColor3f(0.0f, 0.5f, 0.5f); glLineWidth(2.5f);
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
    float logoAngle = (totalTextAngleDeg / 2.0f) + 5.0f; glPushMatrix(); glRotatef(logoAngle, 0, 1, 0); glPopMatrix();
    glPopMatrix();
}

// ==================================================================================
// FUNGSI GEDUNG PASCASARJANA (G10)
// ==================================================================================
void drawStrokeText(const char* text, float x, float y, float z, float scale) {
    glPushMatrix(); glTranslatef(x, y, z); glScalef(scale, scale, scale);
    for (float offset = 0.0f; offset <= 2.2f; offset += 0.3f) {
        glPushMatrix(); glTranslatef(offset, 0.0f, 0.0f); glScalef(scale, scale * 1.3f, scale);
        for (const char* c = text; *c != '\0'; c++) {
            glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *c);
        }
        glPopMatrix();
    }
    glPopMatrix();
}

static void drawGedungPasca2(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);

    // --- DIMENSI ---
    float width = 90.0f; float height = 22.0f;   float depth = 39.0f;

    // 1. BADAN GEDUNG UTAMA
    glPushMatrix();
    glTranslatef(0.0f, height / 2.0f, 0.0f); glScalef(width, height, depth); glColor3f(0.92f, 0.94f, 0.98f); glutSolidCube(1.0f);
    glPopMatrix();

    // 2. ATAP LIMAS (WARNA GENTENG MERAH)
    glPushMatrix(); glTranslatef(0, height, 0); float roofH = 11.0f; float pad = 3.0f; glColor3f(0.85f, 0.35f, 0.1f);  glBegin(GL_TRIANGLES);

    // Depan
    glVertex3f(-(width / 2) - pad, 0, (depth / 2) + pad); glVertex3f((width / 2) + pad, 0, (depth / 2) + pad); glVertex3f(0, roofH, 0);
    // Belakang
    glVertex3f((width / 2) + pad, 0, -(depth / 2) - pad); glVertex3f(-(width / 2) - pad, 0, -(depth / 2) - pad); glVertex3f(0, roofH, 0);
    // Kiri
    glVertex3f(-(width / 2) - pad, 0, -(depth / 2) - pad); glVertex3f(-(width / 2) - pad, 0, (depth / 2) + pad); glVertex3f(0, roofH, 0);
    // Kanan
    glVertex3f((width / 2) + pad, 0, (depth / 2) + pad); glVertex3f((width / 2) + pad, 0, -(depth / 2) - pad); glVertex3f(0, roofH, 0);
    glEnd();
    glPopMatrix();


    // 3. FASAD DEPAN (PANEL TOSCA DAN NAMA GEDUNG)
    // Bar nama fakultas
    glPushMatrix(); glTranslatef(0, height - 2, (depth / 2) + 1.2f); glScalef((width - 6.4), 4.0f, 1.4f);  glColor3f(0.0f, 0.4f, 0.45f); glutSolidCube(1); glPopMatrix();

    // Pilar kiri & kanan
    float colW = 4.0f;  float colPos = (width / 2) - 15.0f;
    // Pilar kiri & kanan
    float colW2 = 4.0f; float colPos2 = (width / 2) - 15.0f;

    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix(); glTranslatef(side * (colPos - 17), height / 2, (depth / 2) + 0.8f); glScalef(colW, height, 4.0f); glColor3f(0.0f, 0.55f, 0.55f); glutSolidCube(1); glPopMatrix();
    }
    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix();
        glTranslatef(side * (colPos2 + 14), height / 2, (depth / 2) + 0.8f);
        glScalef(colW, height, 2.0f);
        glColor3f(0.6f, 0.6f, 0.6f);
        glutSolidCube(1);
        glPopMatrix();
    }

    // TEKS NAMA GEDUNG
    glPushMatrix();  glColor3f(1, 1, 1); float textZ = (depth / 2) + 2.8f;
    // Baris 1: PASCASARJANA
    drawStrokeText("PASCASARJANA", -6.0f, height - 7.0f, textZ, 0.10f);
    // Baris 2: FAKULTAS EKONOMIKA & BISNIS
    drawStrokeText("FAKULTAS EKONOMIKA & BISNIS", -9.0f, height - 9.0f, textZ, 0.08f);
    glPopMatrix();

    // 5. KANOPI LOBI DEPAN
    glPushMatrix(); glTranslatef(0, 9.5f, (depth / 2) + 6.0f); glScalef(20, 1.5f, 10);  glColor3f(0.15f, 0.15f, 0.20f); glutSolidCube(1); glPopMatrix();
    // latar pascasarjana
    glPushMatrix(); glTranslatef(0, -0.2f, (depth / 2) + 10.7f); glScalef(93, 0.5f, 22);  glColor3f(1.0f, 1.0f, 1.0f);  glutSolidCube(1); glPopMatrix();
    // Pintu kaca utama
    glPushMatrix();  glTranslatef(0, 5.0f, (depth / 2) + 0.5f); glScalef(10.0f, 9.0f, 0.5f); glColor3f(0.05f, 0.05f, 0.10f);  glutSolidCube(1); glPopMatrix();
    // atas Pintu kaca utama
    glPushMatrix();  glTranslatef(0, 15.0f, (depth / 2) + 0.5f); glScalef(22.0f, 9.0f, 0.5f); glColor3f(0.05f, 0.05f, 0.10f); glutSolidCube(1); glPopMatrix();

    glPopMatrix();
}


// ========================================================================
// FUNGSI GEDUNG G1
// ========================================================================
static void drawGedungG1(float px, float py, float pz, float sx, float sy, float sz) {
    glPushMatrix();
    // Posisikan gedung (pivot di tengah bawah)
    glTranslatef(px, py + (sy / 2.0f), pz);

    // --- 1. DINDING UTAMA GEDUNG (WARNA PUTIH) ---
    glPushMatrix(); glScalef(sx, sy, sz); glColor3f(0.92f, 0.92f, 0.95f); glutSolidCube(1.0f); glPopMatrix();

    // --- 1.B ATAP GENTENG (WARNA ORANYE/MERAH BATA) ---
    // Tambahan atap limas di atas gedung
    glPushMatrix();
    glTranslatef(0.0f, (sy / 2.0f), 0.0f);  float roofHeight = 10.0f; float pad = 1.0f; glColor3f(0.75f, 0.3f, 0.1f);
    glBegin(GL_TRIANGLES);
    // Sisi Depan
    glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(-(sx / 2.0f) - pad, 0.0f, (sz / 2.0f) + pad); glVertex3f((sx / 2.0f) + pad, 0.0f, (sz / 2.0f) + pad); glVertex3f(0.0f, roofHeight, 0.0f); // Puncak

    // Sisi Belakang
    glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f((sx / 2.0f) + pad, 0.0f, -(sz / 2.0f) - pad);  glVertex3f(-(sx / 2.0f) - pad, 0.0f, -(sz / 2.0f) - pad);  glVertex3f(0.0f, roofHeight, 0.0f); // Puncak

    // Sisi Kiri
    glNormal3f(-1.0f, 0.5f, 0.0f);  glVertex3f(-(sx / 2.0f) - pad, 0.0f, -(sz / 2.0f) - pad); glVertex3f(-(sx / 2.0f) - pad, 0.0f, (sz / 2.0f) + pad); glVertex3f(0.0f, roofHeight, 0.0f); // Puncak

    // Sisi Kanan
    glNormal3f(1.0f, 0.5f, 0.0f); glVertex3f((sx / 2.0f) + pad, 0.0f, (sz / 2.0f) + pad); glVertex3f((sx / 2.0f) + pad, 0.0f, -(sz / 2.0f) - pad); glVertex3f(0.0f, roofHeight, 0.0f); // Puncak
    glEnd();
    glPopMatrix();

    // --- 2. LISPLANG ATAP (WARNA HIJAU TOSCA) ---
    // Bagian atap datar yang menonjol khas gedung G1
    glPushMatrix(); glTranslatef(0.0f, (sy / 2.0f) - 1.0f, 0.0f); glScalef(sx + 2.0f, 2.5f, sz + 2.0f); glColor3f(0.0f, 0.6f, 0.6f); glutSolidCube(1.0f); glPopMatrix();

    // --- 3. PILAR-PILAR VERTIKAL / SIRIP (WARNA HIJAU TOSCA) ---
    int numPilar = 8;      float pilarW = 1.5f;     float pilarD = 1.5f;
    float startX = -(sx / 2.0f) + 2.0f;  float gap = (sx - 4.0f) / (numPilar - 1);
    glColor3f(0.0f, 0.6f, 0.6f);

    for (int i = 0; i < numPilar; i++) {
        float curX = startX + (i * gap);

        // Pilar Belakang
        glPushMatrix(); glTranslatef(curX, 0.0f, -(sz / 2.0f) - (pilarD / 2.0f) + 0.1f); glScalef(pilarW, sy, pilarD); glutSolidCube(1.0f); glPopMatrix();
    }

    // --- 4. PEMBATAS LANTAI 1 & 2 (BALOK PUTIH) ---
    glPushMatrix(); glTranslatef(0.0f, 0.0f, (sz / 2.0f) + 0.6f);  glScalef(sx + 1.0f, 1.5f, 0.5f); glColor3f(0.95f, 0.95f, 0.95f);   glutSolidCube(1.0f); glPopMatrix();

    // --- 5. JENDELA KACA (GELAP) ---
    // Menggambar kaca di sela-sela antar pilar
    glColor3f(0.15f, 0.15f, 0.2f);
    float winH = (sy / 2.0f) - 3.0f;

    for (int i = 0; i < numPilar - 1; i++) {
        float midX = startX + (i * gap) + (gap / 2.0f);

        // Jendela Lantai 1 (Bawah)
        glPushMatrix(); glTranslatef(midX, -(sy / 4.0f) - 0.5f, (sz / 2.0f) + 0.2f); glScalef(gap - 0.2f, winH, 0.2f); glutSolidCube(1.0f); glPopMatrix();

        // Jendela Lantai 2 (Atas)
        glPushMatrix(); glTranslatef(midX, (sy / 4.0f) + 0.5f, (sz / 2.0f) + 0.2f); glScalef(gap - 0.2f, winH, 0.2f); glutSolidCube(1.0f);  glPopMatrix();
    }

    // --- 6. AREA PINTU MASUK UTAMA (LOBI) ---
    // Kotak kaca di tengah bawah
    glPushMatrix(); glTranslatef(0.0f, -(sy / 2.0f) + 2.5f, (sz / 2.0f) + 0.5f);  glScalef(gap * 1.5f, 5.0f, 0.5f); glColor3f(0.1f, 0.1f, 0.1f); glutSolidCube(1.0f); glPopMatrix();

    // Kanopi kecil di atas pintu
    glPushMatrix();
    glTranslatef(-2.6f, -(sy / 2.0f) + 10.7f, (sz / 2.0f) + 2.0f); glScalef(gap * 1.0f, 0.4f, 18.0f); glColor3f(0.2f, 0.2f, 0.2f);  glutSolidCube(1.0f);  glPopMatrix();
    // latar G1
    glPushMatrix();
    glTranslatef(-0.1f, -(sy / 2.0f) + -0.24f, (sz / 2.0f) + 7.0f); glScalef(88, 0.5f, 15); glColor3f(1.0f, 1.0f, 1.0f); glutSolidCube(1);
    glPopMatrix();

    glPopMatrix();
}


// ==================================================================================
// FUNGSI MONUMEN DEPAN
// ==================================================================================
static void drawMonumentDepan(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(200.0f, 0.0f, 1.0f, 0.0f);

    // --- 1. PAPAN "LIGAIKU" ---
    glPushMatrix();
    glTranslatef(-8.0f, 0.0f, 0.0f);
    glPushMatrix(); glTranslatef(0.0f, 2.0f, 0.0f); glScalef(8.0f, 4.0f, 2.0f); glColor3f(1.0f, 0.8f, 0.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 6.0f, 0.0f); glScalef(8.0f, 4.0f, 2.0f); glColor3f(0.0f, 0.1f, 0.4f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 9.5f, 1.1f); glColor3f(1.0f, 0.8f, 0.0f); drawDisk(0.0f, 1.5f, 20); glTranslatef(0.0f, 0.0f, 0.1f); glColor3f(1.0f, 1.0f, 1.0f); drawDisk(0.0f, 1.0f, 20); glPopMatrix();
    glPopMatrix();

    // --- 2. PAPAN BATU HITAM ---
    glPushMatrix();
    glTranslatef(4.0f, 0.0f, 0.0f);
    glPushMatrix(); glTranslatef(0.0f, 1.5f, 0.0f); glScalef(12.0f, 3.0f, 2.5f); glColor3f(0.2f, 0.2f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 4.5f, 0.0f); glScalef(10.0f, 3.0f, 0.5f); glColor3f(0.1f, 0.1f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
    glPopMatrix();
    glPopMatrix();
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
    float textScaleX = 0.018f; float textScaleY = 0.018f; float charGap = 15.0f;
    float totalWidthUnits = 0.0f;
    for (int i = 0; i < len; i++) { float w = glutStrokeWidth(GLUT_STROKE_ROMAN, textString[i]); if (textString[i] == ' ') w = 80.0f; totalWidthUnits += w; }
    if (len > 0) totalWidthUnits += (len - 1) * charGap;
    float totalArcLength = totalWidthUnits * textScaleX;
    float totalTextAngleRad = totalArcLength / 27.0f;
    float totalTextAngleDeg = totalTextAngleRad * (180.0f / 3.14159f);
    float currentAngle = totalTextAngleDeg / 2.0f;
    glColor3f(0.0f, 0.5f, 0.5f); glLineWidth(2.5f);
    for (int j = 0; j < len; j++) {
        char c = textString[j]; float w = glutStrokeWidth(GLUT_STROKE_ROMAN, c); if (c == ' ') w = 80.0f;
        float charArcLen = (w + charGap) * textScaleX; float charAngleDeg = (charArcLen / 27.0f) * (180.0f / 3.14159f);
        glPushMatrix(); glRotatef(currentAngle - (charAngleDeg / 2.0f), 0, 1, 0); glTranslatef(0.0f, 6.5f, textZ); glScalef(textScaleX, textScaleY, 1.0f); glTranslatef(-w / 2.0f, 0.0f, 0.0f);
        if (c != ' ') glutStrokeCharacter(GLUT_STROKE_ROMAN, c); glPopMatrix(); currentAngle -= charAngleDeg;
    }
    glLineWidth(1.0f);

    // 3. LOGO
    float logoAngle = (totalTextAngleDeg / 2.0f) + 5.0f;
    glPushMatrix(); glRotatef(logoAngle, 0, 1, 0); glTranslatef(0.0f, 6.8f, textZ); glColor3f(1.0f, 0.8f, 0.0f);
    glPushMatrix(); drawDisk(0.0f, 1.5f, 30); drawCylinder(1.5f, 1.5f, 0.3f, 30); glTranslatef(0.0f, 0.0f, 0.3f); drawDisk(0.0f, 1.5f, 30); glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

// --- FUNGSI REVISI: GEDUNG REKTORAT ---
static void drawGedungRektoratFEB(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(270.0f, 0.0f, 1.0f, 0.0f);

    // Dinding
    glPushMatrix(); glColor3f(0.8f, 0.75f, 0.6f); glTranslatef(0.0f, 10.0f, 0.0f); glScalef(60.0f, 20.0f, 15.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.8f, 0.75f, 0.6f); glTranslatef(0.0f, 10.0f, -67.5f); glScalef(58.0f, 20.0f, 120.0f); glutSolidCube(1.0f); glPopMatrix();

    // Pintu Kaca
    glPushMatrix(); glColor3f(0.7f, 0.7f, 0.7f); glTranslatef(0.0f, 5.0f, 7.6f); glScalef(12.0f, 10.0f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.1f, 0.1f, 0.3f);
    glPushMatrix(); glTranslatef(-2.8f, 5.0f, 7.7f); glScalef(5.0f, 9.0f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(2.8f, 5.0f, 7.7f); glScalef(5.0f, 9.0f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glPushMatrix(); glTranslatef(-0.5f, 5.0f, 8.0f); glScalef(0.2f, 2.0f, 0.2f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.5f, 5.0f, 8.0f); glScalef(0.2f, 2.0f, 0.2f); glutSolidCube(1.0f); glPopMatrix();

    // Atap
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glTranslatef(0.0f, 18.0f, 5.0f); glScalef(64.0f, 5.0f, 30.0f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.6f);
    glPushMatrix(); glTranslatef(0.0f, 16.0f, 20.1f); glScalef(64.5f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 20.0f, 20.1f); glScalef(64.5f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(-32.2f, 20.0f, 5.0f); glScalef(1.0f, 1.0f, 31.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(32.2f, 20.0f, 5.0f); glScalef(1.0f, 1.0f, 31.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glTranslatef(0.0f, 18.0f, -70.0f); glScalef(64.0f, 5.0f, 120.0f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.6f);
    glPushMatrix(); glTranslatef(-32.2f, 20.0f, -70.0f); glScalef(1.0f, 1.0f, 120.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(32.2f, 20.0f, -70.0f); glScalef(1.0f, 1.0f, 120.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 20.0f, -130.1f); glScalef(64.5f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();

    // Pilar & Nama
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); glTranslatef(0.0f, 18.0f, 22.0f); glScalef(26.0f, 5.0f, 10.0f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.6f);
    glPushMatrix(); glTranslatef(0.0f, 16.0f, 27.1f); glScalef(26.5f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 20.0f, 27.1f); glScalef(26.5f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(-13.2f, 20.0f, 22.0f); glScalef(1.0f, 1.0f, 11.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(13.2f, 20.0f, 22.0f); glScalef(1.0f, 1.0f, 11.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.0f, 0.1f, 0.4f); glTranslatef(0.0f, 19.0f, 27.2f); glScalef(22.0f, 4.0f, 0.5f); glutSolidCube(1.0f); glColor3f(1.0f, 1.0f, 1.0f); glTranslatef(0.0f, 0.0f, 0.6f); glScalef(0.8f, 0.6f, 0.1f); glutSolidCube(1.0f); glPopMatrix();

    glColor3f(0.0f, 0.6f, 0.6f); float pH = 16.0f; float pR = 1.5f;
    glPushMatrix(); glTranslatef(-28.0f, 0.0f, 16.0f); glRotatef(-90, 1, 0, 0); drawCylinder(pR, pR, pH, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(28.0f, 0.0f, 16.0f); glRotatef(-90, 1, 0, 0); drawCylinder(pR, pR, pH, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(-12.0f, 0.0f, 20.0f); glRotatef(-90, 1, 0, 0); drawCylinder(pR, pR, pH, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(12.0f, 0.0f, 20.0f); glRotatef(-90, 1, 0, 0); drawCylinder(pR, pR, pH, 16); glPopMatrix();

    glColor3f(0.2f, 0.3f, 0.4f);
    for (float zPos = -20.0f; zPos > -115.0f; zPos -= 15.0f) {
        glPushMatrix(); glTranslatef(-29.1f, 10.0f, zPos); glScalef(1.0f, 6.0f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(29.1f, 10.0f, zPos); glScalef(1.0f, 6.0f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
    }
    glPopMatrix();
}

static void drawAuditoriumG3(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);

    float bodyW = 40.0f; float bodyH = 22.0f; float bodyL = 100.0f;

    // Struktur
    glPushMatrix(); glColor3f(0.95f, 0.95f, 0.95f); glTranslatef(0.0f, bodyH / 2.0f, -bodyL / 2.0f); glScalef(bodyW, bodyH, bodyL); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.2f, 0.2f, 0.2f); glTranslatef(0.0f, bodyH, -bodyL / 2.0f); glScalef(bodyW + 2.0f, 1.0f, bodyL + 2.0f); glutSolidCube(1.0f); glPopMatrix();

    // Atap
    glPushMatrix(); glTranslatef(0.0f, bodyH, -bodyL / 2.0f); glColor3f(0.7f, 0.25f, 0.1f);
    float overhang = 2.0f; float w = (bodyW / 2.0f) + overhang; float l = (bodyL / 2.0f) + overhang; float h = 18.0f;
    glBegin(GL_TRIANGLES);
    glNormal3f(0.0f, 0.5f, 1.0f); glVertex3f(0.0f, h, 0.0f); glVertex3f(-w, 0.0f, l); glVertex3f(w, 0.0f, l);
    glNormal3f(1.0f, 0.5f, 0.0f); glVertex3f(0.0f, h, 0.0f); glVertex3f(w, 0.0f, l); glVertex3f(w, 0.0f, -l);
    glNormal3f(0.0f, 0.5f, -1.0f); glVertex3f(0.0f, h, 0.0f); glVertex3f(w, 0.0f, -l); glVertex3f(-w, 0.0f, -l);
    glNormal3f(-1.0f, 0.5f, 0.0f); glVertex3f(0.0f, h, 0.0f); glVertex3f(-w, 0.0f, -l); glVertex3f(-w, 0.0f, l);
    glEnd();
    glColor3f(0.2f, 0.1f, 0.0f); glLineWidth(2.0f);
    glBegin(GL_LINES); glVertex3f(0.0f, h, 0.0f); glVertex3f(-w, 0.0f, l); glVertex3f(0.0f, h, 0.0f); glVertex3f(w, 0.0f, l);
    glVertex3f(0.0f, h, 0.0f); glVertex3f(w, 0.0f, -l); glVertex3f(0.0f, h, 0.0f); glVertex3f(-w, 0.0f, -l);
    glVertex3f(-w, 0.0f, l); glVertex3f(w, 0.0f, l); glVertex3f(w, 0.0f, l); glVertex3f(w, 0.0f, -l);
    glVertex3f(w, 0.0f, -l); glVertex3f(-w, 0.0f, -l); glVertex3f(-w, 0.0f, -l); glVertex3f(-w, 0.0f, l);
    glEnd(); glLineWidth(1.0f); glPopMatrix();

    // Samping
    int numPillars = 8; float pillarGap = bodyL / numPillars;
    for (int i = 0; i <= numPillars; i++) {
        float zPos = -(i * pillarGap);
        glPushMatrix(); glColor3f(0.0f, 0.6f, 0.6f); glTranslatef(-bodyW / 2.0f - 1.0f, bodyH / 2.0f, zPos); glScalef(2.0f, bodyH, 2.0f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(bodyW / 2.0f + 1.0f, bodyH / 2.0f, zPos); glScalef(2.0f, bodyH, 2.0f); glutSolidCube(1.0f); glPopMatrix();
        if (i < numPillars && i != 2) {
            float winZ = zPos - (pillarGap / 2.0f); glColor3f(0.2f, 0.2f, 0.3f);
            glPushMatrix(); glTranslatef(-bodyW / 2.0f - 0.1f, 15.0f, winZ); glScalef(0.5f, 5.0f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
            glPushMatrix(); glTranslatef(-bodyW / 2.0f - 0.1f, 5.0f, winZ); glScalef(0.5f, 5.0f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
            glPushMatrix(); glTranslatef(bodyW / 2.0f + 0.1f, 15.0f, winZ); glScalef(0.5f, 5.0f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
            glPushMatrix(); glTranslatef(bodyW / 2.0f + 0.1f, 5.0f, winZ); glScalef(0.5f, 5.0f, 8.0f); glutSolidCube(1.0f); glPopMatrix();
        }
    }

    // Depan
    glPushMatrix(); glColor3f(0.15f, 0.15f, 0.15f); glTranslatef(0.0f, 5.0f, 0.1f); glScalef(bodyW, 10.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.0f, 0.6f, 0.6f); glTranslatef(0.0f, 5.0f, 1.0f); glScalef(4.0f, 10.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.1f, 0.1f, 0.1f); glTranslatef(0.0f, 16.0f, 1.0f); glScalef(7.0f, 12.0f, 0.5f); glutSolidCube(1.0f); glPopMatrix();

    // Text
    glPushMatrix(); glColor3f(1.0f, 1.0f, 1.0f); float wG = (float)glutStrokeWidth(GLUT_STROKE_ROMAN, 'G'); float w3 = (float)glutStrokeWidth(GLUT_STROKE_ROMAN, '3'); float totalW = (wG + w3) * 0.05f;
    glTranslatef(-totalW / 2.0f, 15.0f, 1.5f); glScalef(0.05f, 0.05f, 0.05f); glLineWidth(4.0f); glutStrokeCharacter(GLUT_STROKE_ROMAN, 'G'); glutStrokeCharacter(GLUT_STROKE_ROMAN, '3'); glLineWidth(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.8f, 0.8f, 0.8f); glTranslatef(-18.0f, 6.0f, 1.0f); glScalef(0.012f, 0.012f, 0.012f); const char* txtAud = "AUDITORIUM"; for (int j = 0; j < strlen(txtAud); j++) glutStrokeCharacter(GLUT_STROKE_ROMAN, txtAud[j]); glPopMatrix();

    // Detail
        glPushMatrix(); glColor3f(0.8f, 0.8f, 0.8f); glTranslatef(10.0f, 7.0f, 1.0f); glScalef(14.0f, 0.5f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(10.0f, 5.5f, 1.0f); glScalef(10.0f, 0.5f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glColor3f(0.0f, 0.6f, 0.6f); glTranslatef(-19.0f, 11.0f, 0.0f); glScalef(2.0f, 22.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glColor3f(0.0f, 0.6f, 0.6f); glTranslatef(19.0f, 11.0f, 0.0f); glScalef(2.0f, 22.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();

    // Pintu
    for (int d = -1; d <= 1; d += 2) {
        float doorSideX = d * (bodyW / 2.0f + 0.1f); float doorSideZ = -30.0f;
        glPushMatrix(); glTranslatef(doorSideX, 4.0f, doorSideZ); glRotatef(d * 90.0f, 0.0f, 1.0f, 0.0f);
        glColor3f(0.95f, 0.95f, 0.95f); glPushMatrix(); glScalef(8.0f, 8.0f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.2f, 0.25f, 0.3f); glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.1f); glScalef(7.0f, 7.0f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.95f, 0.95f, 0.95f); glPushMatrix(); glTranslatef(0.0f, 0.0f, 0.2f); glScalef(0.5f, 7.0f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 1.5f, 0.2f); glScalef(7.0f, 0.5f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, -1.5f, 0.2f); glScalef(7.0f, 0.5f, 0.5f); glutSolidCube(1.0f); glPopMatrix();
        glPopMatrix();
    }
    glPopMatrix();
}

static void drawGedungG3Unesa(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    float glassX = 35.0f; float glassZ = 10.0f; float radius = 14.0f; float towerH = 45.0f;

    for (int i = 0; i < 100; i++) {
        glPushMatrix(); float theta = (i * 15.0f) * (3.14159f / 180.0f);
        float sx = glassX + cos(theta + 3.14f) * 9.0f; float sz = glassZ + sin(theta + 3.14f) * 9.0f; float sy = (i * (towerH / 100.0f));
        glTranslatef(sx, sy, sz); glRotatef(-i * 15.0f, 0.0f, 1.0f, 0.0f); glScalef(7.0f, 0.5f, 2.5f); glColor3f(0.9f, 0.9f, 0.9f); glutSolidCube(1.0f); glPopMatrix();
    }
    glPushMatrix(); glTranslatef(glassX, 0.0f, glassZ); glRotatef(-90, 1, 0, 0); glColor3f(0.8f, 0.8f, 0.8f); drawCylinder(2.5f, 2.5f, towerH, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(glassX, towerH, glassZ); glRotatef(-90, 1, 0, 0); glColor3f(1.0f, 1.0f, 1.0f); drawDisk(0.0f, radius + 2.0f, 30); glPopMatrix();

    glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glDepthMask(GL_FALSE);
    glPushMatrix(); glTranslatef(glassX, 0.0f, glassZ); glRotatef(-90, 1, 0, 0); glColor4f(0.7f, 0.9f, 1.0f, 0.25f); drawCylinder(radius, radius, towerH, 32);
    glLineWidth(3.0f); glColor4f(0.0f, 0.6f, 0.5f, 0.9f); for (int k = 0; k < 8; k++) { glPushMatrix(); glRotatef(k * 45.0f, 0, 0, 1); glTranslatef(radius, 0, 0); drawCylinder(0.4f, 0.4f, towerH, 4); glPopMatrix(); }
    drawDisk(radius, radius + 0.5f, 32); glTranslatef(0, 0, towerH / 2); drawDisk(radius, radius + 0.5f, 32); glTranslatef(0, 0, towerH / 2); drawDisk(radius, radius + 0.5f, 32); glPopMatrix(); glDepthMask(GL_TRUE); glDisable(GL_BLEND);

    glPushMatrix(); glTranslatef(8.0f, 20.0f, 5.0f); glScalef(25.0f, 40.0f, 20.0f); glColor3f(0.35f, 0.35f, 0.35f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(15.0f, 14.0f, 25.0f); glScalef(50.0f, 1.0f, 30.0f); glColor3f(0.8f, 0.4f, 0.3f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.7f, 0.7f, 0.7f);
    glPushMatrix(); glTranslatef(30.0f, 0.0f, 35.0f); glRotatef(-90, 1, 0, 0); drawCylinder(1.5f, 1.5f, 14.0f, 16); glPopMatrix();
    glPushMatrix(); glTranslatef(0.0f, 0.0f, 35.0f); glRotatef(-90, 1, 0, 0); drawCylinder(1.5f, 1.5f, 14.0f, 16); glPopMatrix();

    float leftWingX = -40.0f;
    glPushMatrix(); glTranslatef(leftWingX, 18.0f, 0.0f); glScalef(70.0f, 36.0f, 25.0f); glColor3f(0.95f, 0.95f, 0.95f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.0f, 0.6f, 0.6f);
    glPushMatrix(); glTranslatef(leftWingX - 32.0f, 23.0f, 13.0f); glScalef(4.0f, 26.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(leftWingX, 35.0f, 13.0f); glScalef(68.0f, 2.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(leftWingX, 23.0f, 13.0f); glScalef(68.0f, 2.0f, 2.0f); glutSolidCube(1.0f); glPopMatrix();
    glColor3f(0.2f, 0.3f, 0.4f);
    for (int row = 0; row < 2; row++) { for (int col = 0; col < 6; col++) { glPushMatrix(); float jx = (leftWingX - 25.0f) + (col * 10.0f); float jy = 29.0f - (row * 12.0f); glTranslatef(jx, jy, 12.8f); glScalef(6.0f, 6.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix(); } }

    glPushMatrix(); glTranslatef(15.0f, 0.0f, 50.0f); glColor3f(0.8f, 0.8f, 0.8f); glPushMatrix(); glRotatef(-90, 1, 0, 0); drawCylinder(0.3f, 0.1f, 35.0f, 8); glPopMatrix();
    glPushMatrix(); glTranslatef(6.0f, 33.5f, 0.0f); glScalef(12.0f, 3.0f, 0.2f); glColor3f(0.9f, 0.1f, 0.1f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(6.0f, 30.5f, 0.0f); glScalef(12.0f, 3.0f, 0.2f); glColor3f(1.0f, 1.0f, 1.0f); glutSolidCube(1.0f); glPopMatrix(); glPopMatrix();
    glPopMatrix();
}

static void drawEnvironment() {
    glPushMatrix(); glColor3f(0.1f, 0.8f, 0.1f); glBegin(GL_QUADS); glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(-300.0f, 0.0f, -400.0f); glVertex3f(-300.0f, 0.0f, 300.0f); glVertex3f(300.0f, 0.0f, 300.0f); glVertex3f(300.0f, 0.0f, -400.0f); glEnd(); glPopMatrix();
    glPushMatrix(); glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS); glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(-55.0f, 0.1f, -350.0f); glVertex3f(-55.0f, 0.1f, 150.0f); glVertex3f(-25.0f, 0.1f, 150.0f); glVertex3f(-25.0f, 0.1f, -350.0f); glEnd();
    glBegin(GL_QUADS); glNormal3f(0.0f, 1.0f, 0.0f); glVertex3f(-25.0f, 0.1f, 50.0f); glVertex3f(-25.0f, 0.1f, 80.0f); glVertex3f(200.0f, 0.1f, 80.0f); glVertex3f(200.0f, 0.1f, 50.0f); glEnd();
    glPopMatrix();
}

static void drawText(float x, float y, const char* string) {
    glDisable(GL_LIGHTING); glDisable(GL_COLOR_MATERIAL); glRasterPos2f(x, y); int len = (int)strlen(string); for (int i = 0; i < len; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]); glEnable(GL_COLOR_MATERIAL); glEnable(GL_LIGHTING);
}

static void displayTextParams() {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity(); gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity(); glColor3f(1.0f, 1.0f, 1.0f); char buffer[120];
    float angleDeg = angle * (180.0f / 3.14159f); sprintf(buffer, "Pos: %.0f, %.0f, %.0f | Angle: %.0f deg | WASD = Jalan | Spasi/C = Naik/Turun", x, y, z, angleDeg);
    drawText(10, 690, buffer); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

// =========================================================================
// GEDUNG G2: REVISI TOTAL MIRIP FOTO (PANJANG KE BELAKANG & JENDELA)
// =========================================================================
static void drawGedungG2(float px, float py, float pz) {
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(-90.0f, 0.0f, 1.0f, 0.0f); // Rotasi 90 derajat ke kanan

    // Dimensi
    float g2Length = 45.0f; // Lebar muka depan
    float g2Width = 100.0f; // Panjang ke belakang (sama sengan G3)
    float g2Height = 20.0f;

    // 1. BODY UTAMA (Abu-abu terang)
    glPushMatrix();
    glColor3f(0.85f, 0.85f, 0.85f);
    glTranslatef(0.0f, g2Height / 2.0f, -g2Width / 2.0f);
    glScalef(g2Length, g2Height, g2Width);
    glutSolidCube(1.0f);
    glPopMatrix();

    // --- FASAD DEPAN (Berdasarkan Foto) ---
    float facadeZ = 0.1f; // Sedikit di depan body utama
    float bottomY = 0.0f;
    float topY = g2Height;

    // Panel Batu Hitam Kiri & Kanan
    glColor3f(0.2f, 0.2f, 0.2f); // Dark Grey/Black stone
    glBegin(GL_QUADS);
    // Kiri
    glVertex3f(-g2Length / 2.0f, bottomY, facadeZ);
    glVertex3f(-7.0f, bottomY, facadeZ); // Batas dengan panel tengah
    glVertex3f(-7.0f, topY, facadeZ);
    glVertex3f(-g2Length / 2.0f, topY, facadeZ);
    // Kanan
    glVertex3f(7.0f, bottomY, facadeZ); // Batas dengan panel tengah
    glVertex3f(g2Length / 2.0f, bottomY, facadeZ);
    glVertex3f(g2Length / 2.0f, topY, facadeZ);
    glVertex3f(7.0f, topY, facadeZ);
    glEnd();

    // Panel Tengah Cyan (Biru Muda)
    glColor3f(0.0f, 0.7f, 0.8f); // Cyan
    glBegin(GL_QUADS);
    glVertex3f(-7.0f, bottomY, facadeZ);
    glVertex3f(7.0f, bottomY, facadeZ);
    glVertex3f(7.0f, topY, facadeZ);
    glVertex3f(-7.0f, topY, facadeZ);
    glEnd();

    // Garis Horizontal Putih di Panel Tengah
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 1; i <= 8; ++i) {
        float h = i * (g2Height / 9.0f);
        glPushMatrix();
        glTranslatef(0.0f, h, facadeZ + 0.05f);
        glScalef(14.0f, 0.3f, 0.1f); // Lebar 14 (dari -7 sampai 7)
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // Pilar Vertikal Cyan di Ujung Kiri & Kanan
    glColor3f(0.0f, 0.7f, 0.8f); // Cyan
    float pillarW = 2.0f;
    glPushMatrix(); glTranslatef(-g2Length / 2.0f + pillarW / 2.0f, g2Height / 2.0f, facadeZ + 0.1f); glScalef(pillarW, g2Height, 0.4f); glutSolidCube(1.0f); glPopMatrix();
    glPushMatrix(); glTranslatef(g2Length / 2.0f - pillarW / 2.0f, g2Height / 2.0f, facadeZ + 0.1f); glScalef(pillarW, g2Height, 0.4f); glutSolidCube(1.0f); glPopMatrix();

    // Kanopi Putih di Atas
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(0.0f, g2Height + 0.5f, facadeZ + 2.0f); // Sedikit menonjol ke depan
    glScalef(g2Length + 2.0f, 1.0f, 4.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // TULISAN (Di Panel Hitam Kiri)
    glColor3f(1.0f, 1.0f, 1.0f); // Putih
    // "G2"
    glPushMatrix();
    glTranslatef(-20.0f, 16.0f, facadeZ + 0.2f);
    glScalef(0.04f, 0.04f, 0.04f);
    glLineWidth(3.0f);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, 'G');
    glutStrokeCharacter(GLUT_STROKE_ROMAN, '2');
    glLineWidth(1.0f);
    glPopMatrix();

    // "FAKULTAS..."
    glPushMatrix();
    glTranslatef(-21.0f, 8.0f, facadeZ + 0.2f);
    glScalef(0.006f, 0.006f, 0.006f);
    std::string text1 = "FAKULTAS EKONOMIKA DAN BISNIS";
    for (char c : text1) glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();
    // "UNIVERSITAS..."
    glPushMatrix();
    glTranslatef(-20.0f, 6.5f, facadeZ + 0.2f);
    glScalef(0.006f, 0.006f, 0.006f);
    std::string text2 = "UNIVERSITAS NEGERI SURABAYA";
    for (char c : text2) glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();

    // POT TANAMAN DI DEPAN
    for (float xPos = -18.0f; xPos <= 18.0f; xPos += 5.0f) {
        if (xPos > -6.0f && xPos < 6.0f) continue; // Kosongkan bagian tengah (pintu masuk imajiner)
        glColor3f(0.4f, 0.4f, 0.4f); // Pot Abu-abu
        glPushMatrix(); glTranslatef(xPos, 1.5f, facadeZ + 2.0f); glScalef(2.5f, 3.0f, 2.5f); glutSolidCube(1.0f); glPopMatrix();
        glColor3f(0.1f, 0.6f, 0.1f); // Tanaman Hijau
        glPushMatrix(); glTranslatef(xPos, 4.0f, facadeZ + 2.0f); glutSolidSphere(1.8f, 12, 12); glPopMatrix();
    }

    // --- JENDELA SAMPING (SEPANJANG KEDALAMAN 100.0f) ---
    glColor3f(0.1f, 0.1f, 0.2f); // Kaca Biru Gelap
    float winW = 0.2f; // Tebal jendela keluar
    float winH = 5.0f; // Tinggi jendela
    float winL = 4.0f; // Lebar jendela sepanjang z

    // Loop dari depan (Z sedikit di belakang fasad) sampai belakang
    for (float zDist = -2.0f; zDist > -g2Width + 2.0f; zDist -= 7.0f) {
        // SISI KIRI (Negative X)
        glPushMatrix(); glTranslatef(-g2Length / 2.0f - 0.1f, 6.0f, zDist); glScalef(winW, winH, winL); glutSolidCube(1.0f); glPopMatrix(); // Lt 1
        glPushMatrix(); glTranslatef(-g2Length / 2.0f - 0.1f, 14.0f, zDist); glScalef(winW, winH, winL); glutSolidCube(1.0f); glPopMatrix(); // Lt 2

        // SISI KANAN (Positive X)
        glPushMatrix(); glTranslatef(g2Length / 2.0f + 0.1f, 6.0f, zDist); glScalef(winW, winH, winL); glutSolidCube(1.0f); glPopMatrix(); // Lt 1
        glPushMatrix(); glTranslatef(g2Length / 2.0f + 0.1f, 14.0f, zDist); glScalef(winW, winH, winL); glutSolidCube(1.0f); glPopMatrix(); // Lt 2
    }

    glPopMatrix();
}

void handleMovement() {
    float speed = 0.4f; // Sedikit dipercepat
    if (keyStates['w']) { x += lx * speed; z += lz * speed; }
    if (keyStates['s']) { x -= lx * speed; z -= lz * speed; }
    if (keyStates['a']) { x += lz * speed; z -= lx * speed; }
    if (keyStates['d']) { x -= lz * speed; z += lx * speed; }
    if (keyStates[32]) { y += 1.0f; } if (keyStates['c']) { y -= 1.0f; if (y < 1.75f) y = 1.75f; }
}

static void display() {
    handleMovement();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glLoadIdentity();
    gluLookAt(x, y, z, x + lx, y + sin(pitch), z + lz, 0.0f, 1.0f, 0.0f);
    GLfloat light_pos[] = { 0.0f, 200.0f, 100.0f, 0.0f }; glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    drawEnvironment();
    // call sarjana & G1
    drawGedungG1(74.0f, 0.0f, 14.0f, 87.0f, 24.0f, 52.0f);
    drawGedungPasca2(70.0f, 0.0f, 120.0f);
    drawGapuraPASCA(-16.0f, 0.0f, 85.0f); // Gapura Pascasarjana
    drawTexture(textures[0], 24.4, 21, 120, 39, 20, 0, 90, 180);
    drawTexture(textures[1], 41.4, 18, 99, 26, 28, 0, 0, 180);
    drawTexture(textures[2], 98, 18, 99, 26, 28, 0, 0, 180);
    drawTexture(textures[3], 29.47, 23, 14, 53, 24, 0, 90, 180);
    drawTexture(textures[4], 74, 23, 41.1, 88, 24, 0, 180, 180);

    drawGedungG3Unesa(40.0f, 0.0f, -250.0f);
    drawMonumentDepan(10.0f, 0.0f, -300.0f);
    drawGedungRektoratFEB(14.0f, 0.0f, -159.0f);
    drawGapuraFEB(-16.0f, 0.0f, 44.0f);

    

    // G3 (Auditorium)
    drawAuditoriumG3(9.0f, 0.0f, -90.0f);

    // G2 (Mirip Foto, Panjang ke belakang)
    // GESER KE X=15.0f SUPAYA DEKAT JALAN (SAMA SEPERTI GEDUNG LAINNYA)
    drawGedungG2(15.0f, 0.0f, -40.0f);

    // Gedung Samping G2 (Abu-abu)
    // GESER KE X=55.0f AGAR TETAP DI SAMPING G2


    displayTextParams(); glutSwapBuffers();
}

static void keyDown(unsigned char key, int x, int y) { keyStates[key] = true; if (key == 27) exit(0); }
static void keyUp(unsigned char key, int x, int y) { keyStates[key] = false; }
static void processMouseMove(int xx, int yy) {
    int centerX = 1280 / 2; int centerY = 720 / 2; float deltaX = (xx - centerX) * 0.002f; float deltaY = (yy - centerY) * 0.002f;
    angle += deltaX; pitch -= deltaY; if (pitch > 1.5f) pitch = 1.5f; if (pitch < -1.5f) pitch = -1.5f;
    lx = sin(angle); lz = -cos(angle); if (xx != centerX || yy != centerY) glutWarpPointer(centerX, centerY);
}
static void init() {
    glEnable(GL_DEPTH_TEST); glEnable(GL_LIGHTING); glEnable(GL_LIGHT0); glEnable(GL_NORMALIZE); glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    GLfloat ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f }; GLfloat diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient); glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse); glutSetCursor(GLUT_CURSOR_NONE);

    textures[textureCount++] = loadTexture("gedung.png");
    textures[textureCount++] = loadTexture("pasca2.png");
    textures[textureCount++] = loadTexture("pasca3.png");
    textures[textureCount++] = loadTexture("g11.png");
    textures[textureCount++] = loadTexture("g12.png");

}
static void reshape(int w, int h) { if (h == 0) h = 1; float ratio = w * 1.0f / h; glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(45.0f, ratio, 0.1f, 1000.0f); glMatrixMode(GL_MODELVIEW); }

int main(int argc, char** argv) {
    glutInit(&argc, argv); glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100); glutInitWindowSize(1280, 720); glutCreateWindow("GEDUNG FEB UNESA - KELOMPOK 4 PTI 2024 C");
    init(); glutDisplayFunc(display); glutReshapeFunc(reshape); glutIdleFunc(display);
    glutKeyboardFunc(keyDown); glutKeyboardUpFunc(keyUp); glutPassiveMotionFunc(processMouseMove); glutMainLoop(); return 0;
}
