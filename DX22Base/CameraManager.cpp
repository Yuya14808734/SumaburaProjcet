#include "CameraManager.h"

CameraManager& CameraManager::GetInstance()
{
	static CameraManager instance;

	return instance;
}

CameraManager::CameraManager()
	:m_pNowCamera(nullptr)
{

}

CameraManager::~CameraManager()
{

}

void CameraManager::CameraUpdate()
{
	if (m_pNowCamera != nullptr)
	{
		m_pNowCamera->Update();
	}
}

CameraBase* CameraManager::GetSceneCamera() const
{
	return m_pNowCamera;
}

void CameraManager::SetSceneCamera(std::string CameraName)
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

bool CameraManager::CreateCamera(CameraBase* pCamera, std::string CameraName)
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

CameraBase* CameraManager::GetCamera(std::string CameraName)
{
	//カメラがあるかを探す
	const std::map<std::string, CameraBase*>::iterator it = m_CameraList.find(CameraName);

	if (it == m_CameraList.end())
	{
		return nullptr;
	}

	return (*it).second;
}

CameraBase ** CameraManager::GetCameraPointer()
{
	return &m_pNowCamera;
}

void CameraManager::DestroyCamera(std::string CameraName, bool MemoryDelete)
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

void CameraManager::DestroyAllCamera(bool MemoryDelete)
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
