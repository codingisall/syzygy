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

#ifndef SYZYGY_KASKO_REPORTER_H_
#define SYZYGY_KASKO_REPORTER_H_

#include "base/macros.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/process/process_handle.h"
#include "base/strings/string16.h"
#include "syzygy/kasko/minidump_type.h"
#include "syzygy/kasko/report_repository.h"
#include "syzygy/kasko/service_bridge.h"

namespace base {
class TimeDelta;
}  // namespace base

namespace kasko {

class ServiceBridge;
class UploadThread;

// Implements the reporter process lifetime. Maintains state, operates a
// reporter RPC service, and configures background uploading of reports.
//
// Reports that exceed upload retry limits will be moved to a permanent failure
// destination. The reports consist of two files: a minidump file (extension
// kPermanentFailureMinidumpExtension, which is '.dmp') and a crash keys file
// (extension kPermanentFailureCrashKeysExtension, which is '.kys'). The two
// file names will be identical apart from the extension. The crash keys file
// will contain a JSON dictionary mapping crash key names to string values.
class Reporter {
 public:
  // The extension given to crash keys files in the permanent failure directory.
  static const base::char16* const kPermanentFailureCrashKeysExtension;
  // The extension given to minidump files in the permanent failure directory.
  static const base::char16* const kPermanentFailureMinidumpExtension;
  // The parameter name assigned to the uploaded minidump file.
  static const base::char16* const kMinidumpUploadFilePart;
  // The stream type assigned to the protobuf stream in the uploaded minidump
  // file.
  static const uint32_t kProtobufStreamType;

  // Creates a Reporter process. The process is already running in the
  // background when this method returns.
  // @param endpoint_name The RPC endpoint name to listen on.
  // @param url The URL that crash reports should be uploaded to.
  // @param data_directory The directory where crash reports will be generated
  //     and stored for uploading.
  // @param permanent_failure_directory The directory where crash reports that
  //     have exceeded retry limits will be moved to.
  // @param upload_interval The minimum interval between two upload operations.
  // @param retry_interval The minimum interval between upload attempts for a
  //     single crash report.
  // @returns a Reporter instance if successful.
  static scoped_ptr<Reporter> Create(
      const base::string16& endpoint_name,
      const base::string16& url,
      const base::FilePath& data_directory,
      const base::FilePath& permanent_failure_directory,
      const base::TimeDelta& upload_interval,
      const base::TimeDelta& retry_interval);

  ~Reporter();

  // Sends a diagnostic report for a specified process with the specified crash
  // keys.
  // @param process_handle A handle to the process to report on.
  // @param minidump_type The type of minidump to be included in the report.
  // @param crash_keys Crash keys to include in the report.
  void SendReportForProcess(
      base::ProcessHandle process_handle,
      MinidumpType minidump_type,
      const std::map<base::string16, base::string16>& crash_keys);

  // Shuts down and destroys a Reporter process. Blocks until all background
  // tasks have terminated.
  // @param instance The Reporter process instance to shut down.
  static void Shutdown(scoped_ptr<Reporter> instance);

 private:
  // Instantiates a Reporter process instance. Does not start any background
  // processes.
  // @param report_repository The report repository to store reports in.
  // @param upload_thread An upload thread that is configured to upload reports
  //     from |report_repository|.
  // @param endpoint_name The RPC endpoint name to listen on.
  // @param temporary_minidump_directory A directory where minidumps may be
  //     temporarily stored before uploading.
  Reporter(scoped_ptr<ReportRepository> report_repository,
           scoped_ptr<UploadThread> upload_thread,
           const base::string16& endpoint_name,
           const base::FilePath& temporary_minidump_directory);

  // A repository for generated reports.
  scoped_ptr<ReportRepository> report_repository_;

  // A background upload scheduler.
  scoped_ptr<UploadThread> upload_thread_;

  // The directory where minidumps will be initially created.
  base::FilePath temporary_minidump_directory_;

  // An RPC service endpoint.
  ServiceBridge service_bridge_;

  DISALLOW_COPY_AND_ASSIGN(Reporter);
};

}  // namespace kasko

#endif  // SYZYGY_KASKO_REPORTER_H_
