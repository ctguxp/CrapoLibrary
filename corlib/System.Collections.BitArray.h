#pragma once
#include "System.Collections.ICollection.h"

namespace System
  {
  namespace Collections
    {
    class CRAPOCOREDLL_API BitArray : public ICollection
      {
      private:
        int32     _length;
        int32     _version;
#pragma warning(disable:4251)
        IntArray  _array;
#pragma warning(default:4251)
      public:
        BitArray(int32 length = 0);
        BitArray(BoolArray& /*values*/);
        BitArray(ByteArray& /*bytes*/);
        BitArray(IntArray& /*values*/);
        BitArray(int32 /*length*/, bool /*defaultValue*/); 
        BitArray(const BitArray& /*bitArray*/);
        virtual ~BitArray();
        BitArray& operator = (const BitArray& /*bitArray*/);
        bool operator[] (int32 index) const;

        template<class T>
        void CopyTo(Array<T>& arr, int index)
          {
          if(arr.IsNull())
            throw ArgumentNullException(L"arr");
          if(index < 0)
            throw ArgumentOutOfRangeException(L"index");

          /*if(array.Rank != 1)
          throw new ArgumentException ("array", "Array rank must be 1");*/

          if(index >= (int32)arr.Length() && _length > 0)
            throw ArgumentException(L"index", L"index is greater than array.Length");

          // in each case, check to make sure enough space in array

          if(typeid(bool) == typeid(T))
            {
            if((int32)arr.Length() - index < _length)
              throw ArgumentException ();

            bool* barray = (bool*)arr.ToPtr();

            //// Copy the bits into the array
            for(int32 i = 0; i < _length; i++)
              barray[index + i] = Get(i);

            }
          else if(typeid(byte) == typeid(T))
            {
            int32 numbytes = (_length + 7) / 8;

            if(((int32)arr.Length() - index) < numbytes)
              throw ArgumentException();

            byte* barray = (byte*)arr.ToPtr();

            // Copy the bytes into the array
            for(int32 i = 0; i < numbytes; i++)
              barray[index + i] = GetByte(i);

            }
          else if(typeid(int32) == typeid(T))
            {
            Array<int32>::Copy(_array.ToPtr(), 0, (int32*)arr.ToPtr(), index, (_length + 31) / 32);
            } 
          else 
            {
            throw ArgumentException(L"array", L"Unsupported type");
            }
          }

        BitArray& And(BitArray& /*value*/);
        BitArray& Or(BitArray& /*value*/);
        BitArray& Not();
        BitArray& Xor(BitArray& /*value*/);
        bool IsReadOnly();
        bool Get(int32 /*index*/) const;
        int32 Length();
        void Length(int32 /*value*/);
        void Set(int32 /*index*/, bool /*value*/);
        void SetAll(bool /*value*/);
        // From ICollection
        virtual int32 Count() override;
        virtual bool IsSynchronized() override;
        // From IEnumberable
        virtual IEnumerator* GetEnumerator() override;
      private:
        void CheckOperand(BitArray& /*operand*/);
        byte GetByte(int32 byteIndex);
        void SetByte(int32 /*byteIndex*/, byte /*value*/);
      private:
        class BitArrayEnumerator : public IEnumerator
          {
          private:
            BitArrayEnumerator& operator=(BitArrayEnumerator const&);
          private:
            BitArray& _bitArray;
			      GCObject  _current;
			      int32     _index;
            int32     _version;
          public:
            BitArrayEnumerator(BitArray& /*bitArray*/);
            virtual ~BitArrayEnumerator();
            virtual GCObject& Current() override;
            virtual bool MoveNext() override;
            virtual void Reset() override;
          private:
            void CheckVersion();
          };
      };
    }
  }
