#pragma once
#include <cstddef>
#include <cassert>

namespace CM
{
	template<typename Ty, std::size_t N>
	class Array
	{
	public:
		Array() : mSize(0) {}

		void PushBack(const Ty& value)
		{
			assert(mSize < N && "Array capacity exceeded");
			mDatas[mSize++] = value;
		}

		void PopBack()
		{
			assert(mSize > 0 && "Array is empty");
			--mSize;
		}

		Ty& operator[](std::size_t index)
		{
			assert(index < mSize && "Index out of bounds");
			return mDatas[index];
		}

		const Ty& operator[](std::size_t index) const
		{
			assert(index < mSize && "Index out of bounds");
			return mDatas[index];
		}

		std::size_t Size() const noexcept
		{
			return mSize;
		}

		constexpr std::size_t Capacity() const noexcept
		{
			return N;
		}

		Ty* begin() noexcept { return mDatas; }
		const Ty* begin() const noexcept { return mDatas; }

		Ty* end() noexcept { return mDatas + mSize; }
		const Ty* end() const noexcept { return mDatas + mSize; }


		Ty* Find(const Ty& value) noexcept
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				if (mDatas[i] == value)
				{
					return &mDatas[i];
				}
			}
			return end();
		}

		const Ty* Find(const Ty& value) const noexcept
		{
			for (std::size_t i = 0; i < mSize; ++i)
			{
				if (mDatas[i] == value)
				{
					return &mDatas[i];
				}
			}
			return end();
		}

		void Erase(std::size_t index)
		{
			assert(index < mSize && "Index out of bounds");
			for (std::size_t i = index; i < mSize - 1; ++i)
			{
				mDatas[i] = mDatas[i + 1];
			}
			--mSize;
		}

	private:
		Ty mDatas[N];
		std::size_t mSize;
	};
}

namespace CM
{
    template<typename Ty, std::size_t N>
    class Array<Ty*, N>
    {
    public:
        Array() : mSize(0)
        {
            for (std::size_t i = 0; i < N; ++i)
            {
                mDatas[i] = nullptr;
            }
        }

        void PushBack(Ty* value)
        {
            assert(mSize < N && "Array capacity exceeded");
            mDatas[mSize++] = value;
        }

        void PopBack()
        {
            assert(mSize > 0 && "Array is empty");
            --mSize;
            mDatas[mSize] = nullptr;
        }

        Ty*& operator[](std::size_t index)
        {
            assert(index < mSize && "Index out of bounds");
            return mDatas[index];
        }

        const Ty* const& operator[](std::size_t index) const
        {
            assert(index < mSize && "Index out of bounds");
            return mDatas[index];
        }

        std::size_t Size() const noexcept
        {
            return mSize;
        }

        constexpr std::size_t Capacity() const noexcept
        {
            return N;
        }

        Ty** begin() noexcept { return mDatas; }
        Ty* const* begin() const noexcept { return mDatas; }

        Ty** end() noexcept { return mDatas + mSize; }
        Ty* const* end() const noexcept { return mDatas + mSize; }

        Ty** Find(Ty* value) noexcept
        {
            for (std::size_t i = 0; i < mSize; ++i)
            {
                if (mDatas[i] == value)
                {
                    return &mDatas[i];
                }
            }
            return end();
        }

        Ty* const* Find(Ty* value) const noexcept
        {
            for (std::size_t i = 0; i < mSize; ++i)
            {
                if (mDatas[i] == value)
                {
                    return &mDatas[i];
                }
            }
            return end();
        }

        void Erase(Ty* value)
        {
            size_t idx = 0;
            for (; idx < mSize; ++idx)
            {
                if (value == mDatas[idx])
                {
                    break;
                }
            }

            if (idx != mSize)
            {
                for (std::size_t i = idx; i < mSize - 1; ++i)
                {
                    mDatas[i] = mDatas[i + 1];
                }
                --mSize;
                mDatas[mSize] = nullptr;
            }
        }

        void Erase(std::size_t index)
        {
            assert(index < mSize && "Index out of bounds");
            for (std::size_t i = index; i < mSize - 1; ++i)
            {
                mDatas[i] = mDatas[i + 1];
            }
            --mSize;
            mDatas[mSize] = nullptr; 
        }

    private:
        Ty* mDatas[N];
        std::size_t mSize;
    };
}