#pragma once

#include <vector>

// Wrapper klasse om de crashes te voorkomen
template <class T>
struct ListenersList
{
    std::vector<T*> active;
    std::vector<T*> pendingAdd;
    std::vector<T*> pendingRemove;
    int dispatchDepth = 0;

    void add(T* lis)
    {
        if (lis == nullptr)
            return;
        if (dispatchDepth > 0)
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
        if (dispatchDepth > 0)
        {
            pendingRemove.push_back(lis);
            return;
        }

        std::erase(active, lis);
    }

    template <class F>
    void dispatch(F&& fn)
    {
        ++dispatchDepth;
        for (std::size_t i = 0; i < active.size(); ++i)
        {
            T* lis = active[i];
            // sla listeners die eerder waren weggehaald over
            if (std::find(pendingRemove.begin(), pendingRemove.end(), lis) != pendingRemove.end())
                continue;
            fn(*lis);
        }
        --dispatchDepth;

        if (dispatchDepth == 0)
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
};
