#include "world.h"
#include "vec3.h"
#include "linalg.h"
#include "screen.h"
#include "keyboard.h"
#include "vec2.h"
#include "mat2.h"
namespace world {
        
    struct camera {
        vec3  pos;
        vec3  viewdir;
        float aspect;  // aspect ratio (width/height)
        float fovy; // field of view in y direction
        float near; // near clipping plane
        float far; // far clipping plane
    };

    camera cam;
    void handleKeyboardInput(keycode kc, bool pressed) {
        if (!pressed) {
            return;
        }

        if (kc == None) {
            return;
        }

        if (pressed) {
            if (kc == LeftArrow) {
                vec4 newviewdir = rotate(0.1f, vec3(0, 1, 0)) * vec4(cam.viewdir, 0);
                cam.viewdir = vec3(newviewdir.x, newviewdir.y, newviewdir.z).normalize();
            }
            else if (kc == RightArrow) {
                vec4 newviewdir = rotate(-0.1f, vec3(0, 1, 0)) * vec4(cam.viewdir, 0);
                cam.viewdir = vec3(newviewdir.x, newviewdir.y, newviewdir.z).normalize();
            }
            else if (kc == UpArrow) {
                vec4 newviewdir = rotate(0.1f, vec3(1, 0, 0)) * vec4(cam.viewdir, 0);
                cam.viewdir = vec3(newviewdir.x, newviewdir.y, newviewdir.z).normalize();
            }
            else if (kc == DownArrow) {
                vec4 newviewdir = rotate(-0.1f, vec3(1, 0, 0)) * vec4(cam.viewdir, 0);
                cam.viewdir = vec3(newviewdir.x, newviewdir.y, newviewdir.z).normalize();
            }
        }
    }

    void init(int screenWidth, int screenHeight) {
        cam.pos = vec3(0, 0, 10);
        cam.viewdir = vec3(0, 0, -1);
        cam.aspect = (float) screenWidth / screenHeight;
        cam.fovy = 90.f;
        cam.near = 0.01f;
        cam.far = 20.f;

        keyboard::subscribeToInputEvent(handleKeyboardInput);
    }

    void run() {
       static constexpr int TRIANGLES_NUM = 4;
        vec3 triangleVerticesList[TRIANGLES_NUM][3] = {
            { /* front */
                vec3(-1, -1, 1),
                vec3(1, -1, 1),
                vec3(-1, 1, 1),
            },
            {
                vec3(1, -1, 1),
                vec3(-1, 1, 1),
                vec3(1, 1, 1),
            },
            { /* back */
                vec3(-1, -1, -1),
                vec3(1, -1, -1),
                vec3(-1, 1, -1),
            },
            {
                vec3(1, -1, -1),
                vec3(-1, 1, -1),
                vec3(1, 1, -1),
            },
        };



        for (float t = 0;; t+= 0.05f) {
            for (int row = 0; row < 200; row++) {
                for (int col = 0; col < 320; col++) {
                    screen::putpixel(col, row, 0, 0, 0);
                }
            }
            for (int i = 0; i < TRIANGLES_NUM; i++) {
            vec3* triangleVertices = triangleVerticesList[i];
            vec4 ccsVerts[3];

            mat4 M = rotate(t, vec3(0, 1, 0)) * scale(3,3,3);
            mat4 V = viewing(cam.pos, cam.pos + cam.viewdir, vec3(0, 1, 0));
            mat4 P = perspective(cam.fovy, cam.aspect, cam.near, cam.far);

            ccsVerts[0] = P * V * M * vec4(triangleVertices[0], 1);
            ccsVerts[1] = P * V * M * vec4(triangleVertices[1], 1);
            ccsVerts[2] = P * V * M * vec4(triangleVertices[2], 1);

            vec3 ndcsVerts[3];
            ndcsVerts[0] = (1 / ccsVerts[0].w) * vec3(ccsVerts[0].x, ccsVerts[0].y, ccsVerts[0].z);
            ndcsVerts[1] = (1 / ccsVerts[1].w) * vec3(ccsVerts[1].x, ccsVerts[1].y, ccsVerts[1].z);
            ndcsVerts[2] = (1 / ccsVerts[2].w) * vec3(ccsVerts[2].x, ccsVerts[2].y, ccsVerts[2].z);

            float R = screen::width();
            float L = 0;
            float T = 0;
            float B = screen::height();
            vec3 dcsVerts[3];
            dcsVerts[0] = vec3(
                0.5 * (ndcsVerts[0].x + 1) * (R - L) + L,
                0.5 * (ndcsVerts[0].y + 1) * (T - B) + B,
                0.5 * (ndcsVerts[0].z + 1)
            );
            dcsVerts[1] = vec3(
                0.5 * (ndcsVerts[1].x + 1) * (R - L) + L,
                0.5 * (ndcsVerts[1].y + 1) * (T - B) + B,
                0.5 * (ndcsVerts[1].z + 1)
            );
            dcsVerts[2] = vec3(
                0.5 * (ndcsVerts[2].x + 1) * (R - L) + L,
                0.5 * (ndcsVerts[2].y + 1) * (T - B) + B,
                0.5 * (ndcsVerts[2].z + 1)
            );

            vec2 v0 = vec2(dcsVerts[0].x, dcsVerts[0].y);
            vec2 v1 = vec2(dcsVerts[1].x, dcsVerts[1].y);
            vec2 v2 = vec2(dcsVerts[2].x, dcsVerts[2].y);
            for (int row = 0; row < 200; row++) {
                for (int col = 0; col < 320; col++) {
                    vec2 pixelv(col, row);
                    if (pointInTriangle(pixelv, v0, v1, v2)) {
                        screen::putpixel(col, row, 255, row, col);
                    }
                }
            }

            // screen::drawline(dcsVerts[0].x, dcsVerts[0].y, dcsVerts[1].x, dcsVerts[1].y, 255, 0, 0);
            // screen::drawline(dcsVerts[0].x, dcsVerts[0].y, dcsVerts[2].x, dcsVerts[2].y, 0, 255, 0);
            // screen::drawline(dcsVerts[2].x, dcsVerts[2].y, dcsVerts[1].x, dcsVerts[1].y, 0, 0, 255);
            }
            for (int sleep = 0; sleep < 100; sleep++) {
            for (int sleep1 = 0; sleep1 < 100; sleep1++) {
            for (int sleep2 = 0; sleep2 < 100; sleep2++) {
                screen::putpixel(5, 5, sleep, sleep1, sleep2);
            }
            }
            }
        }

    }
}
