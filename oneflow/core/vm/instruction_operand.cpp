#include "oneflow/core/vm/instruction_operand.msg.h"
#include "oneflow/core/vm/instruction.pb.h"
#include "oneflow/core/common/util.h"

namespace oneflow {
namespace vm {

void InstructionOperand::__Init__(const InstructionOperandProto& proto) {
  if (proto.has_const_operand()) {
    mutable_const_operand()->mutable_operand()->__Init__(proto.const_operand());
  } else if (proto.has_mutable_operand()) {
    mutable_mutable_operand()->mutable_operand()->__Init__(proto.mutable_operand());
  } else if (proto.has_mut2_operand()) {
    mutable_mut2_operand()->mutable_operand()->__Init__(proto.mut2_operand());
  } else if (proto.has_const_host_operand()) {
    mutable_const_host_operand()->mutable_operand()->__Init__(proto.const_host_operand());
  } else if (proto.has_mutable_host_operand()) {
    mutable_mutable_host_operand()->mutable_operand()->__Init__(proto.mutable_host_operand());
  } else if (proto.has_mut2_host_operand()) {
    mutable_mut2_host_operand()->mutable_operand()->__Init__(proto.mut2_host_operand());
  } else if (proto.has_sep()) {
    mutable_sep();
  } else if (proto.has_double_i_operand()) {
    set_double_i_operand(proto.double_i_operand());
  } else if (proto.has_int64_i_operand()) {
    set_int64_i_operand(proto.int64_i_operand());
  } else if (proto.has_uint64_i_operand()) {
    set_uint64_i_operand(proto.uint64_i_operand());
  } else if (proto.has_bool_i_operand()) {
    set_bool_i_operand(proto.bool_i_operand());
  } else {
    UNIMPLEMENTED();
  }
}

}  // namespace vm
}  // namespace oneflow