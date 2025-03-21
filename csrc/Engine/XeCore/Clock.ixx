export module xe.xeCore.Clock;

import std;

import xe.xeCore.xeBaseDataType;
import xe.xeCore.xeOrdinals;


namespace xe
{
	////////////////////////////////////////////////////////////
	/// \brief Utility class that measures the elapsed time
	///
	/// The clock starts automatically after being constructed.
	///
	////////////////////////////////////////////////////////////
	export class Clock
	{
	public:
		Clock()
		{
			status = false;
			m_ref_point = 0;
			m_stop_point = 0;
		}
		////////////////////////////////////////////////////////////
		/// \brief Get the elapsed time
		///
		/// This function returns the time elapsed since the last call
		/// to `restart()` (or the construction of the instance if `restart()`
		/// has not been called).
		///
		/// \return microseconds elapsed
		///
		////////////////////////////////////////////////////////////
		[[nodiscard]] xeInt64 GetElapsedTime()
		{
			xeInt64 out_time = 0;
			status_lock.lock();
			if (status)
			{
				out_time = GetCurTime() - m_ref_point;
				status_lock.unlock();
			}
			else
			{
				out_time = m_stop_point - m_ref_point;
				status_lock.unlock();
			}
			return out_time;
		}

		////////////////////////////////////////////////////////////
		/// \brief Check whether the clock is running
		///
		/// \return `true` if the clock is running, `false` otherwise
		///
		////////////////////////////////////////////////////////////
		[[nodiscard]] bool IsRunning()
		{
			status_lock.lock();
			bool o_status = status;
			status_lock.unlock();
			return status;
		}

		////////////////////////////////////////////////////////////
		/// \brief Start the clock
		///
		/// \see `stop`
		///
		////////////////////////////////////////////////////////////
		void Start()
		{
			status_lock.lock();
			status = true;
			m_ref_point = GetCurTime();
			m_stop_point = 0;
			status_lock.unlock();
		}

		////////////////////////////////////////////////////////////
		/// \brief Stop the clock
		///
		/// \see `start`
		///
		////////////////////////////////////////////////////////////
		void Stop()
		{
			status_lock.lock();
			status = false;
			m_stop_point = GetCurTime();
			status_lock.unlock();
		}

		////////////////////////////////////////////////////////////
		/// \brief Restart the clock
		///
		/// This function puts the time counter back to zero, returns
		/// the elapsed time, and leaves the clock in a running state.
		///
		/// \return Time elapsed
		///
		/// \see `reset`
		///
		////////////////////////////////////////////////////////////
		void Restart()
		{
			status_lock.lock();
			status = true;
			m_ref_point = GetCurTime();
			m_stop_point = 0;
			status_lock.unlock();
		}

		////////////////////////////////////////////////////////////
		/// \brief Reset the clock
		///
		/// This function puts the time counter back to zero, returns
		/// the elapsed time, and leaves the clock in a paused state.
		///
		/// \return Time elapsed
		///
		/// \see `restart`
		///
		////////////////////////////////////////////////////////////
		void Reset()
		{
			status_lock.lock();
			status = false;
			m_stop_point = 0;
			m_ref_point = 0;
			status_lock.unlock();
		}
	private:
		////////////////////////////////////////////////////////////
		// Member data
		////////////////////////////////////////////////////////////
		xeUint64 m_ref_point;							// Time of last reset
		xeUint64 m_stop_point;							//!< Time of last stop
		bool status;
		std::mutex status_lock;

		xeInt64 GetCurTime() const;
	};
}