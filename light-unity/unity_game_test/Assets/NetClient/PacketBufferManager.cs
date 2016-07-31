using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace wise
{

namespace net
{

    public class PacketBufferManager
    {
        int BufferSize = 0;
        int ReadPos = 0;
        int WritePos = 0;

        int HeaderSize = 0;
        int MaxPacketSize = 0;
        byte[] PacketData;

        public bool Init(int size, int headerSize, int maxPacketSize)
        {
            if (size < (maxPacketSize * 2) || size < 1 || headerSize < 1 || maxPacketSize < 1)
            {
                return false;
            }

            BufferSize = size;
            PacketData = new byte[size];
            HeaderSize = headerSize;
            MaxPacketSize = maxPacketSize;
            return true;
        }

        public bool Write(byte[] data, int pos, int size)
        {
            if (data == null || (data.Length < (pos + size)))
            {
                return false;
            }

            var remainBufferSize = BufferSize - WritePos;

            if (remainBufferSize < size)
            {
                return false;
            }

            Buffer.BlockCopy(data, pos, PacketData, WritePos, size);
            WritePos += size;
            return true;
        }

        public ArraySegment<byte> Read()
        {
            var enableReadSize = WritePos - ReadPos;

            if (enableReadSize < HeaderSize)
            {
                return new ArraySegment<byte>();
            }

            var packetDataSize = BitConverter.ToUInt32(PacketData, (ReadPos + sizeof(UInt32))); // sizeof(UInt32)는 커맨드 부분을 건너뛰는 코드다.
            if (enableReadSize < packetDataSize)
            {
                return new ArraySegment<byte>();
            }

            var completePacketData = new ArraySegment<byte>(PacketData, ReadPos, (int)packetDataSize);
            ReadPos += (int)packetDataSize;

            return completePacketData;
        }

        bool NextFree()
        {
            var enableWriteSize = BufferSize - WritePos;

            if (enableWriteSize < MaxPacketSize)
            {
                return false;
            }

            return true;
        }

        public void Compress()
        {
            if (WritePos == ReadPos)
            {
                WritePos = 0;
                ReadPos = 0;
            }
            else
            {
                var enableReadSize = WritePos - ReadPos;
                Buffer.BlockCopy(PacketData, 0, PacketData, ReadPos, enableReadSize);
                ReadPos = 0;
                WritePos = enableReadSize;
            }
        }
    }

} // namespace net

} // namespace wise