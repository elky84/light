// automatically generated, do not modify

namespace PACKET
{

using System;
using FlatBuffers;

public sealed class BROADCAST : Table {
  public static BROADCAST GetRootAsBROADCAST(ByteBuffer _bb) { return GetRootAsBROADCAST(_bb, new BROADCAST()); }
  public static BROADCAST GetRootAsBROADCAST(ByteBuffer _bb, BROADCAST obj) { return (obj.__init(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public BROADCAST __init(int _i, ByteBuffer _bb) { bb_pos = _i; bb = _bb; return this; }

  public string USERNAME { get { int o = __offset(4); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetUSERNAMEBytes() { return __vector_as_arraysegment(4); }
  public int USERINDEX { get { int o = __offset(6); return o != 0 ? bb.GetInt(o + bb_pos) : (int)0; } }
  public string MESSAGE { get { int o = __offset(8); return o != 0 ? __string(o + bb_pos) : null; } }
  public ArraySegment<byte>? GetMESSAGEBytes() { return __vector_as_arraysegment(8); }

  public static Offset<BROADCAST> CreateBROADCAST(FlatBufferBuilder builder,
      StringOffset USER_NAMEOffset = default(StringOffset),
      int USER_INDEX = 0,
      StringOffset MESSAGEOffset = default(StringOffset)) {
    builder.StartObject(3);
    BROADCAST.AddMESSAGE(builder, MESSAGEOffset);
    BROADCAST.AddUSERINDEX(builder, USER_INDEX);
    BROADCAST.AddUSERNAME(builder, USER_NAMEOffset);
    return BROADCAST.EndBROADCAST(builder);
  }

  public static void StartBROADCAST(FlatBufferBuilder builder) { builder.StartObject(3); }
  public static void AddUSERNAME(FlatBufferBuilder builder, StringOffset USERNAMEOffset) { builder.AddOffset(0, USERNAMEOffset.Value, 0); }
  public static void AddUSERINDEX(FlatBufferBuilder builder, int USERINDEX) { builder.AddInt(1, USERINDEX, 0); }
  public static void AddMESSAGE(FlatBufferBuilder builder, StringOffset MESSAGEOffset) { builder.AddOffset(2, MESSAGEOffset.Value, 0); }
  public static Offset<BROADCAST> EndBROADCAST(FlatBufferBuilder builder) {
    int o = builder.EndObject();
    return new Offset<BROADCAST>(o);
  }
};


}
