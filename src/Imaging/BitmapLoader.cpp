#include "stdafx.h"
#include "Imaging.h"
//#include "BitmapLoader.h"

#pragma comment(lib, "JFIF")
#pragma comment(lib, "PNG")

#include "GIF/GIF.h"
#pragma comment(lib, "GIF")

namespace System
{
namespace Imaging
{

// Little endian reading

int8 ReadInt8(IO::Stream* stream)
{
	int8 v;
	if (stream->Read(&v, 1) != 1)
		raise(Exception(L"End of stream"));
	return v;
}

int16 ReadInt16(IO::Stream* stream)
{
	int16 v;

	if (stream->Read(&v, 2) != 2)
		raise(Exception(L"End of stream"));

	return LittleEndian16(v);
}

int32 ReadInt32(IO::Stream* stream)
{
	int32 v;

	if (stream->Read(&v, 4) != 4)
		raise(Exception(L"End of stream"));

	return LittleEndian32(v);
}

BMPDecoder::BMPDecoder()
{
}

ErrorCode BMPDecoder::ReadMarker(IO::Stream* stream)
{
	uint16 bfType = ReadInt16(stream);
	/*
	if (stream->Read(&bfType, sizeof(bfType)) != sizeof(bfType))
		return -1;
		*/

	if (bfType != 0x4d42)
	{
		return -1;
	}

	return 0;
}

int BMPDecoder::ReadHeader(IO::Stream* stream, BitmapFormat* bitmapFormat)
{
	if (stream == NULL) raise(ArgumentNullException());
	if (bitmapFormat == NULL) raise(ArgumentNullException());

	IO::BinaryReader reader(stream);

//	bfh.bfReserved1 = 0;
//	bfh.bfReserved2 = 0;
//	bfh.bfOffBits = m_stream->Seek(0, System::IO::STREAM_SEEK_CUR);

	BITMAPFILEHEADER bfh;

	reader >> bfh.bfSize;
	reader >> bfh.bfReserved1;
	reader >> bfh.bfReserved2;
	reader >> bfh.bfOffBits;

//	stream.Read(&bfh, sizeof(bfh));

//	stream.Read(&m_bih, sizeof(m_bih));
	reader >> m_bih.biSize;
	reader >> m_bih.biWidth;
	reader >> m_bih.biHeight;
	reader >> m_bih.biPlanes;
	reader >> m_bih.biBitCount;
	reader >> m_bih.biCompression;
	reader >> m_bih.biSizeImage;
	reader >> m_bih.biXPelsPerMeter;
	reader >> m_bih.biYPelsPerMeter;
	reader >> m_bih.biClrUsed;
	reader >> m_bih.biClrImportant;

	stream->position += m_bih.biSize - sizeof(m_bih);

//	stream.Seek(m_bih.biSize - sizeof(m_bih), IO::STREAM_SEEK_CUR);

	bitmapFormat->width = m_bih.biWidth;
	bitmapFormat->height = m_bih.biHeight;

	if (m_bih.biBitCount == 8)
	{
		bitmapFormat->pixelFormat = PixelFormat_R8_UNORM;
	}
	else if (m_bih.biBitCount == 24)
	{
		bitmapFormat->pixelFormat = PixelFormat_R8G8B8A8_UNORM;
	}
	else if (m_bih.biBitCount == 32)
	{
		bitmapFormat->pixelFormat = PixelFormat_R8G8B8A8_UNORM;
	}
	else
		return -1;

	return 0;

	/*
	bitmapLoader->m_bitmap = new LDraw::Bitmap(bih.biWidth, bih.biHeight, LDraw::PixelFormat_t::RGB_32);

	LDraw::BitmapData bitmapData;
	bitmapLoader->m_bitmap->LockBits(NULL, LDraw::ImageLockModeWrite, &bitmapData);
*/
}

int BMPDecoder::ReadBitmap(IO::Stream* stream, void* data, int rowbytes)
{
	if (stream == NULL) raise(ArgumentNullException());
	if (data == NULL) raise(ArgumentNullException());

	int srcrowbytes = ROWBYTES_DWORD(m_bih.biWidth, m_bih.biBitCount);
	uint8* scanline = new ubyte[rowbytes];

	if (m_bih.biBitCount == 8)
	{
		RGBQUAD palette[256];
		uint ncolors = 1 << m_bih.biBitCount;
		stream->Read(palette, ncolors*sizeof(RGBQUAD));

		for (int y = 0; y < m_bih.biHeight; ++y)
		{
			stream->Read(scanline, srcrowbytes);

			ubyte* dest = (ubyte*)data + rowbytes * (m_bih.biHeight - y - 1);
		//	uint8* dest = bitmapData.Scan0 + bitmapData.Stride * (y);

			for (int x = 0; x < m_bih.biWidth; ++x)
			{
				uint8 index = scanline[x];
				dest[0] = palette[index].rgbBlue;
				dest[1] = palette[index].rgbGreen;
				dest[2] = palette[index].rgbRed;
				dest[3] = 255;
				dest += 4;
			}
		}
	}
	else if (m_bih.biBitCount == 24)
	{
		for (int y = 0; y < m_bih.biHeight; ++y)
		{
			stream->Read(scanline, srcrowbytes);

			ubyte* dest = (ubyte*)data + rowbytes * (m_bih.biHeight - y - 1);
		//	uint8* dest = bitmapData.Scan0 + bitmapData.Stride * (y);

			uint8* src = scanline;

			for (int x = 0; x < m_bih.biWidth; ++x)
			{
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
				dest[3] = 255;
				dest += 4;
				src += 3;
			}
		}
	}
	else if (m_bih.biBitCount == 32)
	{
		for (int y = 0; y < m_bih.biHeight; ++y)
		{
			stream->Read(scanline, srcrowbytes);

			ubyte* dest = (ubyte*)data + rowbytes * (m_bih.biHeight - y - 1);
		//	uint8* dest = bitmapData.Scan0 + bitmapData.Stride * (y);

			uint8* src = scanline;

			for (int x = 0; x < m_bih.biWidth; ++x)
			{
				dest[0] = src[2];
				dest[1] = src[1];
				dest[2] = src[0];
				dest[3] = 255;//src[3];
				dest += 4;
				src += 4;
			}
		}
	}
	else
		return -1;

	delete[] scanline;

	//bitmapLoader->m_bitmap->UnlockBits(&bitmapData);

	return 0;
}

namespace DX
{

//  (Direct3D 9)

/*
 * DDPIXELFORMAT
 */
typedef struct _DDPIXELFORMAT
{
    uint32       dwSize;                 // size of structure
    uint32       dwFlags;                // pixel format flags
    uint32       dwFourCC;               // (FOURCC code)
    union
    {
        uint32   dwRGBBitCount;          // how many bits per pixel
        uint32   dwYUVBitCount;          // how many bits per pixel
        uint32   dwZBufferBitDepth;      // how many total bits/pixel in z buffer (including any stencil bits)
        uint32   dwAlphaBitDepth;        // how many bits for alpha channels
        uint32   dwLuminanceBitCount;    // how many bits per pixel
        uint32   dwBumpBitCount;         // how many bits per "buxel", total
        uint32   dwPrivateFormatBitCount;// Bits per pixel of private driver formats. Only valid in texture
                                        // format list and if DDPF_D3DFORMAT is set
    };// DUMMYUNIONNAMEN(1);
    union
    {
        uint32   dwRBitMask;             // mask for red bit
        uint32   dwYBitMask;             // mask for Y bits
        uint32   dwStencilBitDepth;      // how many stencil bits (note: dwZBufferBitDepth-dwStencilBitDepth is total Z-only bits)
        uint32   dwLuminanceBitMask;     // mask for luminance bits
        uint32   dwBumpDuBitMask;        // mask for bump map U delta bits
        uint32   dwOperations;           // DDPF_D3DFORMAT Operations
    };// DUMMYUNIONNAMEN(2);
    union
    {
        uint32   dwGBitMask;             // mask for green bits
        uint32   dwUBitMask;             // mask for U bits
        uint32   dwZBitMask;             // mask for Z bits
        uint32   dwBumpDvBitMask;        // mask for bump map V delta bits
        struct
        {
            uint16    wFlipMSTypes;       // Multisample methods supported via flip for this D3DFORMAT
            uint16    wBltMSTypes;        // Multisample methods supported via blt for this D3DFORMAT
        } MultiSampleCaps;

    };// DUMMYUNIONNAMEN(3);
    union
    {
        uint32   dwBBitMask;             // mask for blue bits
        uint32   dwVBitMask;             // mask for V bits
        uint32   dwStencilBitMask;       // mask for stencil bits
        uint32   dwBumpLuminanceBitMask; // mask for luminance in bump map
    };// DUMMYUNIONNAMEN(4);
    union
    {
        uint32   dwRGBAlphaBitMask;      // mask for alpha channel
        uint32   dwYUVAlphaBitMask;      // mask for alpha channel
        uint32   dwLuminanceAlphaBitMask;// mask for alpha channel
        uint32   dwRGBZBitMask;          // mask for Z channel
        uint32   dwYUVZBitMask;          // mask for Z channel
    };// DUMMYUNIONNAMEN(5);
} DDPIXELFORMAT;

typedef struct _DDSCAPS2
{
    uint32       dwCaps;         // capabilities of surface wanted
    uint32       dwCaps2;
    uint32       dwCaps3;
    union
    {
        uint32       dwCaps4;
        uint32       dwVolumeDepth;
    }; //DUMMYUNIONNAMEN(1);
} DDSCAPS2;

struct DDSURFACEDESC2
{
	uint32 dwSize;
	uint32 dwFlags;
	uint32 dwHeight;
	uint32 dwWidth;
	uint32 dwPitchOrLinearSize;
	uint32 dwDepth;
	uint32 dwMipMapCount;
	uint32 dwReserved1[11];
	DDPIXELFORMAT ddpfPixelFormat;
	DDSCAPS2 ddsCaps;
	uint32 dwReserved2;
};

}	// DX

int ReadDDS(BitmapLoader* bitmapLoader, IO::Stream* stream)
{
	uint32 dwMagic;
	if (stream->Read(&dwMagic, 4) != 4)
		return -1;

	DX::DDSURFACEDESC2 header;
	if (stream->Read(&header, sizeof(header)) != sizeof(header))
		raise(Exception(L"End of file"));

	return 0;
};

struct TargaHeader
{
	uint8 idlen;
	uint8 colorType;
	uint8 imageType;
};

class TargaLoader
{
public:
	CTOR TargaLoader();

