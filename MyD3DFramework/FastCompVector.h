#pragma once

#include "Component.h"

namespace CM
{
	template <typename CompType>
	class FastCompVector final
	{
		static_assert(std::is_pointer<CompType>::value); //개발단계, 포인터에 대해서만 실험
	public:
		FastCompVector() = default;
		FastCompVector(size_t inReserve);
		~FastCompVector() = default;

		inline void Insert(const CompType& inItem)
		{
			auto iter = m_hashRepo.find(inItem->GetComponentID());
			assert(iter == m_hashRepo.end());

			if (iter == m_hashRepo.end())
			{
				size_t size = m_vecRepo.size();
				assert(size >= m_validSize);

				if (size == m_validSize)
				{
					m_vecRepo.push_back(inItem);
				}
				else
				{
					m_vecRepo[m_validSize] = inItem;
				}

				m_hashRepo[inItem->GetComponentID()] = std::make_pair(m_validSize, inItem);
				++m_validSize;
			}
		}

		inline void Remove(const CompType& inItem)
		{
			size_t inCompID = inItem->GetComponentID();

			auto iter = m_hashRepo.find(inCompID);

			assert(iter != m_hashRepo.end());

			if (iter != m_hashRepo.end())
			{
				const Pair& itemToRemove = iter->second;

				if (m_vecRepo.size() > 1)
				{
					m_vecRepo[0] = nullptr;
				}
				else
				{
					CompType nextComp = m_vecRepo[m_validSize - 1];
					m_vecRepo[itemToRemove.first] = nextComp;
					m_vecRepo[m_validSize - 1] = nullptr;
					m_hashRepo[nextComp->GetComponentID()].first = itemToRemove.first;
				}

				m_hashRepo.erase(iter);
				--m_validSize;
			}
		}

		inline const std::vector<CompType>& GetVector() const
		{
			return m_vecRepo;
		}

		inline void Reserve(size_t inReserve)
		{
			m_vecRepo.reserve(inReserve);
			m_hashRepo.reserve(inReserve);
		}

		inline void CleanGarbage()
		{
			if (m_validSize > 0)
			{
				m_vecRepo.erase(m_vecRepo.begin() + m_validSize, m_vecRepo.end());
			}
		}

		inline bool IsContation(const CompType& inItem)
		{
			return m_hashRepo.contains(inItem->GetComponentID());
		}

	private:
		using Pair = std::pair<size_t, CompType>;

		std::unordered_map<size_t, Pair> m_hashRepo;
		std::vector<CompType> m_vecRepo;
		size_t m_validSize = 0;
	};

	template<typename CompType>
	inline FastCompVector<CompType>::FastCompVector(size_t inReserve)
	{
		m_hashRepo.reserve(inReserve);
		m_vecRepo.reserve(inReserve);
	}
}
