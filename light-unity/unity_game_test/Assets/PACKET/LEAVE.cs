// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class LEAVE : Table {
  public static LEAVE GetRootAsLEAVE(ByteBuffer _bb) { return GetRootAsLEAVE(_bb, new LEAVE()); }
  public static LEAVE GetRootAsLEAVE(ByteBuffer _bb, LEAVE obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public LEAVE __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public string USERNAME { get { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetUSERNAMEBytes() { return __vector_as_arraysegment(4); }
  public int USERINDEX { get { int o = __offset(6); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }

  public static Offset<LEAVE> CreateLEAVE(FlatBufferBuilder builder,
      StringOffset USER_NAMEOffset = default(StringOffset),
      int USER_INDEX = 0) {
    builder.StartObject(2);
    LEAVE.AddUSERINDEX(builder, USER_INDEX);
    LEAVE.AddUSERNAME(builder, USER_NAMEOffset);
    return LEAVE.EndLEAVE(builder);
  }

  public static void StartLEAVE(FlatBufferBuilder builder) { builder.StartObject(2); }
  public static void AddUSERNAME(FlatBufferBuilder builder, StringOffset USERNAMEOffset) { builder.AddOffset(0, USERNAMEOffset.Value, 0); }
  public static void AddUSERINDEX(FlatBufferBuilder builder, int USERINDEX) { builder.AddInt(1, USERINDEX, 0); }
  public static Offset<LEAVE> EndLEAVE(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<LEAVE>(o);
  }
};


}
