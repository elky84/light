// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class PLAYER_DATA : Table {
  public static PLAYER_DATA GetRootAsPLAYER_DATA(ByteBuffer _bb) { return GetRootAsPLAYER_DATA(_bb, new PLAYER_DATA()); }
  public static PLAYER_DATA GetRootAsPLAYER_DATA(ByteBuffer _bb, PLAYER_DATA obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public PLAYER_DATA __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public int ID { get { int o = __offset(4); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }
  public string NAME { get { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetNAMEBytes() { return __vector_as_arraysegment(6); }

  public static Offset<PLAYER_DATA> CreatePLAYER_DATA(FlatBufferBuilder builder,
      int ID = 0,
      StringOffset NAMEOffset = default(StringOffset)) {
    builder.StartObject(2);
    PLAYER_DATA.AddNAME(builder, NAMEOffset);
    PLAYER_DATA.AddID(builder, ID);
    return PLAYER_DATA.EndPLAYER_DATA(builder);
  }

  public static void StartPLAYER_DATA(FlatBufferBuilder builder) { builder.StartObject(2); }
  public static void AddID(FlatBufferBuilder builder, int ID) { builder.AddInt(0, ID, 0); }
  public static void AddNAME(FlatBufferBuilder builder, StringOffset NAMEOffset) { builder.AddOffset(1, NAMEOffset.Value, 0); }
  public static Offset<PLAYER_DATA> EndPLAYER_DATA(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<PLAYER_DATA>(o);
  }
};


}
