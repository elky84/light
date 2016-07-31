// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class ENTER : Table {
  public static ENTER GetRootAsENTER(ByteBuffer _bb) { return GetRootAsENTER(_bb, new ENTER()); }
  public static ENTER GetRootAsENTER(ByteBuffer _bb, ENTER obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public ENTER __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public string USERNAME { get { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetUSERNAMEBytes() { return __vector_as_arraysegment(4); }
  public string HASH { get { int o = __offset(6); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetHASHBytes() { return __vector_as_arraysegment(6); }
  public int USERINDEX { get { int o = __offset(8); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }

  public static Offset<ENTER> CreateENTER(FlatBufferBuilder builder,
      StringOffset USER_NAMEOffset = default(StringOffset),
      StringOffset HASHOffset = default(StringOffset),
      int USER_INDEX = 0) {
    builder.StartObject(3);
    ENTER.AddUSERINDEX(builder, USER_INDEX);
    ENTER.AddHASH(builder, HASHOffset);
    ENTER.AddUSERNAME(builder, USER_NAMEOffset);
    return ENTER.EndENTER(builder);
  }

  public static void StartENTER(FlatBufferBuilder builder) { builder.StartObject(3); }
  public static void AddUSERNAME(FlatBufferBuilder builder, StringOffset USERNAMEOffset) { builder.AddOffset(0, USERNAMEOffset.Value, 0); }
  public static void AddHASH(FlatBufferBuilder builder, StringOffset HASHOffset) { builder.AddOffset(1, HASHOffset.Value, 0); }
  public static void AddUSERINDEX(FlatBufferBuilder builder, int USERINDEX) { builder.AddInt(2, USERINDEX, 0); }
  public static Offset<ENTER> EndENTER(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<ENTER>(o);
  }
};


}
