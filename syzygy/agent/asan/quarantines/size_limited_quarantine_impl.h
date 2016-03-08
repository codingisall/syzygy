// Copyright 2014 Google Inc. All Rights Reserved.
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
//
// Internal implementation of a size-limited quarantine. This file is not
// meant to be included directly.

#ifndef SYZYGY_AGENT_ASAN_QUARANTINES_SIZE_LIMITED_QUARANTINE_IMPL_H_
#define SYZYGY_AGENT_ASAN_QUARANTINES_SIZE_LIMITED_QUARANTINE_IMPL_H_

namespace agent {
namespace asan {
namespace quarantines {

template<typename OT, typename SFT>
bool SizeLimitedQuarantineImpl<OT, SFT>::Push(
    const Object& object) {
  size_t size = size_functor_(object);
  if (max_object_size_ != kUnboundedSize && size > max_object_size_)
    return false;
  if (max_quarantine_size_ != kUnboundedSize && size > max_quarantine_size_)
    return false;
  if (!PushImpl(object))
    return false;

  // Note that if a thread gets preempted here, the size/count will be wrong,
  // until the thread resumes.
  ScopedQuarantineSizeCountLock size_count_lock(size_count_);
  size_count_.Increment(size, 1);

  return true;
}

template<typename OT, typename SFT>
bool SizeLimitedQuarantineImpl<OT, SFT>::Pop(
    Object* object) {
  DCHECK_NE(static_cast<Object*>(NULL), object);

  if (max_quarantine_size_ == kUnboundedSize)
    return false;

  {
    ScopedQuarantineSizeCountLock size_count_lock(size_count_);
    if (size_count_.size() <= max_quarantine_size_)
      return false;
  }

  if (!PopImpl(object))
    return false;

  // Note that if a thread gets preempted here, the size/count will be wrong,
  // until the thread resumes.
  size_t size = size_functor_(*object);
  ScopedQuarantineSizeCountLock size_count_lock(size_count_);
  size_count_.Decrement(size, 1);

  return true;
}

template<typename OT, typename SFT>
void SizeLimitedQuarantineImpl<OT, SFT>::Empty(
    ObjectVector* objects) {
  DCHECK_NE(static_cast<ObjectVector*>(NULL), objects);
  EmptyImpl(objects);

  // In order for the quarantine to remain long-term consistent we need to
  // remove a size and count consistent with the output of EmptyImpl. Simply
  // setting the size and count to zero could introduce inconsistency, as they
  // may not yet reflect the contributions of some of the elements returned by
  // EmptyImpl.
  int32_t net_size = 0;
  for (size_t i = 0; i < objects->size(); ++i) {
    size_t size = size_functor_(objects->at(i));
    net_size += size;
  }

  ScopedQuarantineSizeCountLock size_count_lock(size_count_);
  size_count_.Decrement(net_size, objects->size());
}

template <typename OT, typename SFT>
size_t SizeLimitedQuarantineImpl<OT, SFT>::GetCountForTesting() {
  ScopedQuarantineSizeCountLock size_count_lock(size_count_);
  return size_count_.count();
}

template<typename OT, typename SFT>
size_t SizeLimitedQuarantineImpl<OT, SFT>::GetLockId(
    const Object& object) {
  return GetLockIdImpl(object);
}

template<typename OT, typename SFT>
void SizeLimitedQuarantineImpl<OT, SFT>::Lock(size_t id) {
  LockImpl(id);
}

template<typename OT, typename SFT>
void SizeLimitedQuarantineImpl<OT, SFT>::Unlock(size_t id) {
  UnlockImpl(id);
}

}  // namespace quarantines
}  // namespace asan
}  // namespace agent

#endif  // SYZYGY_AGENT_ASAN_QUARANTINES_SIZE_LIMITED_QUARANTINE_IMPL_H_
