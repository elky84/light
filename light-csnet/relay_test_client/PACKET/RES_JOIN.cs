// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class RES_JOIN : Table {
  public static RES_JOIN GetRootAsRES_JOIN(ByteBuffer _bb) { return GetRootAsRES_JOIN(_bb, new RES_JOIN()); }
  public static RES_JOIN GetRootAsRES_JOIN(ByteBuffer _bb, RES_JOIN obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public RES_JOIN __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public PLAYER_DATA DATA { get { return GetDATA(new PLAYER_DATA()); } }
  public PLAYER_DATA GetDATA(PLAYER_DATA obj) { int o = __offset(4); return o != 0 ? obj.__init(__indirect(o + bb_pos), bb) : null; }
  public PLAYER_DATA GetPLAYERDATAS(int j) { return GetPLAYERDATAS(new PLAYER_DATA(), j); }
  public PLAYER_DATA GetPLAYERDATAS(PLAYER_DATA obj, int j) { int o = __offset(6); return o != 0 ? obj.__init(__indirect(__vector(o) + j * 4), bb) : null; }
  public int PLAYERDATASLength { get { int o = __offset(6); return o != 0 ? __vector_len(o) : 0; } }

  public static Offset<RES_JOIN> CreateRES_JOIN(FlatBufferBuilder builder,
      Offset<PLAYER_DATA> DATAOffset = default(Offset<PLAYER_DATA>),
      VectorOffset PLAYER_DATASOffset = default(VectorOffset)) {
    builder.StartObject(2);
    RES_JOIN.AddPLAYERDATAS(builder, PLAYER_DATASOffset);
    RES_JOIN.AddDATA(builder, DATAOffset);
    return RES_JOIN.EndRES_JOIN(builder);
  }

  public static void StartRES_JOIN(FlatBufferBuilder builder) { builder.StartObject(2); }
  public static void AddDATA(FlatBufferBuilder builder, Offset<PLAYER_DATA> DATAOffset) { builder.AddOffset(0, DATAOffset.Value, 0); }
  public static void AddPLAYERDATAS(FlatBufferBuilder builder, VectorOffset PLAYERDATASOffset) { builder.AddOffset(1, PLAYERDATASOffset.Value, 0); }
  public static VectorOffset CreatePLAYERDATASVector(FlatBufferBuilder builder, Offset<PLAYER_DATA>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static void StartPLAYERDATASVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<RES_JOIN> EndRES_JOIN(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<RES_JOIN>(o);
  }
};


}
