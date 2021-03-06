#include "stdafx.h"
#include "PNG.h"
#include "PNGFormat.h"

namespace System
{
using namespace IO;

namespace Imaging
{

NGFormat::DATStream::DATStream(uint32 chunkID, IO::Stream* stream) : m_stream(stream)
{
	m_chunkID = chunkID;

	m_idata_offset = 0;
	m_idata_length = 0;

	m_pos = 0;
}

size_t NGFormat::DATStream::Read(void* pv, size_t cb)
{
	uint8* p = (uint8*)pv;

	ULONG i;
	for (i = 0; i < cb; ++i)
	{
		if (m_idata_offset == m_idata_length)
		{
			if (!NextDAT())
				break;
		}

		if (m_idata_offset < m_idata_length)
		{
			m_pos++;
			m_idata_offset++;
			size_t nRead = m_stream->Read(p, 1);
			p++;

			if (m_idata_offset == m_idata_length)
			{
				uint32 crc;
				nRead = m_stream->Read(&crc, 4);
				crc = BigEndian32(crc);
			}
		}
	}

	return i;
}

uint64 NGFormat::DATStream::Seek(int64 offset, IO::SeekOrigin origin)
{
	if (origin == IO::STREAM_SEEK_CUR)
	{
		if (offset < 0)	// Can only seek forward
			THROW(-1);

		for (LONGLONG i = 0; i < offset; i++)
		{
			uint8 b;
			Read(&b, 1);
		}
	}
	else if (origin == IO::STREAM_SEEK_SET)
	{
		if (offset < m_pos)	// Can only seek forward
			THROW(-1);

		for (LONGLONG i = m_pos; i < offset; i++)
		{
			uint8 b;
			Read(&b, 1);
		}
	}
	else
		THROW(-1);

	return m_pos;
}

uint64 NGFormat::DATStream::GetSize()
{
	// TODO
	THROW(-1);
	return -1;
}

/*
ISequentialByteStream* NGFormat::DATStream::Clone() const
{
	THROW(-1);
	return NULL;
}
*/

size_t NGFormat::DATStream::Write(const void* pv, size_t cb)
{
	raise(SystemException("Not implemented"));
	return 0;
}

bool NGFormat::DATStream::NextDAT()
{
	size_t nRead;

	while (1)
	{
		uint32 length;
		nRead = m_stream->Read(&length, 4);
		if (nRead != 4) raise(IOException("read error"));
		length = BigEndian32(length);

		uint32 type;
		nRead = m_stream->Read(&type, 4);
		type = BigEndian32(type);
//		TRACE("%4.4s %X\n", &type, type);

		uint64 ppos = m_stream->GetPosition();//Seek(0, IO::STREAM_SEEK_CUR);

		if (type == m_chunkID)
		{
			m_idata_length = length;
			m_idata_offset = 0;
			return true;
		}

		m_stream->Seek(ppos+length, System::IO::STREAM_SEEK_SET);

		uint32 crc;
		nRead = m_stream->Read(&crc, 4);
		crc = BigEndian32(crc);

		if (type == CK_IEND)
		{
			break;
		}
	}

	return false;
}

NGFormat::PNGImage::PNGImage(const IHDR* ihdr, PaletteEntry* paletteEntry, IO::Stream* stream)
{
	DATStream* datstream = new DATStream(CK_IDAT, stream);

	m_imageStream = new PNGImageStream(datstream);
	m_imageStream->m_width = ihdr->Width;
	m_imageStream->m_colorType = ihdr->ColorType;
	m_imageStream->m_filter = ihdr->Filter;
	m_imageStream->m_interlace = ihdr->Interlace;

	m_imageStream->m_npass = 0;

	m_paletteEntry = paletteEntry;

//	m_imageStream->m_passwidth = (m_imageStream->m_width+7)/8;
}

NGFormat::JPEGAImage::JPEGAImage(const JHDR* jhdr, IO::Stream* stream)
{
	if (jhdr == NULL) raise(ArgumentException());

	m_hdr = *jhdr;

	m_jpegStream = new JPEGImageStream(new DATStream(CK_JDAT, stream->Clone()));

	if (jhdr->Color_type == 8 || jhdr->Color_type == 10)
	{
		m_alphaStream = NULL;

		if (jhdr->Alpha_sample_depth != 0)
			raise(Exception("not supported"));

		if (jhdr->Alpha_compression_method != 0)
			raise(Exception("not supported"));

		if (jhdr->Alpha_filter_method != 0)
			raise(Exception("not supported"));

		if (jhdr->Alpha_interlace_method != 0)
			raise(Exception("not suppported"));
	}
	else if (jhdr->Color_type == 12 || jhdr->Color_type == 14)
	{
		if (jhdr->Alpha_compression_method == 0)
		{
			if (!(
				jhdr->Alpha_sample_depth == 1 ||
				jhdr->Alpha_sample_depth == 2 ||
				jhdr->Alpha_sample_depth == 4 ||
				jhdr->Alpha_sample_depth == 8 ||
				jhdr->Alpha_sample_depth == 16))
			{
				raise(Exception("not supported"));
			}

			m_alphaStream = new PNGImageStream(new DATStream(CK_IDAT, stream->Clone()));
			m_alphaStream->m_width = jhdr->Width;
			m_alphaStream->m_colorType = 0;	// grayscale
			m_alphaStream->m_filter = jhdr->Alpha_filter_method;

			m_alphaStream->Start();
		}
		else if (jhdr->Alpha_compression_method == 8)
		{
			if (jhdr->Alpha_sample_depth != 8)
				raise(Exception("not supported"));

			ASSERT(0);
		}
		else
			raise(Exception("Alpha_compression_method not supported"));
	}
	else
		raise(Exception("not supported"));
}

NGFormat::PNGImageStream::PNGImageStream(DATStream* stream)
{
	m_decoder = new ZLIBDecoder(stream);
}

NGFormat::PNGImageStream::~PNGImageStream()
{
	delete [] m_scanlineBuffer[0];
	delete [] m_scanlineBuffer[1];
}

void NGFormat::PNGImageStream::Start()
{
//	m_decoder->start();
//	m_decoder->start_block();

	if (m_colorType == 0)	// grayscale
	{
		m_pixelBytes = 1;
	}
	else if (m_colorType == 2)	// rgb
	{
		m_pixelBytes = 3;
	}
	else if (m_colorType == 3)	// indexed
	{
		m_pixelBytes = 1;
	}
	else if (m_colorType == 4)	// grayscale with alpha
	{
		m_pixelBytes = 2;
	}
	else if (m_colorType == 6)	// rgb with alpha
	{
		m_pixelBytes = 4;
	}
	else
		ASSERT(0);

	if (m_interlace == 1)
	{
		switch (m_npass)
		{
		case 0: m_passwidth = (m_width+7)/8; break;
		case 1: m_passwidth = (m_width+5)/8; break;
		case 2: m_passwidth = (m_width+3)/4; break;
		case 3: m_passwidth = (m_width+1)/4; break;
		case 4: m_passwidth = (m_width+1)/2; break;
		case 5: m_passwidth = (m_width)/2; break;
		case 6: m_passwidth = m_width; break;
		}
	}
	else
	{
		m_passwidth = m_width;
	}

	m_scanlineBytes = m_pixelBytes * m_passwidth;

	// We use two buffers for two scanlines, and we reserve a pixel to the left of the start of the scanline
	m_scanlineBuffer[0] = new uint8[m_scanlineBytes+m_pixelBytes];
	m_scanlineBuffer[1] = new uint8[m_scanlineBytes+m_pixelBytes];

	std::memset(m_scanlineBuffer[1], 0, m_scanlineBytes+m_pixelBytes);// clear the 'previous' scanline to zero
	std::memset(m_scanlineBuffer[0], 0, m_pixelBytes);	// clear the pixel before the start of the scanline to zero

	m_scanlinePtr[0] = m_scanlineBuffer[0]+m_pixelBytes;
	m_scanlinePtr[1] = m_scanlineBuffer[1]+m_pixelBytes;

	m_curscan = 0;
}

int NGFormat::PNGImageStream::DecodeScanline()
{
	// filter byte
	uint8 filterType;
	if (m_decoder->Read(&filterType, 1) != 1)
		return -1;

	if (m_decoder->Read(m_scanlinePtr[m_curscan], m_scanlineBytes) != m_scanlineBytes)
		return -1;

	switch (filterType)
	{
	case 0:	// None
		// Do nothing
		break;

	case 1:	// Sub
		{
			uint8* curx = m_scanlinePtr[m_curscan];
			uint8* recona = curx - m_pixelBytes;
			unsigned int x;
			switch (m_pixelBytes)
			{
			case 4:
				for (x = 0; x < m_width; ++x)
				{
					*curx++ += *recona++;
					*curx++ += *recona++;
					*curx++ += *recona++;
					*curx++ += *recona++;
				}
				break;

			case 3:
				for (x = 0; x < m_width; ++x)
				{
					*curx++ += *recona++;
					*curx++ += *recona++;
					*curx++ += *recona++;
				}
				break;

			case 2:
				for (x = 0; x < m_width; ++x)
				{
					*curx++ += *recona++;
					*curx++ += *recona++;
				}
				break;

			case 1:
				for (x = 0; x < m_width; ++x)
				{
					*curx++ += *recona++;
				}
				break;
			}
		}
		break;

	case 2:	// Up
		{
			uint8* curx = m_scanlinePtr[m_curscan];
			uint8* reconb = m_scanlinePtr[!m_curscan];
			unsigned int x;
			switch (m_pixelBytes)
			{
			case 4:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += *reconb++;
					*curx++ += *reconb++;
					*curx++ += *reconb++;
					*curx++ += *reconb++;
				}
				break;

			case 3:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += *reconb++;
					*curx++ += *reconb++;
					*curx++ += *reconb++;
				}
				break;

			case 2:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += *reconb++;
					*curx++ += *reconb++;
				}
				break;

			case 1:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += *reconb++;
				}
				break;
			}
		}
		break;

	case 3:	// Average
		{
			uint8* curx = m_scanlinePtr[m_curscan];
			uint8* recona = curx - m_pixelBytes;
			uint8* reconb = m_scanlinePtr[!m_curscan];
			unsigned int x;
			switch (m_pixelBytes)
			{
			case 4:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
				}
				break;

			case 3:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
				}
				break;

			case 2:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
				}
				break;

			case 1:
				for (x = 0; x < m_width; x++)
				{
					*curx++ += ((int)*recona++ + (int)*reconb++) / 2;
				}
				break;
			}
		}
		break;

	case 4:	// Paeth
		{
			uint8* curx = m_scanlinePtr[m_curscan];
			uint8* recona = curx - m_pixelBytes;
			uint8* reconb = m_scanlinePtr[!m_curscan];
			uint8* reconc = reconb - m_pixelBytes;
			for (unsigned int x = 0; x < m_width*m_pixelBytes; ++x)
			{
				*curx++ += PaethPredictor(*recona++, *reconb++, *reconc++);
			}
		}
		break;

	default:
		return -1;
	}

	m_curscan = !m_curscan;	// swap between the two scanline buffers

	return 0;
}

