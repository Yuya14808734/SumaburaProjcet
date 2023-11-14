#include "CameraManager.h"

CameraManger& CameraManger::GetInstance()
{
	static CameraManger instance;

	return instance;
}

CameraManger::CameraManger()
	:m_pNowCamera(nullptr)
{

}

CameraManger::~CameraManger()
{

}

void CameraManger::CameraUpdate()
{
	if (m_pNowCamera != nullptr)
	{
		m_pNowCamera->Update();
	}
}

CameraBase* CameraManger::GetSceneCamera() const
{
	return m_pNowCamera;
}

void CameraManger::SetSceneCamera(std::string& CameraName)
{
	//カメラがあるかを探す
	std::map<std::string, CameraBase*>::iterator it = m_CameraList.find(CameraName);

	if (it == m_CameraList.end())
	{
		return;
	}

	//あれば設定
	m_pNowCamera = m_CameraList[CameraName];
}

bool CameraManger::AddCamera(CameraBase* pCamera, std::string& CameraName)
{
	//カメラがあるかを探す
	std::map<std::string, CameraBase*>::iterator it = m_CameraList.find(CameraName);

	if (it != m_CameraList.end())
	{
		if (pCamera != nullptr)
		{
			delete pCamera;
		}

		return false;
	}

	//なければ追加する
	m_CameraList.insert(std::make_pair(CameraName, pCamera));

	return true;
}

CameraBase* CameraManger::GetCamera(std::string& CameraName)
{
	//カメラがあるかを探す
	const std::map<std::string, CameraBase*>::iterator it = m_CameraList.find(CameraName);

	if (it == m_CameraList.end())
	{
		return nullptr;
	}

	return (*it).second;
}

CameraBase ** CameraManger::GetCameraPointer()
{
	return &m_pNowCamera;
}

void CameraManger::DestroyCamera(std::string& CameraName, bool MemoryDelete)
{
	//カメラがあるかを探す
	std::map<std::string, CameraBase*>::iterator it = m_CameraList.find(CameraName);

	if (it == m_CameraList.end())
	{
		return;
	}

	if (MemoryDelete)
	{
		delete (*it).second;
	}

	//あれば削除
	m_CameraList.erase(it);
}

void CameraManger::DestroyAllCamera(bool MemoryDelete)
{
	//すべてのカメラを削除
	if (MemoryDelete)
	{
		for (std::pair<std::string, CameraBase*> camera : m_CameraList)
		{
			delete camera.second;
		}
	}

	m_CameraList.clear();
}
