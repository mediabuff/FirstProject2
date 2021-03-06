// LQuantizer.cpp : Implementation of CLQuantizer
#include "stdafx.h"
#include "LQuant.h"
#include "LQuantizer.h"

#define ROWBYTES(width,bitcount)			((((width)*(bitcount)+31) >> 3) & 0xfffc)

BYTE WebsafePalette[256][3] =
{
	0, 0, 0,
	51, 0, 0,
	102, 0, 0,
	153, 0, 0,
	204, 0, 0,
	255, 0, 0,
	0, 51, 0,
	51, 51, 0,
	102, 51, 0,
	153, 51, 0,
	204, 51, 0,
	255, 51, 0,
	0, 102, 0,
	51, 102, 0,
	102, 102, 0,
	153, 102, 0,
	204, 102, 0,
	255, 102, 0,
	0, 153, 0,
	51, 153, 0,
	102, 153, 0,
	153, 153, 0,
	204, 153, 0,
	255, 153, 0,
	0, 204, 0,
	51, 204, 0,
	102, 204, 0,
	153, 204, 0,
	204, 204, 0,
	255, 204, 0,
	0, 255, 0,
	51, 255, 0,
	102, 255, 0,
	153, 255, 0,
	204, 255, 0,
	255, 255, 0,
	0, 0, 51,
	51, 0, 51,
	102, 0, 51,
	153, 0, 51,
	204, 0, 51,
	255, 0, 51,
	0, 51, 51,
	51, 51, 51,
	102, 51, 51,
	153, 51, 51,
	204, 51, 51,
	255, 51, 51,
	0, 102, 51,
	51, 102, 51,
	102, 102, 51,
	153, 102, 51,
	204, 102, 51,
	255, 102, 51,
	0, 153, 51,
	51, 153, 51,
	102, 153, 51,
	153, 153, 51,
	204, 153, 51,
	255, 153, 51,
	0, 204, 51,
	51, 204, 51,
	102, 204, 51,
	153, 204, 51,
	204, 204, 51,
	255, 204, 51,
	0, 255, 51,
	51, 255, 51,
	102, 255, 51,
	153, 255, 51,
	204, 255, 51,
	255, 255, 51,
	0, 0, 102,
	51, 0, 102,
	102, 0, 102,
	153, 0, 102,
	204, 0, 102,
	255, 0, 102,
	0, 51, 102,
	51, 51, 102,
	102, 51, 102,
	153, 51, 102,
	204, 51, 102,
	255, 51, 102,
	0, 102, 102,
	51, 102, 102,
	102, 102, 102,
	153, 102, 102,
	204, 102, 102,
	255, 102, 102,
	0, 153, 102,
	51, 153, 102,
	102, 153, 102,
	153, 153, 102,
	204, 153, 102,
	255, 153, 102,
	0, 204, 102,
	51, 204, 102,
	102, 204, 102,
	153, 204, 102,
	204, 204, 102,
	255, 204, 102,
	0, 255, 102,
	51, 255, 102,
	102, 255, 102,
	153, 255, 102,
	204, 255, 102,
	255, 255, 102,
	0, 0, 153,
	51, 0, 153,
	102, 0, 153,
	153, 0, 153,
	204, 0, 153,
	255, 0, 153,
	0, 51, 153,
	51, 51, 153,
	102, 51, 153,
	153, 51, 153,
	204, 51, 153,
	255, 51, 153,
	0, 102, 153,
	51, 102, 153,
	102, 102, 153,
	153, 102, 153,
	204, 102, 153,
	255, 102, 153,
	0, 153, 153,
	51, 153, 153,
	102, 153, 153,
	153, 153, 153,
	204, 153, 153,
	255, 153, 153,
	0, 204, 153,
	51, 204, 153,
	102, 204, 153,
	153, 204, 153,
	204, 204, 153,
	255, 204, 153,
	0, 255, 153,
	51, 255, 153,
	102, 255, 153,
	153, 255, 153,
	204, 255, 153,
	255, 255, 153,
	0, 0, 204,
	51, 0, 204,
	102, 0, 204,
	153, 0, 204,
	204, 0, 204,
	255, 0, 204,
	0, 51, 204,
	51, 51, 204,
	102, 51, 204,
	153, 51, 204,
	204, 51, 204,
	255, 51, 204,
	0, 102, 204,
	51, 102, 204,
	102, 102, 204,
	153, 102, 204,
	204, 102, 204,
	255, 102, 204,
	0, 153, 204,
	51, 153, 204,
	102, 153, 204,
	153, 153, 204,
	204, 153, 204,
	255, 153, 204,
	0, 204, 204,
	51, 204, 204,
	102, 204, 204,
	153, 204, 204,
	204, 204, 204,
	255, 204, 204,
	0, 255, 204,
	51, 255, 204,
	102, 255, 204,
	153, 255, 204,
	204, 255, 204,
	255, 255, 204,
	0, 0, 255,
	51, 0, 255,
	102, 0, 255,
	153, 0, 255,
	204, 0, 255,
	255, 0, 255,
	0, 51, 255,
	51, 51, 255,
	102, 51, 255,
	153, 51, 255,
	204, 51, 255,
	255, 51, 255,
	0, 102, 255,
	51, 102, 255,
	102, 102, 255,
	153, 102, 255,
	204, 102, 255,
	255, 102, 255,
	0, 153, 255,
	51, 153, 255,
	102, 153, 255,
	153, 153, 255,
	204, 153, 255,
	255, 153, 255,
	0, 204, 255,
	51, 204, 255,
	102, 204, 255,
	153, 204, 255,
	204, 204, 255,
	255, 204, 255,
	0, 255, 255,
	51, 255, 255,
	102, 255, 255,
	153, 255, 255,
	204, 255, 255,
	255, 255, 255,
};

