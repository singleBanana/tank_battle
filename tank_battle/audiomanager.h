#ifndef _AUDIOMANAGER_H
#define _AUDIOMANAGER_H

#include <hge.h>

class AudioManager
{
	static AudioManager* m_instance;

	HGE* m_hge;

	HEFFECT m_effbgm;
	HCHANNEL m_chbgm;

	int m_bgmvolumn;

public:
	static AudioManager* GetInstance();

	int GetBGMVolumn();
	void SetBGMVolumn(int v);

	void PlayEffect(HEFFECT eff);

	void PlayBGM(HEFFECT bgm);
	void PauseBGM();
	void ResumeBGM();
	void StopBGM();
	void BGMSlide(float t, int v); //v: The new volume value, 0..100. - 1 = leave current. - 2 = fade to 0 and stop.
};

#endif