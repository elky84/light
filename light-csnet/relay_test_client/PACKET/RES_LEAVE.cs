// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class RES_LEAVE : Table {
  public static RES_LEAVE GetRootAsRES_LEAVE(ByteBuffer _bb) { return GetRootAsRES_LEAVE(_bb, new RES_LEAVE()); }
  public static RES_LEAVE GetRootAsRES_LEAVE(ByteBuffer _bb, RES_LEAVE obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public RES_LEAVE __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public int USERINDEX { get { int o = __offset(4); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }

  public static Offset<RES_LEAVE> CreateRES_LEAVE(FlatBufferBuilder builder,
      int USER_INDEX = 0) {
    builder.StartObject(1);
    RES_LEAVE.AddUSERINDEX(builder, USER_INDEX);
    return RES_LEAVE.EndRES_LEAVE(builder);
  }

  public static void StartRES_LEAVE(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddUSERINDEX(FlatBufferBuilder builder, int USERINDEX) { builder.AddInt(0, USERINDEX, 0); }
  public static Offset<RES_LEAVE> EndRES_LEAVE(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<RES_LEAVE>(o);
  }
};


}
