// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class REQ_READY : Table {
  public static REQ_READY GetRootAsREQ_READY(ByteBuffer _bb) { return GetRootAsREQ_READY(_bb, new REQ_READY()); }
  public static REQ_READY GetRootAsREQ_READY(ByteBuffer _bb, REQ_READY obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public REQ_READY __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }


  public static void StartREQ_READY(FlatBufferBuilder builder) { builder.StartObject(0); }
  public static Offset<REQ_READY> EndREQ_READY(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<REQ_READY>(o);
  }
};


}
