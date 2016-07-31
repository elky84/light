// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class JOINED : Table {
  public static JOINED GetRootAsJOINED(ByteBuffer _bb) { return GetRootAsJOINED(_bb, new JOINED()); }
  public static JOINED GetRootAsJOINED(ByteBuffer _bb, JOINED obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public JOINED __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public PLAYER_DATA DATA { get { return GetDATA(new PLAYER_DATA()); } }
  public PLAYER_DATA GetDATA(PLAYER_DATA obj) { int o = __offset(4); return o != 0 ? obj.__init(__indirect(o + bb_pos), bb) : null; }

  public static Offset<JOINED> CreateJOINED(FlatBufferBuilder builder,
      Offset<PLAYER_DATA> DATAOffset = default(Offset<PLAYER_DATA>)) {
    builder.StartObject(1);
    JOINED.AddDATA(builder, DATAOffset);
    return JOINED.EndJOINED(builder);
  }

  public static void StartJOINED(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddDATA(FlatBufferBuilder builder, Offset<PLAYER_DATA> DATAOffset) { builder.AddOffset(0, DATAOffset.Value, 0); }
  public static Offset<JOINED> EndJOINED(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<JOINED>(o);
  }
};


}
