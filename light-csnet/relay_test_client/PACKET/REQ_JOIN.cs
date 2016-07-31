// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class REQ_JOIN : Table {
  public static REQ_JOIN GetRootAsREQ_JOIN(ByteBuffer _bb) { return GetRootAsREQ_JOIN(_bb, new REQ_JOIN()); }
  public static REQ_JOIN GetRootAsREQ_JOIN(ByteBuffer _bb, REQ_JOIN obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public REQ_JOIN __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public string USERNAME { get { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetUSERNAMEBytes() { return __vector_as_arraysegment(4); }
  public string HASH { get { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetHASHBytes() { return __vector_as_arraysegment(6); }
  public int USERINDEX { get { int o = __offset(8); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }

  public static Offset<REQ_JOIN> CreateREQ_JOIN(FlatBufferBuilder builder,
      StringOffset USER_NAMEOffset = default(StringOffset),
      StringOffset HASHOffset = default(StringOffset),
      int USER_INDEX = 0) {
    builder.StartObject(3);
    REQ_JOIN.AddUSERINDEX(builder, USER_INDEX);
    REQ_JOIN.AddHASH(builder, HASHOffset);
    REQ_JOIN.AddUSERNAME(builder, USER_NAMEOffset);
    return REQ_JOIN.EndREQ_JOIN(builder);
  }

  public static void StartREQ_JOIN(FlatBufferBuilder builder) { builder.StartObject(3); }
  public static void AddUSERNAME(FlatBufferBuilder builder, StringOffset USERNAMEOffset) { builder.AddOffset(0, USERNAMEOffset.Value, 0); }
  public static void AddHASH(FlatBufferBuilder builder, StringOffset HASHOffset) { builder.AddOffset(1, HASHOffset.Value, 0); }
  public static void AddUSERINDEX(FlatBufferBuilder builder, int USERINDEX) { builder.AddInt(2, USERINDEX, 0); }
  public static Offset<REQ_JOIN> EndREQ_JOIN(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<REQ_JOIN>(o);
  }
};


}