BYTE WindowsPalette[256][3] =
{
0, 0, 0,
128, 0, 0,
0, 128, 0,
128, 128, 0,
0, 0, 128,
128, 0, 128,
0, 128, 128,
128, 128, 128,
192, 220, 192,
166, 202, 240,
42, 63, 170,
42, 63, 255,
42, 95, 0,
42, 95, 85,
42, 95, 170,
42, 95, 255,
42, 127, 0,
42, 127, 85,
42, 127, 170,
42, 127, 255,
42, 159, 0,
42, 159, 85,
42, 159, 170,
42, 159, 255,
42, 191, 0,
42, 191, 85,
42, 191, 170,
42, 191, 255,
42, 223, 0,
42, 223, 85,
42, 223, 170,
42, 223, 255,
42, 255, 0,
42, 255, 85,
42, 255, 170,
42, 255, 255,
85, 0, 0,
85, 0, 85,
85, 0, 170,
85, 0, 255,
85, 31, 0,
85, 31, 85,
85, 31, 170,
85, 31, 255,
85, 63, 0,
85, 63, 85,
85, 63, 170,
85, 63, 255,
85, 95, 0,
85, 95, 85,
85, 95, 170,
85, 95, 255,
85, 127, 0,
85, 127, 85,
85, 127, 170,
85, 127, 255,
85, 159, 0,
85, 159, 85,
85, 159, 170,
85, 159, 255,
85, 191, 0,
85, 191, 85,
85, 191, 170,
85, 191, 255,
85, 223, 0,
85, 223, 85,
85, 223, 170,
85, 223, 255,
85, 255, 0,
85, 255, 85,
85, 255, 170,
85, 255, 255,
127, 0, 0,
127, 0, 85,
127, 0, 170,
127, 0, 255,
127, 31, 0,
127, 31, 85,
127, 31, 170,
127, 31, 255,
127, 63, 0,
127, 63, 85,
127, 63, 170,
127, 63, 255,
127, 95, 0,
127, 95, 85,
127, 95, 170,
127, 95, 255,
127, 127, 0,
127, 127, 85,
127, 127, 170,
127, 127, 255,
127, 159, 0,
127, 159, 85,
127, 159, 170,
127, 159, 255,
127, 191, 0,
127, 191, 85,
127, 191, 170,
127, 191, 255,
127, 223, 0,
127, 223, 85,
127, 223, 170,
127, 223, 255,
127, 255, 0,
127, 255, 85,
127, 255, 170,
127, 255, 255,
170, 0, 0,
170, 0, 85,
170, 0, 170,
170, 0, 255,
170, 31, 0,
170, 31, 85,
170, 31, 170,
170, 31, 255,
170, 63, 0,
170, 63, 85,
170, 63, 170,
170, 63, 255,
170, 95, 0,
170, 95, 85,
170, 95, 170,
170, 95, 255,
170, 127, 0,
170, 127, 85,
170, 127, 170,
170, 127, 255,
170, 159, 0,
170, 159, 85,
170, 159, 170,
170, 159, 255,
170, 191, 0,
170, 191, 85,
170, 191, 170,
170, 191, 255,
170, 223, 0,
170, 223, 85,
170, 223, 170,
170, 223, 255,
170, 255, 0,
170, 255, 85,
170, 255, 170,
170, 255, 255,
212, 0, 0,
212, 0, 85,
212, 0, 170,
212, 0, 255,
212, 31, 0,
212, 31, 85,
212, 31, 170,
212, 31, 255,
212, 63, 0,
212, 63, 85,
212, 63, 170,
212, 63, 255,
212, 95, 0,
212, 95, 85,
212, 95, 170,
212, 95, 255,
212, 127, 0,
212, 127, 85,
212, 127, 170,
212, 127, 255,
212, 159, 0,
212, 159, 85,
212, 159, 170,
212, 159, 255,
212, 191, 0,
212, 191, 85,
212, 191, 170,
212, 191, 255,
212, 223, 0,
212, 223, 85,
212, 223, 170,
212, 223, 255,
212, 255, 0,
212, 255, 85,
212, 255, 170,
212, 255, 255,
255, 0, 85,
255, 0, 170,
255, 31, 0,
255, 31, 85,
255, 31, 170,
255, 31, 255,
255, 63, 0,
255, 63, 85,
255, 63, 170,
255, 63, 255,
255, 95, 0,
255, 95, 85,
255, 95, 170,
255, 95, 255,
255, 127, 0,
255, 127, 85,
255, 127, 170,
255, 127, 255,
255, 159, 0,
255, 159, 85,
255, 159, 170,
255, 159, 255,
255, 191, 0,
255, 191, 85,
255, 191, 170,
255, 191, 255,
255, 223, 0,
255, 223, 85,
255, 223, 170,
255, 223, 255,
255, 255, 85,
255, 255, 170,
204, 204, 255,
255, 204, 255,
51, 255, 255,
102, 255, 255,
153, 255, 255,
204, 255, 255,
0, 127, 0,
0, 127, 85,
0, 127, 170,
0, 127, 255,
0, 159, 0,
0, 159, 85,
0, 159, 170,
0, 159, 255,
0, 191, 0,
0, 191, 85,
0, 191, 170,
0, 191, 255,
0, 223, 0,
0, 223, 85,
0, 223, 170,
0, 223, 255,
0, 255, 85,
0, 255, 170,
42, 0, 0,
42, 0, 85,
42, 0, 170,
42, 0, 255,
42, 31, 0,
42, 31, 85,
42, 31, 170,
42, 31, 255,
42, 63, 0,
42, 63, 85,
255, 251, 240,
160, 160, 164,
128, 128, 128,
255, 0, 0,
0, 255, 0,
255, 255, 0,
0, 0, 255,
255, 0, 255,
0, 255, 255,
255, 255, 255,
};

