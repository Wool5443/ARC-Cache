#include <iostream>
#include <climits>
#include <cstring>
#include "Utils.hpp"
#include "Cache.hpp"

using namespace mlib;
using namespace err;

LOG_INIT_FILE("../log.txt");

extern "C" int DRAW_HUGE_PENIS_AHAH(void);

constexpr std::size_t PAGE_SIZE = 1024;

using Key = std::size_t;

using Page = String;

struct PageGetter
{
    Page operator()(const Key& key)
    {
        auto text = String::ReadFromFile("../Words.txt").value;

        auto lines = text.Split("\n");
        LOG_ERROR_IF(lines.error);

        if (key < lines.value.length)
            return lines.value[key];
        return {};
    }
};

int main()
{
    Cache<Key, Page, PageGetter, 100> cache;

    Page zeroPage = PageGetter()(0);

    for (std::size_t i = 0; i < 1000; i++)
    {
        Result<Page> res = cache.FindPage(i % 200);
        Page correct = PageGetter()(i % 200);
        if (res.value != correct)
        {
            LOG("FUCK, correct: %s found: %s\n", correct.RawPtr(), res.value.RawPtr());
        }
    }

    // DRAW_HUGE_PENIS_AHAH();

    return 0;
}
