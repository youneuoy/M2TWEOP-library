#include "tAutodeskMemoryStream.h"

FbxStream::EState tAutodeskMemoryStream::GetState()
{
   return _state;
}

bool tAutodeskMemoryStream::Open(void* pStreamData)
{
   _state = EState::eOpen;
   _position = 0;
   return true;
}

bool tAutodeskMemoryStream::Close()
{
   _state = EState::eClosed;
   _position = 0;
   return true;
}

bool tAutodeskMemoryStream::Flush()
{
   return true;
}

int tAutodeskMemoryStream::Write(const void*, int)
{
   _errorCode = 1;
   return 0;
}

int tAutodeskMemoryStream::Read(void* buffer, int count) const
{
   long remain = _length - _position;
   if (count > remain)
   {
      const_cast<tAutodeskMemoryStream&>(*this)._errorCode = 1;
      return 0;
   }
   memcpy_s(buffer, count, _stream + _position, count);
   const_cast<tAutodeskMemoryStream&>(*this)._position += count;
   return count;
}

int tAutodeskMemoryStream::GetReaderID() const
{
   return _readerId;
}

int tAutodeskMemoryStream::GetWriterID() const
{
   return -1;
}

void tAutodeskMemoryStream::Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos)
{
   long offset = static_cast<long>(pOffset);
   switch (pSeekPos)
   {
   case FbxFile::ESeekPos::eCurrent:
      _position += offset;
      break;
   case FbxFile::ESeekPos::eEnd:
      _position = _length - offset;
      break;
   default:
      _position = offset;
      break;
   }
}

long tAutodeskMemoryStream::GetPosition() const
{
   return _position;
}

void tAutodeskMemoryStream::SetPosition(long pPosition)
{
   _position = pPosition;
}

int tAutodeskMemoryStream::GetError() const
{
   return _errorCode;
}

void tAutodeskMemoryStream::ClearError()
{
   _errorCode = 0;
}
