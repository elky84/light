// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class REQ_LEAVE : Table {
  public static REQ_LEAVE GetRootAsREQ_LEAVE(ByteBuffer _bb) { return GetRootAsREQ_LEAVE(_bb, new REQ_LEAVE()); }
  public static REQ_LEAVE GetRootAsREQ_LEAVE(ByteBuffer _bb, REQ_LEAVE obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public REQ_LEAVE __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }


  public static void StartREQ_LEAVE(FlatBufferBuilder builder) { builder.StartObject(0); }
  public static Offset<REQ_LEAVE> EndREQ_LEAVE(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<REQ_LEAVE>(o);
  }
};


}
