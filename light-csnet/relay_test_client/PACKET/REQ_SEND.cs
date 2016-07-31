// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class REQ_SEND : Table {
  public static REQ_SEND GetRootAsREQ_SEND(ByteBuffer _bb) { return GetRootAsREQ_SEND(_bb, new REQ_SEND()); }
  public static REQ_SEND GetRootAsREQ_SEND(ByteBuffer _bb, REQ_SEND obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public REQ_SEND __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public string MESSAGE { get { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetMESSAGEBytes() { return __vector_as_arraysegment(4); }

  public static Offset<REQ_SEND> CreateREQ_SEND(FlatBufferBuilder builder,
      StringOffset MESSAGEOffset = default(StringOffset)) {
    builder.StartObject(1);
    REQ_SEND.AddMESSAGE(builder, MESSAGEOffset);
    return REQ_SEND.EndREQ_SEND(builder);
  }

  public static void StartREQ_SEND(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddMESSAGE(FlatBufferBuilder builder, StringOffset MESSAGEOffset) { builder.AddOffset(0, MESSAGEOffset.Value, 0); }
  public static Offset<REQ_SEND> EndREQ_SEND(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<REQ_SEND>(o);
  }
};


}
