// Stolen from the Autodesk forums.

#pragma once
#include "fbxsdk.h"

class tAutodeskMemoryStream : public FbxStream
{
public:
   inline tAutodeskMemoryStream(
      char* stream,
      long length,
      int readerId)
   :_stream(stream)
   ,_length(length)
   ,_position(0)
   ,_state(EState::eClosed)
   ,_errorCode(0)
   ,_readerId(readerId)
   {
   }

   // Inherited via FbxStream
   virtual EState GetState() override;
   virtual bool Open(void* pStreamData) override;
   virtual bool Close() override;
   virtual bool Flush() override;
   virtual int Write(const void*, int) override;
   virtual int Read(void*, int) const override;
   virtual int GetReaderID() const override;
   virtual int GetWriterID() const override;
   virtual void Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos) override;
   virtual long GetPosition() const override;
   virtual void SetPosition(long pPosition) override;
   virtual int GetError() const override;
   virtual void ClearError() override;

private:
   char* _stream;
   long _length;
   long _position;
   EState _state;
   int _errorCode;
   int _readerId;
};
