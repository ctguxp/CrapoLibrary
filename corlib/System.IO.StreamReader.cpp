#include "pch.h"
#include "System.IO.StreamReader.h"
#include "System.IO.h"
#include "System.Text.Encoding.h"
#include "System.h"

namespace System
  {
  namespace IO
    {

    ByteArray StreamReader::_input_buffer_recycle;
    CharArray StreamReader::_decoded_buffer_recycle;

    StreamReader::StreamReader(String path)
      :_input_buffer()
      ,_decoded_buffer()
      ,_decoded_count(0)
      ,_pos(0)
      ,_buffer_size(0)
      ,_do_checks(0)
      ,_encoding(Text::Encoding::UTF8())
      ,_decoder()
      ,_base_stream()
      ,_mayBlock(false)
      {
      _base_stream.Set(File::OpenRead(path));
      Initialize(true, DefaultFileBufferSize); 
      }
    StreamReader::StreamReader(Stream* s)
      :_input_buffer()
      ,_decoded_buffer()
      ,_decoded_count(0)
      ,_pos(0)
      ,_buffer_size(0)
      ,_do_checks(0)
      ,_encoding(Text::Encoding::UTF8())
      ,_decoder()
      ,_base_stream(s)
      ,_mayBlock(false)
      {
      Initialize(true, DefaultFileBufferSize);
      }
    StreamReader::~StreamReader()
      {
      if(_base_stream.Get() != nullptr)
        _base_stream->Close();
      }
    void StreamReader::Initialize(bool detectEncodingFromByteOrderMarks, int bufferSize)
      {
      if(nullptr == _base_stream.Get())
        throw ArgumentNullException(L"stream");
      if (nullptr == _encoding)
        throw ArgumentNullException (L"encoding");
      if (!_base_stream->CanRead())
        throw ArgumentException(L"Cannot read stream");
      if(bufferSize <= 0)
        throw ArgumentOutOfRangeException(L"bufferSize", L"The minimum size of the buffer must be positive");

      if(bufferSize < MinimumBufferSize)
        bufferSize = MinimumBufferSize;

      // since GetChars() might add flushed character, it 
      // should have additional char buffer for extra 1 
      // (probably 1 is ok, but might be insufficient. I'm not sure)
      auto decoded_buffer_size = _encoding->GetMaxCharCount(bufferSize) + 1;

      //
      // Instead of allocating a new default buffer use the
      // last one if there is any available
      //
      if (bufferSize <= DefaultBufferSize && !_input_buffer_recycle.IsNull()) 
        {
        // TODO : lock (_input_buffer_recycle_lock) 
        //{
        if(!_input_buffer_recycle.IsNull()) 
          {
          _input_buffer = _input_buffer_recycle;
          _input_buffer_recycle.Length(0);
          }

        if(!_decoded_buffer_recycle.IsNull() && decoded_buffer_size <= (int)_decoded_buffer_recycle.Length()) 
          {
          _decoded_buffer = _decoded_buffer_recycle;
          _decoded_buffer_recycle.Length(0);
          }
        //}
        }

      if(_input_buffer.IsNull())
        _input_buffer.Length(bufferSize);
      else
        _input_buffer.Length(bufferSize);

      if(_decoded_buffer.IsNull())
        _decoded_buffer.Length(decoded_buffer_size);
      else
        _decoded_buffer.Length(0);

      _buffer_size = bufferSize;
      _decoder.Set(_encoding->GetDecoder());

      ByteArray preamble = _encoding->GetPreamble();
      _do_checks = detectEncodingFromByteOrderMarks ? 1 : 0;
      _do_checks += (preamble.Length() == 0) ? 0 : 2;
      }

    int StreamReader::ReadBuffer()
      {
      _pos = 0;

      // keep looping until the decoder gives us some chars
      _decoded_count = 0;
      do {
        auto cbEncoded = _base_stream->Read(_input_buffer, 0, _buffer_size);
        if(cbEncoded <= 0)
          return 0;

        _decoded_count = ReadBufferCore(cbEncoded);
        } while(_decoded_count == 0);

      return _decoded_count;
      }

    int StreamReader::Read()
      {
      CheckState();

      if(_pos >= _decoded_count && ReadBuffer() == 0)
        return -1;

      return _decoded_buffer[_pos++];
      }

    int StreamReader::ReadBufferCore(int cbEncoded)
      {
      int parse_start;

      _mayBlock = cbEncoded < _buffer_size;
      if(_do_checks > 0)
        {
        Text::Encoding* old = _encoding;
        parse_start = DoChecks(cbEncoded);
        if (old != _encoding)
          {
          int old_decoded_size = old->GetMaxCharCount(_buffer_size) + 1;
          int new_decoded_size = _encoding->GetMaxCharCount(_buffer_size) + 1;
          if (old_decoded_size != new_decoded_size)
            _decoded_buffer.Length(new_decoded_size);
          _decoder.Set(_encoding->GetDecoder());
          }
        _do_checks = 0;
        cbEncoded -= parse_start;
        } 
      else 
        {
        parse_start = 0;
        }

      return _decoder->GetChars(_input_buffer, parse_start, cbEncoded, _decoded_buffer, 0);
      }

    void StreamReader::CheckState()
      {
      if(_base_stream.Get() == nullptr)
        throw ArgumentException(L"StreamReader", L"Cannot read from a closed StreamReader");
      //throw new ObjectDisposedException ("StreamReader", "Cannot read from a closed StreamReader");

      //#if NET_4_5
      //			if (async_task != null && !async_task.IsCompleted)
      //				throw new InvalidOperationException ();
      //#endif
      }

    int StreamReader::DoChecks(int count)
      {
      using namespace Text;
      if((_do_checks & 2) == 2)
        {
        ByteArray preamble = _encoding->GetPreamble();
        int c = (int)preamble.Length();
        if (count >= c)
          {
          int i;

          for (i = 0; i < c; i++)
            if(_input_buffer [i] != preamble [i])
              break;

          if (i == c)
            return i;
          }
        }

      if((_do_checks & 1) == 1)
        {
        if (count < 2)
          return 0;

        if(_input_buffer[0] == 0xfe && _input_buffer [1] == 0xff)
          {
          _encoding = Text::Encoding::BigEndianUnicode();
          return 2;
          }
        if(_input_buffer[0] == 0xff && _input_buffer [1] == 0xfe && count < 4) 
          {
          // If we don't have enough bytes we can't check for UTF32, so use Unicode
          _encoding = Text::Encoding::Unicode();
          return 2;
          }

        if(count < 3)
          return 0;

        if(_input_buffer [0] == 0xef && _input_buffer [1] == 0xbb && _input_buffer [2] == 0xbf)
          {
          _encoding = Text::Encoding::UTF8Unmarked();
          return 3;
          }

        if (count < 4)
          {
          if(_input_buffer [0] == 0xff && _input_buffer [1] == 0xfe && _input_buffer [2] != 0)
            {
            _encoding = Text::Encoding::Unicode();
            return 2;
            }
          return 0;
          }

        /*if(_input_buffer [0] == 0 && _input_buffer [1] == 0 && _input_buffer [2] == 0xfe && _input_buffer [3] == 0xff)
        {
        _encoding = Encoding.BigEndianUTF32;
        return 4;
        }*/

        if(_input_buffer [0] == 0xff && _input_buffer [1] == 0xfe) 
          {
          /*if(_input_buffer [2] == 0 && _input_buffer[3] == 0) 
          {
          _encoding = Encoding.UTF32;
          return 4;
          }*/

          _encoding = Text::Encoding::Unicode();
          return 2;
          }
        }

      return 0;
      }

    } // namespace IO
  }
