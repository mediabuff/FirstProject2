#include "stdafx.h"
#include "GUI2.h"

// TODO remove
#include "LVOC/VOCLoader.h"
#pragma comment(lib, "LVOC.lib")

namespace System
{

TimeDuration::TimeDuration(uint64 durationNanoSeconds) : m_durationNanoSeconds(durationNanoSeconds)
{
}

TimeDuration::TimeDuration(double seconds) : m_durationNanoSeconds((double)seconds * 1000000000)
{
}

double TimeDuration::get_days()
{
	return (double)m_durationNanoSeconds * ((double)(24*60*60)/1000000000);
}

double TimeDuration::get_hours()
{
	return (double)m_durationNanoSeconds * ((double)(60*60)/1000000000);
}

double TimeDuration::get_minutes()
{
	return (double)m_durationNanoSeconds * (60.0/1000000000);
}

double TimeDuration::get_seconds()
{
	return (double)m_durationNanoSeconds * (1.0/1000000000);
}

IO::TextWriter& WriteToStream(IO::TextWriter& stream, const TimeDuration& duration)
{
	if (duration.m_durationNanoSeconds < 1000000000/60)
	{
		double seconds = (double)duration.m_durationNanoSeconds * (1.0/1000000000);
		stream << seconds << "s";
	}
	else
	{
		uint minutes = duration.m_durationNanoSeconds / (1000000000/60);
		uint seconds = duration.m_durationNanoSeconds % (1000000000/60);

		stream << minutes << ":" << seconds;
	}

	return stream;
}

namespace Gui
{

WaveformData::WaveformData()
{
}

uint64 WaveformData::get_SampleCount()
{
	return m_nsamples;
}

uint32 WaveformData::get_SampleCount32()
{
	return (uint32)m_nsamples;
}

uint64 WaveformData::get_SampleCount64()
{
	return m_nsamples;
}

TimeDuration WaveformData::get_Duration()
{
	return TimeDuration((double)m_nsamples / m_wfx.nSamplesPerSec);
}

// static
WaveformData* WaveformData::FromFile(StringIn url)
{
	Net::UrlBinding binding;
	_Ptr<IO::Stream> stream = binding.BindToObject(url, nullptr);
	if (stream == nullptr)
	{
		return nullptr;
	}

	MediaShow::VOCLoader loader;
	loader.Open(stream);
	loader.ReadFormatSignature();
	loader.ReadHeader();

	WaveformData* w = new WaveformData;

	w->m_audioBytes = loader.m_nSamples * loader.m_wfx.nBlockAlign;
	w->m_wfx = loader.m_wfx;
	w->m_wfx.cbSize = sizeof(WAVEFORMATEX);
	w->m_audioData = new byte[w->m_audioBytes];
	w->m_nsamples = loader.m_nSamples;

	loader.ReadSamples(w->m_audioData, loader.m_nSamples);

	return w;
}

GUIEXT WaveformVisual* Show(WaveformData* waveformdata)
{
	WaveformVisual* view = new WaveformVisual;

	waveformdata->m_views.push_back(view);
	view->set_Data(waveformdata);

	Window* win = new Window(view);
	win->Show();

	return view;
}

GUIEXT Object* Play(WaveformData* waveformdata)
{
	AudioSourceVoice* voice = new AudioSourceVoice(waveformdata);

	voice->Create();
	voice->Play();

	return voice;
}

vector<WaveformData*> playing;

}	// Gui
}	// System
