#pragma once

#include <algorithm>
#include <vector>

// Wrapper klasse voor de listeners om de crashes te voorkomen
// Het heeft een template, zodat elke listener apart afgehandeld kan worden
template <class T>
class ListenersList
{
public:
    void add(T* lis)
    {
        if (lis == nullptr)
            return;
        if (dispatchCount > 0)
        {
            pendingAdd.push_back(lis);
            return;
        }

        if (std::find(active.begin(), active.end(), lis) == active.end())
            active.push_back(lis);
    }

    void remove(T* lis)
    {
        if (lis == nullptr)
            return;

        std::erase(pendingAdd, lis);

        if (dispatchCount > 0)
        {
            pendingRemove.push_back(lis);
            return;
        }

        std::erase(active, lis);
    }

    template <class F>
    void dispatch(F&& fn)
    {
        ++dispatchCount;
        for (std::size_t i = 0; i < active.size(); ++i)
        {
            T* lis = active[i];
            // sla listeners die eerder waren weggehaald over
            if (std::find(pendingRemove.begin(), pendingRemove.end(), lis) != pendingRemove.end())
                continue;
            fn(*lis);
        }
        --dispatchCount;

        if (dispatchCount == 0)
            flush();
    }

    void flush()
    {
        for (T* lis : pendingRemove)
            std::erase(active, lis);
        pendingRemove.clear();

        for (T* lis : pendingAdd)
        {
            if (std::find(active.begin(), active.end(), lis) == active.end())
                active.push_back(lis);
        }
        pendingAdd.clear();
    }

private:
    std::vector<T*> active;
    std::vector<T*> pendingAdd;    // De listeners die nog worden toegevoegd
    std::vector<T*> pendingRemove; // De listeners die nog worden verwijderd
    int dispatchCount = 0;
};