int NGFormat::PNGImage::ReadScanline(uint8* dest)
{
	int iret = m_imageStream->DecodeScanline();
	if (iret < 0)
		return iret;

	const uint8* src = m_imageStream->GetScanlinePtr();

	if (m_imageStream->m_colorType == 0)	// grayscale
	{
		for (unsigned int x = 0; x < m_imageStream->m_passwidth; ++x)
		{
			*dest++ = *src;
			*dest++ = *src;
			*dest++ = *src;
			src++;
		}
	}
	else if (m_imageStream->m_colorType == 2)	// rgb
	{
		for (unsigned int x = 0; x < m_imageStream->m_passwidth; ++x)
		{
			dest[0] = src[2];
			dest[1] = src[1];
			dest[2] = src[0];

			dest += 3;
			src += 3;
		}
	}
	else if (m_imageStream->m_colorType == 3)	// indexed color
	{
		if (m_paletteEntry)
		{
			for (unsigned int x = 0; x < m_imageStream->m_passwidth; ++x)
			{
				dest[0] = m_paletteEntry[*src].b;
				dest[1] = m_paletteEntry[*src].g;
				dest[2] = m_paletteEntry[*src].r;

				dest += 3;
				src += 1;
			}
		}
	}
	else if (m_imageStream->m_colorType == 4)	// grayscale with alpha
	{
		for (unsigned int x = 0; x < m_imageStream->m_passwidth; ++x)
		{
			// premultiply
			uint8 gray = src[0] * (int)src[1] / 255;
			dest[0] = gray;
			dest[1] = gray;
			dest[2] = gray;
			dest[3] = src[1];
			dest += 4;
			src += 2;
		}
	}
	else if (m_imageStream->m_colorType == 6)	// rgb+alpha
	{
		for (uint x = 0; x < m_imageStream->m_passwidth; ++x)
		{
			// premultiply
			dest[0] = src[2] * (int)src[3] / 255;
			dest[1] = src[1] * (int)src[3] / 255;
			dest[2] = src[0] * (int)src[3] / 255;
			dest[3] = src[3];
			dest += 4;
			src += 4;
		}
	}
	else
		ASSERT(0);

	return 0;
}

