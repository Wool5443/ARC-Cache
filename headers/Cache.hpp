#ifndef MLIB_CACHE_HPP
#define MLIB_CACHE_HPP

#include <unordered_map>
#include "LinkedList.hpp"

template<typename KeyT, typename PageT, typename SlowPageGetter, std::size_t MAX_PAGES>
class Cache final
{
    enum ListType { LRU_LIST, LFU_LIST };
    struct PagePosition
    {
        std::size_t index;
        ListType    listType;
    };

    using KeyPage = std::pair<KeyT, PageT>;

    mlib::LinkedList<KeyPage, true> m_lruList{MAX_PAGES / 2};
    mlib::LinkedList<KeyPage, true> m_lfuList{MAX_PAGES / 2};

    std::unordered_map<KeyT, PagePosition> m_table{MAX_PAGES};
public:
    Cache() noexcept = default;

    err::Result<PageT> FindPage(const KeyT& key)
    {
        auto found = m_table.find(key);
        if (found == m_table.cend())
            return addPage(key);
        return fetchPage(found->second);
    }
private:
    err::Result<PageT> fetchPage(PagePosition position)
    {
        std::cout << "FETCHED\n";
        if (position.listType == LRU_LIST)
        {
            err::Result<KeyPage> result = m_lruList.Pop(position.index);
            RETURN_ERROR_RESULT(result.error, {}, PageT);

            KeyT lastElemKey = m_lfuList[m_lfuList.Tail()].first;
            err::ErrorCode error = m_lfuList.PushFront(result.value);
            if (error == err::ERROR_NO_MEMORY)
                m_table.erase(lastElemKey);
            else
                RETURN_ERROR_RESULT(error, {}, PageT);

            position = { m_lfuList.Head(), LFU_LIST };
            m_table[result.value.first] = position;

            return std::move(result.value.second);
        }
        else if (position.listType == LFU_LIST)
        {
            PageT page = m_lfuList[position.index].second;
            RETURN_ERROR_RESULT(m_lfuList.MoveToFront(position.index),
                                {}, PageT);

            position.index = m_lfuList.Head();

            return page;
        }

        RETURN_ERROR(err::ERROR_BAD_VALUE);
        return err::ERROR_BAD_VALUE;
    }

    err::Result<PageT> addPage(const KeyT& key)
    {
        std::cout << "SLOW ADDED\n";
        err::Result<PageT> pageRes = SlowPageGetter()(key);
        RETURN_RESULT(pageRes);

        err::ErrorCode error = m_lruList.PushFront(
            { key, pageRes.value }
        );
        if (error == err::ERROR_NO_MEMORY)
            m_table.erase(key);

        m_table[key] = { m_lruList.Head(), LRU_LIST };
        return pageRes;
    }
};

#endif