BYTE MacintoshPalette[256][3] =
{
255, 255, 255,
255, 255, 204,
255, 255, 153,
255, 255, 102,
255, 255, 51,
255, 255, 0,
255, 204, 255,
255, 204, 204,
255, 204, 153,
255, 204, 102,
255, 204, 51,
255, 204, 0,
255, 153, 255,
255, 153, 204,
255, 153, 153,
255, 153, 102,
255, 153, 51,
255, 153, 0,
255, 102, 255,
255, 102, 204,
255, 102, 153,
255, 102, 102,
255, 102, 51,
255, 102, 0,
255, 51, 255,
255, 51, 204,
255, 51, 153,
255, 51, 102,
255, 51, 51,
255, 51, 0,
255, 0, 255,
255, 0, 204,
255, 0, 153,
255, 0, 102,
255, 0, 51,
255, 0, 0,
204, 255, 255,
204, 255, 204,
204, 255, 153,
204, 255, 102,
204, 255, 51,
204, 255, 0,
204, 204, 255,
204, 204, 204,
204, 204, 153,
204, 204, 102,
204, 204, 51,
204, 204, 0,
204, 153, 255,
204, 153, 204,
204, 153, 153,
204, 153, 102,
204, 153, 51,
204, 153, 0,
204, 102, 255,
204, 102, 204,
204, 102, 153,
204, 102, 102,
204, 102, 51,
204, 102, 0,
204, 51, 255,
204, 51, 204,
204, 51, 153,
204, 51, 102,
204, 51, 51,
204, 51, 0,
204, 0, 255,
204, 0, 204,
204, 0, 153,
204, 0, 102,
204, 0, 51,
204, 0, 0,
153, 255, 255,
153, 255, 204,
153, 255, 153,
153, 255, 102,
153, 255, 51,
153, 255, 0,
153, 204, 255,
153, 204, 204,
153, 204, 153,
153, 204, 102,
153, 204, 51,
153, 204, 0,
153, 153, 255,
153, 153, 204,
153, 153, 153,
153, 153, 102,
153, 153, 51,
153, 153, 0,
153, 102, 255,
153, 102, 204,
153, 102, 153,
153, 102, 102,
153, 102, 51,
153, 102, 0,
153, 51, 255,
153, 51, 204,
153, 51, 153,
153, 51, 102,
153, 51, 51,
153, 51, 0,
153, 0, 255,
153, 0, 204,
153, 0, 153,
153, 0, 102,
153, 0, 51,
153, 0, 0,
102, 255, 255,
102, 255, 204,
102, 255, 153,
102, 255, 102,
102, 255, 51,
102, 255, 0,
102, 204, 255,
102, 204, 204,
102, 204, 153,
102, 204, 102,
102, 204, 51,
102, 204, 0,
102, 153, 255,
102, 153, 204,
102, 153, 153,
102, 153, 102,
102, 153, 51,
102, 153, 0,
102, 102, 255,
102, 102, 204,
102, 102, 153,
102, 102, 102,
102, 102, 51,
102, 102, 0,
102, 51, 255,
102, 51, 204,
102, 51, 153,
102, 51, 102,
102, 51, 51,
102, 51, 0,
102, 0, 255,
102, 0, 204,
102, 0, 153,
102, 0, 102,
102, 0, 51,
102, 0, 0,
51, 255, 255,
51, 255, 204,
51, 255, 153,
51, 255, 102,
51, 255, 51,
51, 255, 0,
51, 204, 255,
51, 204, 204,
51, 204, 153,
51, 204, 102,
51, 204, 51,
51, 204, 0,
51, 153, 255,
51, 153, 204,
51, 153, 153,
51, 153, 102,
51, 153, 51,
51, 153, 0,
51, 102, 255,
51, 102, 204,
51, 102, 153,
51, 102, 102,
51, 102, 51,
51, 102, 0,
51, 51, 255,
51, 51, 204,
51, 51, 153,
51, 51, 102,
51, 51, 51,
51, 51, 0,
51, 0, 255,
51, 0, 204,
51, 0, 153,
51, 0, 102,
51, 0, 51,
51, 0, 0,
0, 255, 255,
0, 255, 204,
0, 255, 153,
0, 255, 102,
0, 255, 51,
0, 255, 0,
0, 204, 255,
0, 204, 204,
0, 204, 153,
0, 204, 102,
0, 204, 51,
0, 204, 0,
0, 153, 255,
0, 153, 204,
0, 153, 153,
0, 153, 102,
0, 153, 51,
0, 153, 0,
0, 102, 255,
0, 102, 204,
0, 102, 153,
0, 102, 102,
0, 102, 51,
0, 102, 0,
0, 51, 255,
0, 51, 204,
0, 51, 153,
0, 51, 102,
0, 51, 51,
0, 51, 0,
0, 0, 255,
0, 0, 204,
0, 0, 153,
0, 0, 102,
0, 0, 51,
238, 0, 0,
221, 0, 0,
187, 0, 0,
170, 0, 0,
136, 0, 0,
119, 0, 0,
85, 0, 0,
68, 0, 0,
34, 0, 0,
17, 0, 0,
0, 238, 0,
0, 221, 0,
0, 187, 0,
0, 170, 0,
0, 136, 0,
0, 119, 0,
0, 85, 0,
0, 68, 0,
0, 34, 0,
0, 17, 0,
0, 0, 238,
0, 0, 221,
0, 0, 187,
0, 0, 170,
0, 0, 136,
0, 0, 119,
0, 0, 85,
0, 0, 68,
0, 0, 34,
0, 0, 17,
238, 238, 238,
221, 221, 221,
187, 187, 187,
170, 170, 170,
136, 136, 136,
119, 119, 119,
85, 85, 85,
68, 68, 68,
34, 34, 34,
17, 17, 17,
0, 0, 0,
};

