#pragma once

namespace light
{

namespace view
{

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	Camera(const CameraType cameraType);
	~Camera();

	void strafe(float units); // left/right
	void fly(float units);    // up/down
	void walk(float units);   // forward/backward

	void pitch(float angle); // rotate on right vector
	void yaw(float angle);   // rotate on up vector
	void roll(float angle);  // rotate on look vector

	void getViewMatrix(D3DXMATRIX* V); 
	void setCameraType(const CameraType cameraType); 

public:

	const D3DXVECTOR3& Position() const
	{
		return m_position;
	}

	void Position(D3DXVECTOR3& pos)
	{
		m_position = pos;
	}

	const D3DXVECTOR3& Right() const
	{
		return m_right;
	}

	const D3DXVECTOR3& Up() const
	{
		return m_up;
	}

	const D3DXVECTOR3& Look() const
	{
		return m_look;
	}

	void Look(D3DXVECTOR3& pos)
	{
		m_look = pos;
	}

private:
	CameraType  m_camera_type;

	D3DXVECTOR3 m_right;

	D3DXVECTOR3 m_up;

	D3DXVECTOR3 m_look;

	D3DXVECTOR3 m_position;
};

} // namespace view

} // namespace view
