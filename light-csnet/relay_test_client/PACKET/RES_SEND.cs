// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class RES_SEND : Table {
  public static RES_SEND GetRootAsRES_SEND(ByteBuffer _bb) { return GetRootAsRES_SEND(_bb, new RES_SEND()); }
  public static RES_SEND GetRootAsRES_SEND(ByteBuffer _bb, RES_SEND obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public RES_SEND __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public string GetMESSAGE(int j) { int o = __offset(4); return o != 0 ? __string(__vector(o) + j * 4) : null; }
  public int MESSAGELength { get { int o = __offset(4); return o != 0 ? __vector_len(o) : 0; } }

  public static Offset<RES_SEND> CreateRES_SEND(FlatBufferBuilder builder,
      VectorOffset MESSAGEOffset = default(VectorOffset)) {
    builder.StartObject(1);
    RES_SEND.AddMESSAGE(builder, MESSAGEOffset);
    return RES_SEND.EndRES_SEND(builder);
  }

  public static void StartRES_SEND(FlatBufferBuilder builder) { builder.StartObject(1); }
  public static void AddMESSAGE(FlatBufferBuilder builder, VectorOffset MESSAGEOffset) { builder.AddOffset(0, MESSAGEOffset.Value, 0); }
  public static VectorOffset CreateMESSAGEVector(FlatBufferBuilder builder, StringOffset[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static void StartMESSAGEVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<RES_SEND> EndRES_SEND(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<RES_SEND>(o);
  }
};


}
