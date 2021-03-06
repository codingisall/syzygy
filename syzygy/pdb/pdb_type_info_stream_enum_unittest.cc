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

#include "syzygy/pdb/pdb_type_info_stream_enum.h"

#include "base/files/file_util.h"
#include "gtest/gtest.h"
#include "syzygy/core/unittest_util.h"
#include "syzygy/pdb/unittest_util.h"

namespace pdb {

TEST(PdbTypeInfoStreamEnumTest, ReadValidHeaderTypeInfoStream) {
  base::FilePath valid_type_info_path =
      testing::GetSrcRelativePath(testing::kValidPdbTypeInfoStreamPath);

  scoped_refptr<pdb::PdbFileStream> valid_type_info_stream =
      testing::GetStreamFromFile(valid_type_info_path);

  TypeInfoEnumerator enumerator;

  EXPECT_TRUE(enumerator.Init(valid_type_info_stream.get()));

  // Test the actual values from the header to ensure correct sampling.
  EXPECT_EQ(56, enumerator.type_info_header().len);
  EXPECT_EQ(4096, enumerator.type_info_header().type_min);
  EXPECT_EQ(8673, enumerator.type_info_header().type_max);
}

TEST(PdbTypeInfoStreamEnumTest, ReadValidTypeInfoStream) {
  base::FilePath valid_type_info_path =
      testing::GetSrcRelativePath(testing::kValidPdbTypeInfoStreamPath);

  scoped_refptr<pdb::PdbFileStream> valid_type_info_stream =
      testing::GetStreamFromFile(valid_type_info_path);

  TypeInfoEnumerator enumerator;

  EXPECT_TRUE(enumerator.Init(valid_type_info_stream.get()));
  while (!enumerator.EndOfStream()) {
    EXPECT_TRUE(enumerator.NextTypeInfoRecord());
  }
  // Test if the enumerator walked the whole type info stream.
  EXPECT_EQ(enumerator.type_info_header().len +
                enumerator.type_info_header().type_info_data_size,
            valid_type_info_stream->pos());
}

TEST(PdbTypeInfoStreamEnumTest, ReadValidTypeInfoStreamNonSequentially) {
  base::FilePath valid_type_info_path =
      testing::GetSrcRelativePath(testing::kValidPdbTypeInfoStreamPath);

  scoped_refptr<pdb::PdbFileStream> valid_type_info_stream =
      testing::GetStreamFromFile(valid_type_info_path);

  TypeInfoEnumerator enumerator;

  EXPECT_TRUE(enumerator.Init(valid_type_info_stream.get()));

  const uint32_t kMinIndex = enumerator.type_info_header().type_min;
  const uint32_t kMaxIndex = enumerator.type_info_header().type_max;

  // Try to jump beyond the end of stream.
  EXPECT_FALSE(enumerator.SeekRecord(kMaxIndex));

  // And also below the beginning.
  EXPECT_FALSE(enumerator.SeekRecord(kMinIndex - 1));

  // Jump in the middle of the stream.
  const uint32_t kTestRecord = (kMaxIndex + kMinIndex) / 2;
  EXPECT_TRUE(enumerator.SeekRecord(kTestRecord));
  EXPECT_EQ(kTestRecord, enumerator.type_id());

  // Store the start_position to check later.
  const size_t kTestPosition = enumerator.start_position();

  // Jump back near to the beginning.
  EXPECT_TRUE(enumerator.SeekRecord(kMinIndex + 5));
  EXPECT_EQ(kMinIndex + 5, enumerator.type_id());

  // Jump back and compare.
  EXPECT_TRUE(enumerator.SeekRecord(kTestRecord));
  EXPECT_EQ(kTestRecord, enumerator.type_id());
  EXPECT_EQ(kTestPosition, enumerator.start_position());

  // Jump ahead again.
  const size_t kOffset = 13;
  EXPECT_LT(kTestRecord + kOffset, kMaxIndex);
  EXPECT_TRUE(enumerator.SeekRecord(kTestRecord + kOffset));
  EXPECT_EQ(kTestRecord + kOffset, enumerator.type_id());
}

TEST(PdbTypeInfoStreamEnumTest, ReadInvalidDataTypeInfoStream) {
  base::FilePath invalid_type_info_path =
      testing::GetSrcRelativePath(testing::kInvalidDataPdbTypeInfoStreamPath);

  scoped_refptr<pdb::PdbFileStream> invalid_type_info_stream =
      testing::GetStreamFromFile(invalid_type_info_path);

  TypeInfoEnumerator enumerator;

  EXPECT_TRUE(enumerator.Init(invalid_type_info_stream.get()));
  bool result = true;

  // The first corrupted item should be in the first 50 type info records
  // otherwise this test fails.
  for (int i = 0; i < 50; i++) {
    if (!enumerator.NextTypeInfoRecord()) {
      result = false;
      break;
    }
  }
  EXPECT_FALSE(result) << "No corrupt entry was found in the first 50 "
                       << "records of the invalid PDB file.";
}

TEST(PdbTypeInfoStreamEnumTest, ReadInvalidHeaderTypeInfoStream) {
  base::FilePath invalid_type_info_path =
      testing::GetSrcRelativePath(testing::kInvalidHeaderPdbTypeInfoStreamPath);

  scoped_refptr<pdb::PdbFileStream> invalid_type_info_stream =
      testing::GetStreamFromFile(invalid_type_info_path);

  TypeInfoEnumerator enumerator;

  EXPECT_FALSE(enumerator.Init(invalid_type_info_stream.get()));
}

}  // namespace pdb