//////////////////////////////////

STDMETHODIMP CLQuantizer::CInputPin::ReceiveConnection(ILPin *pConnector, ILMediaType* pMediaType)
{
	CComQIPtr<ILVideoMediaType> videoMediaType = pMediaType;
	if (videoMediaType == NULL) return E_NOINTERFACE;

	CLPinImpl::ReceiveConnection(pConnector, pMediaType);

	m_mediaType = videoMediaType;

	j_decompress_ptr	cinfo = &m_pFilter->info;
	ZeroMemory(cinfo, sizeof(*cinfo));

	long width;
	long height;
	m_mediaType->GetWidth(&width);
	m_mediaType->GetHeight(&height);

	CComObject<CMediaType>::CreateInstance(&m_pFilter->m_pOutput->m_pMediaType);
	m_pFilter->m_pOutput->m_pMediaType->AddRef();
	m_pFilter->m_pOutput->m_pMediaType->m_pOutput = m_pFilter->m_pOutput;

	cinfo->output_width = width;
	m_pFilter->m_height = height;

	cinfo->desired_number_of_colors = 256;//numcolors;

	int dither_method = 1;
	if (dither_method == 0)
		cinfo->dither_mode = JDITHER_NONE;
	else if (dither_method == 1)
		cinfo->dither_mode = JDITHER_FS;

	m_pFilter->m_in = (LPBYTE*)GlobalAlloc(0, m_pFilter->m_height*sizeof(LPBYTE));
	m_pFilter->m_out = (LPBYTE*)GlobalAlloc(0, m_pFilter->m_height*sizeof(LPBYTE));

	return S_OK;
}

