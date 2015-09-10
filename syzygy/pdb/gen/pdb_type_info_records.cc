// Copyright 2015 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This file is generated by generate_type_info_records.py, DO NOT MODIFY.

#include "syzygy/pdb/gen/pdb_type_info_records.h"
#include "syzygy/pdb/pdb_stream.h"

namespace pdb {

LeafArray::LeafArray() : body_{},
                         size_{},
                         name_{} {}

bool LeafArray::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafArray, data);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &size_))
    return false;
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafBClass::LeafBClass() : body_{},
                           offset_{} {}

bool LeafBClass::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafBClass, offset);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &offset_))
    return false;

  return true;
}

LeafBitfield::LeafBitfield() : body_{} {}

bool LeafBitfield::Initialize(PdbStream* stream) {
  size_t to_read = sizeof(body_);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }

  return true;
}

LeafClass::LeafClass() : body_{},
                         size_{},
                         name_{},
                         decorated_name_{} {}

bool LeafClass::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafClass, data);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &size_))
    return false;
  if (!ReadWideString(stream, &name_))
    return false;
  if ((property().decorated_name_present != 0) &&
      !ReadWideString(stream, &decorated_name_)) {
    return false;
  }

  return true;
}

LeafEnumerate::LeafEnumerate() : body_{},
                                 value_{},
                                 name_{} {}

bool LeafEnumerate::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafEnumerate, value);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &value_))
    return false;
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafFriendCls::LeafFriendCls() : body_{} {}

bool LeafFriendCls::Initialize(PdbStream* stream) {
  size_t to_read = sizeof(body_);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }

  return true;
}

LeafFriendFcn::LeafFriendFcn() : body_{},
                                 name_{} {}

bool LeafFriendFcn::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafFriendFcn, name);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafIndex::LeafIndex() : body_{} {}

bool LeafIndex::Initialize(PdbStream* stream) {
  size_t to_read = sizeof(body_);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }

  return true;
}

LeafMember::LeafMember() : body_{},
                           offset_{},
                           name_{} {}

bool LeafMember::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafMember, offset);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &offset_))
    return false;
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafMethod::LeafMethod() : body_{},
                           name_{} {}

bool LeafMethod::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafMethod, name);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafMFunction::LeafMFunction() : body_{} {}

bool LeafMFunction::Initialize(PdbStream* stream) {
  size_t to_read = sizeof(body_);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }

  return true;
}

LeafModifier::LeafModifier() : body_{},
                               attr_{} {}

bool LeafModifier::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafModifier, attr);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadBasicType(stream, &attr_))
    return false;

  return true;
}

LeafNestType::LeafNestType() : body_{},
                               name_{} {}

bool LeafNestType::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafNestTypeEx, name);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafOneMethod::LeafOneMethod() : body_{},
                                 vbaseoff_{},
                                 name_{} {}

bool LeafOneMethod::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafOneMethod, vbaseoff);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if ((attr().mprop == Microsoft_Cci_Pdb::CV_MTintro ||
       attr().mprop == Microsoft_Cci_Pdb::CV_MTpureintro) &&
      !ReadBasicType(stream, &vbaseoff_)) {
    return false;
  }
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafPointer::LeafPointer() : body_{},
                             attr_{},
                             containing_class_{},
                             pmtype_{} {}

bool LeafPointer::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafPointer::LeafPointerBody, attr);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadBasicType(stream, &attr_))
    return false;
  if ((attr().ptrmode == Microsoft_Cci_Pdb::CV_PTR_MODE_PMEM ||
       attr().ptrmode == Microsoft_Cci_Pdb::CV_PTR_MODE_PMFUNC) &&
      !ReadBasicType(stream, &containing_class_)) {
    return false;
  }
  if ((attr().ptrmode == Microsoft_Cci_Pdb::CV_PTR_MODE_PMEM ||
       attr().ptrmode == Microsoft_Cci_Pdb::CV_PTR_MODE_PMFUNC) &&
      !ReadBasicType(stream, &pmtype_)) {
    return false;
  }

  return true;
}

LeafProcedure::LeafProcedure() : body_{} {}

bool LeafProcedure::Initialize(PdbStream* stream) {
  size_t to_read = sizeof(body_);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }

  return true;
}

LeafSTMember::LeafSTMember() : body_{},
                               name_{} {}

bool LeafSTMember::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafSTMember, name);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadWideString(stream, &name_))
    return false;

  return true;
}

LeafUnion::LeafUnion() : body_{},
                         size_{},
                         name_{},
                         decorated_name_{} {}

bool LeafUnion::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafUnion, data);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &size_))
    return false;
  if (!ReadWideString(stream, &name_))
    return false;
  if ((property().decorated_name_present != 0) &&
      !ReadWideString(stream, &decorated_name_)) {
    return false;
  }

  return true;
}

LeafVBClass::LeafVBClass() : body_{},
                             vbpoff_{},
                             vboff_{} {}

bool LeafVBClass::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafVBClass, vbpoff);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadUnsignedNumeric(stream, &vbpoff_))
    return false;
  if (!ReadUnsignedNumeric(stream, &vboff_))
    return false;

  return true;
}

LeafVFuncOff::LeafVFuncOff() : body_{},
                               type_{},
                               offset_{} {}

bool LeafVFuncOff::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafVFuncOff, pad0);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadBasicType(stream, &type_))
    return false;
  if (!ReadBasicType(stream, &offset_))
    return false;

  return true;
}

LeafVFuncTab::LeafVFuncTab() : body_{},
                               index_{} {}

bool LeafVFuncTab::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::LeafVFuncTab, pad0);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if (!ReadBasicType(stream, &index_))
    return false;

  return true;
}

MethodListRecord::MethodListRecord() : body_{},
                                       vbaseoff_{} {}

bool MethodListRecord::Initialize(PdbStream* stream) {
  size_t to_read = offsetof(Microsoft_Cci_Pdb::mlMethod, vbaseoff);
  size_t bytes_read = 0;
  if (!stream->ReadBytes(&body_, to_read, &bytes_read) ||
      bytes_read != to_read) {
    return false;
  }
  if ((attr().mprop == Microsoft_Cci_Pdb::CV_MTintro ||
       attr().mprop == Microsoft_Cci_Pdb::CV_MTpureintro) &&
      !ReadBasicType(stream, &vbaseoff_)) {
    return false;
  }

  return true;
}

}  // namespace pdb
