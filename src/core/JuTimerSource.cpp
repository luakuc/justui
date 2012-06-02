#include "JuTimerSource.h"

#include "JuTimerEvent.h"
#include "JuDebug.h"

#include <memory.h>

JuTimerSource::JuTimerSource(int interval) : m_interval(interval) {
	if (gettimeofday(&m_timeval, NULL) == -1) {
		memset(&m_timeval, 0, sizeof(struct timeval));
	}
}

JuTimerSource::~JuTimerSource() {

}

int JuTimerSource::check() {
	struct timeval now;

	if (gettimeofday(&now, NULL) == -1) {
		return 1;
	}

	int timeout = m_interval - (now.tv_sec * 1000000 + now.tv_usec - (m_timeval.tv_sec * 1000000 + m_timeval.tv_usec));

	if (timeout <= 0) {
		if (gettimeofday(&m_timeval, NULL) == -1) {
			memset(&m_timeval, 0, sizeof(struct timeval));
		}
	}

	timeout = timeout < 0 ? 0 : timeout;
	return timeout;
}

void JuTimerSource::dispatch() {
	JUDEBUG("interval = %d", m_interval);
	JuTimerEvent event((long)this);
	this->dispatchEvent((JuEvent *)&event);
}

int JuTimerSource::getFd() const {
	return -1;
}
