/*==============================================================================

	âπê∫çƒê∂ÉNÉâÉX[audio.h]
                                                             Author:äsÅ@ìNâF
															 Date  :2025/07/09
==============================================================================*/
#ifndef AUDIO_H
#define AUDIO_H

#include <xaudio2.h>
#include "Engine/Components/component.h"


class Audio : public Component
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};

	float					m_Volume = 1.0f;
	float					m_FadeSpeed = 0.01f;

public:
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();

	void Load(const char *FileName);
	void Play(bool Loop = false);

	void Stop();
	void SetVolume(float volume);
	void Fade();

	bool IsAutoDraw() const override { return false; }
};

#endif //AUDIO_H

