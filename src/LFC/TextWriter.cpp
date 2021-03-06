#include "stdafx.h"
#include "LFC.h"

namespace System
{
namespace IO
{

NullTextWriter NullTextWriter::writer;

ImmutableString<char> true_str("true");
ImmutableString<char> false_str("false");

///////////

TextWriter::TextWriter() :
	m_base(10)
//	m_true(&true_str), m_false(&false_str)
{
}

void TextWriter::setbase(int base)
{
	m_base = base;
}

void TextWriter::Write(bool bit)
{
}

void TextWriter::Write(char ch)
{
}

void TextWriter::Write(wchar_t ch)
{
}

void TextWriter::Write(signed char number)
{
	char buffer[8];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(unsigned char number)
{
	char buffer[8];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(short number)
{
	char buffer[8];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(unsigned short number)
{
	char buffer[8];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(int number)
{
	char buffer[16];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(unsigned int number)
{
	char buffer[16];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(long number)
{
	char buffer[24];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(unsigned long number)
{
	char buffer[24];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(int64 number)
{
	char buffer[24];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(uint64 number)
{
	char buffer[24];
	WriteNumberAndNull(buffer, number);
	WriteCStr(buffer);
}

void TextWriter::Write(float number)
{
}

void TextWriter::Write(double number)
{
}

size_t TextWriter::WriteChars(const char32_t* p, size_t len)
{
	ASSERT(0);
	return 0;
}

TextWriter& operator << (TextWriter& stream, bool bit)
{
	stream << (bit? stream.m_true: stream.m_false);
	return stream;
}

TextWriter& operator << (TextWriter& stream, char ch)
{
	stream.WriteChars(&ch, 1);
	return stream;
}

TextWriter& operator << (TextWriter& stream, wchar_t ch)
{
	stream.WriteChars(&ch, 1);
	return stream;
}

TextWriter& operator << (TextWriter& stream, signed char number)
{
	char buffer[8];
	WriteNumberAndNull(buffer, number);
	stream.WriteCStr(buffer);

	return stream;
}

TextWriter& operator << (TextWriter& writer, unsigned char number)
{
	char buffer[8];
	WriteNumberAndNull(buffer, number);
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, short number)
{
	char buffer[16];
	WriteNumberAndNull(buffer, number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, unsigned short number)
{
	char buffer[16];
	WriteNumberAndNull(buffer, number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, int number)
{
	char buffer[16];
	WriteNumberAndNull(buffer, number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, unsigned int number)
{
	char buffer[16];
	WriteNumberAndNull(buffer, number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, long number)
{
	char buffer[32];
	WriteNumberAndNull(buffer, (int)number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, unsigned long number)
{
	char buffer[32];
	WriteNumberAndNull(buffer, (unsigned int)number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, int64 number)
{
	char buffer[32];
	WriteNumberAndNull(buffer, number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, uint64 number)
{
	char buffer[32];
	WriteNumberAndNull(buffer, number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, float number)
{
	char buffer[32];
	sprintf_s(buffer, "%f", number, writer.getbase());
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, double number)
{
	char buffer[32];
	sprintf_s(buffer, "%f", number);
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, long double number)
{
	char buffer[32];
	sprintf_s(buffer, "%f", number);
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, void* p)
{
	char buffer[32];
	WriteNumberAndNull(buffer, (int)p, 16);

	writer << "0x";
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, const void* p)
{
	char buffer[32];
	WriteNumberAndNull(buffer, (int)p, 16);

	writer << "0x";
	writer.WriteCStr(buffer);

	return writer;
}

TextWriter& operator << (TextWriter& writer, char* cstr)
{
	writer.WriteCStr(cstr);
	return writer;
}

TextWriter& operator << (TextWriter& writer, const char* cstr)
{
	writer.WriteCStr(cstr);
	return writer;
}

TextWriter& operator << (TextWriter& writer, wchar_t* cstr)
{
	writer.WriteCStr(cstr);
	return writer;
}

TextWriter& operator << (TextWriter& writer, const wchar_t* cstr)
{
	writer.WriteCStr(cstr);
	return writer;
}

}	// IO
}	// System
