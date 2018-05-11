#include "audiomanager.h"
#include "gameplay.h"

AudioManager* AudioManager::m_instance = nullptr;

AudioManager* AudioManager::GetInstance()
{
	if (!m_instance)
	{
		m_instance = new AudioManager;
		m_instance->m_hge = GamePlay::GetInstance()->GetHGE();
		m_instance->m_effbgm = 0;
		m_instance->m_chbgm = 0;
		m_instance->m_bgmvolumn = 100;
	}

	return m_instance;
}

int AudioManager::GetBGMVolumn()
{
	return m_bgmvolumn;
}

void AudioManager::SetBGMVolumn(int v)
{
	if (v >= 0 && v <= 100)
	{
		m_bgmvolumn = v;
		m_hge->Channel_SetVolume(m_chbgm, m_bgmvolumn);
	}
}

void AudioManager::PlayEffect(HEFFECT eff)
{
	if (eff)
	{
		m_hge->Effect_Play(eff);
	}
}

void AudioManager::PlayBGM(HEFFECT bgm)
{
	if (bgm)
	{
		if (!m_effbgm)
		{
			m_effbgm = bgm;
			m_chbgm = m_hge->Effect_PlayEx(bgm, m_bgmvolumn, 0, 1.0f, true);
		}
		else
		{
			if (m_effbgm != bgm)
			{
				m_effbgm = bgm;
				if (m_hge->Channel_IsPlaying(m_chbgm))
				{
					m_hge->Channel_Stop(m_chbgm);
				}
				m_chbgm = m_hge->Effect_PlayEx(bgm, m_bgmvolumn, 0, 1.0f, true);
			}
		}
	}
}

void AudioManager::PauseBGM()
{
	if (m_chbgm)
	{
		m_hge->Channel_Pause(m_chbgm);
	}
}

void AudioManager::ResumeBGM()
{
	if (m_chbgm)
	{
		m_hge->Channel_Resume(m_chbgm);
	}
}

void AudioManager::StopBGM()
{
	if (m_chbgm)
	{
		m_hge->Channel_Stop(m_chbgm);
	}
}

void AudioManager::BGMSlide(float t, int v)
{
	if (m_chbgm)
	{
		m_hge->Channel_SlideTo(m_chbgm, t, v);
	}
}