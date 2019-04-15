#include "Camera.hh"

Camera::Camera() {
    camMatrix = MultMat4(
        Rx(yrot), MultMat4(Ry(xrot), T(-camPos.x, -camPos.y, -camPos.z)));

    float ratio = 2;
    float fov = 0.1;
    projectionMatrix =
        frustum(-fov * ratio, fov * ratio, -fov, fov, 0.2, 500.0);
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

void Camera::invertPitch() { yrot *= -1; }

void Camera::updatePos(float mx, float mz) {
    camPos.x += mx * camSpeed * cos(xrot) - mz * camSpeed * sin(xrot);
    camPos.z += mx * camSpeed * sin(xrot) + mz * camSpeed * cos(xrot);
}

void Camera::updateCamMatrix() {
    camMatrix = MultMat4(
        Rx(yrot), MultMat4(Ry(xrot), T(-camPos.x, -camPos.y, -camPos.z)));
}