void NGFormat::JPEGAImage::StartScan()
{
	m_jpegStream->Start();
}

int NGFormat::JPEGAImage::ReadScanline(uint8* dest)
{
	int iret = m_jpegStream->DecodeScanline();
	if (iret < 0)
		return iret;

	const uint8* image = m_jpegStream->GetScanlinePtr();

	if (m_alphaStream)
	{
		m_alphaStream->DecodeScanline();

		const uint8* alpha = m_alphaStream->GetScanlinePtr();

		for (int i = 0; i < m_hdr.Width; i++)
		{
			// premultiply
			dest[0] = image[2] * (int)*alpha / 255;
			dest[1] = image[1] * (int)*alpha / 255;
			dest[2] = image[0] * (int)*alpha / 255;
			dest[3] = *alpha;
			dest += 4;
			image += 3;
			alpha += 1;
		}
	}
	else
	{
		memcpy(dest, image, m_hdr.Width*3);
	}

	return 0;
}

NGFormat::JPEGImageStream::JPEGImageStream(DATStream* stream)
{
	if (stream == NULL) raise(ArgumentNullException());

	if (JPEGDecoder::ReadMarker(stream) != 0)
	{
		raise(Exception(WSTR("Expected JFIF marker not found")));
	}

	m_stream = stream;
	m_jpeg = new JPEGDecoder();

	BitmapFormat bmformat;
	m_jpeg->ReadHeader(stream, &bmformat);
}

