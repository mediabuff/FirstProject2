#include "stdafx.h"
#include "GUI2.h"

namespace System
{
namespace Gui
{

IMP_DEPENDENCY_PROPERTY(bool, AudioSourceVoice, IsPlaying, false)
IMP_DEPENDENCY_PROPERTY(float, AudioSourceVoice, Volume, 1.0f)
IMP_DEPENDENCY_PROPERTY(float, AudioSourceVoice, FrequencyRatio, 1.0f)
IMP_DEPENDENCY_PROPERTY(int64, AudioSourceVoice, SamplesPlayed, 0LL)

DependencyClass* AudioSourceVoice::get_Class()
{
	static DependencyClass depclass(typeid(thisClass), baseClass::get_Class());

	static DependencyProperty* properties[] =
	{
		get_IsPlayingProperty(),
		get_VolumeProperty(),
		get_FrequencyRatioProperty(),
		get_SamplesPlayedProperty(),
	};

	return &depclass;
}

AudioSourceVoice::AudioSourceVoice() : DependencyObject(get_Class())
{
}

AudioSourceVoice::AudioSourceVoice(WaveformData* waveformdata) : DependencyObject(get_Class())
{
	m_wave = waveformdata;
}

void AudioSourceVoice::Create()
{
	WAVEFORMATEX wfmt = m_wave->get_Format();
	pXAudio2->CreateSourceVoice(
		&m_pSourceVoice, &wfmt,
		0, XAUDIO2_DEFAULT_FREQ_RATIO, nullptr, nullptr, nullptr);
}

AudioSourceVoice* pp;

void CALLBACK AudTimerProc(HWND, UINT, UINT_PTR, DWORD)
{
	XAUDIO2_VOICE_STATE state;
	pp->m_pSourceVoice->GetState(&state);

	for (auto it = pp->m_wave->m_views.begin(); it != pp->m_wave->m_views.end(); ++it)
	{
		(*it)->set_AtSample(state.SamplesPlayed);
	}
}

void AudioSourceVoice::Play()
{
//	IXAudio2SourceVoice* pSourceVoice = NULL;
	/*
	pXAudio2->CreateSourceVoice( &m_pSourceVoice, (WAVEFORMATEX*)&m_wfx,
								 0, XAUDIO2_DEFAULT_FREQ_RATIO, NULL, NULL, NULL);
*/

	uint32 audioBytes;

	if (m_wave->m_audioBytes > XAUDIO2_MAX_BUFFER_BYTES)
	{
		audioBytes = XAUDIO2_MAX_BUFFER_BYTES;
	}
	else
	{
		audioBytes = (uint32)m_wave->m_audioBytes;
	}

	XAUDIO2_BUFFER buffer;
	buffer.AudioBytes = audioBytes;
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopBegin = 0;
	buffer.LoopCount = 0;
	buffer.LoopLength = 0;
	buffer.pAudioData = m_wave->m_audioData;
	buffer.pContext = nullptr;
	buffer.PlayBegin = 0;
	buffer.PlayLength = 0;

	m_pSourceVoice->SubmitSourceBuffer(&buffer);
	m_pSourceVoice->Start(0, XAUDIO2_COMMIT_NOW);

	pp = this;
	::SetTimer(nullptr, 2, 100, AudTimerProc);
}

}	// Gui
}	// System
