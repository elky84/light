// automatically generated by the FlatBuffers compiler, do not modify

#ifndef FLATBUFFERS_GENERATED_PACKET_PACKET_H_
#define FLATBUFFERS_GENERATED_PACKET_PACKET_H_

#include "flatbuffers/flatbuffers.h"

namespace PACKET {

struct ENTER;

struct LEAVE;

struct BROADCAST;

struct ENTER FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
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

struct ENTERBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_USER_NAME(flatbuffers::Offset<flatbuffers::String> USER_NAME) { fbb_.AddOffset(ENTER::VT_USER_NAME, USER_NAME); }
  void add_HASH(flatbuffers::Offset<flatbuffers::String> HASH) { fbb_.AddOffset(ENTER::VT_HASH, HASH); }
  void add_USER_INDEX(int32_t USER_INDEX) { fbb_.AddElement<int32_t>(ENTER::VT_USER_INDEX, USER_INDEX, 0); }
  ENTERBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  ENTERBuilder &operator=(const ENTERBuilder &);
  flatbuffers::Offset<ENTER> Finish() {
    auto o = flatbuffers::Offset<ENTER>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<ENTER> CreateENTER(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> USER_NAME = 0,
   flatbuffers::Offset<flatbuffers::String> HASH = 0,
   int32_t USER_INDEX = 0) {
  ENTERBuilder builder_(_fbb);
  builder_.add_USER_INDEX(USER_INDEX);
  builder_.add_HASH(HASH);
  builder_.add_USER_NAME(USER_NAME);
  return builder_.Finish();
}

struct LEAVE FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_USER_NAME = 4,
    VT_USER_INDEX = 6
  };
  const flatbuffers::String *USER_NAME() const { return GetPointer<const flatbuffers::String *>(VT_USER_NAME); }
  int32_t USER_INDEX() const { return GetField<int32_t>(VT_USER_INDEX, 0); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_USER_NAME) &&
           verifier.Verify(USER_NAME()) &&
           VerifyField<int32_t>(verifier, VT_USER_INDEX) &&
           verifier.EndTable();
  }
};

struct LEAVEBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_USER_NAME(flatbuffers::Offset<flatbuffers::String> USER_NAME) { fbb_.AddOffset(LEAVE::VT_USER_NAME, USER_NAME); }
  void add_USER_INDEX(int32_t USER_INDEX) { fbb_.AddElement<int32_t>(LEAVE::VT_USER_INDEX, USER_INDEX, 0); }
  LEAVEBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  LEAVEBuilder &operator=(const LEAVEBuilder &);
  flatbuffers::Offset<LEAVE> Finish() {
    auto o = flatbuffers::Offset<LEAVE>(fbb_.EndTable(start_, 2));
    return o;
  }
};

inline flatbuffers::Offset<LEAVE> CreateLEAVE(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> USER_NAME = 0,
   int32_t USER_INDEX = 0) {
  LEAVEBuilder builder_(_fbb);
  builder_.add_USER_INDEX(USER_INDEX);
  builder_.add_USER_NAME(USER_NAME);
  return builder_.Finish();
}

struct BROADCAST FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  enum {
    VT_USER_NAME = 4,
    VT_USER_INDEX = 6,
    VT_MESSAGE = 8
  };
  const flatbuffers::String *USER_NAME() const { return GetPointer<const flatbuffers::String *>(VT_USER_NAME); }
  int32_t USER_INDEX() const { return GetField<int32_t>(VT_USER_INDEX, 0); }
  const flatbuffers::String *MESSAGE() const { return GetPointer<const flatbuffers::String *>(VT_MESSAGE); }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_USER_NAME) &&
           verifier.Verify(USER_NAME()) &&
           VerifyField<int32_t>(verifier, VT_USER_INDEX) &&
           VerifyField<flatbuffers::uoffset_t>(verifier, VT_MESSAGE) &&
           verifier.Verify(MESSAGE()) &&
           verifier.EndTable();
  }
};

struct BROADCASTBuilder {
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_USER_NAME(flatbuffers::Offset<flatbuffers::String> USER_NAME) { fbb_.AddOffset(BROADCAST::VT_USER_NAME, USER_NAME); }
  void add_USER_INDEX(int32_t USER_INDEX) { fbb_.AddElement<int32_t>(BROADCAST::VT_USER_INDEX, USER_INDEX, 0); }
  void add_MESSAGE(flatbuffers::Offset<flatbuffers::String> MESSAGE) { fbb_.AddOffset(BROADCAST::VT_MESSAGE, MESSAGE); }
  BROADCASTBuilder(flatbuffers::FlatBufferBuilder &_fbb) : fbb_(_fbb) { start_ = fbb_.StartTable(); }
  BROADCASTBuilder &operator=(const BROADCASTBuilder &);
  flatbuffers::Offset<BROADCAST> Finish() {
    auto o = flatbuffers::Offset<BROADCAST>(fbb_.EndTable(start_, 3));
    return o;
  }
};

inline flatbuffers::Offset<BROADCAST> CreateBROADCAST(flatbuffers::FlatBufferBuilder &_fbb,
   flatbuffers::Offset<flatbuffers::String> USER_NAME = 0,
   int32_t USER_INDEX = 0,
   flatbuffers::Offset<flatbuffers::String> MESSAGE = 0) {
  BROADCASTBuilder builder_(_fbb);
  builder_.add_MESSAGE(MESSAGE);
  builder_.add_USER_INDEX(USER_INDEX);
  builder_.add_USER_NAME(USER_NAME);
  return builder_.Finish();
}

}  // namespace PACKET

#endif  // FLATBUFFERS_GENERATED_PACKET_PACKET_H_
