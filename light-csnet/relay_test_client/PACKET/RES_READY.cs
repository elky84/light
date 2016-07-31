// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class RES_READY : Table {
  public static RES_READY GetRootAsRES_READY(ByteBuffer _bb) { return GetRootAsRES_READY(_bb, new RES_READY()); }
  public static RES_READY GetRootAsRES_READY(ByteBuffer _bb, RES_READY obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public RES_READY __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public bool GAMESTART { get { int o = __offset(4); return o != 0 ? 0!=bb.Get(o + bb_pos) : (bool)false; } }

  public static Offset<RES_READY> CreateRES_READY(FlatBufferBuilder builder,
      bool GAME_START = false) {
    builder.StartObject(1);
    RES_READY.AddGAMESTART(builder, GAME_START);
    return RES_READY.EndRES_READY(builder);
  }

  public static void StartRES_READY(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddGAMESTART(FlatBufferBuilder builder, bool GAMESTART) { builder.AddBool(0, GAMESTART, false); }
  public static Offset<RES_READY> EndRES_READY(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<RES_READY>(o);
  }
};


}
