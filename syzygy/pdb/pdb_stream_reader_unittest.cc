// Copyright 2012 Google Inc. All Rights Reserved.
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

#include "syzygy/pdb/pdb_stream_reader.h"

#include "gtest/gtest.h"
#include "syzygy/pdb/pdb_byte_stream.h"

namespace pdb {

namespace {

const size_t kTestDataLen = 593;

class PdbStreamReaderTest : public testing::Test {
 public:
  void SetUp() override {
    // Make some test data.
    data_.reserve(kTestDataLen);
    for (size_t i = 0; i < kTestDataLen; ++i)
      data_.push_back(static_cast<uint8_t>(i));

    stream_ = new PdbByteStream();
    stream_->Init(&data_.at(0), data_.size());
  }

  std::vector<uint8_t> data_;
  scoped_refptr<PdbByteStream> stream_;
};

}  // namespace

TEST(PdbStreamReaderTest, Read) {
  const uint8_t kData[] = { 0, 1, 2, 10 };
  scoped_refptr<PdbByteStream> stream(new PdbByteStream());
  stream->Init(kData, sizeof(kData));

  PdbStreamReader reader(stream.get());

  uint8_t data[sizeof(kData)] = {};
  EXPECT_EQ(0U, reader.Position());
  EXPECT_FALSE(reader.AtEnd());
  EXPECT_TRUE(reader.Read(sizeof(data), data));
  EXPECT_EQ(sizeof(kData), reader.Position());
  EXPECT_TRUE(reader.AtEnd());
  EXPECT_EQ(0U, ::memcmp(kData, data, sizeof(kData)));

  EXPECT_FALSE(reader.Read(1, data));

  // Seek the underlying stream back to the start, and redo the read.
  ASSERT_TRUE(stream->Seek(0));
  EXPECT_EQ(0U, reader.Position());
  EXPECT_FALSE(reader.AtEnd());
  EXPECT_TRUE(reader.Read(sizeof(data), data));
  EXPECT_EQ(0U, ::memcmp(kData, data, sizeof(kData)));
}

using PdbStreamReaderWithPositionTest = PdbStreamReaderTest;

TEST_F(PdbStreamReaderWithPositionTest, ReadAll) {
  PdbStreamReaderWithPosition reader(stream_.get());
  EXPECT_FALSE(reader.AtEnd());
  EXPECT_EQ(0U, reader.Position());

  uint8_t buf[kTestDataLen] = {};
  EXPECT_TRUE(reader.Read(kTestDataLen, buf));
  EXPECT_EQ(0, ::memcmp(&data_.at(0), buf, sizeof(buf)));

  EXPECT_TRUE(reader.AtEnd());
  EXPECT_EQ(kTestDataLen, reader.Position());
  EXPECT_FALSE(reader.Read(1, buf));
}

TEST_F(PdbStreamReaderWithPositionTest, ReadPartial) {
  // Create a stream over a subset of the pdb stream_.
  const size_t kStartOffs = 4;
  const size_t kLength = 7;
  PdbStreamReaderWithPosition reader(kStartOffs, kLength, stream_.get());
  EXPECT_FALSE(reader.AtEnd());
  EXPECT_EQ(0U, reader.Position());

  uint8_t buf[kLength] = {};
  EXPECT_TRUE(reader.Read(sizeof(buf), buf));
  EXPECT_EQ(0, ::memcmp(&data_.at(kStartOffs), buf, kLength));

  EXPECT_TRUE(reader.AtEnd());
  EXPECT_EQ(kLength, reader.Position());
  EXPECT_FALSE(reader.Read(1, buf));
}

TEST_F(PdbStreamReaderWithPositionTest, SetStream) {
  // Test the SetStream case.
  const size_t kStartOffs = 19;
  const size_t kLength = 37;
  PdbStreamReaderWithPosition reader;
  reader.SetStream(kStartOffs, kLength, stream_.get());
  EXPECT_FALSE(reader.AtEnd());
  EXPECT_EQ(0U, reader.Position());

  uint8_t buf[kLength] = {};
  EXPECT_TRUE(reader.Read(sizeof(buf), buf));
  EXPECT_EQ(0, ::memcmp(&data_.at(kStartOffs), buf, kLength));

  EXPECT_TRUE(reader.AtEnd());
  EXPECT_EQ(kLength, reader.Position());
  EXPECT_FALSE(reader.Read(1, buf));
}

TEST_F(PdbStreamReaderWithPositionTest, EmptyTailRead) {
  // Create an empty stream over the tail of the stream.
  PdbStreamReaderWithPosition tail_empty(stream_->length(), 0, stream_.get());
  EXPECT_TRUE(tail_empty.AtEnd());
  EXPECT_EQ(0U, tail_empty.Position());
  uint8_t buf[1] = {};
  EXPECT_FALSE(tail_empty.Read(1, buf));
}

TEST_F(PdbStreamReaderWithPositionTest, EmptyCenterRead) {
  // Create an empty stream over center of the stream.
  PdbStreamReaderWithPosition middle_empty(stream_->length() / 2, 0,
                                           stream_.get());
  EXPECT_TRUE(middle_empty.AtEnd());
  EXPECT_EQ(0U, middle_empty.Position());
  uint8_t buf[1] = {};
  EXPECT_FALSE(middle_empty.Read(1, buf));
}

}  // namespace pdb
