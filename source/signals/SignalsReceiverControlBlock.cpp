/**
 * \file
 * \brief SignalsReceiverControlBlock class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-03-14
 */

#include "distortos/signals/SignalsReceiverControlBlock.hpp"

#include "distortos/scheduler/getScheduler.hpp"
#include "distortos/scheduler/Scheduler.hpp"

#include "distortos/architecture/InterruptMaskingLock.hpp"

#include <cerrno>

namespace distortos
{

namespace synchronization
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

int SignalsReceiverControlBlock::generateSignal(const uint8_t signalNumber)
{
	if (threadControlBlock_ == nullptr)
		return EINVAL;

	architecture::InterruptMaskingLock interruptMaskingLock;

	const auto ret = pendingSignalSet_.add(signalNumber);
	if (ret != 0)
		return ret;

	if (waitingSignalSet_ == nullptr)
		return 0;

	const auto testResult = waitingSignalSet_->test(signalNumber);
	if (testResult.first != 0)
		return testResult.first;
	if (testResult.second == false)	// signalNumber is not "waited for"?
		return 0;

	scheduler::getScheduler().unblock(threadControlBlock_->getIterator());
	return 0;
}

SignalSet SignalsReceiverControlBlock::getPendingSignalSet() const
{
	return pendingSignalSet_;
}

}	// namespace synchronization

}	// namespace distortos