STDMETHODIMP CLQuantizer::CInputPin::Receive(ILMediaSample *pSample)
{
	j_decompress_ptr	cinfo = &m_pFilter->info;

	CComQIPtr<ILVideoSample> videoSample = pSample;

	if (m_pFilter->m_pOutput->m_sample == NULL)
	{
		m_pFilter->m_pOutput->m_allocator->GetBuffer((ILMediaSample**)&m_pFilter->m_pOutput->m_sample);


	//	CComObject<CVideoSample>::CreateInstance(&m_pFilter->m_pOutput->m_pOutSample);
	//	m_pFilter->m_pOutput->m_pOutSample->AddRef();

		/*
		videoSample->GetWidth(&m_pFilter->m_pOutput->m_pOutSample->m_width);
		videoSample->GetHeight(&m_pFilter->m_pOutput->m_pOutSample->m_height);
		m_pFilter->m_pOutput->m_pOutSample->m_rowbytes = ROWBYTES(m_pFilter->m_pOutput->m_pOutSample->m_width, 8);
		m_pFilter->m_pOutput->m_pOutSample->m_bits = (LPBYTE)GlobalAlloc(0, m_pFilter->m_pOutput->m_pOutSample->m_rowbytes*m_pFilter->m_pOutput->m_pOutSample->m_height);
		*/

		{
			BYTE (* palette)[3] = WebsafePalette;
			int numcolors = 256;

			if (palette)	// Use predefined palette
			{
				cinfo->actual_number_of_colors = 256;//numcolors;
				cinfo->enable_2pass_quant = FALSE;

				cinfo->colormap = (unsigned char**)new char[(JDIMENSION)3*4];

				for (int i = 0; i < 3; i++)
				{
					cinfo->colormap[i] = (unsigned char*)new char[(JDIMENSION) 256];
				}

				for (i = 0; i < numcolors; i++)
				{
					cinfo->colormap[0][i] = palette[i][2];
					cinfo->colormap[1][i] = palette[i][1];
					cinfo->colormap[2][i] = palette[i][0];
				}
			}
			else
			{
				cinfo->enable_2pass_quant = TRUE;
			}

			jinit_2pass_quantizer(cinfo);
		}

		/*
		if (cinfo->enable_2pass_quant)
		{
			(*cinfo->cquantize->start_pass) (cinfo, TRUE);
		}
		*/

		(*cinfo->cquantize->start_pass) (cinfo, FALSE);

		/*
		if (cinfo->enable_2pass_quant)
		{
			(*cinfo->cquantize->finish_pass) (cinfo);
		}
		*/

		/*
		{
			int numcolors = cinfo->desired_number_of_colors;

			for (int i = 0; i < numcolors; i++)
			{
				pPalette[i].alpha = 255;
				pPalette[i].red = cinfo->colormap[2][i];
				pPalette[i].green = cinfo->colormap[1][i];
				pPalette[i].blue = cinfo->colormap[0][i];
			}
		}
		*/
	}

//	m_pFilter->SetupQuantizer(videoSample, NULL, 256, 1);

	CComQIPtr<ILVideoSample> insample = pSample;
	LVideoSampleData insampledata;
	insample->LockBits(&insampledata);

	LVideoSampleData outsampledata;
	m_pFilter->m_pOutput->m_sample->LockBits(&outsampledata);

	for (int y = 0; y < m_pFilter->m_height; y++)
	{
		m_pFilter->m_in[y] = (LPBYTE)insampledata.idata + insampledata.rowbytes*y;
		m_pFilter->m_out[y] = (LPBYTE)outsampledata.idata + outsampledata.rowbytes*y;
	}

	(*cinfo->cquantize->color_quantize) (cinfo, m_pFilter->m_in, m_pFilter->m_out, m_pFilter->m_height);

// Send the modified sample downstream
	CComQIPtr<ILMemInputPin> memInput = m_pFilter->m_pOutput->m_connectedTo;
	if (memInput)
	{
		memInput->Receive(m_pFilter->m_pOutput->m_sample);
	}

	return S_OK;
}