	int ReadHeader(IO::Stream* stream);
	int ReadBitmap(IO::Stream* stream, ubyte* scan0, int components, int stride);

	int GetXOrigin() const
	{
		return m_x;
	}

	int GetYOrigin() const
	{
		return m_y;
	}

	int GetWidth() const
	{
		return m_width;
	}

	int GetHeight() const
	{
		return m_height;
	}

protected:

	TargaHeader m_header;

	uint16 m_colorMapOrigin;
	uint16 m_colorMapLength;
	uint8 m_colorEntrySize;
	uint16 m_x;
	uint16 m_y;
	uint16 m_width;
	uint16 m_height;
	uint8 m_pixelSize;
	char* m_id;

	struct
	{
	unsigned char		nat : 4,
						reserved : 1,
						origin : 1,
						interleaving : 2;
	}
	m_flags;

	int m_pixel_size;
};

TargaLoader::TargaLoader()
{
}

int TargaLoader::ReadHeader(IO::Stream* stream)
{
	if (stream->Read(&m_header, sizeof(m_header)) != sizeof(m_header))
		raise(Exception(S(L"End of stream")));

	if (	(m_header.imageType != 0) &&
			(m_header.imageType != 1) &&
			(m_header.imageType != 2) &&
			(m_header.imageType != 3) &&
			(m_header.imageType != 9) &&
			(m_header.imageType != 10) &&
			(m_header.imageType != 11) &&
			(m_header.imageType != 32) &&
			(m_header.imageType != 33))
	{
		return -1;
	}

	m_colorMapOrigin = ReadInt16(stream);
	m_colorMapLength = ReadInt16(stream);
	m_colorEntrySize = ReadInt8(stream);

	m_x = ReadInt16(stream);
	m_y = ReadInt16(stream);
	m_width = ReadInt16(stream);
	m_height = ReadInt16(stream);
	m_pixelSize = ReadInt8(stream);

	if (stream->Read(&m_flags, 1) != 1)
		raise(Exception(L"End of stream"));

	m_id = new char[m_header.idlen];
	stream->Read(m_id, m_header.idlen);

	if (m_header.colorType > 1)
	{
		return -1;
	}

	m_pixel_size = m_pixelSize >> 3;
	
	if ((m_pixel_size < 1) || (m_pixel_size > 4))
	{
		return -1;
	}

	return 0;
}

int TargaLoader::ReadBitmap(IO::Stream* stream, ubyte* scan0, int components, int stride)
{
	if (m_header.imageType & 8)	// rle-compressed
	{
		ASSERT(0);
	}
	else	// Uncompressed
	{
		if (m_header.imageType == 2)	// True-color
		{
			ubyte* rowdata = new ubyte[m_width * m_pixel_size];

			for (int y = 0; y < m_height; y++)
			{
				stream->Read(rowdata, m_width * m_pixel_size);

				ubyte* src = rowdata;
				ubyte* dst = scan0 + stride * y;

				if (components == 3)
				{
					if (m_pixel_size == 3)
					{
						for (int x = 0; x < m_width; x++)
						{
							dst[0] = src[0];
							dst[1] = src[1];
							dst[2] = src[2];

							dst += 3;
							src += 3;
						}
					}
					else if (m_pixel_size == 4)
					{
						for (int x = 0; x < m_width; x++)
						{
							dst[0] = src[0];// * src[3] / 255;
							dst[1] = src[1];// * src[3] / 255;
							dst[2] = src[2];// * src[3] / 255;

							dst += 3;
							src += 4;
						}
					}
					else
						ASSERT(0);
				}
				else
					ASSERT(0);
			}

			delete[] rowdata;
		}
		else
		{
			ASSERT(0);
		}
	}

	return 0;
}

int ReadTGA(BitmapLoader* bitmapLoader, IO::Stream* stream)
{
	ASSERT(0);
	/*
	TargaLoader loader;
	int err = loader.ReadHeader(stream);
	if (err < 0) return err;

	bitmapLoader->m_bitmap = new LDraw::Bitmap(loader.GetWidth(), loader.GetHeight(), LDraw::PixelFormat_t::RGB_24);
	LDraw::BitmapData bitmapdata;
	bitmapLoader->m_bitmap->LockBits(&LDraw::RectI(0, 0, loader.GetWidth(), loader.GetHeight()), LDraw::ImageLockModeWrite, &bitmapdata);

	loader.ReadBitmap(stream, bitmapdata.Scan0, 3, bitmapdata.Stride);

	bitmapLoader->m_bitmap->UnlockBits(&bitmapdata);
	*/
	return 0;
}

#if 0
int PNGDecoder::Decode(IO::ISequentialByteStream* stream, void* data, int rowbytes)
{
	NGFormat* png = m_png;

//	bitmapLoader->m_bitmap = new LDraw::Bitmap(width, height, pixelformat);

//	m_pNode->m_pWindow->InvalidateRender();	// Rerender now that we've allocated the bitmap
//	m_pNode->m_pWindow->OnArrange(m_pNode->m_pWindow->get_ActualSize());	// Rerender now that we've allocated the bitmap

	png->GetImage()->StartScan();

	LDraw::BitmapData bitmapdata;
	{
		if (((PNGDecoder*)png)->m_ihdr.Interlace == 0)
		{
			for (int row = 0; row < height; row++)
			{
			//	m_pImage->m_lock.Lock();
			//	bitmapLoader->m_bitmap->LockBits(&LDraw::RectI(0, row, width, 1), LDraw::ImageLockModeWrite, &bitmapdata);

				uint8* dest = (uint*8)data + rowbytes*row;

				int hr = png->GetImage()->ReadScanline(dest);

			//	bitmapLoader->m_bitmap->UnlockBits(&bitmapdata);
				//m_pImage->m_lock.Unlock();

				if (hr < 0)
				{
					break;
				}

				/*
				if ((row & 7) == 0)
				{
					m_pNode->m_pWindow->RenderChanged();
				}
				*/
			}
		}
		else if (((PNGDecoder*)png)->m_ihdr.Interlace == 1)
		{
			for (int npass = 0; npass < 7; npass++)
			{
				int h;

				switch (npass)
				{
				case 0: h = (height+7)/8; break;
				case 1: h = (height+7)/8; break;
				case 2: h = (height+3)/8; break;
				case 3: h = (height+3)/4; break;
				case 4: h = (height+1)/4; break;
				case 5: h = (height+1)/2; break;
				case 6: h = (height)/2; break;
				}

				for (int row = 0; row < h; row++)
				{
					if (npass == 6)
					{
					//	m_pImage->m_lock.Lock();
					//	bitmapLoader->m_bitmap->LockBits(&LDraw::RectI(0, (1+row*2), width, 1), LDraw::ImageLockModeWrite, &bitmapdata);

					//	uint8* dest = bitmapdata.Scan0;// + bitmapdata.Stride*(1+row*2);
						uint8* dest = (uint*8)data + rowbytes*row;

						int hr = png->GetImage()->ReadScanline(dest);
					//	bitmapLoader->m_bitmap->UnlockBits(&bitmapdata);
					//	m_pImage->m_lock.Unlock();

						if (hr < 0)
							break;
					}
					else
					{
						if (((PNGDecoder*)png)->m_image->m_imageStream->DecodeScanline() < 0)
							break;
					}

					/*
					if ((row & 7) == 0)
					{
						m_pNode->m_pWindow->RenderChanged();
					}
					*/
				}
			//	m_pNode->m_pWindow->RenderChanged();

				((PNGDecoder*)png)->NextPass();
			}
		}
	}

//	m_pNode->m_pWindow->RenderChanged();
}

int ReadPNGHeader(IO::ISequentialByteStream* stream, ImageHeader* header)
{
	ULONG nRead;

	uint32 format;
	nRead = stream->Read(&format, 4);
	if (nRead != 4)
	{
	//	nRead = stream->Read(&format, 4);
		return -1;
	}

	if (format != 0x474e5089/*PNG*/ && format != 0x474e4a8b/*JNG*/)
	{
		return -1;
	}

	uint32 value;
	nRead = stream->Read(&value, 4);
	if (nRead != 4)
	{
		return -1;
	}
	if (value != 0x0a1a0a0d)
	{
		return -1;
	}

	NGFormat* png;

	if (format == 0x474e5089)
	{
		png = new PNGDecoder;
	}
	else
	{
		png = new JNGDecoder;
	}

	png->SetStream(stream);
	png->ReadHeader();
	png->CreateImage();

	LDraw::PixelFormat_t pixelformat;
	if (png->GetColorType() == 6)	// rgb+alpha
	{
		pixelformat = LDraw::PixelFormat_t::RGBAP_32;
	}
	else if (png->GetColorType() == 4)	// grayscale+alpha
	{
		pixelformat = LDraw::PixelFormat_t::RGBAP_32;
	}
	else
	{
		//vih->vih->bmiHeader.biBitCount = 24;//m_pFilter->m_pInputPin->m_bitcount;
		pixelformat = LDraw::PixelFormat_t::RGB_24;
	}

	header->width = png->GetWidth();
	header->height = png->GetHeight();
	header->decoder = new PNGBitmapFormatDecoder(png);

	return 0;
}

int ReadGIFHeader(IO::ISequentialByteStream* stream, ImageHeader* header)
{
	Imaging::GIFFormat* gifFile = new Imaging::GIFFormat;
	int hr = gifFile->Open(stream);
	if (hr < 0)
		return -1;

	gifFile->AddNextFrame();

	int width = gifFile->GetWidth();
	int height = gifFile->GetHeight();

	return 0;
}

void ReadGIF()
{
	bitmapLoader->m_bitmap = new LDraw::Bitmap(width, height, LDraw::PixelFormat_t::RGB_24);

//	m_pNode->m_pWindow->InvalidateRender();	// Rerender now that we've allocated the bitmap

	gifFile->StartScan(gifFile->m_frames[0]);

	LDraw::BitmapData bitmapdata;
//	m_pImage->m_bitmap.LockBits(&LDraw::RectI(0, 0, width, height), LDraw::ImageLockModeWrite, &bitmapdata);
	{
		for (int y = 0; y < height; y++)
		{
		//	m_pImage->m_lock.Lock();
			bitmapLoader->m_bitmap->LockBits(&LDraw::RectI(0, y, width, 1), LDraw::ImageLockModeWrite, &bitmapdata);
			uint8* dest = bitmapdata.Scan0;// + bitmapdata.Stride*y;

			int hr = gifFile->ReadScanline(gifFile->m_frames[0], dest, 24);

			bitmapLoader->m_bitmap->UnlockBits(&bitmapdata);
		//	m_pImage->m_lock.Unlock();

			if (hr < 0)
				break;

		//	m_pNode->m_pWindow->RenderChanged();
		}
	}

//	m_pImage->m_bitmap.UnlockBits(&bitmapdata);

//	m_pNode->m_pWindow->RenderChanged();

	return 0;
}
#endif

// static
ClassType* BitmapLoader::DetermineDecoder(IO::Stream* stream)
{
	uint64 startpos = stream->position;

	int n;
	for (n = 0; n < 3; ++n)
	{
		int ret = -1;

		switch (n)
		{
		case 0:
			{
				ASSERT(sizeof(JPEGDecoder) == typeid(JPEGDecoder).GetType()->get_sizeof());

				ret = JPEGDecoder::ReadMarker(stream);
				if (ret == 0)
				{
					return typeid(JPEGDecoder)->AsClass();
				}
			}
			break;

			/*
		case 1:
			{
				ret = ReadGIFHeader(&header, stream);
			}
			break;
			*/

		case 2:
			{
				ASSERT(sizeof(PNGDecoder) == typeid(PNGDecoder).GetType()->get_sizeof());

				if (PNGDecoder::ReadMarker(stream) == 0)
				{
					return typeid(PNGDecoder)->AsClass();
				}
			//	ret = ReadPNGHeader(&header, stream);
			}
			break;

		case 1:
			{
				ASSERT(sizeof(BMPDecoder) == typeid(BMPDecoder).GetType()->get_sizeof());

				if (BMPDecoder::ReadMarker(stream) == 0)
				{
					return typeid(BMPDecoder)->AsClass();
				}
			}
			break;

			/*
		case 4:
			{
				ret = ReadTGA(&header, stream);
			}
			break;
			*/
		}

		if (ret == 0)
			break;

		stream->position = startpos;
	}

	return nullptr;
}

BitmapLoader::BitmapLoader()
{
}

BitmapLoader::~BitmapLoader()
{
}

void* BitmapLoader::Load(StringIn filename, BitmapFormat* bitmapFormat, void* (*allocmem)(size_t size), void (*freemem)(void* pv))
{
	IO::FileStream stream(filename, IO::FileMode_Open, IO::FileAccess_Read);

	if (IO::FileStream::GetLastError() == 0)
	{
		return Load(&stream, bitmapFormat, allocmem);
	}

	return nullptr;
}

template<class T> class _Ptr
{
public:

	CTOR _Ptr(ClassType* pClass)
	{
		m_dtor = pClass->GetDestructor();
		if (m_dtor)
		{
			if (m_dtor->get_Access() < AccessSpec_Public)
			{
				raise(Exception("destructor is" + toString(m_dtor->get_Access())));
			}
		}
	}

	~_Ptr()
	{
		if (m_dtor)
		{
			m_dtor->void_invoke_method(m_p);
		}
	}

	_Ptr& operator = (T* p)
	{
		m_p = p;
		return *this;
	}

	operator T* () const
	{
		return m_p;
	}

	T* m_p;
	Declarator* m_dtor;
};

void* BitmapLoader::Load(IO::Stream* stream, BitmapFormat* bitmapFormat, void* (*allocmem)(size_t size), void (*freemem)(void* pv))
{
	ClassType* pDecoderClass = DetermineDecoder(stream);
	if (pDecoderClass == nullptr)
	{
		raise(Exception("not a recognized bitmap format"));
	}

	_Ptr<Object> decoderObject(pDecoderClass);

	decoderObject = newobj(pDecoderClass);
	IBitmapFormatDecoder* pDecoder = dynamic_cast<IBitmapFormatDecoder*>(decoderObject.m_p);
	if (pDecoder == nullptr)
	{
		raise(Exception(L"not a bitmap decoder"));
	}

	//BitmapFormat bitmapFormat;
	pDecoder->ReadHeader(stream, bitmapFormat);
	if (!(bitmapFormat->width > 0 && bitmapFormat->height > 0))
	{
		raise(Exception(L"Invalid bitmap size"));
	}

	uint pixelbytes = 4;
	uint rowPitch = alignment4(bitmapFormat->width*pixelbytes);
	byte* data = (byte*)allocmem(rowPitch * bitmapFormat->height);

	pDecoder->ReadBitmap(stream, data, rowPitch);

	/*
	Declarator* dtor = pDecoderClass->GetDestructor(AccessSpec_Public);
	if (dtor)
	{
		dtor->void_invoke_method(decoderObject);
	}
	*/

	return data;
}

}	// Imaging
}	// System
