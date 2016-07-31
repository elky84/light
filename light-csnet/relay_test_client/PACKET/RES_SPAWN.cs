// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class RES_SPAWN : Table {
  public static RES_SPAWN GetRootAsRES_SPAWN(ByteBuffer _bb) { return GetRootAsRES_SPAWN(_bb, new RES_SPAWN()); }
  public static RES_SPAWN GetRootAsRES_SPAWN(ByteBuffer _bb, RES_SPAWN obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public RES_SPAWN __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public int MONSTERID { get { int o = __offset(4); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }

  public static Offset<RES_SPAWN> CreateRES_SPAWN(FlatBufferBuilder builder,
      int MONSTER_ID = 0) {
    builder.StartObject(1);
    RES_SPAWN.AddMONSTERID(builder, MONSTER_ID);
    return RES_SPAWN.EndRES_SPAWN(builder);
  }

  public static void StartRES_SPAWN(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddMONSTERID(FlatBufferBuilder builder, int MONSTERID) { builder.AddInt(0, MONSTERID, 0); }
  public static Offset<RES_SPAWN> EndRES_SPAWN(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<RES_SPAWN>(o);
  }
};


}
