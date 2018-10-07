/**
 * Copyright Soramitsu Co., Ltd. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef IROHA_SYNCHRONIZER_HPP
#define IROHA_SYNCHRONIZER_HPP

#include <rxcpp/rx-observable.hpp>

#include "network/peer_communication_service.hpp"
#include "synchronizer/synchronizer_common.hpp"

namespace iroha {
  namespace synchronizer {

    /**
     * Synchronizer is interface for fetching missed blocks
     */
    class Synchronizer {
     public:
      /**
       * After synchronization this observable emits zero or more blocks plus
       * outcome of synchronization
       */
      virtual rxcpp::observable<SynchronizationEvent> on_commit_chain() = 0;

      virtual ~Synchronizer() = default;
    };

  }  // namespace synchronizer
}  // namespace iroha
#endif  // IROHA_SYNCHRONIZER_HPP
