
#include <algorithm>
#include <memory>

#include "GL_utilities.h"
#include "MicroGlut.h"
#include "VectorUtils3.h"
#include "loadobj.h"

#include "Render.hh"

bool onGround { false };

bool mouseInit { false };
int mousex;
int mousey;
int buttonState = GLUT_UP;

Render render {};

// How many keys?????
bool isPressed[500];

/**
 * Only executed the key action function if the button was not previously
 * pressed.
 */
template <typename Function>
void handleKeyAction(char c, Function f) {
    if (glutKeyIsDown(c) && !isPressed[(int)c]) {
        f();
        isPressed[(int)c] = true;
    } else if (!glutKeyIsDown(c)) {
        isPressed[(int)c] = false;
    }
}

void updateCam() {
    float mx = 0;
    float mz = 0;

    if (glutKeyIsDown('p')) render.cam->increase();
    if (glutKeyIsDown('o')) render.cam->decrease();
    if (glutKeyIsDown('a')) mx -= 1;
    if (glutKeyIsDown('d')) mx += 1;
    if (glutKeyIsDown('s')) mz += 1;
    if (glutKeyIsDown('w')) mz -= 1;

    if (glutKeyIsDown('r')) render.cam->up();
    if (glutKeyIsDown('e')) render.cam->down();

    handleKeyAction('i', [&]() { render.cam->invertPitch(); });
    handleKeyAction('l', [&]() { onGround = !onGround; });
    handleKeyAction(
        't', [&]() { render.shouldRenderHUD = !render.shouldRenderHUD; });

    render.cam->updatePos(mx, mz);

    if (onGround) {
        float h = render.terrain->height(render.cam->x(), render.cam->z());
        render.cam->y() = h + 1.86;
    }

    render.cam->updateCamMatrix();
}

void timer(int i) {
    glutTimerFunc(20, &timer, i);
    glutPostRedisplay();
    updateCam();
}

void mouseButton(int button, int state, int x, int y) { buttonState = state; }

void mouseMove(int x, int y) {
    if (buttonState == GLUT_DOWN && mouseInit) {
        float dx = (x - mousex) / 200.f;
        float dy = (y - mousey) / 200.f;
        render.cam->rotateX(dx);
        render.cam->rotateY(dy);
    }

    mousex = x;
    mousey = y;
    mouseInit = true;
}

void _display() { render.display(); }

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glutInitContextVersion(3, 2);
    glutInitWindowSize(render.SCREEN_WIDTH, render.SCREEN_HEIGHT);
    glutCreateWindow("WAWW");
    glutDisplayFunc(_display);
    render.init();
    glutTimerFunc(20, &timer, 0);
    glutMouseFunc(mouseButton);
    glutPassiveMotionFunc(mouseMove);
    glutMainLoop();
    exit(0);
}