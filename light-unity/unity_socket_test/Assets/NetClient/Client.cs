using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Diagnostics;
using System.Collections;

namespace wise
{

    namespace net
    {

        public class Client
        {
            ClientNetwork Network = new ClientNetwork();
            PacketBufferManager PacketBuffer = new PacketBufferManager();
            Queue<byte[]> SendPacketQueue = new Queue<byte[]>();

            bool IsNetworkThreadRunning = false;
            System.Threading.Thread NetworkReadThread = null;
            System.Threading.Thread NetworkSendThread = null;

            List<BufferPacketData> RecvPacketList = new List<BufferPacketData>();
            private object recv_lock = new object();
            private object send_lock = new object();

            public const int PACKET_ID_DISCONNECTED = int.MaxValue;

            //생성자
            public Client()
            {
                PacketBuffer.Init((8096 * 100), header_size, 8096);
            }
            
            //원하는 패킷을 읽고싶을때 사용할 것, 기본적으로 모든 패킷은 이곳에 저장된다.
            public BufferPacketData ReadAndRemoveData(int _event)
            {
                for (int i = 0; RecvPacketList.Count > i; i++)
                {
                    if (RecvPacketList[i].PacketID == _event)
                    {
                        BufferPacketData return_data = RecvPacketList[i];
                        RecvPacketList.RemoveAt(i);
                        return return_data;
                    }
                }
                return null;
            }

            public BufferPacketData ReadData()
            {
                lock(recv_lock)
                {
                    if (RecvPacketList.Count > 0)
                    {
                        BufferPacketData return_data = RecvPacketList[0];
                        RecvPacketList.RemoveAt(0);
                        return return_data;
                    }
                }
                return null;
            }

            //연결을 해제하는 메소드
            public void Close()
            {
                Network.Close();

                IsNetworkThreadRunning = false;
                NetworkReadThread.Join();
                NetworkSendThread.Join();

                SetDisconnectd();
            }

            // 접속하기 버튼을 눌렀을 경우 실행되는 이벤트 핸들러
            public Boolean Connect(String ip, int port)
            {
                //주소와 포트번호를 이용하여 네트워크를 연결
                if (Network.Connect(ip, port))
                {
                    IsNetworkThreadRunning = true;
                    NetworkReadThread = new System.Threading.Thread(this.NetworkReadProcess);
                    NetworkReadThread.Start();

                    NetworkSendThread = new System.Threading.Thread(this.NetworkSendProcess);
                    NetworkSendThread.Start();
                    IsNetworkThreadRunning = true;
                    return true;
                }
                else
                {
                    return false;
                }
            }

            // 접속 종료
            public void DisConnect()
            {
                Network.Close();
            }

            void SetDisconnectd()
            {
                lock (recv_lock)
                {
                    BufferPacketData packet = new BufferPacketData();
                    packet.PacketID = PACKET_ID_DISCONNECTED;
                    packet.DataSize = (uint)header_size;
                    packet.Index = 0;
                    packet.Sequence = 0;

                    RecvPacketList.Add(packet); // 연결 끊김 이벤트를 패킷으로 전달.
                }

                lock (send_lock)
                {
                    SendPacketQueue.Clear();
                }
                //서버를 위하여 읽고 썻던 큐를 전부 비운다.
            }

            static UInt16 sequence = 0;

            // 패킷 보내기
            public bool Send(int _event, byte[] bodyData, uint index = 0)
            {
                if(Network.IsConnected() == false)
                {
                    return false;
                }
                
                BufferPacketData header = new BufferPacketData();
                header.PacketID = (UInt32)_event;
                header.DataSize = (uint)(bodyData.Length + header_size);
                header.Index = index;
                header.Sequence = ++sequence;
                header.BufferData = new byte[bodyData.Length];
                Buffer.BlockCopy(bodyData, 0, header.BufferData, 0, bodyData.Length);

                lock (send_lock)
                {
                    SendPacketQueue.Enqueue(GetBytes_Bind(header));
                }
                return true;
            }

