#pragma once
#include "System.IO.TextReader.h"
#include "System.String.h"
#include "System.Text.Decoder.h"
#include "System.IO.Stream.h"

#pragma warning(disable:4251)
namespace System
  {
  namespace Text
    {
    class Encoding;
    }
  namespace IO
    {
    class CRAPOCOREDLL_API StreamReader : public TextReader
      {
      enum
        {
        DefaultBufferSize = 1024,
		    DefaultFileBufferSize = 4096,
		    MinimumBufferSize = 128,
        };
      private:
        ByteArray        _input_buffer;
        static ByteArray _input_buffer_recycle;
        CharArray        _decoded_buffer;
        static CharArray _decoded_buffer_recycle;
        int              _decoded_count;
        int              _pos;
        int              _buffer_size;
        int              _do_checks;
        Text::Encoding*  _encoding;
        Text::GCDecoder  _decoder;
        GCStream         _base_stream;
        bool             _mayBlock;
      public:
        StreamReader(String);
        StreamReader(Stream*);
        ~StreamReader();
        virtual int Read() override;
      protected:
        void Initialize(bool detectEncodingFromByteOrderMarks, int bufferSize);
        void CheckState();
        int ReadBuffer();
        int ReadBufferCore(int cbEncoded);
        int DoChecks(int count);
      };
    }
  }
#pragma warning(default:4251)

