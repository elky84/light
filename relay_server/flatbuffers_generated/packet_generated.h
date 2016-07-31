// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_PACKET_PACKET_H_
#define FLATBUFFERS_GENERATED_PACKET_PACKET_H_

#include "flatbuffers/flatbuffers.h"

namespace PACKET {

struct PLAYER_DATA;

struct REQ_JOIN;

struct RES_JOIN;

struct JOINED;

struct REQ_LEAVE;

struct RES_LEAVE;

struct REQ_SPAWN;

struct RES_SPAWN;

struct REQ_READY;

struct RES_READY;

struct REQ_SEND;

struct RES_SEND;

struct PLAYER_DATA FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_ID = 4,
    VT_NAME = 6
  };
  int32_t ID() const { return GetField<int32_t>(VT_ID, 0); }
  const flatbuffers::String *NAME() const { return GetPointer<const flatbuffers::String *>(VT_NAME); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_ID) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_NAME) &&
           verifier.Verify(NAME()) &&
           verifier.EndTable();
  }
};

struct PLAYER_DATABuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_ID(int32_t ID) { fbb_.AddElement<int32_t>(PLAYER_DATA::VT_ID, ID, 0); }
  void add_NAME(flatbuffers::Offset<flatbuffers::String> NAME) { fbb_.AddOffset(PLAYER_DATA::VT_NAME, NAME); }
  PLAYER_DATABuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  PLAYER_DATABuilder &operator=(const PLAYER_DATABuilder &);
  flatbuffers::Offset<PLAYER_DATA> Finish() {
    auto o = flatbuffers::Offset<PLAYER_DATA>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<PLAYER_DATA> CreatePLAYER_DATA(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t ID = 0,
   flatbuffers::Offset<flatbuffers::String> NAME = 0) {
  PLAYER_DATABuilder builder_(_fbb);
  builder_.add_NAME(NAME);
  builder_.add_ID(ID);
  return builder_.Finish();
}

struct REQ_JOIN FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_USER_NAME = 4,
    VT_HASH = 6,
    VT_USER_INDEX = 8
  };
  const flatbuffers::String *USER_NAME() const { return GetPointer<const flatbuffers::String *>(VT_USER_NAME); }
  const flatbuffers::String *HASH() const { return GetPointer<const flatbuffers::String *>(VT_HASH); }
  int32_t USER_INDEX() const { return GetField<int32_t>(VT_USER_INDEX, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_USER_NAME) &&
           verifier.Verify(USER_NAME()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_HASH) &&
           verifier.Verify(HASH()) &&
           VerifyField<int32_t>(verifier, VT_USER_INDEX) &&
           verifier.EndTable();
  }
};

struct REQ_JOINBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_USER_NAME(flatbuffers::Offset<flatbuffers::String> USER_NAME) { fbb_.AddOffset(REQ_JOIN::VT_USER_NAME, USER_NAME); }
  void add_HASH(flatbuffers::Offset<flatbuffers::String> HASH) { fbb_.AddOffset(REQ_JOIN::VT_HASH, HASH); }
  void add_USER_INDEX(int32_t USER_INDEX) { fbb_.AddElement<int32_t>(REQ_JOIN::VT_USER_INDEX, USER_INDEX, 0); }
  REQ_JOINBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  REQ_JOINBuilder &operator=(const REQ_JOINBuilder &);
  flatbuffers::Offset<REQ_JOIN> Finish() {
    auto o = flatbuffers::Offset<REQ_JOIN>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<REQ_JOIN> CreateREQ_JOIN(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> USER_NAME = 0,
   flatbuffers::Offset<flatbuffers::String> HASH = 0,
   int32_t USER_INDEX = 0) {
  REQ_JOINBuilder builder_(_fbb);
  builder_.add_USER_INDEX(USER_INDEX);
  builder_.add_HASH(HASH);
  builder_.add_USER_NAME(USER_NAME);
  return builder_.Finish();
}

struct RES_JOIN FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_DATA = 4,
    VT_PLAYER_DATAS = 6
  };
  const PLAYER_DATA *DATA() const { return GetPointer<const PLAYER_DATA *>(VT_DATA); }
  const flatbuffers::Vector<flatbuffers::Offset<PLAYER_DATA>> *PLAYER_DATAS() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<PLAYER_DATA>> *>(VT_PLAYER_DATAS); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DATA) &&
           verifier.VerifyTable(DATA()) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_PLAYER_DATAS) &&
           verifier.Verify(PLAYER_DATAS()) &&
           verifier.VerifyVectorOfTables(PLAYER_DATAS()) &&
           verifier.EndTable();
  }
};

