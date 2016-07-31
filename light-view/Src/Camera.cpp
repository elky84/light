#include "light-view.h"
#include "Camera.h"

namespace light
{

namespace view
{

Camera::Camera(const CameraType cameraType)
: m_camera_type(cameraType)
, m_position(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
, m_right(D3DXVECTOR3(1.0f, 0.0f, 0.0f))
, m_up(D3DXVECTOR3(0.0f, 1.0f, 0.0f))
, m_look(D3DXVECTOR3(0.0f, 0.0f, 1.0f))
{
}

Camera::~Camera()
{

}

void Camera::walk(float units)
{
	// move only on xz plane for land object
	if( m_camera_type == LANDOBJECT )
		m_position += D3DXVECTOR3(m_look.x, 0.0f, m_look.z) * units;

	if( m_camera_type == AIRCRAFT )
		m_position += m_look * units;
}

void Camera::strafe(float units)
{
	// move only on xz plane for land object
	if( m_camera_type == LANDOBJECT )
		m_position += D3DXVECTOR3(m_right.x, 0.0f, m_right.z) * units;

	if( m_camera_type == AIRCRAFT )
		m_position += m_right * units;
}

void Camera::fly(float units)
{
	// move only on y-axis for land object
	if( m_camera_type == LANDOBJECT )
		m_position.y += units;

	if( m_camera_type == AIRCRAFT )
		m_position += m_up * units;
}

void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &m_right,	angle);

	// rotate m_up and m_look around m_right vector
	D3DXVec3TransformCoord(&m_up,&m_up, &T);
	D3DXVec3TransformCoord(&m_look,&m_look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	// rotate around world y (0, 1, 0) always for land object
	if( m_camera_type == LANDOBJECT )
		D3DXMatrixRotationY(&T, angle);

	// rotate around own up vector for aircraft
	if( m_camera_type == AIRCRAFT )
		D3DXMatrixRotationAxis(&T, &m_up, angle);

	// rotate m_right and m_look around m_up or y-axis
	D3DXVec3TransformCoord(&m_right,&m_right, &T);
	D3DXVec3TransformCoord(&m_look,&m_look, &T);
}

void Camera::roll(float angle)
{
	// only roll for aircraft type
	if( m_camera_type == AIRCRAFT )
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &m_look,	angle);

		// rotate m_up and m_right around m_look vector
		D3DXVec3TransformCoord(&m_right,&m_right, &T);
		D3DXVec3TransformCoord(&m_up,&m_up, &T);
	}
}

void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// Keep camera's axes orthogonal to eachother
	D3DXVec3Normalize(&m_look, &m_look);

	D3DXVec3Cross(&m_up, &m_look, &m_right);
	D3DXVec3Normalize(&m_up, &m_up);

	D3DXVec3Cross(&m_right, &m_up, &m_look);
	D3DXVec3Normalize(&m_right, &m_right);

	// Build the view matrix:
	float x = -D3DXVec3Dot(&m_right, &m_position);
	float y = -D3DXVec3Dot(&m_up, &m_position);
	float z = -D3DXVec3Dot(&m_look, &m_position);

	(*V)(0,0) = m_right.x; (*V)(0, 1) = m_up.x; (*V)(0, 2) = m_look.x; (*V)(0, 3) = 0.0f;
	(*V)(1,0) = m_right.y; (*V)(1, 1) = m_up.y; (*V)(1, 2) = m_look.y; (*V)(1, 3) = 0.0f;
	(*V)(2,0) = m_right.z; (*V)(2, 1) = m_up.z; (*V)(2, 2) = m_look.z; (*V)(2, 3) = 0.0f;
	(*V)(3,0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

void Camera::setCameraType(const CameraType cameraType)
{
	m_camera_type = cameraType;
}

} // namespace view

} // namespace view
