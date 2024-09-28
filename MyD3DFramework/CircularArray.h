#pragma once

namespace CM
{

	template <typename Ty, size_t CAPACITY>
	class CircularArray {
	public:
		CircularArray()
			: m_head(0)
			, m_tail(0)
			, m_count(0)
		{
		}

		~CircularArray()
		{
		}

		constexpr inline size_t Size() const
		{
			return m_count;
		}

		constexpr inline size_t Capacity() const
		{
			return CAPACITY;
		}

		void PushBack(const Ty& item)
		{
			VERTIFY(!IsFull(), "Buffer is full");

			m_items[m_head] = item;
			m_head = (m_head + 1) % CAPACITY;
			++m_count;
		}

		void PopFront()
		{
			VERTIFY(!IsEmpty(), "Buffer is empty");

			m_tail = (m_tail + 1) % CAPACITY;
			--m_count;
		}

		Ty Front() const
		{
			VERTIFY(!IsEmpty(), "Buffer is empty");

			return m_items[m_tail];
		}

		Ty Back() const
		{
			VERTIFY(!IsEmpty(), "Buffer is empty");

			size_t backIndex = (m_head + CAPACITY - 1) % CAPACITY;
			return m_items[backIndex];
		}

		bool IsEmpty() const
		{
			return m_count == 0;
		}

		bool IsFull() const
		{
			return m_count == CAPACITY;
		}

	private:
		Ty m_items[CAPACITY];
		size_t m_head;
		size_t m_tail;
		size_t m_count;
	};
}