struct RES_JOINBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_DATA(flatbuffers::Offset<PLAYER_DATA> DATA) { fbb_.AddOffset(RES_JOIN::VT_DATA, DATA); }
  void add_PLAYER_DATAS(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<PLAYER_DATA>>> PLAYER_DATAS) { fbb_.AddOffset(RES_JOIN::VT_PLAYER_DATAS, PLAYER_DATAS); }
  RES_JOINBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  RES_JOINBuilder &operator=(const RES_JOINBuilder &);
  flatbuffers::Offset<RES_JOIN> Finish() {
    auto o = flatbuffers::Offset<RES_JOIN>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<RES_JOIN> CreateRES_JOIN(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<PLAYER_DATA> DATA = 0,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<PLAYER_DATA>>> PLAYER_DATAS = 0) {
  RES_JOINBuilder builder_(_fbb);
  builder_.add_PLAYER_DATAS(PLAYER_DATAS);
  builder_.add_DATA(DATA);
  return builder_.Finish();
}

struct JOINED FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_DATA = 4
  };
  const PLAYER_DATA *DATA() const { return GetPointer<const PLAYER_DATA *>(VT_DATA); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_DATA) &&
           verifier.VerifyTable(DATA()) &&
           verifier.EndTable();
  }
};

struct JOINEDBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_DATA(flatbuffers::Offset<PLAYER_DATA> DATA) { fbb_.AddOffset(JOINED::VT_DATA, DATA); }
  JOINEDBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  JOINEDBuilder &operator=(const JOINEDBuilder &);
  flatbuffers::Offset<JOINED> Finish() {
    auto o = flatbuffers::Offset<JOINED>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<JOINED> CreateJOINED(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<PLAYER_DATA> DATA = 0) {
  JOINEDBuilder builder_(_fbb);
  builder_.add_DATA(DATA);
  return builder_.Finish();
}

struct REQ_LEAVE FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct REQ_LEAVEBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  REQ_LEAVEBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  REQ_LEAVEBuilder &operator=(const REQ_LEAVEBuilder &);
  flatbuffers::Offset<REQ_LEAVE> Finish() {
    auto o = flatbuffers::Offset<REQ_LEAVE>(fbb_.EndTable(start_, 0));
    return o;
  }
};

inline flatbuffers::Offset<REQ_LEAVE> CreateREQ_LEAVE(flatbuffers::FlatBufferBuilder &_fbb) {
  REQ_LEAVEBuilder builder_(_fbb);
  return builder_.Finish();
}

struct RES_LEAVE FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_USER_INDEX = 4
  };
  int32_t USER_INDEX() const { return GetField<int32_t>(VT_USER_INDEX, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_USER_INDEX) &&
           verifier.EndTable();
  }
};

struct RES_LEAVEBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_USER_INDEX(int32_t USER_INDEX) { fbb_.AddElement<int32_t>(RES_LEAVE::VT_USER_INDEX, USER_INDEX, 0); }
  RES_LEAVEBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  RES_LEAVEBuilder &operator=(const RES_LEAVEBuilder &);
  flatbuffers::Offset<RES_LEAVE> Finish() {
    auto o = flatbuffers::Offset<RES_LEAVE>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<RES_LEAVE> CreateRES_LEAVE(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t USER_INDEX = 0) {
  RES_LEAVEBuilder builder_(_fbb);
  builder_.add_USER_INDEX(USER_INDEX);
  return builder_.Finish();
}

struct REQ_SPAWN FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MONSTER_ID = 4
  };
  int32_t MONSTER_ID() const { return GetField<int32_t>(VT_MONSTER_ID, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_MONSTER_ID) &&
           verifier.EndTable();
  }
};

struct REQ_SPAWNBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_MONSTER_ID(int32_t MONSTER_ID) { fbb_.AddElement<int32_t>(REQ_SPAWN::VT_MONSTER_ID, MONSTER_ID, 0); }
  REQ_SPAWNBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  REQ_SPAWNBuilder &operator=(const REQ_SPAWNBuilder &);
  flatbuffers::Offset<REQ_SPAWN> Finish() {
    auto o = flatbuffers::Offset<REQ_SPAWN>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<REQ_SPAWN> CreateREQ_SPAWN(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t MONSTER_ID = 0) {
  REQ_SPAWNBuilder builder_(_fbb);
  builder_.add_MONSTER_ID(MONSTER_ID);
  return builder_.Finish();
}

struct RES_SPAWN FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MONSTER_ID = 4
  };
  int32_t MONSTER_ID() const { return GetField<int32_t>(VT_MONSTER_ID, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<int32_t>(verifier, VT_MONSTER_ID) &&
           verifier.EndTable();
  }
};

