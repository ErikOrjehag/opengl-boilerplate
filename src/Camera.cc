#include "Camera.hh"

Camera::Camera() {
    camMatrix = MultMat4(
        Rx(yrot), MultMat4(Ry(xrot), T(-camPos.x, -camPos.y, -camPos.z)));

    projectionMatrix = frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 50.0);
}

void Camera::increase() { camSpeed += SPEED_DELTA; }
void Camera::decrease() {
    camSpeed = std::max<double>(0, camSpeed - SPEED_DELTA);
    ;
}

void Camera::up() { camPos.y += camSpeed; }
void Camera::down() { camPos.y -= camSpeed; }

void Camera::rotateX(float dx) { xrot += dx; }
void Camera::rotateY(float dy) { yrot += dy; }

void Camera::updatePos(float mx, float mz) {
    camPos.x += mx * camSpeed * cos(xrot) - mz * camSpeed * sin(xrot);
    camPos.z += mx * camSpeed * sin(xrot) + mz * camSpeed * cos(xrot);
}

void Camera::updateCamMatrix() {
    camMatrix = MultMat4(
        Rx(yrot), MultMat4(Ry(xrot), T(-camPos.x, -camPos.y, -camPos.z)));
}
