/*
Copyright Soramitsu Co., Ltd. 2016 All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <infra/flatbuf/main_generated.h>
#include "autogen_extend.h"

#include <memory>
#include <string>

namespace flatbuffer_service {

std::string toString(const ::iroha::Transaction& tx) {}

/**
 * toConsensusEvent
 * - Encapsulate a transaction in a consensus event. Argument fromTx will be
 *   deeply copied and create new consensus event that has the copied transaction.
 */
std::unique_ptr<::iroha::ConsensusEvent> toConsensusEvent(
    const iroha::Transaction& fromTx) {

  flatbuffers::FlatBufferBuilder fbbConsensusEvent;

  // At first, peerSignatures is empty. (Is this right?)
  std::vector<flatbuffers::Offset<Signature>> peerSignatures;

  std::vector<flatbuffers::Offset<iroha::Signature>> signatures;

  // Tempolary implementation: Currently, #(tx) is one.
  const auto& aSignature = fromTx.signatures()->Get(0);

  std::vector<uint8_t> signatureBlob(
    aSignature->signature()->begin(),
    aSignature->signature()->end()
  );

  signatures.push_back(
    ::iroha::CreateSignatureDirect(
      fbbConsensusEvent,
      aSignature->publicKey()->c_str(),
      &signatureBlob,
      1234567
    )
  );

  std::vector<uint8_t> hashes(
    fromTx.hash()->begin(),
    fromTx.hash()->end()
  );

  std::vector<uint8_t> data(
    fromTx.attachment()->data()->begin(),
    fromTx.attachment()->data()->end()
  );

  auto attachmentOffset = ::iroha::CreateAttachmentDirect(
    fbbConsensusEvent,
    fromTx.attachment()->mime()->c_str(),
    &data
  );

  std::vector<flatbuffers::Offset<Transaction>> transactions;

  // TODO: Currently, #(transaction) is one.
  transactions.push_back(
    ::iroha::CreateTransactionDirect(
      fbbConsensusEvent,
      fromTx.creatorPubKey()->c_str(),
      fromTx.command_type(),
      flatbuffer_service::CreateCommandDirect(
        fbbConsensusEvent,
        fromTx.command(),
        fromTx.command_type()
      ),
      &signatures,
      &hashes,
      attachmentOffset
    )
  );

  auto consensusEventOffset = ::iroha::CreateConsensusEventDirect(
    fbbConsensusEvent,
    &peerSignatures,
    &transactions
  );

  fbbConsensusEvent.Finish(consensusEventOffset);

  auto flatbuf = fbbConsensusEvent.ReleaseBufferPointer();

  return std::unique_ptr<::iroha::ConsensusEvent>(
    flatbuffers::GetMutableRoot<::iroha::ConsensusEvent>(flatbuf.get())
  );
}

std::unique_ptr<::iroha::ConsensusEvent> addSignature(
    const std::unique_ptr<iroha::ConsensusEvent>& event) {}

} // namespace flatbuffer_service