int NGFormat::JPEGImageStream::DecodeScanline()
{
	m_jpeg->NextScanline();
	return 0;
}

NGFormat::NGFormat()
{
	m_stream = NULL;
}

void NGFormat::SetStream(IO::Stream* stream)
{
	if (m_stream != NULL) raise(Exception("Can only set stream once"));	// Can only set once
	if (stream == NULL) raise(ArgumentNullException());

	m_stream = stream;
}

PNGDecoder::PNGDecoder() :
	m_paletteEntry(NULL),
	m_image(NULL)
{
}

PNGDecoder::~PNGDecoder()
{
	delete m_image;
}

int PNGDecoder::ReadMarker(IO::Stream* stream)
{
	uint32 format;
	if (stream->Read(&format, 4) != 4) return -1;
	if (format != 0x474e5089/*PNG*/)
	{
		return -1;
	}

	uint32 value;
	if (stream->Read(&value, 4) != 4) return -1;
	if (value != 0x0a1a0a0d) return -1;

	return 0;
}

void PNGDecoder::ReadSignature()
{
	ReadMarker(m_stream);

#if 0
	uint32 format;
	if (m_stream.Read(&format, 4) != 4) THROW(-1);
	if (format != 0x474e5089/*PNG*/)
	{
		THROW(-1);
	}

	uint32 value;
	if (m_stream.Read(&value, 4) != 4) THROW(-1);
	if (value != 0x0a1a0a0d) THROW(-1);
#endif
}

