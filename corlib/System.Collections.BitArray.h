#pragma once
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class BitArray : public ICollection
      {
      private:
        int32     _length;
        int32     _version;
        IntArray  _array;
      public:
        BitArray(int32 length = 0);
        BitArray(BoolArray& /*values*/);
        BitArray(ByteArray& /*bytes*/);
        BitArray(IntArray& /*values*/);
        BitArray(const BitArray& bitArray);
        virtual ~BitArray();
        BitArray& operator = (const BitArray& bitArray);
        bool Get(int32 /*index*/);
        int32 Length();
        void Length(int32 value);
        void Set(int32 /*index*/, bool /*value*/);
        // From ICollection
        virtual sizet Count() override;
        virtual bool IsSynchronized() override;
        // From IEnumberable
        virtual IEnumerator* GetEnumerator() override;
      private:
        void SetByte(int32 byteIndex, byte value);
      };
    }
  }