STDMETHODIMP CLQuantizer::CInputPin::EndOfStream()
{
	j_decompress_ptr	cinfo = &m_pFilter->info;

	if (m_pFilter->m_in)
	{
		(*cinfo->cquantize->finish_pass) (cinfo);

		for (int i = 0; i < 3; i++)
		{
			delete cinfo->colormap[i];
		}

		delete cinfo->colormap;

		if (m_pFilter->m_in) GlobalFree(m_pFilter->m_in);
		if (m_pFilter->m_out) GlobalFree(m_pFilter->m_out);
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////////
// CLQuantizer

#if 0
BOOL CLQuantizer::SetupQuantizer(ILVideoSample * pImage, BYTE palette[256][3], int numcolors, int dither_method)
{


	long width;
	long height;
	pImage->GetWidth(&width);
	pImage->GetHeight(&height);

	//ImageDef* inImage = pImage;

	LVideoSampleData sampledata;
	pImage->LockBits(&sampledata);

	for (int y = 0; y < m_height; y++)
	{
		m_in[y] = (LPBYTE)sampledata.idata + sampledata.rowbytes*y;
		m_out[y] = m_pOutputPin->m_pOutSample->m_bits + m_pOutputPin->m_pOutSample->m_rowbytes*y;
	}

	if (palette)	// Use predefined palette
	{
		cinfo->actual_number_of_colors = numcolors;
		cinfo->enable_2pass_quant = FALSE;

		cinfo->colormap = (unsigned char**)new char[(JDIMENSION)3*4];

		for (int i = 0; i < 3; i++)
		{
		   cinfo->colormap[i] = (unsigned char*)new char[(JDIMENSION) 256];
		}

		for (i = 0; i < numcolors; i++)
		{
			cinfo->colormap[0][i] = palette[i][2];
			cinfo->colormap[1][i] = palette[i][1];
			cinfo->colormap[2][i] = palette[i][0];
		}
	}
	else
	{
		cinfo->enable_2pass_quant = TRUE;
	}

	jinit_2pass_quantizer(cinfo);

	return TRUE;
}
#endif

#if 0
void CQuantize::StartOptimizePalette()
{
	j_decompress_ptr	cinfo = &info;

	if (cinfo->enable_2pass_quant)
	{
		(*cinfo->cquantize->start_pass) (cinfo, TRUE);
	}
}

void CQuantize::EndOptimizePalette()
{
	j_decompress_ptr	cinfo = &info;

	if (cinfo->enable_2pass_quant)
	{
		(*cinfo->cquantize->finish_pass) (cinfo);
	}
}

#if 0
void CQuantize::CalculateOptimizedPalette(ImageDef* pImage)
{
	j_decompress_ptr	cinfo = &info;

	if (cinfo->enable_2pass_quant)
	{
		(*cinfo->cquantize->color_quantize) (cinfo, m_in, m_out, m_height);
	}
}
#endif

void CQuantize::StartPass2()
{
// Prepare next pass(es)
	j_decompress_ptr	cinfo = &info;

	(*cinfo->cquantize->start_pass) (cinfo, FALSE);
}

void CQuantize::Quantize2()
{
	j_decompress_ptr	cinfo = &info;

	(*cinfo->cquantize->color_quantize) (cinfo, m_in, m_out, m_height);
}

void CQuantize::CleanupQuantizer()
{
	j_decompress_ptr	cinfo = &info;

	if (m_in)
	{
		(*cinfo->cquantize->finish_pass) (cinfo);

		for (int i = 0; i < 3; i++)
		{
			delete cinfo->colormap[i];
		}

		delete cinfo->colormap;

		if (m_in) GlobalFree(m_in);
		if (m_out) GlobalFree(m_out);
	}
}

void CQuantize::GetPalette(PixelDef* pPalette)
{
	j_decompress_ptr	cinfo = &info;

	int numcolors = cinfo->desired_number_of_colors;

	for (int i = 0; i < numcolors; i++)
	{
		pPalette[i].alpha = 255;
		pPalette[i].red = cinfo->colormap[2][i];
		pPalette[i].green = cinfo->colormap[1][i];
		pPalette[i].blue = cinfo->colormap[0][i];
	}
}

#endif
