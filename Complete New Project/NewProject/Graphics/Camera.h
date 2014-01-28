#ifndef CAMERA_H
#define CAMERA_H

#include <Windows.h>
#include "xnamath.h"

// Camera object that handles the current view matrix according to it's
// position and orientation
class _DECLSPEC_ALIGN_16_ Camera {
public:
    Camera();
    ~Camera();

    XMFLOAT4X4& updateCamera(float dt);

private:
	XMFLOAT3 mEye;
	XMFLOAT3 mAt;
	XMFLOAT3 mUp;
    XMFLOAT4X4 mView;
};

#endif
