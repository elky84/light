// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class REQ_SPAWN : Table {
  public static REQ_SPAWN GetRootAsREQ_SPAWN(ByteBuffer _bb) { return GetRootAsREQ_SPAWN(_bb, new REQ_SPAWN()); }
  public static REQ_SPAWN GetRootAsREQ_SPAWN(ByteBuffer _bb, REQ_SPAWN obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public REQ_SPAWN __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public int MONSTERID { get { int o = __offset(4); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }

  public static Offset<REQ_SPAWN> CreateREQ_SPAWN(FlatBufferBuilder builder,
      int MONSTER_ID = 0) {
    builder.StartObject(1);
    REQ_SPAWN.AddMONSTERID(builder, MONSTER_ID);
    return REQ_SPAWN.EndREQ_SPAWN(builder);
  }

  public static void StartREQ_SPAWN(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddMONSTERID(FlatBufferBuilder builder, int MONSTERID) { builder.AddInt(0, MONSTERID, 0); }
  public static Offset<REQ_SPAWN> EndREQ_SPAWN(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<REQ_SPAWN>(o);
  }
};


}