JNGDecoder::JNGDecoder()
{
	m_image = NULL;
}

JNGDecoder::~JNGDecoder()
{
	delete m_image;
}

void JNGDecoder::ReadSignature()
{
	uint32 format;
	if (m_stream->Read(&format, 4) != 4) THROW(-1);
	if (format != 0x474e4a8b/*JNG*/)
	{
		raise(Exception("Not JNG"));
	}

	uint32 value;
	if (m_stream->Read(&value, 4) != 4) raise(Exception("Read error"));
	if (value != 0x0a1a0a0d) raise(Exception("Not JNG"));
}

int PNGDecoder::ReadHeader(IO::Stream* stream, BitmapFormat* bitmapFormat)
{
	m_stream = stream;

	ReadHeader();

	bitmapFormat->width = m_ihdr.Width;
	bitmapFormat->height = m_ihdr.Height;
	switch (GetColorType())
	{
	case 0:	// grayscale
		bitmapFormat->pixelFormat = PixelFormat_R8_UNORM;
		break;

	case 2:	// rgb
		bitmapFormat->pixelFormat = PixelFormat_R8G8B8A8_UNORM;
		break;

	case 4:	// grayscale+alpha
		// TODO ??
	//	bitmapFormat->pixelFormat = PixelFormat_R8G8_UNORM;
		bitmapFormat->pixelFormat = PixelFormat_R8G8B8A8_UNORM;
		break;

	case 6:	// rgb+alpha
		bitmapFormat->pixelFormat = PixelFormat_R8G8B8A8_UNORM;
		break;

	default:
		raise(Exception("Unknown colortype"));	// TODO
	}

	return 0;
}