            private static BufferPacketData GetBindAck(ArraySegment<byte> array_segment)
            {
                BufferPacketData packet = new BufferPacketData();

                System.IO.MemoryStream ms = new System.IO.MemoryStream(array_segment.Array, array_segment.Offset, array_segment.Count, false);
                System.IO.BinaryReader br = new System.IO.BinaryReader(ms);

                packet.PacketID = br.ReadUInt32();
                packet.DataSize = br.ReadUInt32();
                packet.Index = br.ReadUInt32();
                packet.Sequence = br.ReadUInt16();
                var buffer_size = (int)packet.DataSize - header_size;
                packet.BufferData = new byte[buffer_size];
                packet.BufferData = br.ReadBytes(buffer_size);

                br.Close();
                ms.Close();

                return packet;
            }

            private static int header_size = sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt32) + sizeof(UInt16);

            // 문자열 뒤쪽에 위치한 null 을 제거한 후에 공백문자를 제거한다.
            private static string ExtendedTrim(string source)
            {
                string dest = source;
                int index = dest.IndexOf('\0');
                if (index > -1)
                {
                    dest = source.Substring(0, index + 1);
                }

                return dest.TrimEnd('\0').Trim();
            }

            //네트워크에서 읽어내는 프로세스 
            void NetworkReadProcess()
            {
                while (IsNetworkThreadRunning)
                {
                    Thread.Sleep(1);
                    if (Network.IsConnected() == false)
                    {
                        continue;
                    }
                    //연결 여부를 확인해서 끊겼다면 처음으로 돌아간다.

                    var recvData = Network.Receive();
                    //Network 변수에서 데이터를 받음

                    if (recvData.Count > 0)
                    {
                        //이 데이터에 뭔가가 있을 경우
                        PacketBuffer.Write(recvData.Array, recvData.Offset, recvData.Count);
                        //패킷 버퍼에 해당하는 내용을 입력시킨다.
                        while (true)
                        {
                            var data = PacketBuffer.Read();
                            if (data.Count < 1)
                            {
                                break;
                            }

                            var packet = GetBindAck(data);

                            lock (recv_lock)
                            {
                                RecvPacketList.Add(packet);
                            }
                        }

                        PacketBuffer.Compress();
                    }
                }
            }

            void NetworkSendProcess()
            {
                while (IsNetworkThreadRunning)
                {
                    Thread.Sleep(1);
                    if (Network.IsConnected() == false)
                    {
                        continue;
                    }

                    lock (send_lock)
                    {
                        if (SendPacketQueue.Count > 0)
                        {
                            var packet = SendPacketQueue.Dequeue();
                            Network.Send(packet);
                        }
                    }
                }
            }

            //각 데이터를 프로토콜에 맞도록 정리한 클래스 (구조체)
            [Serializable]
            public class BufferPacketData
            {
                public UInt32 PacketID;
                public UInt32 DataSize;
                public UInt32 Index;
                public UInt16 Sequence;

                public byte[] BufferData;
            }

            // 인증 패킷 구조체를 바이트 배열로 변환하는 함수
            public static byte[] GetBytes_Bind(BufferPacketData packet)
            {
                byte[] btBuffer = new byte[packet.DataSize];

                System.IO.MemoryStream ms = new System.IO.MemoryStream(btBuffer, true);
                System.IO.BinaryWriter bw = new System.IO.BinaryWriter(ms);

                bw.Write(packet.PacketID);
                bw.Write(packet.DataSize);
                bw.Write(packet.Index);
                bw.Write(packet.Sequence);
                bw.Write(packet.BufferData, 0, packet.BufferData.Length);

                bw.Close();
                ms.Close();

                return btBuffer;
            }

            public Boolean IsConnected()
            {
                return Network.IsConnected();
            }

        }

    } // namespace net

} // namespace wise