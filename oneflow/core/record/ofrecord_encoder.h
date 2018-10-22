#ifndef ONEFLOW_CORE_RECORD_OFRECORD_ENCODER_H_
#define ONEFLOW_CORE_RECORD_OFRECORD_ENCODER_H_

#include "oneflow/core/kernel/kernel_util.h"
#include "oneflow/core/record/ofrecord_decoder.h"

namespace oneflow {

class OFRecordEncoderIf {
 public:
  OF_DISALLOW_COPY_AND_MOVE(OFRecordEncoderIf);
  virtual ~OFRecordEncoderIf() = default;

  static void EncodeOneDataId(DeviceCtx* ctx, const char* data_id_str, OFRecord& record) {
    Feature tmp_feature;
    tmp_feature.mutable_bytes_list()->add_value(data_id_str);
    CHECK(record.mutable_feature()->insert({"data_id", tmp_feature}).second);
  }
  virtual void EncodeOneCol(DeviceCtx*, const Blob* in_blob, int64_t in_offset, Feature&,
                            const std::string& field_name, int64_t one_col_elem_num) const = 0;
  virtual void EncodeMultiCol(DeviceCtx*, const Blob* in_blob,
                              const std::vector<int64_t>& in_offset, Feature&,
                              const std::string& field_name, int64_t one_col_elem_num) const = 0;

 protected:
  OFRecordEncoderIf() = default;
};

template<EncodeCase encode_case, typename T>
class OFRecordEncoderImpl;

OFRecordEncoderIf* GetOFRecordEncoder(EncodeCase, DataType);

}  // namespace oneflow

#endif  // ONEFLOW_CORE_RECORD_OFRECORD_ENCODER_H_