int PNGDecoder::ReadBitmap(IO::Stream* stream, void* data, int rowbytes)
{
	CreateImage();

	GetImage()->StartScan();

	int height = m_ihdr.Height;

	if (m_ihdr.Interlace == 0)
	{
		for (int row = 0; row < height; ++row)
		{
		//	m_pImage->m_lock.Lock();
		//	bitmapLoader->m_bitmap->LockBits(&LDraw::RectI(0, row, width, 1), LDraw::ImageLockModeWrite, &bitmapdata);

			uint8* dest = (uint8*)data + rowbytes*row;

			int hr = GetImage()->ReadScanline(dest);

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
	else if (m_ihdr.Interlace == 1)
	{
		for (int npass = 0; npass < 7; ++npass)
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

			for (int row = 0; row < h; ++row)
			{
				if (npass == 6)
				{
				//	m_pImage->m_lock.Lock();
				//	bitmapLoader->m_bitmap->LockBits(&LDraw::RectI(0, (1+row*2), width, 1), LDraw::ImageLockModeWrite, &bitmapdata);

				//	uint8* dest = bitmapdata.Scan0;// + bitmapdata.Stride*(1+row*2);
					uint8* dest = (uint8*)data + rowbytes*row;

					int hr = GetImage()->ReadScanline(dest);
				//	bitmapLoader->m_bitmap->UnlockBits(&bitmapdata);
				//	m_pImage->m_lock.Unlock();

					if (hr < 0)
						break;
				}
				else
				{
					if (m_image->m_imageStream->DecodeScanline() < 0)
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

			NextPass();
		}
	}

	return 0;
}

ErrorCode PNGDecoder::ReadHeader()
{
	size_t nRead;

	while (1)
	{
		uint32 length;
		nRead = m_stream->Read(&length, 4);
		if (nRead != 4) return Error;
		length = BigEndian32(length);

		uint32 type;
		nRead = m_stream->Read(&type, 4);
//		TRACE("%4.4s %X\n", &type, type);
		type = BigEndian32(type);

		int64 ppos = m_stream->Seek(0, IO::STREAM_SEEK_CUR);

		switch (type)
		{
		case CK_IHDR:
			{
				if (length != 13/*sizeof(m_ihdr)*/) return Error;
				nRead = m_stream->Read(&m_ihdr, length);
				if (nRead != length) return Error;

				m_ihdr.Width = BigEndian32(m_ihdr.Width);
				m_ihdr.Height = BigEndian32(m_ihdr.Height);

				if (m_ihdr.Width <= 0 || m_ihdr.Height <= 0)
				{
					return Error;
				}

				if (m_ihdr.Filter != 0)
				{
					return Error;
				}
			}
			break;

			//
		case CK_PLTE:
			{
				m_paletteEntry = new PaletteEntry[256];
				nRead = m_stream->Read(m_paletteEntry, length);
				if (nRead != length) return Error;
			}
			break;
		}

		m_stream->Seek(ppos+length, System::IO::STREAM_SEEK_SET);

		uint32 crc;
		nRead = m_stream->Read(&crc, 4);
		crc = BigEndian32(crc);

		if (type == CK_IHDR)
		{
			if (m_ihdr.ColorType != 3)	// indexed
			{
				break;
			}
		}

		if (type == CK_PLTE)
		{
			break;
		}

		if (type == CK_IEND)
		{
			break;
		}
	}

	return Success;
}

void PNGDecoder::NextPass()
{
	++m_image->m_imageStream->m_npass;
	m_image->m_imageStream->Start();
}

void PNGDecoder::CreateImage()
{
	m_image = new PNGImage(&m_ihdr, m_paletteEntry, m_stream);
}

uint8 JNGDecoder::GetColorType() const
{
	if (m_image->m_alphaStream)
		return 4;	// rgb + alpha
	else
		return 2;	// rgb
}

ErrorCode JNGDecoder::ReadHeader()
{
	size_t nRead;

	while (1)
	{
		uint32 length;
		nRead = m_stream->Read(&length, 4);
		if (nRead != 4) return Error;
		length = BigEndian32(length);

		uint32 type;
		nRead = m_stream->Read(&type, 4);
//		TRACE("%4.4s %X\n", &type, type);
		type = BigEndian32(type);

		int64 ppos = m_stream->Seek(0, IO::STREAM_SEEK_CUR);

		switch (type)
		{
		case CK_JHDR:
			{
				if (length != sizeof(m_jhdr)) return Error;
				nRead = m_stream->Read(&m_jhdr, length);
				if (nRead != length) return Error;

				m_jhdr.Width = BigEndian32(m_jhdr.Width);
				m_jhdr.Height = BigEndian32(m_jhdr.Height);

			//	DATStream* datstream = new DATStream(CK_JDAT, m_stream);
				/*
				PNGCompressedImage* compressed = new PNGCompressedImage(datstream);
				compressed->m_width = m_ihdr.Width;
				compressed->m_colorType = m_ihdr.ColorType;
				compressed->m_filterType = m_ihdr.FilterType;
				*/
			}
			break;
		}

		m_stream->Seek(ppos+length, System::IO::STREAM_SEEK_SET);

		uint32 crc;
		nRead = m_stream->Read(&crc, 4);
		crc = BigEndian32(crc);

		if (type == CK_JHDR)
		{
			break;
		}

		if (type == CK_IEND)
		{
			break;
		}
	}

	return 0;
}

void JNGDecoder::CreateImage()
{
	if (m_image) THROW(-1);
	m_image = new JPEGAImage(&m_jhdr, m_stream);
}

}	// Imaging
}
