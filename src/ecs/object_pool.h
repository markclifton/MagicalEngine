
//... tbd
/*
#pragma once

#include <atomic>
#include <iterator>
#include <map>
#include <vector>

#include "utils/non_copyable.h"

template <typename T>
class ObjectPool;

class ObjectPoolBase {};

class ObjectPoolManager : ME::NonCopyable {
public:
    static ObjectPoolManager& instance() {
        static ObjectPoolManager mgr;
        return mgr;
    }
    
    template <typename T>
    ObjectPool<T>* allocate() {
        if( !get<T>() ) {
            std::unique_ptr<ObjectPoolBase> temp ( new ObjectPool<T>() );
            m_map[getTypeId<T>()] = std::move(temp);
        }
        return get<T>();
    }

    template <typename T>
    ObjectPool<T>* get() {
        if(m_map.find(getTypeId<T>()) != m_map.end()) {
            return (ObjectPool<T>*)m_map[getTypeId<T>()].get();
        }
        return nullptr;
    }

private:
    template <class key>
    int getTypeId() {
        static const int id = m_lastId++;
        return id;
    }

    std::atomic_int m_lastId;
    std::map<int, std::unique_ptr<ObjectPoolBase>> m_map;
};

template <typename T>
class ObjectPool : ME::NonCopyable, public ObjectPoolBase {
public:
    void add(T component) { m_pool.push_back(component); }
    void clear() { m_pool.clear(); }
    auto begin() { return m_pool.begin(); }
    auto end() { return m_pool.end(); }
    auto count() { return m_pool.size(); }

protected:
private:
    std::vector<T> m_pool;
};
*/
