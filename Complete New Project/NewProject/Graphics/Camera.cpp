#include "Camera.h"

Camera::Camera() {
    // Default look at values
	XMVECTOR Eye = XMVectorSet( 0.0f, 4.0f, -10.0f, 0.0f );
	XMVECTOR At = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

    XMStoreFloat3(&mEye, Eye);
    XMStoreFloat3(&mAt, At);
    XMStoreFloat3(&mUp, Up);

	XMMATRIX View = XMMatrixLookAtLH( Eye, At, Up );
    XMStoreFloat4x4(&mView, View);

}

Camera::~Camera() {
}

XMFLOAT4X4& Camera::updateCamera(float dt) {
	XMVECTOR Eye = XMLoadFloat3(&mEye);
	XMVECTOR At = XMLoadFloat3(&mAt);
	XMVECTOR Up = XMLoadFloat3(&mUp);

    // Update the view matrix based on the eye, at and up vectors
	XMMATRIX View = XMMatrixLookAtLH( Eye, At, Up );
    XMStoreFloat4x4(&mView, View);

    return mView;
}
