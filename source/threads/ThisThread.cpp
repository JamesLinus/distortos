/**
 * \file
 * \brief ThisThread namespace implementation
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/ThisThread.hpp"

#include "distortos/internal/scheduler/getScheduler.hpp"
#include "distortos/internal/scheduler/Scheduler.hpp"

#include "distortos/Thread.hpp"

#include <cerrno>

namespace distortos
{

namespace ThisThread
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

#ifdef CONFIG_THREAD_DETACH_ENABLE

int detach()
{
	return ThisThread::get().detach();
}

#endif	// def CONFIG_THREAD_DETACH_ENABLE

Thread& get()
{
	return internal::getScheduler().getCurrentThreadControlBlock().getOwner();
}

uint8_t getEffectivePriority()
{
	return internal::getScheduler().getCurrentThreadControlBlock().getEffectivePriority();
}

uint8_t getPriority()
{
	return internal::getScheduler().getCurrentThreadControlBlock().getPriority();
}

void setPriority(const uint8_t priority, const bool alwaysBehind)
{
	internal::getScheduler().getCurrentThreadControlBlock().setPriority(priority, alwaysBehind);
}

int sleepFor(const TickClock::duration duration)
{
	return sleepUntil(TickClock::now() + duration + TickClock::duration{1});
}

int sleepUntil(const TickClock::time_point timePoint)
{
	auto& scheduler = internal::getScheduler();
	internal::ThreadList sleepingList;
	const auto ret = scheduler.blockUntil(sleepingList, ThreadState::sleeping, timePoint);
	return ret == ETIMEDOUT ? 0 : ret;
}

void yield()
{
	internal::getScheduler().yield();
}

}	// namespace ThisThread

}	// namespace distortos