struct RES_SPAWNBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_MONSTER_ID(int32_t MONSTER_ID) { fbb_.AddElement<int32_t>(RES_SPAWN::VT_MONSTER_ID, MONSTER_ID, 0); }
  RES_SPAWNBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  RES_SPAWNBuilder &operator=(const RES_SPAWNBuilder &);
  flatbuffers::Offset<RES_SPAWN> Finish() {
    auto o = flatbuffers::Offset<RES_SPAWN>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<RES_SPAWN> CreateRES_SPAWN(flatbuffers::FlatBufferBuilder &_fbb,
   int32_t MONSTER_ID = 0) {
  RES_SPAWNBuilder builder_(_fbb);
  builder_.add_MONSTER_ID(MONSTER_ID);
  return builder_.Finish();
}

struct REQ_READY FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           verifier.EndTable();
  }
};

struct REQ_READYBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  REQ_READYBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  REQ_READYBuilder &operator=(const REQ_READYBuilder &);
  flatbuffers::Offset<REQ_READY> Finish() {
    auto o = flatbuffers::Offset<REQ_READY>(fbb_.EndTable(start_, 0));
    return o;
  }
};

inline flatbuffers::Offset<REQ_READY> CreateREQ_READY(flatbuffers::FlatBufferBuilder &_fbb) {
  REQ_READYBuilder builder_(_fbb);
  return builder_.Finish();
}

struct RES_READY FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_GAME_START = 4
  };
  bool GAME_START() const { return GetField<uint8_t>(VT_GAME_START, 0) != 0; }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_GAME_START) &&
           verifier.EndTable();
  }
};

struct RES_READYBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_GAME_START(bool GAME_START) { fbb_.AddElement<uint8_t>(RES_READY::VT_GAME_START, static_cast<uint8_t>(GAME_START), 0); }
  RES_READYBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  RES_READYBuilder &operator=(const RES_READYBuilder &);
  flatbuffers::Offset<RES_READY> Finish() {
    auto o = flatbuffers::Offset<RES_READY>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<RES_READY> CreateRES_READY(flatbuffers::FlatBufferBuilder &_fbb,
   bool GAME_START = false) {
  RES_READYBuilder builder_(_fbb);
  builder_.add_GAME_START(GAME_START);
  return builder_.Finish();
}

struct REQ_SEND FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MESSAGE = 4
  };
  const flatbuffers::String *MESSAGE() const { return GetPointer<const flatbuffers::String *>(VT_MESSAGE); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MESSAGE) &&
           verifier.Verify(MESSAGE()) &&
           verifier.EndTable();
  }
};

struct REQ_SENDBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_MESSAGE(flatbuffers::Offset<flatbuffers::String> MESSAGE) { fbb_.AddOffset(REQ_SEND::VT_MESSAGE, MESSAGE); }
  REQ_SENDBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  REQ_SENDBuilder &operator=(const REQ_SENDBuilder &);
  flatbuffers::Offset<REQ_SEND> Finish() {
    auto o = flatbuffers::Offset<REQ_SEND>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<REQ_SEND> CreateREQ_SEND(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> MESSAGE = 0) {
  REQ_SENDBuilder builder_(_fbb);
  builder_.add_MESSAGE(MESSAGE);
  return builder_.Finish();
}

struct RES_SEND FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_MESSAGE = 4
  };
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *MESSAGE() const { return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>> *>(VT_MESSAGE); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MESSAGE) &&
           verifier.Verify(MESSAGE()) &&
           verifier.VerifyVectorOfStrings(MESSAGE()) &&
           verifier.EndTable();
  }
};

struct RES_SENDBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_MESSAGE(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> MESSAGE) { fbb_.AddOffset(RES_SEND::VT_MESSAGE, MESSAGE); }
  RES_SENDBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  RES_SENDBuilder &operator=(const RES_SENDBuilder &);
  flatbuffers::Offset<RES_SEND> Finish() {
    auto o = flatbuffers::Offset<RES_SEND>(fbb_.EndTable(start_, 1));
    return o;
  }
};

inline flatbuffers::Offset<RES_SEND> CreateRES_SEND(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String>>> MESSAGE = 0) {
  RES_SENDBuilder builder_(_fbb);
  builder_.add_MESSAGE(MESSAGE);
  return builder_.Finish();
}

}  // namespace PACKET

#endif  // FLATBUFFERS_GENERATED_PACKET_PACKET_H_
