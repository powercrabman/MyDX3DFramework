#pragma once

namespace CM
{
    template <typename KeyTy, typename ItemTy>
    class SortedVector
    {
        static_assert(std::is_constructible<KeyTy, const KeyTy&>::value&&
            std::is_same<decltype(std::declval<KeyTy>() < std::declval<KeyTy>()), bool>::value,
            "KeyTy.");

    public:
        using iterator = typename std::vector<std::pair<KeyTy, ItemTy>>::iterator;
        using const_iterator = typename std::vector<std::pair<KeyTy, ItemTy>>::const_iterator;

        SortedVector() = default;

        SortedVector(size_t capacity)
        {
            m_items.reserve(capacity);
        }

        SortedVector(const std::vector<std::pair<KeyTy, ItemTy>>& init_items)
        {
            for (const auto& item : init_items)
            {
                Insert(item.first, item.second);
            }
        }

        void Insert(const KeyTy& key, const ItemTy& item)
        {
            auto it = BinarySearch(key);
            VERTIFY(it == m_items.end(), "Key Is Already Exist");
            m_items.insert(it, std::make_pair(key, item));
        }

        void Remove(const KeyTy& key)
        {
            auto it = BinarySearch(key);
            VERTIFY(it != m_items.end(), "Key not found");
            m_items.erase(it);
        }

        const ItemTy& Search(const KeyTy& key) const
        {
            auto it = BinarySearch(key);
            VERTIFY(it != m_items.end(), "Key not found");

            return it->second;
        }

        ItemTy& Search(const KeyTy& key)
        {
            auto it = BinarySearch(key);
            VERTIFY(it != m_items.end(), "Key not found");

            return it->second;
        }

        const std::vector<std::pair<KeyTy, ItemTy>>& Items() const
        {
            return m_items;
        }

        /* Iterator Áö¿ø */
        iterator begin() { return m_items.begin(); }
        const_iterator begin() const { return m_items.begin(); }
        iterator end() { return m_items.end(); }
        const_iterator end() const { return m_items.end(); }

    private:
        std::vector<std::pair<KeyTy, ItemTy>>::iterator BinarySearch(const KeyTy& key)
        {
            return std::lower_bound(m_items.begin(), m_items.end(),
                std::make_pair(key, ItemTy()),
                [](const auto& a, const auto& b) { return a.first < b.first; });
        }

    private:
        std::vector<std::pair<KeyTy, ItemTy>> m_items;
    };
}